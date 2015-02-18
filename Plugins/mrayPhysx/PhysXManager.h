
#ifndef ___PhysXManager___
#define ___PhysXManager___

#include <IPhysicManager.h>
#include <IPhysicMaterial.h>
#include <map>
#include "CPhysXMaterial.h"
#include "IPhysXJoint.h"
#include "PhysXCharController.h"
#include "IPhysicalCloth.h"
#include "CPhysXNode.h"
#include "compileConfig.h"

class NxPhysicsSDK;
class NxCookingInterface;
class NxScene;
enum NxSDKCreateError;
class NxActor;
class NxControllerManager;
class NxController;
class NxJoint;
class NxMaterial;
class NxShapeDesc;

class NxDebugRenderable;
class NxUserAllocator;

class NxBoxShapeDesc;
class NxSphereShapeDesc;
class NxPlaneShapeDesc;
class NxHeightFieldShapeDesc;
class NxCapsuleShapeDesc;
class NxConvexShapeDesc;
class NxWheelShapeDesc;
class NxTriangleMeshShapeDesc;
class NxCloth;

class NxClothDesc;

namespace mray
{
	class BenchmarkItem;

namespace physics{

	class PhysXDebugRenderer;
	class IPhysicUserContactReport;
	class PhysXContactReporter;
	class PhysXTriggerReporter;
	class PhysXForceField;
	class PhysXForceFieldShapeGroup;
	class PhysXForceFieldLinearKernal;

	class ErrorStream;

class MRAY_PHYSX_DLL PhysXManager:public IPhysicManager
{
protected:
	static uint s_physXRefCount;
	static NxPhysicsSDK*	m_PhysicsSDK;
	static NxCookingInterface *m_Cooking;
	static NxControllerManager* m_charControllerManager;
	static NxUserAllocator* m_userAllocator;
	static ErrorStream* m_errorStream;


	NxScene*        m_Scene;

	bool			m_enableDebugRender;
	PhysXDebugRenderer* m_renderer;

	typedef std::map<NxActor*,GCPtr<CPhysXNode>> NodesMap;
	typedef std::map<NxController*,GCPtr<PhysXCharController>> ControllerMap;
	typedef std::map<NxJoint*,GCPtr<IPhysicalJoint>> JointsMap;
	typedef std::map<MaterialID,GCPtr<CPhysXMaterial>> MaterialsMap;
	typedef std::map<NxCloth*,GCPtr<IPhysicalCloth>> ClothsMap;
	typedef std::list<PhysXForceField*> ForceFieldList;
	typedef std::list<PhysXForceFieldShapeGroup*> ForceFieldShapeGroupList;
	typedef std::list<PhysXForceFieldLinearKernal*> ForceFieldLinearKernalList;

	std::list<NxActor*> m_requestToRemoveNodes;

	std::list<IPhysicalNode*> m_nodesList;
	NodesMap m_nodes;
	ControllerMap m_controllers;
	JointsMap m_joints;
	MaterialsMap m_materials;
	ClothsMap m_cloths;
	ForceFieldList m_forceFields;
	ForceFieldShapeGroupList m_forceFieldsShapeGroup;
	ForceFieldLinearKernalList m_forceFieldsLinearKernals;

	BenchmarkItem* m_benchmarkItem;
	BenchmarkItem* m_simulateBI;



	PhysXContactReporter* m_physXReport;
	PhysXTriggerReporter* m_physXTriggerReport;

	const mchar* getNxSDKCreateError(const NxSDKCreateError& errorCode) ;
	bool initNX();
	void releaseNX();

	bool _CreateScene(PhysicsSystemDesc* d);
	void _DestroyScene();


	void getPhysicsResults();

	void _performRemoveNodes();

	//////////////////////////////////////////////////////////////////////////

	NxJoint* createDistanceJoint(IPhysicalJointDistanceDesc*d);
	NxJoint* createSphericalJoint(IPhysicalJointSphericalDesc*d);
	NxJoint* createCylindricalJoint(IPhysicalJointCylinderDesc*d);
	NxJoint* createRevoluteJoint(IPhysicalJointRevoluteDesc*d);
	NxJoint* createPulleyJoint(IPhysicalJointPulleyDesc*d);
	NxJoint* createPrismaticJoint(IPhysicalJointPrismaticDesc*d);
	NxJoint* createPointOnLineJoint(IPhysicalJointPointOnLineDesc*d);
	NxJoint* createPointOnPlaneJoint(IPhysicalJointPointOnPlaneDesc*d);
	NxJoint* createFixedJoint(IPhysicalJointFixedDesc*d);
	NxJoint* createD6Joint(IPhysicalJointD6Desc*d);
public:
	PhysXManager(PhysicsSystemDesc* desc);
	virtual~PhysXManager();

