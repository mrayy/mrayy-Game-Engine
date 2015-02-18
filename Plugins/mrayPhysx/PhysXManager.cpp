#include "stdafx.h"


#include "PhysXDebugRenderer.h"

#include "PhysXManager.h"
#include "CPhysXNode.h"
#include "CPhysXMaterial.h"
#include "PhysXCharController.h"
#include "Stream.h"
#include "PhysXCommon.h"

#include "PhysXRevoluteJoint.h"
#include "PhysXDOF6Joint.h"
#include "PhysXFixedJoint.h"
#include "PhysXPointOnPlaneJoint.h"
#include "PhysXCylinderJoint.h"
#include "PhysXDistanceJoint.h"
#include "PhysXPointOnLineJoint.h"
#include "PhysXPrismaticJoint.h"
#include "PhysXPulleyJoint.h"
#include "PhysXSphericalJoint3D.h"
#include "CPhysXMaterial.h"
#include "CPhysXCloth.h"
 
#include "UserAllocator.h"
#include "IPhysicContactStreamIterator.h"
#include "PhysXShape.h"
#include "PhysXSceneQueryReport.h"
#include "PhysXSceneQuery.h"
#include "PhysXObjectConverter.h"

#include "PhysicsLoggerSystem.h"

#include "PhysXForceField.h"
#include "PhysXForceFieldShapeGroup.h"
#include "PhysXForceFieldLinearKernal.h"
#include "IPhysicalTriggerReporter.h"
#include "PhysXDebugRenderer.h"
#include "PhysicsMaterialManager.h"


namespace mray{
namespace physics{

//uncomment this line to allow using VRD
//#ifdef _DEBUG
	#define PHYSX_USE_VRD
//#endif

#define PHYSX_VRD_HOST "localhost"
	//Change this setting to the port on which the VRD is listening, or keep the default: NX_DBG_DEFAULT_PORT
#define PHYSX_VRD_PORT NX_DBG_DEFAULT_PORT
	//Change this setting to decide what type of information is sent to the VRD. Default: NX_DBG_EVENTMASK_EVERYTHING
#define PHYSX_VRD_EVENTMASK NX_DBG_EVENTMASK_EVERYTHING


class ErrorStream : public NxUserOutputStream
{
public:
	void reportError(NxErrorCode e, const char* message, const char* file, int line)
	{
		core::string msg;
		ELogLevel logLevel=ELL_WARNING;
		//printf("%s (%d) :", file, line);
		switch (e)
		{
		case NXE_INVALID_PARAMETER:
			msg= mT("invalid parameter :");
			break;
		case NXE_INVALID_OPERATION:
			msg= mT("invalid operation :");
			break;
		case NXE_OUT_OF_MEMORY:
			msg= mT("out of memory :");
			break;
		case NXE_DB_INFO:
			msg= mT("info :");
			logLevel=ELL_INFO;
			break;
		case NXE_DB_WARNING:
			msg= mT("warning :");
			break;
		default:
			msg=mT("");
		}
		msg+=core::StringConverter::toString(message);
		
		gPhysicsLoggerSystem.log(msg,logLevel);
	}

	NxAssertResponse reportAssertViolation(const char* message, const char* file, int line)
	{
		core::string emsg;
		core::char_to_string(message,emsg);
		core::stringc lstr=core::StringConverter::toString(line);
		core::CException e(file,lstr.c_str(),emsg.c_str());
		e.runError();
		//gLogManager.log(msg,message,ELL_ERROR);
/*
		switch (MessageBoxA(0, message, "AssertViolation, see console for details.", MB_ABORTRETRYIGNORE))
		{
		case IDRETRY:
			return NX_AR_CONTINUE;
		case IDIGNORE:
			return NX_AR_IGNORE;
		case IDABORT:
		default:
			return NX_AR_BREAKPOINT;
		}*/
		return NX_AR_BREAKPOINT;
	}

	void print(const char* message)
	{
		core::string emsg=core::StringConverter::toString(message);
		gPhysicsLoggerSystem.log(emsg,ELL_INFO);
	}
};

class PhysXContactStreamIterator:public IPhysicContactStreamIterator
{
protected:
	NxContactStreamIterator*m_csi;

public:

	PhysXContactStreamIterator(NxContactStreamIterator* csi):m_csi(csi)
	{
		while(m_csi->goNextPair())
		{
			PairsInfo p;

			p.s1=(IPhysicalShape*)m_csi->getShape(0)->userData;
			p.s2=(IPhysicalShape*)m_csi->getShape(1)->userData;
			while(m_csi->goNextPatch())
			{
				PatchInfo patch;
				patch.normal=ToVec3(m_csi->getPatchNormal());
				while(m_csi->goNextPoint())
				{
					PointInfo v;
					v.point=ToVec3(m_csi->getPoint());
					v.force=m_csi->getPointNormalForce();
					v.separation=m_csi->getSeparation();
					patch.points.push_back(v);
				}
				p.patches.push_back(patch);
			}
			m_pairs.push_back(p);
		}
	}


};

class PhysXTriggerReporter:public NxUserTriggerReport
{
public:
	PhysXManager* m_mngr;
	PhysXTriggerReporter(PhysXManager* mngr)
	{
		m_mngr=mngr;
	}
	virtual void onTrigger(NxShape& triggerShape, NxShape& otherShape, NxTriggerFlag status) 
	{
		IPhysicalShape* ts=(IPhysicalShape*)triggerShape.userData;
		IPhysicalShape* os=(IPhysicalShape*)otherShape.userData;

		ETriggerStatus st=PhysXObjectConverter::ConvertTriggerFlag(status);

		m_mngr->GetTriggerReport()->OnTrigger(ts,os,st);
	}

};

class PhysXContactReporter:public NxUserContactReport
{
public:
	PhysXManager* m_mngr;
	PhysXContactReporter(PhysXManager* mngr)
	{
		m_mngr=mngr;
	}
	virtual~PhysXContactReporter(){}
	
