#include "stdafx.h"


#include "IPOAirFan.h"


namespace mray{
namespace physics{

IPOAirFan::IPOAirFan(float f,math::vector3d pos,float iR,float oR)
/*
Position(EPT_VECTOR3Df,mT("position")),
Force(EPT_FLOAT,mT("force"))*/
{
	m_position=pos;
	m_force=f;
	m_iRadius=iR;
	m_iRadiusSQ=m_iRadius*m_iRadius;
	m_oRadius=oR;
	m_oRadiusSQ=m_oRadius*m_oRadius;
}
IPOAirFan::~IPOAirFan(){

}

void IPOAirFan::affect(IPhysicObject*o,float dt){
	int cnt=o->getVertexCount();
	PhysicalVertex*vert=o->getVerticies();
	for (int i=0;i<cnt;++i)
	{
		math::vector3d v=vert[i].pos-m_position;
		float d=v.LengthSQ();
		if(d<=m_oRadiusSQ){
			d=sqrt(d);
			v/=d;
			if(d>m_iRadius){
				float f=(m_oRadius-d)/(m_oRadius-m_iRadius)*m_force;
				vert[i].force+=v*f;
			}else
			{
				vert[i].force+=v*m_force;
			}
		}
	}
}

}
}
