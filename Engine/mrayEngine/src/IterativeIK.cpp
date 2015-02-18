#include "stdafx.h"

#include "IterativeIK.h"
#include "IMovable.h"

 

namespace mray{
namespace animation{

IterativeIK::IterativeIK()
{
	m_maxIteration=50;
	m_allowedError=math::Epsilon;
	m_jacobian=0;
	m_beta=0.7;
	m_query=0;
	calcOrintation(false);
}
IterativeIK::~IterativeIK()
{
	delete m_jacobian;
	delete m_query;
}
void IterativeIK::calcOrintation(bool e)
{
	m_calcOrintation=e;
	m_jCols=m_calcOrintation?6:3;
}
bool IterativeIK::isCalcOrintation()
{
	return m_calcOrintation;
}

void IterativeIK::setMaxIterations(int n)
{
	m_maxIteration=n;
}

int IterativeIK::getMaxIterations()
{
	return m_maxIteration;
}


void IterativeIK::setAllowedError(float e)
{
	m_allowedError=e;
}

float IterativeIK::getAllowedError()
{
	return m_allowedError;
}

void IterativeIK::setJacobian(math::IJacobian*j)
{
	m_jacobian=j;
}
void IterativeIK::setBeta(float b)
{
	m_beta=b;
}
float IterativeIK::getBeta()
{
	return m_beta;
}

void IterativeIK::setJointsChain(IJointQuery* q)
{
	m_query=q;
	if(m_query){
		//m_jointsDOF.resize(m_query->getJointsDOFCount());
		m_changeVector.resize(m_query->getJointsDOFCount());
	}
}
void IterativeIK::setTarget(const math::vector3d&t,const math::quaternion&r)
{
	m_targetPos=t;
	m_targetOri=r;
}

float IterativeIK::computeJacobian()
{
	if(!m_query || !m_jacobian)
		return 0;
	float**m;

	math::vector3d ef_pos=m_query->getEndEffectorPos();
	m_jointsDOF=&m_query->getJointsDOF();
	m_jointsData.resize(m_jointsDOF->size());
	for (int i = 0; i<m_jointsDOF->size(); ++i)
	{
		m_jointsData[i].pivot = (*m_jointsDOF)[i].node->getAbsolutePosition();
		m_jointsData[i].axis = (*m_jointsDOF)[i].GetAxis();
		//1 rotation Axis (quaternion Axis)
		//we choose "Best Axis" 
		/*
		m_jointsData[i].axis=(ef_pos-m_jointsData[i].pivot).crossProduct(m_targetPos-m_jointsData[i].pivot);
		if(m_jointsData[i].axis.LengthSQ()==0){
			m_jointsData[i].axis=math::vector3d::XAxis;
		}else{
			m_jointsData[i].axis.Normalize();
		}*/
	}
	m_jacobian->setRowsColsCount(m_jointsDOF->size(), m_jCols);
	//calc Jacobian's Columns
	const IJointQuery::JointDOF*dofs=&(*m_jointsDOF)[0];
	m=m_jacobian->getMat();
	for (int i = 0; i<m_jointsDOF->size(); ++i)
	{
		//de/d@i=ai x (e-ri)
		//a=axis
		//e=end effector pos
		//a=pivot
		if(dofs[i].type==EIKJointType::Prismatic){
			math::vector3d de=m_jointsData[i].axis.crossProduct(ef_pos-m_jointsData[i].pivot);
			m[i][0]=de.x;
			m[i][1]=de.y;
			m[i][2]=de.z;
			if(m_calcOrintation){
				m[i][3]=m_jointsData[i].axis.x;
				m[i][4]=m_jointsData[i].axis.y;
				m[i][5]=m_jointsData[i].axis.z;
			}
		}else{
			m[i][0]=m_jointsData[i].axis.x;
			m[i][1]=m_jointsData[i].axis.y;
			m[i][2]=m_jointsData[i].axis.z;
			if(m_calcOrintation){
				m[i][3]=0;
				m[i][4]=0;
				m[i][5]=0;
			}
		}
	}

	m_jacobian->calcInverse(m_inverseJacobian);

	return 0;

	//test inverse
	math::IJacobian jjError;
	jjError.setRowsColsCount(m_jacobian->getRowsCount(),m_inverseJacobian.getColsCount());
	m=jjError.getMat();

	float e=0;

	for(int i=0;i<m_jacobian->getRowsCount();++i){
		for(int j=0;j<m_inverseJacobian.getColsCount();++j)
		{
			float sum=0;
			for (int k=0;k<m_jacobian->getColsCount();++k)
			{
				sum+=(*m_jacobian)(i,k)*m_inverseJacobian(k,j);
			}
			m[i][j]=((i==j)?1:0)-sum;
		}
	}


	for(int i=0;i<m_changeVector.size();++i)
	{
		float sum=0;
		for (int k=0;k<m_jCols;++k)
		{
			sum+=m_deltaTarget[k]*m_inverseJacobian(i,k);
		}
		e+=sum*sum;
	}
	return e;
}
float IterativeIK::computeChange()
{
	float **mat=m_inverseJacobian.getMat();
	float total=0;
	//multiply inverseJacobian with the deltaPos to get the change
	for(int i=0;i<m_changeVector.size();++i)
	{
		float sum=0;
		for (int k=0;k<m_jCols;++k)
		{
			sum+=m_deltaTarget[k]*mat[k][i];
		}
		m_changeVector[i]=sum;//math::quaternion(sum, m_jointsData[i].axis);
		//q.toEulerAngles();
		total+=sum;
	}
	return total;
}
void IterativeIK::applyConstraints()
{
}

void IterativeIK::update()
{
	if(!m_query)
		return;
	math::vector3d efPos;
	math::quaternion efOri;
	math::quaternion targetOri;
	math::vector3d targetOriV;
	math::vector3d dPos;

	float beta=m_beta;

	int i=0;
	float eSQ=m_allowedError*m_allowedError;
	float lastChange=0;

	m_query->OnIKBegin();
	do
	{
		efPos=m_query->getEndEffectorPos();

		m_deltaTarget[0]=(m_targetPos.x-efPos.x)*beta;
		m_deltaTarget[1]=(m_targetPos.y-efPos.y)*beta;
		m_deltaTarget[2]=(m_targetPos.z-efPos.z)*beta;


		if(m_calcOrintation){
			efOri=m_query->getEndEffectorOri();

			targetOri=efOri.inverse()*m_targetOri;
			targetOri.toEulerAngles(targetOriV);
			/*
			targetOriV.set(targetOri.x,targetOri.y,targetOri.z);
			s=targetOriV.Length();
			if(s!=0){
				angle=math::toDeg(atan2(s,targetOri.w));
				targetOriV*=2*(angle/s);
			}*/

			m_deltaTarget[3]=(targetOriV.x)*beta;
			m_deltaTarget[4]=(targetOriV.y)*beta;
			m_deltaTarget[5]=(targetOriV.z)*beta;
		}
		computeJacobian();
		/*
		while(computeJacobian()>25)
		{
			m_deltaPos/=2;
		}*/
		float c=computeChange();
		if(abs(c)<=math::Epsilon)
		{
			break;
		}

		/*
		if(c<lastChange && beta>m_beta)
				{
					beta*=2.0f/3.0f;
				}else*/
		/*
		if(c<lastChange && beta<1)
		{
			beta=beta*1.5f;
			if(beta>1)
				beta=1;
		}
		else if (false)
		{
			beta *= 0.5f;
			if (beta < m_beta)
				beta = m_beta;
		}*/
		lastChange=c;
		
		m_query->applyChange(m_changeVector);
		++i;
	}while (((efPos-m_targetPos).LengthSQ()>eSQ ||
		m_calcOrintation && (targetOriV).LengthSQ()>eSQ) && i<m_maxIteration);

	m_query->OnIKEnd();

}

}
}