	virtual void  onContactNotify(NxContactPair& pair, NxU32 events)
	{
		PhysicContactPair cPair;
		NxContactStreamIterator nxIt(pair.stream);
		PhysXContactStreamIterator csit(&nxIt);
		cPair.nodes[0]=(IPhysicalNode*)pair.actors[0]->userData;
		cPair.nodes[1]=(IPhysicalNode*)pair.actors[1]->userData;
		cPair.contactStream=&csit;
		cPair.sumFrictionForce=ToVec3(pair.sumFrictionForce);
		cPair.sumNormalForce=ToVec3(pair.sumNormalForce);

#define CALL_CONTACT_REPORT(flag,method)\
		if(events & flag)\
		{\
			if(m_mngr->GetContactReport())\
				m_mngr->GetContactReport()->method(&cPair);\
			if(cPair.nodes[0]->GetContactReport() && cPair.nodes[0]->GetContactReport()!=m_mngr->GetContactReport() )\
				cPair.nodes[0]->GetContactReport()->method(&cPair);\
			if(cPair.nodes[1]->GetContactReport() && cPair.nodes[1]->GetContactReport()!=m_mngr->GetContactReport() )\
				cPair.nodes[1]->GetContactReport()->method(&cPair);\
		}

		CALL_CONTACT_REPORT(NX_NOTIFY_ON_START_TOUCH,OnPairStartTouch);
		CALL_CONTACT_REPORT(NX_NOTIFY_ON_TOUCH,OnPairTouch);
		CALL_CONTACT_REPORT(NX_NOTIFY_ON_END_TOUCH,OnPairEndTouch);

		CALL_CONTACT_REPORT(NX_NOTIFY_ON_START_TOUCH_FORCE_THRESHOLD,OnPairStartTouchForceThreshold);
		CALL_CONTACT_REPORT(NX_NOTIFY_ON_TOUCH_FORCE_THRESHOLD,OnPairTouchForceThreshold);
		CALL_CONTACT_REPORT(NX_NOTIFY_ON_END_TOUCH_FORCE_THRESHOLD,OnPairEndTouchForceThreshold);

		CALL_CONTACT_REPORT(NX_NOTIFY_ON_IMPACT,OnPairImpact);
		CALL_CONTACT_REPORT(NX_NOTIFY_ON_ROLL,OnPairRoll);
		CALL_CONTACT_REPORT(NX_NOTIFY_ON_SLIDE,OnPairSlide);

	}
};

uint PhysXManager::s_physXRefCount=0;
NxPhysicsSDK* PhysXManager::m_PhysicsSDK=0;
NxCookingInterface* PhysXManager::m_Cooking=0;
NxControllerManager* PhysXManager::m_charControllerManager = 0;
NxUserAllocator* PhysXManager::m_userAllocator = 0;
ErrorStream* PhysXManager::m_errorStream = 0;

PhysXManager::PhysXManager(PhysicsSystemDesc* desc){

	m_benchmarkItem=new BenchmarkItem(mT("Physics"));
	m_physXReport=new PhysXContactReporter(this);
	m_physXTriggerReport=new PhysXTriggerReporter(this);
	m_renderer=new PhysXDebugRenderer();

	m_enableDebugRender=false;

	++s_physXRefCount;
	if(s_physXRefCount==1)
	{
		initNX();
	}
	_CreateScene(desc);
	m_simulateBI=m_benchmarkItem->addSubItem(mT("Simulate"));

}
PhysXManager::~PhysXManager()
{
	unregisterBenchmark();
	_DestroyScene();
	m_nodes.clear();
	--s_physXRefCount;
	if(!s_physXRefCount)
	{
		releaseNX();
	}
	delete m_physXReport;
	delete m_physXTriggerReport;

	delete m_benchmarkItem;
	delete m_renderer;
}

void PhysXManager::clearAll()
{
	clearControllers();
	clearClothes();
	clearForceFields();
	clearJoints();
	clearNodes();
	clearMaterials();
}

void PhysXManager::clearNodes(){
	NodesMap::iterator it=m_nodes.begin();
	for (;it!=m_nodes.end();++it)
	{
		CPhysXNode*phXNode=it->second;
		if(phXNode->getActor())
			m_Scene->releaseActor(*phXNode->getActor());
	}
	m_nodes.clear();
	m_nodesList.clear();
}
void PhysXManager::clearJoints(){
	JointsMap::iterator it=m_joints.begin();
	for (;it!=m_joints.end();++it)
	{
		m_Scene->releaseJoint(*it->first);
	}
	m_joints.clear();
}
void PhysXManager::clearMaterials(){
	MaterialsMap::iterator it=m_materials.begin();
	for (;it!=m_materials.end();++it)
	{

		NxMaterial*m=m_Scene->getMaterialFromIndex(it->first);
		if(m)
			m_Scene->releaseMaterial(*m);
	}
	m_materials.clear();
}

void PhysXManager::clearClothes()
{
	ClothsMap::iterator it=m_cloths.begin();
	for (;it!=m_cloths.end();++it)
	{
		m_Scene->releaseCloth(*it->first);
	}
	m_cloths.clear();
}
void PhysXManager::clearControllers()
{

	ControllerMap::iterator it=m_controllers.begin();
	for (;it!=m_controllers.end();++it)
	{
		m_charControllerManager->releaseController(*it->first);
	}
	m_controllers.clear();
}

void PhysXManager::clearForceFields()
{
 
	ForceFieldList::iterator it=m_forceFields.begin();
	for (;it!=m_forceFields.end();++it)
	{
		NxForceField* f=(*it)->getNxForceField();
		if(f)
			m_Scene->releaseForceField(*f);
		delete *it;
	}
	m_forceFields.clear();
}

void PhysXManager::clearForceFieldsShapeGroups()
{

	ForceFieldShapeGroupList::iterator it=m_forceFieldsShapeGroup.begin();
	for (;it!=m_forceFieldsShapeGroup.end();++it)
	{
		NxForceFieldShapeGroup* f=(*it)->getNxShapeGroup();
		if(f)
			m_Scene->releaseForceFieldShapeGroup(*f);
		delete *it;
	}
	m_forceFieldsShapeGroup.clear();
}

void PhysXManager::clearForceFieldLinearKernals()
{

	ForceFieldLinearKernalList::iterator it=m_forceFieldsLinearKernals.begin();
	for (;it!=m_forceFieldsLinearKernals.end();++it)
	{
		NxForceFieldLinearKernel* f=(*it)->getNxKernal();
		if(f)
			m_Scene->releaseForceFieldLinearKernel(*f);
		delete *it;
	}
	m_forceFieldsLinearKernals.clear();
}


void PhysXManager::removeNode(IPhysicalNode*node){
	CPhysXNode*phXNode=dynamic_cast<CPhysXNode*>(node);
	if(!phXNode || !phXNode->getActor())
		return;
	m_nodes.erase(phXNode->getActor());
	std::list<IPhysicalNode*>::iterator it= m_nodesList.begin();
	for (;it!=m_nodesList.end();++it)
	{
		if(*it==node)
		{
			m_nodesList.erase(it);
			break;
		}
	}


	m_requestToRemoveNodes.push_back(phXNode->getActor());
	FIRE_LISTENR_METHOD(OnNodeRemoved,(this,phXNode));
}
void PhysXManager::removeJoint(IPhysicalJoint*joint){
	JointsMap::iterator it=m_joints.begin();
	for (;it!=m_joints.end();++it)
	{
		IPhysicalJoint*j=it->second;
		if(j==joint){
			m_Scene->releaseJoint(*it->first);
			m_joints.erase(it);
			break;
		}
	}
	FIRE_LISTENR_METHOD(OnJointRemoved,(this,joint));
}
void PhysXManager::removeMaterial(IPhysicMaterial*m){
	CPhysXMaterial *phXmat=dynamic_cast<CPhysXMaterial*>(m);
	if(!phXmat || !phXmat->getMaterial())
		return;
	m_Scene->releaseMaterial(*phXmat->getMaterial());
	m_materials.erase(m->getMaterialIndex());
	FIRE_LISTENR_METHOD(OnMaterialRemoved,(this,m));
}

void PhysXManager::removeCloth(IPhysicalCloth*c)
{
	CPhysXCloth *phXCloth=dynamic_cast<CPhysXCloth*>(c);
	if(!phXCloth || !phXCloth->GetCloth())
		return;
	m_Scene->releaseCloth(*phXCloth->GetCloth());
	m_cloths.erase(phXCloth->GetCloth());
	FIRE_LISTENR_METHOD(OnClothRemoved,(this,c));
}

void PhysXManager::removeController(ICharacterController*c)
{
	PhysXCharController *phXmat=dynamic_cast<PhysXCharController*>(c);
	if(!phXmat || !phXmat->getController())
		return;
	m_charControllerManager->releaseController(*phXmat->getController());
	m_controllers.erase(phXmat->getController());
	FIRE_LISTENR_METHOD(OnCharacterControllerRemoved,(this,c));
}

void PhysXManager::removeForceField(IPhysicalForceField*f)
{
	ForceFieldList::iterator it=m_forceFields.begin();
	for (;it!=m_forceFields.end();++it)
	{
		if(*it ==f)
		{
			NxForceField* f=(*it)->getNxForceField();
			if(f)
				m_Scene->releaseForceField(*f);
			delete *it;
			m_forceFields.erase(it);
			break;
		}
	}
}

void PhysXManager::removeForceFieldShapeGroup(IPhysicalForceFieldShapeGroup*f)
{
	ForceFieldShapeGroupList::iterator it=m_forceFieldsShapeGroup.begin();
	for (;it!=m_forceFieldsShapeGroup.end();++it)
	{
		if(*it ==f)
		{
			NxForceFieldShapeGroup* f=(*it)->getNxShapeGroup();
			if(f)
				m_Scene->releaseForceFieldShapeGroup(*f);
			delete *it;
			m_forceFieldsShapeGroup.erase(it);
			break;
		}
	}
}
void PhysXManager::removeForceFieldLinearKernal(IPhysicalForceFieldLinearKernal*f)
{
	ForceFieldLinearKernalList::iterator it=m_forceFieldsLinearKernals.begin();
	for (;it!=m_forceFieldsLinearKernals.end();++it)
	{
		if(*it ==f)
		{
			NxForceFieldLinearKernel* f=(*it)->getNxKernal();
			if(f)
				m_Scene->releaseForceFieldLinearKernel(*f);
			delete *it;
			m_forceFieldsLinearKernals.erase(it);
			break;
		}
	}
}

void PhysXManager::_performRemoveNodes()
{

	std::list<NxActor*>::iterator it= m_requestToRemoveNodes.begin();
	for(;it!=m_requestToRemoveNodes.end();++it)
		m_Scene->releaseActor(*(*it));
	m_requestToRemoveNodes.clear();
}

CPhysXNode* PhysXManager::getNodeX(NxActor*a){
	NodesMap::iterator it= m_nodes.find(a);
	if(it==m_nodes.end())
		return 0;
	return it->second;
}
PhysXCharController* PhysXManager::getControllerX(NxController*c){
	ControllerMap::iterator it= m_controllers.find(c);
	if(it==m_controllers.end())
		return 0;
	return it->second;
}

CPhysXMaterial* PhysXManager::getMaterialX(NxMaterial*c)
{
	if(!c)return 0;
	MaterialsMap::iterator it= m_materials.find(c->getMaterialIndex());
	if(it==m_materials.end())
		return 0;
	return it->second;
}
void PhysXManager::registerBenchmark(){
	Engine::getInstance().getRootBenchmarking()->addSubItem(m_benchmarkItem);
}
void PhysXManager::unregisterBenchmark()
{
	Engine::getInstance().getRootBenchmarking()->removeSubItem(m_benchmarkItem);
}

NxScene* PhysXManager::getPhysXScene(){
	return m_Scene;
}
NxPhysicsSDK* PhysXManager::getPhysXSDK(){
	return m_PhysicsSDK;
}

void PhysXManager::enableDebugRender(bool e){
	m_enableDebugRender=e;
	m_PhysicsSDK->setParameter(NX_VISUALIZATION_SCALE, e==true ? 1 : 0);
}
bool PhysXManager::isEnableDebugRender(){
	return m_enableDebugRender;
}

const mchar* PhysXManager::getNxSDKCreateError(const NxSDKCreateError& errorCode) 
{
	switch(errorCode) 
	{
		case NXCE_NO_ERROR: return mT("NXCE_NO_ERROR");
		case NXCE_PHYSX_NOT_FOUND: return mT("NXCE_PHYSX_NOT_FOUND");
		case NXCE_WRONG_VERSION: return mT("NXCE_WRONG_VERSION");
		case NXCE_DESCRIPTOR_INVALID: return mT("NXCE_DESCRIPTOR_INVALID");
		case NXCE_CONNECTION_ERROR: return mT("NXCE_CONNECTION_ERROR");
		case NXCE_RESET_ERROR: return mT("NXCE_RESET_ERROR");
		case NXCE_IN_USE_ERROR: return mT("NXCE_IN_USE_ERROR");
		default: return mT("Unknown error");
	}
};
bool PhysXManager::initNX(){
	traceFunction(Physics);
	NxPhysicsSDKDesc desc;
	NxSDKCreateError errorCode = NXCE_NO_ERROR;

	m_userAllocator=new UserAllocator();

	m_Scene=0;
	gLogManager.startSection(mT("PhysX"));
	m_errorStream=new ErrorStream();
	m_PhysicsSDK=NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION, NULL,m_errorStream , desc, &errorCode);
	if (!m_PhysicsSDK)  {
		gLogManager.log(mT("Unable to initialize the PhysX SDK"), getNxSDKCreateError(errorCode),ELL_ERROR);
		gLogManager.endSection(0);
		return false;
	}

