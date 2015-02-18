

/********************************************************************
	created:	2013/05/13
	created:	13:5:2013   19:53
	filename: 	C:\Development\mrayEngine\Projects\VirtualTelesar\VTLib\KinBodyControlComponent.h
	file path:	C:\Development\mrayEngine\Projects\VirtualTelesar\VTLib
	file base:	KinBodyControlComponent
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __KinBodyControlComponent__
#define __KinBodyControlComponent__


#include "IVTComponent.h"
#include "IControllableComponent.h"




namespace mray
{
namespace game
{
	class PhysicsComponent;
}
namespace VT
{

class KinBodyControlComponent:public IVTComponent, public IControllableComponent
{
	DECLARE_RTTI;

protected:
	core::string m_targetBody;
	game::PhysicsComponent* m_attachedBody;
	std::vector<ControlInputValues> m_targetValue;

	math::vector3d m_initalPos;
	math::quaternion m_initialRot;
public:
	DECLARE_PROPERTY_TYPE(TargetNode,core::string,);

public:
	KinBodyControlComponent(game::GameEntityManager*mngr);
	virtual~KinBodyControlComponent();

	const core::string& GetControllableName(){return GetName();}

	virtual bool SetTargetName(const core::string&n){m_targetBody=n;return true;}
	const core::string& GetTargetName(){return m_targetBody;}


	virtual bool InitComponent();

	virtual bool SetEnabled(bool enabled);

	virtual bool SetControlValue(const std::vector<ControlInputValues>&  v,EControlSource src);
	//virtual const std::vector<ControlInputValues>&  GetControlValue(){return m_targetValue;}

	virtual void Update(float dt);

	virtual std::vector<float> GetCurrentValue();
};
DECLARE_GAMECOMPONENT_FACTORY(KinBodyControlComponent);

}
}


#endif
