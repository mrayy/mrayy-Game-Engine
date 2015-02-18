
/********************************************************************
	created:	2008/10/26
	created:	26:10:2008   20:55
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayCEGUItest\mrayCEGUI\IPOAirFan.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayCEGUItest\mrayCEGUI
	file base:	IPOAirFan
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef ___IPOAirFan___
#define ___IPOAirFan___

#include "IPhysicObjectAffector.h"
#include "IPhysicObject.h"
#include <point3d.h>
//#include <cpropertie.h>

namespace mray{
namespace physics{

class MRAY_DLL IPOAirFan:public IPhysicObjectAffector
{
	float m_iRadiusSQ;
	float m_oRadiusSQ;
	float m_iRadius;
	float m_oRadius;

	float m_force;

	math::vector3d m_position;

public:
/*
	rwProperty<IPOAirFan,math::vector3d> Position;
	rwProperty<IPOAirFan,float> Force;*/

	IPOAirFan(float force,math::vector3d pos,float iR,float oR);
	virtual~IPOAirFan();

	const math::vector3d& getPosition() { return m_position; }
	void setPosition(const math::vector3d &val) { m_position = val; }

	const float& getForce() { return m_force; }
	void setForce(const float &val) { m_force = val; }

	virtual void affect(IPhysicObject*o,float dt);
};

}
}


#endif //___IPOAirFan___

