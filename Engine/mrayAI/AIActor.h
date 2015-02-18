
/********************************************************************
	created:	2009/01/31
	created:	31:1:2009   21:47
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\AIActor.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	AIActor
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___AIActor___
#define ___AIActor___

#include <point3d.h>
#include <quaternion.h>
#include <list>
#include <IObjectComponent.h>
#include "AgentObstacle.h"
#include "AITypeDef.h"
#include "IAIPhysics.h"
#include "IVisualizable.h"

namespace mray{
namespace AI{

	class AISystem;
	class INavWorld;
	class IAIArchitect;
	class INavWorldNode;
	class IRenderDevice;


class MRAY_AI_DLL AIActor:public IObjectComponent,public AIMessageReciver,public IVisualizable
{
	DECLARE_RTTI
public:
	AIActor(){}
	virtual~AIActor(){}

	//Actor ID set by the AISystem
	virtual bool SetID(uint id)=0;
	virtual uint GetID()=0;

	virtual void SetAISystem(AISystem*s)=0;
	virtual AISystem* GetAISystem()=0;
	virtual INavWorld* GetNavWorld()=0;

	virtual AgentObstacle* GetObstacle()=0;


	virtual math::vector3d GetPosition()const{return GetPhysicalProp()->GetGlobalPosition();}
	virtual const math::vector3d& GetUp()const=0;
	virtual const math::vector3d& GetHeading()const=0;

	virtual void AttachController(IAIArchitect*controller)=0;
	virtual IAIArchitect* GetController()=0;

	virtual IAIPhysics* GetPhysicalProp()const=0;

	virtual bool IsAIControlled()=0;	//is this agent under AI control or not

	virtual void Update(float dt)=0;	

	virtual INavWorldNode* GetWorldNode()=0;//return the world node that's this agent is in it
	virtual void UpdateWorldNode()=0;

	virtual void EnableVisualize(bool e)=0;
	virtual bool IsEnableVisualize()=0;

	virtual void Visualize(IRenderDevice*dev)=0;

};

typedef std::list<AIActor*> AIActorList;

}
}


#endif //___AIActor___
