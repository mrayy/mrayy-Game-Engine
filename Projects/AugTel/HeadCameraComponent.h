


/********************************************************************
	created:	2014/01/16
	created:	16:1:2014   22:10
	filename: 	C:\Development\mrayEngine\Projects\AugTel\HeadCameraComponent.h
	file path:	C:\Development\mrayEngine\Projects\AugTel
	file base:	HeadCameraComponent
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __HeadCameraComponent__
#define __HeadCameraComponent__


#include "IGameComponent.h"

namespace mray
{
	namespace VT
	{
		class CoupledJointComponent;
	}
namespace AugTel
{

class HeadCameraComponent:public game::IGameComponent
{
	DECLARE_RTTI;
protected:

	math::vector3d m_offset;
	math::vector3d m_angles;
	float m_distance;

	math::quaternion m_orientation;

	core::string m_targetNode;
	VT::CoupledJointComponent* m_headAxis[3];
	scene::IMovable* m_node;

public:
	DECLARE_PROPERTY_TYPE(TargetNode, core::string, );
	DECLARE_PROPERTY_TYPE(Offset, math::vector3d, );
	DECLARE_PROPERTY_TYPE(Angles, math::vector3d, );
public:
	HeadCameraComponent(game::GameEntityManager*m);
	virtual~HeadCameraComponent();

	virtual bool InitComponent();

	bool SetNodeName(const core::string &name);
	const core::string& GetNodeName();

	bool SetOffset(const math::vector3d &v);
	const math::vector3d& GetOffset();

	bool SetAngles(const math::vector3d &v);
	const math::vector3d& GetAngles();

	bool SetDistance(float v);
	float GetDistance();
	VT::CoupledJointComponent** GetHeadControl(){ return m_headAxis; }

	void SetControlValue(const math::vector3d& r);

	scene::IMovable* GetNode(){ return m_node; }

	math::vector3d GetAbsolutePosition();
	math::quaternion GetAbsoluteOrientation();

};
DECLARE_GAMECOMPONENT_FACTORY(HeadCameraComponent);

}
}



#endif