	m_charControllerManager=NxCreateControllerManager(m_userAllocator);
	if(!m_charControllerManager){
		gLogManager.log(mT("Unable to initialize Character Controller Manager"), ELL_WARNING);
	}


	gLogManager.log(mT("PhysX SDK created"),ELL_SUCCESS);
	NxU32 apiRev,descRev, branchId;
	NxU32 vid=m_PhysicsSDK->getInternalVersion(apiRev,descRev, branchId);
	core::string version=core::StringConverter::toString((int)vid);
	gLogManager.log(mT("SDK Version : ")+version,ELL_INFO);

	m_PhysicsSDK->setParameter(NX_SKIN_WIDTH,0.0001);
	//m_PhysicsSDK->setParameter(NX_MAX_ANGULAR_VELOCITY, 8);
	m_PhysicsSDK->setParameter(NX_CONTINUOUS_CD,false);
	m_PhysicsSDK->setParameter(NX_CCD_EPSILON, 0.001f);

	enableDebugRender(false);
	m_PhysicsSDK->setParameter(NX_VISUALIZE_COLLISION_AXES, 1.0f);
	m_PhysicsSDK->setParameter(NX_VISUALIZE_COLLISION_SHAPES, 1.0f);

	m_Cooking=NxGetCookingLib(NX_PHYSICS_SDK_VERSION);
	//m_Cooking->NxInitCooking();
	gLogManager.log(mT("Cooking created"),ELL_SUCCESS);
	gLogManager.endSection(1);
	return true;
}
bool PhysXManager::_CreateScene(PhysicsSystemDesc* d)
{
	NxSceneDesc sceneDesc;
	NxBounds3	tmpBounds;
	sceneDesc.setToDefault();
	if(d)
	{
		sceneDesc.maxTimestep=d->maxTimestep;
		sceneDesc.maxIter=d->maxIter;
		sceneDesc.groundPlane=d->groundPlane;
		sceneDesc.boundsPlanes=d->boundsPlanes;
		sceneDesc.upAxis=d->upAxis;
		sceneDesc.gravity=ToNxVec3(d->gravity);
		if(d->simType==EST_Software)
			sceneDesc.simType=NX_SIMULATION_SW;
		else
			sceneDesc.simType=NX_SIMULATION_HW;
		if(d->maxBounds){
			tmpBounds.set(d->maxBounds->MinP.x,d->maxBounds->MinP.y,d->maxBounds->MinP.z,
				d->maxBounds->MaxP.x,d->maxBounds->MaxP.y,d->maxBounds->MaxP.z);
			sceneDesc.maxBounds=&tmpBounds;
		}
	}
	sceneDesc.isValid();

	m_Scene = m_PhysicsSDK->createScene(sceneDesc);
	if (!m_Scene){
		gLogManager.log(mT("Unable to create a PhysX scene"), ELL_ERROR);
		return false;
	}

 //	NxReal myTimestep = 1.0f/60.0f;     
// 	m_Scene->setTiming(myTimestep / 4.0f, 4, NX_TIMESTEP_FIXED);
	m_Scene->setTiming(d->maxTimestep, d->maxIter, d->useFixedTimeStep? NX_TIMESTEP_FIXED: NX_TIMESTEP_VARIABLE);

	m_Scene->setUserContactReport(m_physXReport);
	m_Scene->setUserTriggerReport(m_physXTriggerReport);

	// Create the default material
	NxMaterial* defaultMaterial = m_Scene->getMaterialFromIndex(0);
	defaultMaterial->setRestitution(0.125f); //! 0.5
	defaultMaterial->setStaticFriction(0.5); //! 0.5
	defaultMaterial->setDynamicFriction(0.5); //! 0.5
	CPhysXMaterial* mtrl=new CPhysXMaterial("DefaultMaterial");
	mtrl->setMaterial(defaultMaterial);
	m_materials[0]=mtrl;


	return true;
}
void PhysXManager::ConnectToRemoteDebugger(const core::string& ip)
{
//#ifdef PHYSX_USE_VRD
	if (m_PhysicsSDK->getFoundationSDK().getRemoteDebugger())
	{
		core::stringc ipStr;
		core::string_to_char(ip.c_str(),ipStr);
		m_PhysicsSDK->getFoundationSDK().getRemoteDebugger()->connect(ipStr.c_str(), PHYSX_VRD_PORT, PHYSX_VRD_EVENTMASK);
	}
//#endif
}
void PhysXManager::DisconnectFromRemoteDebugger()
{
	if (m_PhysicsSDK->getFoundationSDK().getRemoteDebugger())
		m_PhysicsSDK->getFoundationSDK().getRemoteDebugger()->disconnect();
}
bool PhysXManager::IsConnectedWithRemoteDebugger()
{
	if (m_PhysicsSDK->getFoundationSDK().getRemoteDebugger())
		return m_PhysicsSDK->getFoundationSDK().getRemoteDebugger()->isConnected();
	return false;
}
void PhysXManager::SetGravity(const math::vector3d& g)
{
	m_Scene->setGravity(ToNxVec3(g));
}
math::vector3d PhysXManager::GetGravity()
{
	NxVec3 g;
	m_Scene->getGravity(g);
	return ToVec3(g);
}
void PhysXManager::_DestroyScene()
{
	traceFunction(Physics);
	m_Scene->setUserContactReport(0);

	clearForceFields();
	clearForceFieldsShapeGroups();
	clearJoints();
	clearMaterials();
	clearControllers();
	clearNodes();

	_performRemoveNodes();

	if (m_Scene)
	{
		//GetPhysicsResults();  // Make sure to fetchResults() before shutting down
		m_PhysicsSDK->releaseScene(*m_Scene);
	}
}
void PhysXManager::releaseNX()
{
	traceFunction(Physics);
	/* Ageia and PhysX, both stylized and non-stylized, are trademarks or registered trademarks
	of Ageia Technologies Inc. Copyright 2006 Ageia Technologies Inc. */

	if (m_PhysicsSDK)  
		m_PhysicsSDK->release();

	if(m_charControllerManager)
		NxReleaseControllerManager(m_charControllerManager);

	delete m_userAllocator;
	m_userAllocator=0;
	delete m_errorStream;
	gLogManager.log(mT("PhysX shutdown.."), ELL_SUCCESS);
}


void PhysXManager::getPhysicsResults()
{
	m_Scene->flushStream();
	while (!m_Scene->fetchResults(NX_ALL_FINISHED, true));
}


const NxDebugRenderable *PhysXManager::getDebugRenderable(){
	return m_Scene->getDebugRenderable();
}

void PhysXManager::update(float dt)
{
	/*
	static float acc=0;
		acc+=dt;
		if(acc<1.0/60.0)
			return;
		acc=0;*/
	
	//traceFunction(Physics);
	beginUpdate(dt);
	endUpdate();
}


void PhysXManager::beginUpdate(float dt)
{
	if(!m_Scene)return;
	// Start collision and dynamics for delta time since the last frame
	m_Scene->simulate(dt); //1.0/60.0
	{
		ControllerMap::iterator p= m_controllers.begin();
		ControllerMap::iterator end= m_controllers.end();
		for(;p!=end;++p){
			(*p).second->update(dt);
		}
	}
}
void PhysXManager::endUpdate()
{
	if(!m_Scene)return;
	_AUTO_BENCHMARK(m_simulateBI);
	getPhysicsResults();

	_performRemoveNodes();
	{
		ClothsMap::iterator p= m_cloths.begin();
		ClothsMap::iterator end= m_cloths.end();
		for(;p!=end;++p){
			(*p).second->update();
		}
	}
}

void PhysXManager::renderDebug()
{
	m_renderer->renderData(*getDebugRenderable());
}


bool PhysXManager::isShapeSupported(ESupportedShapes s){
	switch(s){
		case ESS_Box:
		case ESS_Capsule:
		case ESS_Convex:
		case ESS_Plane:
		case ESS_Sphere:
		case ESS_Terrain:
		case ESS_Wheel:
		case ESS_TriangleMesh:
			return true;
	}
	return false;
}
bool PhysXManager::isJointSupported(ESupported3DJoints s){
	switch(s){
		case ESJ3D_RevoluteJoint:
		case ESJ3D_PrismaticJoint:
		case ESJ3D_DistanceJoint:
		case ESJ3D_PulleyJoint:
		case ESJ3D_FixedJoint:
		case ESJ3D_D6Joint:
		case ESJ3D_Cylindrical:
		case ESJ3D_Spherical:
		case ESJ3D_PointOnLine:
		case ESJ3D_PointOnPlane:
			return true;
	}
	return false;
}


IPhysicalNode* PhysXManager::createNode(PhysicalNodeDesc*desc)
{
	NxActorDesc actorDesc;
	NxBodyDesc bodyDesc;
	NxActor*actor=0;
	if(!PhysXObjectConverter::ConvertNodeDesc(desc,&actorDesc,&bodyDesc,this))
	{
		return 0;
	}
	actor= m_Scene->createActor(actorDesc);   

	NxArray<NxShapeDesc*, NxAllocatorDefault>::Iterator itr=actorDesc.shapes.begin();

	for (;itr!=actorDesc.shapes.end();itr++)
	{
	//	delete (*itr)->name;
		delete *itr;
	}
	if(!actor)
		return 0;

	GCPtr<CPhysXNode> n=new CPhysXNode(actor,this);
	actor->userData=n.pointer();
	n->SetUserData(desc->userData);
	//n->setActor(actor);

	m_nodes.insert(NodesMap::value_type(actor,n));
	m_nodesList.push_back(n);

	FIRE_LISTENR_METHOD(OnNodeCreated,(this,n));
	return n;
}
//////////////////////////////////////////////////////////////////////////

NxJoint* PhysXManager::createDistanceJoint(IPhysicalJointDistanceDesc*d){
	NxDistanceJointDesc desc;
	PhysXDistanceJoint::ConvertToDesc(d,&desc);

	NxJoint*res=0;

	res=m_Scene->createJoint(desc);

	return res;
}

NxJoint* PhysXManager::createSphericalJoint(IPhysicalJointSphericalDesc*d){
	NxSphericalJointDesc desc;
	PhysXSphericalJoint3D::ConvertToDesc(d,&desc);

	NxJoint*res=0;
	res=m_Scene->createJoint(desc);
	return res;
}
NxJoint* PhysXManager::createRevoluteJoint(IPhysicalJointRevoluteDesc*d){
	NxRevoluteJointDesc desc;
	PhysXRevoluteJoint::ConvertToDesc(d,&desc);


	desc.checkValid();
	NxJoint*res=0;
	res=m_Scene->createJoint(desc);
	return res;
}
NxJoint* PhysXManager::createPulleyJoint(IPhysicalJointPulleyDesc*d){
	NxPulleyJointDesc desc;
	PhysXPulleyJoint::ConvertToDesc(d,&desc);

	NxJoint*res=0;
	res=m_Scene->createJoint(desc);
	return res;
}
NxJoint* PhysXManager::createPrismaticJoint(IPhysicalJointPrismaticDesc*d){
	NxPrismaticJointDesc desc;
	IPhysXJoint::ConvertToDesc(d,&desc);

	NxJoint*res=0;
	res=m_Scene->createJoint(desc);
	return res;
}
NxJoint* PhysXManager::createCylindricalJoint(IPhysicalJointCylinderDesc*d){
	NxCylindricalJointDesc desc;
	IPhysXJoint::ConvertToDesc(d,&desc);

	NxJoint*res=0;
	res=m_Scene->createJoint(desc);
	return res;
}
NxJoint* PhysXManager::createPointOnLineJoint(IPhysicalJointPointOnLineDesc*d){
	NxPointOnLineJointDesc desc;
	IPhysXJoint::ConvertToDesc(d,&desc);

	NxJoint*res=0;
	res=m_Scene->createJoint(desc);
	return res;
}
NxJoint* PhysXManager::createPointOnPlaneJoint(IPhysicalJointPointOnPlaneDesc*d){
	NxPointInPlaneJointDesc desc;
	IPhysXJoint::ConvertToDesc(d,&desc);

	NxJoint*res=0;
	res=m_Scene->createJoint(desc);
	return res;
}
NxJoint* PhysXManager::createFixedJoint(IPhysicalJointFixedDesc*d){
	NxFixedJointDesc desc;
	IPhysXJoint::ConvertToDesc(d,&desc);


	NxJoint*res=0;
	res=m_Scene->createJoint(desc);
	return res;
}
NxJoint* PhysXManager::createD6Joint(IPhysicalJointD6Desc*d){
	NxD6JointDesc desc;
	PhysXDOF6Joint::ConvertToDesc(d,&desc);

	if(!desc.isValid())
	{
		return 0;
	}

	NxJoint*res=0;
	res=m_Scene->createJoint(desc);
	return res;
}


//////////////////////////////////////////////////////////////////////////
IPhysicalJoint* PhysXManager::createJoint(IPhysicalJoint3DDesc*desc){


	GCPtr<IPhysicalJoint> joint;
	NxJoint*nj=0;
	switch(desc->getType()){
		case ESJ3D_RevoluteJoint:
			nj=createRevoluteJoint((IPhysicalJointRevoluteDesc*)desc);
			joint=new PhysXRevoluteJoint(this,nj);
			break;
		case ESJ3D_PrismaticJoint:
			nj=createPrismaticJoint((IPhysicalJointPrismaticDesc*)desc);
			joint=new PhysXPrismaticJoint(this,nj);
			break;
		case ESJ3D_DistanceJoint:
			nj=createDistanceJoint((IPhysicalJointDistanceDesc*)desc);
			joint=new PhysXDistanceJoint(this,nj);
			break;
		case ESJ3D_PulleyJoint:
			nj=createPulleyJoint((IPhysicalJointPulleyDesc*)desc);
			joint=new PhysXPulleyJoint(this,nj);
			break;
		case ESJ3D_FixedJoint:
			nj=createFixedJoint((IPhysicalJointFixedDesc*)desc);
			joint=new PhysXFixedJoint(this,nj);
			break;
		case ESJ3D_D6Joint:
			nj=createD6Joint((IPhysicalJointD6Desc*)desc);
			joint=new PhysXDOF6Joint(this,nj);
			break;
			break;
		case ESJ3D_Cylindrical:
			nj=createCylindricalJoint((IPhysicalJointCylinderDesc*)desc);
			joint=new PhysXCylinderJoint(this,nj);
			break;
		case ESJ3D_Spherical:
			nj=createSphericalJoint((IPhysicalJointSphericalDesc*)desc);
			joint=new PhysXSphericalJoint3D(this,nj);
			break;
		case ESJ3D_PointOnLine:
			nj=createPointOnLineJoint((IPhysicalJointPointOnLineDesc*)desc);
			joint=new PhysXPointOnLineJoint(this,nj);
			break;
		case ESJ3D_PointOnPlane:
			nj=createPointOnPlaneJoint((IPhysicalJointPointOnPlaneDesc*)desc);
			joint=new PhysXPointOnPlaneJoint(this,nj);
			break;

	}
	if(!nj)
	{
		joint=0;
		return GCPtr<IPhysicalJoint>::Null;
	}
	nj->userData=joint.pointer();
	joint->userData=desc->userData;

	m_joints.insert(JointsMap::value_type(nj,joint));
	FIRE_LISTENR_METHOD(OnJointCreated,(this,joint));
	return joint;
}

IPhysicMaterial* PhysXManager::createMaterial(PhysicMaterialDesc*desc)
{
	if(!desc)
		return 0;

	NxMaterialDesc mdesc;
	PhysXObjectConverter::ConvertMaterialDesc(desc,&mdesc);
	GCPtr<CPhysXMaterial> mat=new CPhysXMaterial(desc->name);

	NxMaterial*nxMat= m_Scene->createMaterial(mdesc);
	mat->setMaterial(nxMat);
	m_materials.insert(MaterialsMap::value_type(mat->getMaterialIndex(),mat));

	m_materialManager->RegisterMaterial(mat);

	FIRE_LISTENR_METHOD(OnMaterialCreated,(this,mat));
	return mat;
}


IPhysicalForceField* PhysXManager::createForceField(PhysicalForceFieldDesc* desc)
{
	NxForceFieldDesc nxDesc;
	if(!PhysXObjectConverter::ConvertForceFieldDesc(desc,&nxDesc,this))
		return 0;
	NxForceField* f= m_Scene->createForceField(nxDesc);
	if(!f)
		return 0;
	PhysXForceField* ret=new PhysXForceField(f,this);
	f->userData=(IPhysicalForceField*)ret;
	ret->setUserData(desc->userData);
	m_forceFields.push_back(ret);
	return ret;
}
IPhysicalForceFieldShapeGroup* PhysXManager::createForceFieldShapeGroup(ForceFieldShapeGroupDesc* desc)
{
	NxForceFieldShapeGroupDesc nxDesc;
	if(!PhysXObjectConverter::ConvertForceFieldShapeGroupDesc(desc,&nxDesc,this))
		return 0;
	NxForceFieldShapeGroup* f= m_Scene->createForceFieldShapeGroup(nxDesc);
	if(!f)
		return 0;
	PhysXForceFieldShapeGroup* ret=new PhysXForceFieldShapeGroup(f);
	f->userData=(IPhysicalForceFieldShapeGroup*)ret;
	ret->setUserData(desc->userData);
	m_forceFieldsShapeGroup.push_back(ret);
	return ret;
}
IPhysicalForceFieldLinearKernal* PhysXManager::createForceFieldLinearKernal(PhysicalForceFieldLinearKernalDesc* desc)
{
	NxForceFieldLinearKernelDesc nxDesc;

	if(!PhysXObjectConverter::ConvertForceFieldLinearKernal(desc,&nxDesc,this))
		return 0;
	NxForceFieldLinearKernel* f= m_Scene->createForceFieldLinearKernel(nxDesc);
	if(!f)
		return 0;
	PhysXForceFieldLinearKernal* ret=new PhysXForceFieldLinearKernal(f);
	f->userData=(IPhysicalForceFieldLinearKernal*)ret;
	m_forceFieldsLinearKernals.push_back(ret);
	return ret;
}

ICharacterController* PhysXManager::createCharacterController(IPhysicsCCDesc*desc){

	NxControllerDesc*ndesc=0;

	NxBoxControllerDesc bdesc;
	NxCapsuleControllerDesc cdesc;


	if(desc->getType()==ECCT_Box){
		ndesc=&bdesc;
		PhysicsCCBoxDesc *ccdDesc=(PhysicsCCBoxDesc*)desc;
		bdesc.extents=ToNxVec3(ccdDesc->extent);
	}
	else if(desc->getType()==ECCT_Capsule){
		ndesc=&cdesc;
		PhysicsCCCapsuleDesc *ccdDesc=(PhysicsCCCapsuleDesc*)desc;
		cdesc.radius=ccdDesc->radius;
		cdesc.height=ccdDesc->height;
	}else
		return 0;


	ndesc->skinWidth=desc->skinWidth;
	ndesc->slopeLimit=desc->slopeLimit;
	ndesc->stepOffset=desc->stepOffset;
	ndesc->position=NxExtendedVec3(desc->position.x,desc->position.y,desc->position.z);
	switch(desc->upDir){
		case ECCUD_X:ndesc->upDirection=NX_X;break;
		case ECCUD_Y:ndesc->upDirection=NX_Y;break;
		case ECCUD_Z:ndesc->upDirection=NX_Z;break;
	}
	if(desc->interactionFlag==ECCI_Include){
		ndesc->interactionFlag=NXIF_INTERACTION_INCLUDE;
	}else{
		ndesc->interactionFlag=NXIF_INTERACTION_EXCLUDE;
	}

	NxController*nc= m_charControllerManager->createController(m_Scene,*ndesc);
	GCPtr<PhysXCharController> c=new PhysXCharController(nc,this);
/*
	//offset the center of the shapes
	NxActor*nxact= nc->getActor();
	NxShape*const *shapes=nxact->getShapes();
	for(int i=0;i<nxact->getNbShapes();++i)
	{
		shapes[i]->setLocalPosition(ToNxVec3(desc->position));
	}
*/
	m_controllers.insert(ControllerMap::value_type(nc,c));
	FIRE_LISTENR_METHOD(OnCharacterControllerCreated,(this,c));
	return c;
}

IPhysicalCloth* PhysXManager::createCloth(PhysicalClothDesc* desc)
{
	NxClothDesc pdesc;
	MemoryWriteBuffer buf;
	NxMeshData mReceiveBuffers;
	scene::IMeshBuffer *mRenderMesh=0;
	CPhysXCloth* resultCloth=0;
	video::IHardwareStreamBuffer* normalStream=0;

	if(!desc->meshData)
		return 0;

	if(!PhysXObjectConverter::ConvertClothDesc(desc,&pdesc,this))
		return 0;

	video::IHardwareStreamBuffer* posStream= desc->meshData->getStream(0,video::EMST_Position);
	video::IndexData* indexData= desc->meshData->getIndexData();
	

	int maxVertices=3*posStream->length();
	int maxIndices=indexData->indexCount;
	video::IHardwareIndexBuffer::EIndexType resultIndexType=indexData->indexBuffer->getIndexType();
	if(maxIndices>=65535)
		resultIndexType=video::IHardwareIndexBuffer::EIT_32Bit;
	else resultIndexType=video::IHardwareIndexBuffer::EIT_16Bit;

	mRenderMesh=new scene::SMeshBuffer();
	posStream=mRenderMesh->createStream(0,video::EMST_Position,video::ESDT_Point3f,maxVertices,
		video::IHardwareBuffer::EUT_StaticWriteOnly,false,true);
	normalStream=mRenderMesh->createStream(0,video::EMST_Normal,video::ESDT_Point3f,maxVertices,
		video::IHardwareBuffer::EUT_StaticWriteOnly,false,true);
	video::IHardwareStreamBufferCRef texStream=desc->meshData->getStream(0,video::EMST_Texcoord);
	if(texStream)
	{
		//video::IHardwareStreamBufferCRef ts= mRenderMesh->createStream(0,video::EMST_Texcoord,texStream->getElementData(),maxVertices,video::IHardwareBuffer::EUT_StaticWriteOnly,false,true);
		//ts->copyData(texStream,0,0,texStream->length());
		mRenderMesh->addStream(0,video::EMST_Texcoord,texStream);
	}

	mRenderMesh->createIndexBuffer(resultIndexType,maxIndices,video::IHardwareIndexBuffer::EUT_Dynamic,false,true);
	indexData=mRenderMesh->getIndexData();

	resultCloth=new CPhysXCloth(mRenderMesh);

	pdesc.meshData=resultCloth->getReceiveBuffers();
	pdesc.userData=resultCloth;

	NxCloth* cloth=m_Scene->createCloth(pdesc);
	if(!cloth)
		return 0;

	resultCloth->SetCloth(cloth);
	resultCloth->setUserData(desc->userData);

	m_cloths[resultCloth->GetCloth()]=resultCloth;

	FIRE_LISTENR_METHOD(OnClothCreated,(this,resultCloth));
	return resultCloth;
}


IPhysicMaterial* PhysXManager::GetMaterial(MaterialID id)
{
	MaterialsMap::iterator it= m_materials.find(id);
	if(it==m_materials.end())
		return 0;
	return it->second;
}

void PhysXManager::SetActorPairFlag(IPhysicalNode*n1,IPhysicalNode*n2,EContactReportFlag flag,bool enable)
{
	CPhysXNode*a=(CPhysXNode*)n1;
	CPhysXNode*b=(CPhysXNode*)n2;
	NxU32 flags= m_Scene->getActorPairFlags(*a->getActor(),*b->getActor());
	if(enable)
		flags|=PhysXObjectConverter::convertContactFlag(flag);
	else
		flags&=~PhysXObjectConverter::convertContactFlag(flag);
	m_Scene->setActorPairFlags(*a->getActor(),*b->getActor(),flags);
}
bool PhysXManager::GetActorPairFlag(IPhysicalNode*n1,IPhysicalNode*n2,EContactReportFlag flag)
{
	CPhysXNode*a=(CPhysXNode*)n1;
	CPhysXNode*b=(CPhysXNode*)n2;
	NxU32 flags= m_Scene->getActorPairFlags(*a->getActor(),*b->getActor());
	return (flags & PhysXObjectConverter::convertContactFlag(flag))!=0;
}

void PhysXManager::SetShapePairFlag(IPhysicalShape*n1,IPhysicalShape*n2,EContactReportFlag flag,bool enable)
{
	PhysXShape*a=(PhysXShape*)n1;
	PhysXShape*b=(PhysXShape*)n2;
	NxU32 flags= m_Scene->getShapePairFlags(*a->getNxShape(),*b->getNxShape());
	if(enable)
		flags|=PhysXObjectConverter::convertContactFlag(flag);
	else
		flags&=~PhysXObjectConverter::convertContactFlag(flag);
	m_Scene->setShapePairFlags(*a->getNxShape(),*b->getNxShape(),flags);
}
bool PhysXManager::GetShapePairFlag(IPhysicalShape*n1,IPhysicalShape*n2,EContactReportFlag flag)
{
	PhysXShape*a=(PhysXShape*)n1;
	PhysXShape*b=(PhysXShape*)n2;
	NxU32 flags= m_Scene->getShapePairFlags(*a->getNxShape(),*b->getNxShape());
	return (flags & PhysXObjectConverter::convertContactFlag(flag))!=0;
}


void PhysXManager::SetActorGroupPairFlag(GroupID g1,GroupID g2,EContactReportFlag flag,bool enable)
{
	NxU32 flags= m_Scene->getActorGroupPairFlags(g1,g2);
	if(enable)
		flags|=PhysXObjectConverter::convertContactFlag(flag);
	else
		flags&=~PhysXObjectConverter::convertContactFlag(flag);
	m_Scene->setActorGroupPairFlags(g1,g2,flags);
}
bool PhysXManager::GetActorGroupPairFlag(GroupID g1,GroupID g2,EContactReportFlag flag)
{
	NxU32 flags= m_Scene->getActorGroupPairFlags(g1,g2);
	return (flags & PhysXObjectConverter::convertContactFlag(flag))!=0;
}

void PhysXManager::SetGroupCollisionFlag(GroupID g1,GroupID g2,bool enable)
{
	m_Scene->setGroupCollisionFlag(g1,g2,enable);
}
bool PhysXManager::GetGroupCollisionFlag(GroupID g1,GroupID g2)
{
	return m_Scene->getGroupCollisionFlag(g1,g2);
}

IPhysicSceneQuery* PhysXManager::CreateSceneQuery(const PhysicSceneQueryDesc& desc)
{
	NxSceneQueryDesc sceneQueryDesc;
	sceneQueryDesc.executeMode = desc.execMode ==ESQExec_Syncronous ? NX_SQE_SYNCHRONOUS : NX_SQE_ASYNCHRONOUS;
	sceneQueryDesc.report = new PhysXSceneQueryReport(desc.report);

	
	return new PhysXSceneQuery(m_Scene->createSceneQuery(sceneQueryDesc),desc.report,desc.execMode);
}
IPhysicalShape* PhysXManager::RaycastClosestShape(const math::Ray3d& ray,EShapeType shapeType,PhysicsRaycaseHit&hit,float maxDistance,uint groupID,const CollisionMask* collisionMask)
{
	NxRaycastHit hres;
	hres.shape=0;
	NxGroupsMask mask,*maskPtr=0;
	if(collisionMask)
	{
		mask.bits0=collisionMask->bits[0];
		mask.bits1=collisionMask->bits[1];
		mask.bits2=collisionMask->bits[2];
		mask.bits3=collisionMask->bits[3];
		maskPtr=&mask;
	}
	NxShape* s= m_Scene->raycastClosestShape(NxRay(ToNxVec3(ray.Start),ToNxVec3(ray.Dir)),PhysXObjectConverter::convertShapeType(shapeType),hres,groupID,maxDistance,0xffffffff,maskPtr,0);

	hit.distance=hres.distance;
	hit.worldImpact=ToVec3(hres.worldImpact);
	hit.worldNormal=ToVec3(hres.worldNormal);
	if(hres.shape)
		hit.shape=(IPhysicalShape*)hres.shape->userData;
	hit.materialID=hres.materialIndex;

	if(!s)return 0;
	return (IPhysicalShape*)s->userData;

}

}
}


