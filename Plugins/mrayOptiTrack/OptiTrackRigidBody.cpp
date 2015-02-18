

#include "stdafx.h"
#include "OptiTrackRigidBody.h"



namespace mray
{
namespace animation
{


void OptiTrackRigidBody::SetPosition(const math::vector3d& pos)
{
	if(m_posSamples.size()<5)
		m_posSamples.push_back(pos);
	else
	{
		for(int i=0;i<m_posSamples.size()-1;++i)
		{
			m_posSamples[i]=m_posSamples[i+1];
		}
		m_posSamples[m_posSamples.size()-1]=pos;
	}
	m_position=m_posSamples[0];
	for(int i=1;i<m_posSamples.size();++i)
	{
		m_position+=m_posSamples[i];
	}
	m_position/=(float)m_posSamples.size();
}

void OptiTrackRigidBody::SetOrintation(const math::quaternion& o)
{
	if(m_oriSamples.size()<10)
		m_oriSamples.push_back(o);
	else
	{
		for(int i=0;i<m_oriSamples.size()-1;++i)
		{
			m_oriSamples[i]=m_oriSamples[i+1];
		}
		m_oriSamples[m_oriSamples.size()-1]=o;
	}
	m_orintation=m_oriSamples[0];
	for(int i=1;i<m_oriSamples.size();++i)
	{
		m_orintation+=m_oriSamples[i];
	}
	m_orintation.Normalize();
}
	

}
}
