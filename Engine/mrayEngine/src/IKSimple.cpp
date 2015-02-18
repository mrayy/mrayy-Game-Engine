
#include "stdafx.h"
#include "IKSimple.h"
#include "IMovable.h"


namespace mray
{
namespace animation
{

IKSimple::IKSimple()
:m_query(0),m_maxIteration(100)
{
}
IKSimple::~IKSimple()
{
}

void IKSimple::setMaxIterations(int n)
{
	m_maxIteration=n;
}
int IKSimple::getMaxIterations()
{
	return m_maxIteration;
}

void IKSimple::calcOrintation(bool e)
{
}
bool IKSimple::isCalcOrintation()
{
	return true;
}

void IKSimple::setJointsChain(IJointQuery* q)
{
	m_query=q;
}
void IKSimple::setTarget(const math::vector3d&p,const math::quaternion&r)
{
	m_targetPos=p;
}

void IKSimple::update()
{
	if(!m_query)
		return;
	m_jointsDOF=&m_query->getJointsDOF();
	if(m_jointsDOF->size()==0)
		return;

	math::vector3d ef_pos=m_query->getEndEffectorPos();
	m_jointData.resize(m_jointsDOF->size());
	m_changeRotation.resize(m_jointsDOF->size());
	float legLength=0;
	for(int i=0;i<m_jointsDOF->size()-1;++i)
	{
		m_jointData[i].rotationAxis=((*m_jointsDOF)[i+1].node->getAbsolutePosition()-
			(*m_jointsDOF)[i].node->getAbsolutePosition()).
			crossProduct((*m_jointsDOF)[i+2].node->getAbsolutePosition()-
			(*m_jointsDOF)[i+1].node->getAbsolutePosition());
		math::quaternion q((*m_jointsDOF)[i+1].node->getOrintation());
		if(i>0)
			m_jointData[i-1].rotation=q;
		m_jointData[i].rotationAxis=q.inverse()*m_jointData[i].rotationAxis;
		m_jointData[i].rotationAxis.Normalize();
		m_jointData[i].rotationAngle=((*m_jointsDOF)[i+1].node->getAbsolutePosition()-
			(*m_jointsDOF)[i].node->getAbsolutePosition()).
			getAngleBetween((*m_jointsDOF)[i+1].node->getAbsolutePosition()-
			(*m_jointsDOF)[i+2].node->getAbsolutePosition());

		m_jointData[i].rotationAngle=math::toDeg(m_jointData[i].rotationAngle);

		m_jointData[i].length=((*m_jointsDOF)[i+1].node->getAbsolutePosition()-
			(*m_jointsDOF)[i].node->getAbsolutePosition()).Length();

		legLength+=m_jointData[i].length;
	}
	m_jointData[m_jointData.size()-1].length=((*m_jointsDOF)[m_jointData.size()-1].node->getAbsolutePosition()-ef_pos).Length();

	legLength+=m_jointData[m_jointData.size()-1].length;

	float positionAccuracy=legLength*0.001f;

	float currentDistance=(ef_pos-(*m_jointsDOF)[0].node->getAbsolutePosition()).Length();
	float targetDistance=(m_targetPos-(*m_jointsDOF)[0].node->getAbsolutePosition()).Length();

	float bendingHigh=1,bendingLow=0;
	bool minIsFound=false;
	bool bendMore=false;

	if(targetDistance>currentDistance)
	{
		minIsFound=true;
	}else
	{
		bendMore=true;
	}
	int tries=0;
	while(abs(currentDistance-targetDistance)>positionAccuracy && tries<m_maxIteration)
	{
		float newBending;
		float newAngle;
		if(!minIsFound)newBending=bendingHigh;
		else newBending=(bendingLow+bendingHigh)*0.5f;

		for(int i=0;i<m_jointsDOF->size()-1;++i)
		{
			if(!bendMore)
				newAngle=math::lerp(180.0f,m_jointData[i].rotationAngle,newBending);
			else
				newAngle=m_jointData[i].rotationAngle*(1-newBending) + 
				(m_jointData[i].rotationAngle-30)*newBending;
			float angleDiff=m_jointData[i].rotationAngle-newAngle;
			math::quaternion addRotation=math::quaternion(angleDiff,m_jointData[i].rotationAxis);
			m_changeRotation[i + 1] = angleDiff;// addRotation*m_changeRotation[i];
		}
		currentDistance=(ef_pos-(*m_jointsDOF)[0].node->getAbsolutePosition()).Length();
		if(targetDistance>currentDistance)
			minIsFound=true;

		if(minIsFound)
		{
			if(targetDistance>currentDistance)
				bendingHigh=newBending;
			else bendingLow=newBending;
			if(bendingHigh<0.01)
				break;
		}else
		{
			bendingLow=bendingHigh;
			++bendingHigh;
		}
		++tries;
	}

	float angle=(ef_pos-(*m_jointsDOF)[0].node->getAbsolutePosition()).getAngleBetween(m_targetPos-(*m_jointsDOF)[0].node->getAbsolutePosition());
	math::vector3d axis=(ef_pos-(*m_jointsDOF)[0].node->getAbsolutePosition()).crossProduct(m_targetPos-(*m_jointsDOF)[0].node->getAbsolutePosition());
	angle=math::toDeg(angle);
	m_changeRotation[0] = angle;// math::quaternion(angle, axis)*m_changeRotation[0];
	//m_changeRotation[0] = axis*m_changeRotation[0] * angle;
	m_query->applyChange(m_changeRotation);
}


}
}


