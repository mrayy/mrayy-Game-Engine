

/********************************************************************
	created:	2013/07/13
	created:	13:7:2013   17:28
	filename: 	C:\Development\mrayEngine\Projects\PIDController\ControllerComponent.h
	file path:	C:\Development\mrayEngine\Projects\PIDController
	file base:	ControllerComponent
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __ControllerComponent__
#define __ControllerComponent__


#include "IGameComponent.h"

namespace mray
{
namespace game
{
	class PhysicsComponent;

class ControllerComponent:public IGameComponent
{
	DECLARE_RTTI;
protected:
	PhysicsComponent* m_phComp;
public:
	ControllerComponent(GameEntityManager*);
	virtual~ControllerComponent();

	virtual bool InitComponent();

	virtual void Update(float dt);

	void SetCurrent(float v);
	float GetPosition();
	float GetForce();
};

DECLARE_GAMECOMPONENT_FACTORY(ControllerComponent);

}
}


#endif