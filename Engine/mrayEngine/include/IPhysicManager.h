
/********************************************************************
	created:	2009/01/09
	created:	9:1:2009   23:22
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\src\IPhysicManager.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\src
	file base:	IPhysicManager
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___IPhysicManager___
#define ___IPhysicManager___

#include "ISingleton.h"
#include "IPhysicalNode.h"
#include "PhysicalShapes.h"
#include "IPhysical3DDef.h"
#include "IPhysicalJoint.h"
#include "IPhysicMaterial.h"
#include "ICharacterController.h"
#include "PhysicalCCDesc.h"
#include "PhysicsRaycaseHit.h"
#include "ListenerContainer.h"

namespace mray{
namespace scene{
	class IMovable;
}
namespace physics{

	class IPhysicUserContactReport;
	class IPhysicalTriggerReporter;
	class PhComplexContactReporter;
	class PhTriggerReporterComplex;
	class IPhysicSceneQuery;
	class IPhysicalCloth;
	class PhysicalClothDesc;
	class PhysicalForceFieldDesc;
	class IPhysicalForceField;
	class ForceFieldShapeGroupDesc;
	class IPhysicalForceFieldShapeGroup;
	class IPhysicalForceFieldLinearKernal;
	class PhysicalForceFieldLinearKernalDesc;

	class PhysicsMaterialManager;

	class IPhysicManager;

	class IPhysicalManagerListener
	{
	public:
		virtual void OnNodeCreated(IPhysicManager* c,IPhysicalNode* node){}
		virtual void OnNodeRemoved(IPhysicManager* c,IPhysicalNode* node){}

		virtual void OnMaterialCreated(IPhysicManager* c,IPhysicMaterial* node){}
		virtual void OnMaterialRemoved(IPhysicManager* c,IPhysicMaterial* node){}

		virtual void OnJointCreated(IPhysicManager* c,IPhysicalJoint* node){}
		virtual void OnJointRemoved(IPhysicManager* c,IPhysicalJoint* node){}

		virtual void OnCharacterControllerCreated(IPhysicManager* c,ICharacterController* node){}
		virtual void OnCharacterControllerRemoved(IPhysicManager* c,ICharacterController* node){}

		virtual void OnClothCreated(IPhysicManager* c,IPhysicalCloth* node){}
		virtual void OnClothRemoved(IPhysicManager* c,IPhysicalCloth* node){}
	};

class MRAY_DLL IPhysicManager:public ListenerContainer<IPhysicalManagerListener*>
{
protected:
	PhComplexContactReporter* m_report;
	PhTriggerReporterComplex* m_triggerReport;
	PhysicsMaterialManager* m_materialManager;

	DECLARE_FIRE_METHOD(OnNodeCreated,(IPhysicManager* c,IPhysicalNode* node),(c,node))
	DECLARE_FIRE_METHOD(OnNodeRemoved,(IPhysicManager* c,IPhysicalNode* node),(c,node))
	DECLARE_FIRE_METHOD(OnMaterialCreated,(IPhysicManager* c,IPhysicMaterial* node),(c,node))
	DECLARE_FIRE_METHOD(OnMaterialRemoved,(IPhysicManager* c,IPhysicMaterial* node),(c,node))
	DECLARE_FIRE_METHOD(OnJointCreated,(IPhysicManager* c,IPhysicalJoint* node),(c,node))
	DECLARE_FIRE_METHOD(OnJointRemoved,(IPhysicManager* c,IPhysicalJoint* node),(c,node))
	DECLARE_FIRE_METHOD(OnCharacterControllerCreated,(IPhysicManager* c,ICharacterController* node),(c,node))
	DECLARE_FIRE_METHOD(OnCharacterControllerRemoved,(IPhysicManager* c,ICharacterController* node),(c,node))
	DECLARE_FIRE_METHOD(OnClothCreated,(IPhysicManager* c,IPhysicalCloth* node),(c,node))
	DECLARE_FIRE_METHOD(OnClothRemoved,(IPhysicManager* c,IPhysicalCloth* node),(c,node))
public:

	IPhysicManager();
	virtual~IPhysicManager();

	PhysicsMaterialManager* GetMaterialManager()
	{
		return m_materialManager;
	}

	virtual void ConnectToRemoteDebugger(const core::string& ip=mT("localhost"))=0;
	virtual void DisconnectFromRemoteDebugger()=0;
	virtual bool IsConnectedWithRemoteDebugger()=0;

	virtual void SetGravity(const math::vector3d& g)=0;
	virtual math::vector3d GetGravity()=0;

	virtual void update(float dt)=0;

	//asynchronous updating: begin update
	virtual void beginUpdate(float dt)=0;
	//asynchronous updating: end update
	virtual void endUpdate()=0;
	virtual void renderDebug()=0;

	virtual const std::list<IPhysicalNode*>& GetNodes()=0;

	
	virtual void clearAll()=0;
	virtual void clearNodes()=0;
	virtual void clearJoints()=0;
	virtual void clearMaterials()=0;
	virtual void clearControllers()=0;
	virtual void clearClothes()=0;
	virtual void clearForceFields()=0;
	virtual void clearForceFieldsShapeGroups()=0;
	virtual void clearForceFieldLinearKernals()=0;

	virtual void removeNode(IPhysicalNode*node)=0;
	virtual void removeJoint(IPhysicalJoint*joint)=0;
	virtual void removeMaterial(IPhysicMaterial*m)=0;
	virtual void removeController(ICharacterController*c)=0;
	virtual void removeCloth(IPhysicalCloth*c)=0;
	virtual void removeForceField(IPhysicalForceField*f)=0;
	virtual void removeForceFieldShapeGroup(IPhysicalForceFieldShapeGroup*f)=0;
	virtual void removeForceFieldLinearKernal(IPhysicalForceFieldLinearKernal*f)=0;

	virtual bool isShapeSupported(ESupportedShapes s)=0;
	virtual bool isJointSupported(ESupported3DJoints s)=0;

	virtual IPhysicalNode* createNode(PhysicalNodeDesc*desc)=0;
	virtual IPhysicalJoint* createJoint(IPhysicalJoint3DDesc*desc)=0;
	virtual IPhysicMaterial* createMaterial(PhysicMaterialDesc*desc)=0;
	virtual ICharacterController* createCharacterController(IPhysicsCCDesc*desc)=0;

	virtual IPhysicalCloth* createCloth(PhysicalClothDesc* desc)=0;

	virtual IPhysicalForceField* createForceField(PhysicalForceFieldDesc* desc)=0;
	virtual IPhysicalForceFieldShapeGroup* createForceFieldShapeGroup(ForceFieldShapeGroupDesc* desc)=0;
	virtual IPhysicalForceFieldLinearKernal* createForceFieldLinearKernal(PhysicalForceFieldLinearKernalDesc* desc)=0;

	virtual IPhysicMaterial* GetMaterial(MaterialID id)=0;


	virtual void registerBenchmark()=0;
	virtual void unregisterBenchmark()=0;

	virtual void SetActorPairFlag(IPhysicalNode*n1,IPhysicalNode*n2,EContactReportFlag flag,bool enable)=0;
	virtual bool GetActorPairFlag(IPhysicalNode*n1,IPhysicalNode*n2,EContactReportFlag flagx)=0;

	virtual void SetShapePairFlag(IPhysicalShape*n1,IPhysicalShape*n2,EContactReportFlag flag,bool enable)=0;
	virtual bool GetShapePairFlag(IPhysicalShape*n1,IPhysicalShape*n2,EContactReportFlag flagx)=0;


	virtual void SetActorGroupPairFlag(GroupID g1,GroupID g2,EContactReportFlag flag,bool enable)=0;
	virtual bool GetActorGroupPairFlag(GroupID g1,GroupID g2,EContactReportFlag flagx)=0;

	virtual void SetGroupCollisionFlag(GroupID g1,GroupID g2,bool enable)=0;
	virtual bool GetGroupCollisionFlag(GroupID g1,GroupID g2)=0;

	virtual IPhysicSceneQuery* CreateSceneQuery(const PhysicSceneQueryDesc& desc)=0;

	void AddContactReport(IPhysicUserContactReport*r);
	void RemoveContactReport(IPhysicUserContactReport*r);
	IPhysicUserContactReport* GetContactReport();

	void AddTriggerReport(IPhysicalTriggerReporter*r);
	void RemovTriggerReport(IPhysicalTriggerReporter*r);
	IPhysicalTriggerReporter* GetTriggerReport();

	virtual IPhysicalShape* RaycastClosestShape(const math::Ray3d& ray,EShapeType shapeType,PhysicsRaycaseHit&hit,float maxDistance=math::Infinity,uint groupID=-1,const CollisionMask* collisionMask=0)= 0;
};

}
}


#endif //___IPhysicManager___