	NxPhysicsSDK*	GetSDK(){return m_PhysicsSDK;}
	NxCookingInterface *GetCooking(){return m_Cooking;}
	NxControllerManager* GetCharacterController(){return m_charControllerManager;}
	NxScene* GetScene(){return m_Scene;}

	void ConnectToRemoteDebugger(const core::string& ip=mT("localhost"));
	void DisconnectFromRemoteDebugger();
	bool IsConnectedWithRemoteDebugger();

	virtual void SetGravity(const math::vector3d& g);
	virtual math::vector3d GetGravity();

	void clearAll();
	void clearNodes();
	void clearJoints();
	void clearMaterials();
	void clearClothes();
	void clearControllers();
	void clearForceFields();
	void clearForceFieldsShapeGroups();
	void clearForceFieldLinearKernals();

	void removeNode(IPhysicalNode*node);
	void removeJoint(IPhysicalJoint*joint);
	void removeMaterial(IPhysicMaterial*m);
	void removeController(ICharacterController*c);
	void removeCloth(IPhysicalCloth*c);
	void removeForceField(IPhysicalForceField*f);
	void removeForceFieldShapeGroup(IPhysicalForceFieldShapeGroup*f);
	void removeForceFieldLinearKernal(IPhysicalForceFieldLinearKernal*f);

	NxScene*getPhysXScene();
	NxPhysicsSDK*getPhysXSDK();

	const std::list<IPhysicalNode*>& GetNodes(){ return m_nodesList;}

	const NxDebugRenderable *getDebugRenderable();

	CPhysXNode* getNodeX(NxActor*a);
	PhysXCharController* getControllerX(NxController*c);
	CPhysXMaterial* getMaterialX(NxMaterial*c);

	void update(float dt);
	void beginUpdate(float dt);
	void endUpdate();

	void renderDebug();

	bool isShapeSupported(ESupportedShapes s);
	bool isJointSupported(ESupported3DJoints s);


	//GCPtr<IPhysicalNode> createNode(GCPtr<scene::IMovable> node,IPhysicalShapeDesc*desc);
	IPhysicalNode* createNode(PhysicalNodeDesc*desc);
	IPhysicalJoint* createJoint(IPhysicalJoint3DDesc*desc);
	IPhysicMaterial* createMaterial(PhysicMaterialDesc*desc);

	IPhysicalForceField* createForceField(PhysicalForceFieldDesc* desc);
	IPhysicalForceFieldShapeGroup* createForceFieldShapeGroup(ForceFieldShapeGroupDesc* desc);
	IPhysicalForceFieldLinearKernal* createForceFieldLinearKernal(PhysicalForceFieldLinearKernalDesc* desc);

	ICharacterController* createCharacterController(IPhysicsCCDesc*desc);

	IPhysicalCloth* createCloth(PhysicalClothDesc* desc);

	IPhysicMaterial* GetMaterial(MaterialID id);

	void enableDebugRender(bool e);
	bool isEnableDebugRender();


	void registerBenchmark();
	void unregisterBenchmark();


	void SetActorPairFlag(IPhysicalNode*n1,IPhysicalNode*n2,EContactReportFlag flag,bool enable);
	bool GetActorPairFlag(IPhysicalNode*n1,IPhysicalNode*n2,EContactReportFlag flagx);

	void SetShapePairFlag(IPhysicalShape*n1,IPhysicalShape*n2,EContactReportFlag flag,bool enable);
	bool GetShapePairFlag(IPhysicalShape*n1,IPhysicalShape*n2,EContactReportFlag flagx);


	void SetActorGroupPairFlag(GroupID g1,GroupID g2,EContactReportFlag flag,bool enable);
	bool GetActorGroupPairFlag(GroupID g1,GroupID g2,EContactReportFlag flagx);

	void SetGroupCollisionFlag(GroupID g1,GroupID g2,bool enable);
	bool GetGroupCollisionFlag(GroupID g1,GroupID g2);

	IPhysicSceneQuery* CreateSceneQuery(const PhysicSceneQueryDesc& desc);

	IPhysicalShape* RaycastClosestShape(const math::Ray3d& ray,EShapeType shapeType,PhysicsRaycaseHit&hit,float maxDistance=math::Infinity,uint groupID=-1,const CollisionMask* collisionMask=0);
	/*
	GCPtr<CPhysXSceneNode> addNode(GCPtr<scene::ISceneNode> node,NxActor *actor);

	void updatePhysics(float dt);
	*/

};


}
}

#endif
