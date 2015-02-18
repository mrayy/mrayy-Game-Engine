
/********************************************************************
	created:	2009/07/13
	created:	13:7:2009   23:58
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\ICharacterController.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	ICharacterController
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___ICharacterController___
#define ___ICharacterController___


#include "IPhysicalNode.h"

#include "GCPtr.h"
#include "ICCTypeInterface.h"

namespace mray{
namespace physics{

	enum ECCCollisionFlags
	{
		ECCMF_COLLISION_SIDES	= (1<<0),	//!< Character is colliding to the sides.
		ECCMF_COLLISION_UP		= (1<<1),	//!< Character has collision above.
		ECCMF_COLLISION_DOWN	= (1<<2)	//!< Character has collision below.
	};

class ICharacterController
{
private:
protected:
public:
	ICharacterController(){}
	virtual~ICharacterController(){}

	virtual IPhysicManager* GetCreator()=0;

	virtual void setPosition(const math::vector3d &pos)=0;
	virtual math::vector3d getPosition()=0;

	//virtual void move(const math::vector3d &disp,uint activeGroups,float minDist,float sharpness=1)=0;


	virtual void setMovingDir(const math::vector3d &disp)=0;
	virtual math::vector3d getMovingDir()=0;
	virtual void setCollisionFlags(uint flags)=0;	//check ECCCollisionFlags
	virtual uint getCollisionFlags()=0;	//check ECCCollisionFlags

	virtual math::vector3d getFilteredPosition()=0;

	virtual GCPtr<IPhysicalNode> getActor()=0;

	virtual void setStepOffset(float offset)=0;

	virtual void setCollision(bool e)=0;

	virtual void setInteraction(ECCInteractionFlag e)=0;

	virtual ECCInteractionFlag getInteraction()=0;

	virtual ECCType getType()=0;

	virtual ICCTypeInterface* getTypeInterface()=0;

	virtual PhysicsCCCallBack* getCallback()=0;

	virtual void setCallback(PhysicsCCCallBack*c)=0;

	virtual void update(float dt)=0;
};
MakeSharedPtrType(ICharacterController);

}
}


#endif //___ICharacterController___
