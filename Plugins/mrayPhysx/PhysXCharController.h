
/********************************************************************
	created:	2009/07/14
	created:	14:7:2009   0:25
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayPhysx\mrayPhysx\PhysXCharController.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayPhysx\mrayPhysx
	file base:	PhysXCharController
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___PhysXCharController___
#define ___PhysXCharController___

class NxController;

#include "ICharacterController.h"
#include "compileConfig.h"

namespace mray{
namespace physics{

	class PhysXCharController;

	class ControllerCallBack;


class MRAY_PHYSX_DLL PhysXCharController:public ICharacterController
{
private:
protected:
	GCPtr<IPhysicalNode> m_actor;
	NxController *m_controller;
	IPhysicManager* m_creator;

	ICCTypeInterface* m_interface;

	PhysicsCCCallBack* m_callback;

	ControllerCallBack* m_xcallback;

	math::vector3d m_movingDir;
	uint m_collisionFlags;

public:
	PhysXCharController(NxController *controller,IPhysicManager* creator);
	virtual~PhysXCharController();

	NxController * getController(){return m_controller;}
	IPhysicManager* GetCreator(){return m_creator;}


	virtual void setMovingDir(const math::vector3d &disp);
	virtual math::vector3d getMovingDir();
	virtual void setCollisionFlags(uint flags);	//check ECCCollisionFlags
	virtual uint getCollisionFlags();	//check ECCCollisionFlags

	virtual void setPosition(const math::vector3d &pos);

	virtual math::vector3d getPosition();

	virtual math::vector3d getFilteredPosition();

	virtual GCPtr<IPhysicalNode> getActor();

	virtual void setStepOffset(float offset);

	virtual void setCollision(bool e);

	virtual void setInteraction(ECCInteractionFlag e);

	virtual ECCInteractionFlag getInteraction();

	virtual ECCType getType();

	virtual void update(float dt);

	
	virtual ICCTypeInterface* getTypeInterface(){return m_interface;}

	virtual PhysicsCCCallBack* getCallback(){return m_callback;}
	virtual void setCallback(PhysicsCCCallBack*c){m_callback=c;}

};

}
}


#endif //___PhysXCharController___
