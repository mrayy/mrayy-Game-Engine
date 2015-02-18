
#include "stdafx.h"
#include "Application.h"
#include "AgentPhysics.h"
#include "ObstaclePhysics.h"
#include "ObstacleArchitect.h"
#include "AIRenderDevice.h"
#include "GameAgentArchitect.h"
#include "AgentArchitect.h"
#include "AIObstacleCircle.h"
#include "AIObstacleRectangle.h"
#include "FuzzyRaycastDOS.h"
#include "AIDesire.h"
#include "../common/GameEntity.h"
#include "../common/GameEntityManager.h"
#include "../common/DemoCamera.h"
#include "GameGlobals.h"

#include "DecisionTree.h"
#include "TrainingDataSet.h"
#include "AttributesScheme.h"
#include "DecisionAttributes.h"

#include <PhysXAPI.h>


namespace mray{

	class VisualizingRenderOP:public scene::IRenderable
	{
		bool m_vis;
		bool m_pass;
		bool m_castShadow;
		bool m_recShadow;

		Application* m_app;

		math::matrix4x4 m_trans;
		scene::NodeList m_child;

		core::string m_name;
		uint m_id;

		GCPtr<video::SMaterial> m_mtrl;

	public:
		VisualizingRenderOP(Application*app):m_vis(true),m_pass(false),m_castShadow(false),m_recShadow(false),m_id(0)
		{
			m_mtrl=new video::SMaterial();
			m_mtrl->setRenderState(video::RS_Lighting,video::ES_DontUse);
			m_mtrl->setRenderState(video::RS_Blend,video::ES_Use);
			m_app=app;
		}
		virtual void preRender()
		{}
		virtual void render()
		{
			m_app->getDevice()->setTransformationState(video::TS_WORLD,m_trans);
			m_app->getDevice()->useMaterial(m_mtrl);
			AI::AIRenderDevice dev(m_app->getDevice(),m_app->getFont());
			m_app->getAiSystem()->Visualize(&dev);
			m_app->getTactManager()->Visualize(&dev);
		}
		virtual void update(float dt)
		{
		}

		virtual void setVisible(const bool&  v){m_vis=v;}
		virtual bool &isVisible(){return m_vis;}

		virtual void setPassable(const bool & p){m_pass=p;}
		virtual bool& isPassable(){return m_pass;}

		virtual bool& isShadowCaster(){return m_castShadow;}
		virtual void setShadowCaster(const bool &val) {}

		virtual bool& isShadowReciver(){return m_recShadow;}
		virtual void setShadowReciver(const bool &val) {}

		virtual void setMaterial(GCPtr<video::SMaterial> mtrl,int i=0){}
		virtual GCPtr<video::SMaterial>getMaterial(int i=0){return m_mtrl;}
		virtual int getMaterialCount(){return 1;}

		virtual void setShader(int mesh,GCPtr<video::IGPUShaderProgram> shader){}
		virtual void setShaderCallback(int mesh,GCPtr<video::IShaderConstantsCallback> callback){}

		virtual GCPtr<video::IGPUShaderProgram> getShader(int i)
		{
			return 0;
		}


		virtual const core::string& getNodeName()const
		{
			return m_name;
		}
		virtual void setNodeName(const core::string&name)
		{
		}

		virtual uint& getID()
		{
			return m_id;
		}

		virtual void removeChild(IMovable*elem)
		{
		}
		virtual void addChild(IMovable*elem)
		{
		}
		virtual void removeFromParent()
		{
		}

		virtual IMovable* getParent()
		{
			return 0;
		}
		virtual void setParent(IMovable*p)
		{
		}
		virtual bool NeedChildUpdate()
		{
			return false;
		}

		virtual void attachAnimation(GCPtr<animation::IAnimationTrack> anim)
		{
		}
		virtual GCPtr<animation::IAnimationTrack> getAnimation()
		{
			return 0;
		}

		virtual math::vector3d getAbsolutePosition()const
		{
			return math::vector3d::Zero;
		}
		virtual math::quaternion getAbsoluteOrintation()const
		{
			return math::quaternion::Identity;
		}
		virtual math::vector3d getAbsoluteScale()const
		{
			return math::vector3d(1);
		}
		virtual math::vector3d getPosition()const
		{
			return math::vector3d::Zero;
		}
		virtual math::quaternion getOrintation()const
		{
			return math::quaternion::Identity;
		}
		virtual math::vector3d getScale()const
		{
			return math::vector3d(1);
		}

		virtual void updateAbsoluteTransformation()
		{
		}
		virtual math::matrix4x4&getAbsoluteTransformation()
		{
			return m_trans;
		}
		virtual const math::matrix4x4&getAbsoluteTransformation()const
		{
			return m_trans;
		}
		virtual const math::matrix4x4&getRelativeTransformation()const
		{
			return m_trans;
		}

		virtual void setPosition(const math::vector3d& v)
		{
		}
		virtual void setOrintation(const math::quaternion& v)
		{
		}
		virtual void setScale(const math::vector3d& v)
		{
		}

		virtual math::box3d getBoundingBox()
		{
			return math::box3d(0);
		}
		virtual math::box3d getTransformedBoundingBox()
		{
			return math::box3d(0);
		}

		virtual core::IteratorPair<scene::NodeList> getChildren()
		{
			return m_child;
		}
	};

using namespace core;
using namespace math;

#define  TEST_COMBO_ID 0x100

Application::Application():m_mouseMode(EMM_SetTarget),m_isCameraAttached(false),m_isCreating(false)
{
}
Application::~Application()
{
}

void Application::onEvent(GCPtr<Event> event)
{
	CMRayApplication::onEvent(event);
	if(event->isRecived())return;
	switch(event->getType()){
case ET_Mouse:
	{
		GCPtr<MouseEvent> e=event;
		if(e->Event==MET_LEFTDOWN)
		{
			onMouseDown(e->x,e->y);
		}else if(e->Event==MET_MOVED)
		{
			onMouseMove(e->x,e->y);
		}else if(e->Event==MET_LEFTUP)
		{
			onMouseUp(e->x,e->y);
		}
	}break;
case ET_Keyboard:
	{
		GCPtr<KeyboardEvent> e=event;
		if(e->press){
			switch(e->key)
			{
			case KEY_ESCAPE:
				getWindow()->shutdown();
				break;
			case KEY_1:
				m_mouseMode=EMM_SetTarget;
				//createObstacle(m_camera->getPosition(),10);
				break;
			case KEY_2:
				m_mouseMode=EMM_Create_Agent;
				break;
			case KEY_3:
				m_mouseMode=EMM_Create_SphereObstacle;
				break;
			case KEY_4:
				m_mouseMode=EMM_Create_BoxObstacle;
				break;

			case KEY_Q:
				m_isCameraAttached=!m_isCameraAttached;
				if(m_isCameraAttached)
					attachCameraToAgent();/*
				else
					m_camera->fovY=45;*/
				break;
			}
			return;
		}
	}break;
case ET_GuiEvent:
	{
		GCPtr<GUIEvent> e=event;
		if(e->Event==GET_Mouse_UP){
		}else if(e->Event==GET_Mouse_MOVE){
		}else if(e->Event==GET_SELECT_ITEM)
		{
			if(e->Caller->Id==TEST_COMBO_ID)
			{
				int id=((GUI::GUIComboList*)e->Caller)->getSelectedItem();
				if(id==-1)break;
				++id;
				if(m_selectedActor)
					m_selectedActor->EnableVisualize(false);
				m_selectedActor=m_agents[id];
				m_selectedActor->EnableVisualize(true);

			}
		}
	}break;
	}
}



void Application::onMouseDown(float x,float y)
{
	math::vector3d v;
	math::triangle3d t;
	math::vector3d dir=scene::SceneHelper::getInstance().getVectorFromScreen(m_viewPort->getAbsViewPort(),math::vector2d(x,y),1000,m_camera);
	float dist=scene::SceneHelper::getInstance().getCollisionTriangle(m_worldSelector,math::line3d(m_camera->getPosition(),dir),t,v);
	if(dist>0){
		switch(m_mouseMode){
			case EMM_SetTarget:
				m_interstingMngr->SetInterstingPoint(v);
				break;
			case EMM_Create_Agent:
			case EMM_Create_SphereObstacle:
				//createObstacle(v,7);
				m_isCreating=true;
				m_startCreatingPos=v;
				break;
		}
		
	}
}

void Application::onMouseMove(float x,float y)
{/*
	math::vector3d v;
	math::triangle3d t;
	math::vector3d dir=scene::SceneHelper::getInstance().getVectorFromScreen(m_viewPort->getAbsViewPort(),math::vector2d(x,y),1000,m_camera);
	float dist=scene::SceneHelper::getInstance().getCollisionTriangle(m_worldSelector,math::line3d(m_camera->getPosition(),dir),t,v);
	if(dist>0){
		switch(m_mouseMode){
			case EMM_SetTarget:
				m_interstingMngr->SetInterstingPoint(v);
				break;
			case EMM_Create_Agent:
				createEntity(v);
				break;
			case EMM_Create_SphereObstacle:
				createObstacle(v,7);
				break;
		}

		}*/
	math::vector3d v;
	math::triangle3d t;
	math::vector3d dir=scene::SceneHelper::getInstance().getVectorFromScreen(m_viewPort->getAbsViewPort(),math::vector2d(x,y),1000,m_camera);
	float dist=scene::SceneHelper::getInstance().getCollisionTriangle(m_worldSelector,math::line3d(m_camera->getPosition(),dir),t,v);
	if(dist>=0)
		m_currentMousePos=v;
}

void Application::onMouseUp(float x,float y)
{
	if(!m_isCreating)
		return;
	math::vector3d v;
	math::triangle3d t;
	math::vector3d dir=scene::SceneHelper::getInstance().getVectorFromScreen(m_viewPort->getAbsViewPort(),math::vector2d(x,y),1000,m_camera);
	float dist=scene::SceneHelper::getInstance().getCollisionTriangle(m_worldSelector,math::line3d(m_camera->getPosition(),dir),t,v);
	if(dist>0){
		float r=m_startCreatingPos.getDist(v);
		switch(m_mouseMode){
			case EMM_Create_Agent:
				if(r<0.5)
					r=0.5;
				if(r>3)
					r=3;
				createEntity(m_startCreatingPos,r);
				m_isCreating=false;
				break;
			case EMM_Create_SphereObstacle:
				if(r<2)
					r=2;
				if(r>7)
					r=7;
				createObstacle(m_startCreatingPos,r);
				m_isCreating=false;
				break;
		}

	}
}

void Application::attachCameraToAgent()
{
	if(!m_selectedActor)return;

	AI::IAIArchitect*arch=m_selectedActor->GetController();
	AI::VisionSensor*vision= (AI::VisionSensor*)arch->GetComponent(mT("Vision"));

	//m_camera->fovY=vision->FoV();
}

void Application::createObstacle(const math::vector3d&p,float rad)
{
	AI::NavMeshPolygon* poly;
	math::vector3d pos=p;
	AI::NavWorldNodeListPtr tmp;
	poly=(AI::NavMeshPolygon*)m_navMesh->QueryPolys(pos,tmp);
	if(poly){
		math::vector3d p;
		poly->plane.getIntersectionWithLine(pos,-math::vector3d::YAxis,p);
		pos=p;
		pos.y+=rad*1.5;
	}else
		return;

	physics::SphereShapeDesc sdesc;
	physics::PhysicalNodeDesc nodeDesc;
	physics::PhysicalBodyDesc bodyDesc;
	sdesc.radius=rad;

	nodeDesc.globalPos.setTranslation(pos);

	bodyDesc.mass=2*rad;
	nodeDesc.shapes.push_back(&sdesc);
	nodeDesc.bodyDesc=&bodyDesc;

	GCPtr<physics::IPhysicalNode> phNode=m_phManager->createNode(&nodeDesc);

	GCPtr<scene::ISceneNode> node= getSceneManager()->addSphere(rad,8,8);

	GCPtr<video::SMaterial> mat;
	mat=gMaterialResourceManager.getMaterial(mT("ShadowedMaterial"));
	node->setMaterial(mat,0);
	node->ShadowCaster=true;
	node->ShadowReciver=true;
	node->Visible=true;

	phNode->addForce(math::vector3d(Randomizer::rand01()-0.5,0,Randomizer::rand01()-0.5)*10000);

	AI::AIAgent* agent=new AI::AIAgent(new AI::ObstaclePhysics(phNode),mT("Agent"));
	agent->EnableVisualize(true);
	m_aiSystem->AddActor(agent);
	AI::ObstacleArchitect* architect=new AI::ObstacleArchitect(agent);
	agent->AttachController(architect);
	architect->GetObstacle()->SetShape(new AI::AIObstacleCircle(rad+1));
	architect->GetObstacle()->SetRadius(rad+1);

	m_gameEntManager->addEntity(new GameEntity(node,agent->GetPhysicalProp()));
}

GCPtr<scene::ISceneNode> Application::loadNode(const core::string& path)
{
	GCPtr<OS::IStream> stream=gFileSystem.openFile(path);
	if(stream){
		xml::XMLTree tree;
		if(!tree.load(stream))
			return 0;
		xml::XMLElement*e=tree.getSubElement(mT("ISceneNode"));
		if(!e)return 0;
		GCPtr<scene::ISceneNode> node=new scene::SLODAnimatedMesh(0,0,1,math::quaternion(),0);
		node->loadXMLSettings(e);
		return node;
	}
	return 0;
}
void Application::createEntity(const math::vector3d&p,float rad)
{
	AI::NavMeshPolygon* poly;
	math::vector3d pos=p;
	AI::NavWorldNodeListPtr tmp;
	poly=(AI::NavMeshPolygon*)m_navMesh->QueryPolys(pos,tmp);
	if(poly){
		math::vector3d p;
		poly->plane.getIntersectionWithLine(pos,-math::vector3d::YAxis,p);
		pos=p;
		pos.y+=10+rad;
	}else
		return;


	GCPtr<physics::ICharacterController> phNode;
/*
	GCPtr<physics::IPhysicalNode> phNode;
	physics::SphereShapeDesc sdesc;
	physics::PhysicalNodeDesc nodeDesc;
	physics::PhysicalBodyDesc bodyDesc;
*/
	physics::PhysicsCCCapsuleDesc ccDesc;
	ccDesc.radius=rad;
	ccDesc.height=rad;
	ccDesc.position=pos;
	ccDesc.slopeLimit=0;//(float)cos(math::toRad(70.0f));
/*
	physics::PhysicMaterialDesc mdesc;
	mdesc.dynamicFriction=0.5;
	mdesc.staticFriction=0.3;
	mdesc.restitution=0.5;

	GCPtr<physics::IPhysicMaterial> material=m_phManager->createMaterial(&mdesc);*/
// 
// 	bodyDesc.mass=Randomizer::rand01()*2;
// 	sdesc.radius=5;
// 	sdesc.material=material;
// 	nodeDesc.globalPos.setTranslation(pos);
/*
	nodeDesc.shapes.push_back(&sdesc);
	nodeDesc.bodyDesc=&bodyDesc;

	phNode=m_phManager->createNode(&nodeDesc);
*/

	phNode=m_phManager->createCharacterController(&ccDesc);

	GCPtr<scene::ISceneNode> node=loadNode(mT("RobotX45.xml"));
	if(node){
		node->scale=rad;
		getSceneManager()->addSceneNode(node);
	}
	else
	{
		node= getSceneManager()->addSphere(rad,8,8);

		GCPtr<video::SMaterial> mat;
		mat=gMaterialResourceManager.getMaterial(mT("ShadowedMaterial"));
		node->setMaterial(mat,0);
	}
	node->ShadowCaster=true;
	node->ShadowReciver=true;
	node->Visible=true;


	AI::AIAgent* agent=new AI::AIAgent(new AI::AgentPhysics(phNode),mT("Agent"));

	m_gameEntManager->addEntity(new GameEntity(node,agent->GetPhysicalProp()));

	m_aiSystem->AddActor(agent);
	AI::GameAgentArchitect* architect=new AI::GameAgentArchitect(agent);
	agent->AttachController(architect);
	agent->EnableVisualize(false);
	agent->GetObstacle()->SetShape(new AI::AIObstacleCircle(rad+1));
	architect->SetTeam(m_lastAgentID%3);
	//agent->GetObstacle()->SetShape(new AI::AIObstacleRectangle(ccDesc.radius*1.5,ccDesc.radius*2));
	architect->GetVisionSensor()->SetViewRange(40);
	architect->GetObstacle()->SetRadius(rad+1);
//	architect->SetDynamicObstacleSolver(new AI::LookAheadDOS(architect->GetObstacle(),40));
	architect->SetDynamicObstacleSolver(new AI::FuzzyRaycastDOS(architect->GetObstacle(),architect->GetSensors()));


	++m_lastAgentID;
	m_agents.insert(ActorIDList::value_type(m_lastAgentID,agent));
	m_agentsCList->addItem((core::string(mT("Agent: "))+m_lastAgentID).c_str());

	AI::AIArchitectParser parser;
	GCPtr<OS::IStream> archDesc=gFileSystem.openFile(mT("AgentTest.xml"));
	parser.ParseXML(architect,archDesc);
	archDesc->close();
}

void Application::init(core::array<SOptionElement> *extraOptions)
{
	traceFunction(Application);

	matrix4x4 projection;

	CMRayApplication::init(extraOptions);
	CMRayApplication::loadResourceFile(mT("Resources.stg"));

	OS::ArchiveManager::getInstance().addArchive(mT("GameData.zip"));

	m_lastAgentID=0;
	m_selectedActor=0;

	{
		AI::DecisionTree tree;
		AI::AttributesScheme* scheme=new AI::AttributesScheme();
		AI::TrainingDataSet ds(scheme);
		tree.SetScheme(scheme);
		scheme->AddAttribute(new AI::GenericDecisionAttrib<int>(mT("A"),0,3,1));
		scheme->AddAttribute(new AI::GenericDecisionAttrib<int>(mT("B"),0,2,1));
		scheme->AddAttribute(new AI::GenericDecisionAttrib<int>(mT("C"),0,3,1));
		ds.SetExamplesCount(4);
		ds.SetValue(0,0,0);ds.SetValue(0,1,0);ds.SetValue(0,2,0);
		ds.SetValue(1,0,1);ds.SetValue(1,1,0);ds.SetValue(1,2,0);
		ds.SetValue(2,0,0);ds.SetValue(2,1,1);ds.SetValue(2,2,0);
		ds.SetValue(3,0,1);ds.SetValue(3,1,1);ds.SetValue(3,2,1);
// 		ds.SetValue(4,0,2);ds.SetValue(4,1,0);ds.SetValue(4,2,2);
// 		ds.SetValue(5,0,2);ds.SetValue(5,1,1);ds.SetValue(5,2,0);
		tree.Train(&ds,mT("C"));
		core::array<ushort> sample;
		sample.push_back(scheme->GetAttribute(0)->GetBucketTyped(1));
		sample.push_back(scheme->GetAttribute(1)->GetBucketTyped(0));
		int res=tree.Predict<int>(sample);

		sample.clear();
	}


	{
		GCPtr<OS::IStream> themeStream=gFileSystem.createBinaryFileReader(mT("VistaCG.xml"));
		GUI::GUISkinManager::getInstance().loadTheme(themeStream)->SetDevice(getDevice());
		GUI::GUISkinManager::getInstance().setActiveSkin(mT("VistaCG"));

		//load font
		m_font=gFontResourceManager.getOrCreate(mT("solo5_font.xml"));
		gFontResourceManager.setDefaultFont(m_font);
	}


	m_interstingMngr=new AI::InterstingPointManager();

	//create physics system
	physics::PhysicsSystemDesc phDesc;
	math::box3d maxBounds(math::vector3d(-2000),math::vector3d(+2000));
	phDesc.maxBounds=&maxBounds;
	phDesc.gravity=math::vector3d(0,-98,0);
	m_phManager=physics::createPhysXManager(&phDesc);


	//load world
	if(!loadWorld(mT("World.xml")))
	{
		gLogManager.log(mT("Cann't load world."),ELL_ERROR);
	}

	//create AI System
	AI::AISystemDesc aiDesc;
	aiDesc.worldBB=m_navMesh->GetBoundingBox();
	aiDesc.cellPartitionSize=100;
	m_aiSystem=new AI::AISystem(aiDesc);
	m_aiSystem->SetNavigationWorld(m_navMesh);

	m_gameEntManager=new GameEntityManager();

	m_camera=new DemoCamera(getSceneManager(),100);
	getSceneManager()->addSceneNode(m_camera);

	m_viewPort=getSceneManager()->createViewport(mT("MainVP"),0);
	m_viewPort->setCamera(m_camera);

	GCPtr<scene::SLightNode> light=getSceneManager()->addLight(255,1000,1,math::vector3d(30,200,200));

	if(getDevice()->getCapabilities()->isFeatureSupported(video::EDF_RenderToTarget)){
		//Create Shadow
		projection.buildProjectionMatrixPerspectiveFovRH(m_camera->fovY,
			m_camera->Aspect,1,500);
		light->setProjection(projection);

		GCPtr<video::ITexture> texture=getDevice()->createTexture2D(2048,video::EPixel_Float16_R);
		GCPtr<video::IRenderTarget> shadowRenderTarget=getDevice()->createRenderTarget(mT("shadowRenderTarget"),texture,0,0,0);
		light->setShadowMap(shadowRenderTarget);
	}

	{
		float width=getDevice()->getViewportRect().getWidth();
		m_agentsCList=getGUIManager()->addComboList(mT(""),math::vector2d(width-350,20),
			math::vector2d(300,30),this,0,240,TEST_COMBO_ID);
	}

	//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
	m_visualizeNode=new VisualizingRenderOP(this);
	m_visualizeOP.customRender=true;
	m_visualizeOP.node=m_visualizeNode;

	//////////////////////////////////////////////////////////////////////////
	m_tacticalManager =new AI::TacticalPointManager(m_navMesh);

	Game::GameGlobals::g_system=m_aiSystem;
	Game::GameGlobals::g_tacticalManager=m_tacticalManager;

}


bool Application::loadWorld(const core::string&worldxml)
{
	AI::NavMesh*navMesh=new AI::NavMesh();
	m_navMesh=navMesh;

	GCPtr<OS::IStream> worldXmlModelStream=gFileSystem.openFile(worldxml);
	GCPtr<OS::IStream> worldModelStream;
	core::string world;
	core::string navFile;
	if(worldXmlModelStream){
		xml::XMLTree tree;
		xml::XMLElement*elem;
		if(!tree.load(worldXmlModelStream))
		{
			gLogManager.log(mT("error reading world file: ")+worldxml,ELL_ERROR);
		}
		xml::XMLAttribute*attr;
		elem=tree.getSubElement(mT("World"));
		if(!elem)
		{
			gLogManager.log(mT("Cann't find World tag in world file: ")+worldxml,ELL_ERROR);
		}
		attr=elem->getAttribute(mT("ModelFile"));
		if(!attr)
		{
			gLogManager.log(mT("Cann't find ModelFile attr in world file: ")+worldxml,ELL_ERROR);
		}
		world=attr->value;
		attr=elem->getAttribute(mT("NavigationMesh"));
		if(!attr)
		{
			gLogManager.log(mT("Cann't find NavigationMesh attr in world file: ")+worldxml,ELL_WARNING);
		}
		navFile=attr->value;
	}
	else
	{
		gLogManager.log(mT("Cann't open world file: ")+worldxml,ELL_ERROR);
	}
	m_world=loadNode(world);
	if(m_world){
		getSceneManager()->addSceneNode(m_world);
	}
	else
	{
		m_world=getSceneManager()->addAnimatedMesh(world.c_str());
	}
	if(m_world){
		GCPtr<video::SMaterial> mat;
		m_world->ShadowCaster=true;
		m_world->ShadowReciver=true;
		//m_world->Visible=false;
		GCPtr<scene::SMesh> mesh=m_world->getMesh();
	//	scene::SMeshManipulator::getInstance().recalcNormalsFlat(mesh);
	//	scene::SMeshManipulator::getInstance().makePlanarTextureMapping(mesh);
		/**/for(int i=0;i<mesh->getBuffersCount();++i){
			scene::SMeshManipulator::getInstance().scaleMesh(mesh->getBuffer(i),math::vector3d(15));
			for(int j=0;j<mesh->getBuffer(i)->getMorphTargetCount();++j)
				scene::SMeshManipulator::getInstance().scaleMesh(mesh->getBuffer(i)->getMorphTarget(j),math::vector3d(15));
		}
		m_world->scale=1;
		m_world->updateAbsoluteTransformation();
		mesh->calcCurrBoundingBox();
		GCPtr<scene::IMeshBuffer> buffer=mesh->getBufferByName(mT("floor"));
		m_worldSelector=new scene::CTriangleSelector();
		m_worldSelector->setTriangles(buffer,&m_world->getAbsoluteTransformation());
		GCPtr<video::IHardwareStreamBuffer> vStream=buffer->getStream(0,video::EMST_Position);
		GCPtr<video::IHardwareIndexBuffer> iBuffer=buffer->getIndexData()->indexBuffer;
		math::vector3d*vptr=(math::vector3d*)vStream->lock(0,vStream->getSizeInBytes(),video::IHardwareBuffer::ELO_ReadOnly);
		void*iptr=iBuffer->lock(0,iBuffer->getSizeInBytes(),video::IHardwareBuffer::ELO_ReadOnly);

		navMesh->SetMaxSlope(0.5);
		navMesh->SetMaxStepSize(0.1);

	/*	GCPtr<OS::IStream> stream=gFileSystem.openFile(navFile,OS::BIN_READ);
		if(stream){
			navMesh->Deserialize(stream);
		}else{*/
			navMesh->Generate(vptr,vStream->length(),iptr,iBuffer->getIndexCount(),0,
				iBuffer->getIndexType()==video::IHardwareIndexBuffer::EIT_16Bit,&m_world->getAbsoluteTransformation());

			navMesh->Serialize(gFileSystem.openFile(navFile,OS::BIN_WRITE));
	//	}

		vStream->unlock();
		iBuffer->unlock();
		//node->Visible=false;


		physics::PhysicMaterialDesc mdesc;
		mdesc.dynamicFriction=0.5;
		mdesc.staticFriction=0.3;
		mdesc.restitution=0.5;

		GCPtr<physics::IPhysicMaterial> material=m_phManager->createMaterial(&mdesc);

		physics::TriangleShapeDesc *tDesc=new physics::TriangleShapeDesc[mesh->getBuffersCount()];
		physics::PhysicalNodeDesc nodeDesc;
	//	nodeDesc.globalPos=m_world->getAbsoluteTransformation();
		for(int i=0;i<mesh->getBuffersCount();++i){
			
			tDesc[i].mesh=mesh->getBuffer(i);
			tDesc[i].material=material;
			nodeDesc.shapes.push_back(&tDesc[i]);

		}
		m_phManager->createNode(&nodeDesc);
		nodeDesc.shapes.clear();
		delete [] tDesc;

		return true;
	}
	return false;
}




void Application::update(float dt){
	getSceneManager()->getRenderManager()->addOperation(scene::RGH_Transparent,m_visualizeOP);
	CMRayApplication::update(dt);

	double t0,t1;
	t0=gTimer.getSeconds();
	m_aiSystem->Update(dt);
	t1=gTimer.getSeconds();
	m_aiTime=t1-t0;
	t0=t1;
	m_phManager->update(dt);
	t1=gTimer.getSeconds();
	m_phTime=t1-t0;
	m_gameEntManager->update(dt);

	m_phManager->update(dt);

	if(m_isCameraAttached && m_selectedActor){
		m_camera->position=m_selectedActor->GetPhysicalProp()->GetGlobalPosition();
		m_camera->position().x+=1;
		m_camera->setOrintation(m_selectedActor->GetPhysicalProp()->GetGlobalOrintation());
	}
	//m_interstingMngr->ClearPoint();


	if(m_selectedActor)
	{
		m_tacticalManager->GetVisibleNodesVec(m_selectedActor->GetWorldNode(),m_selectedActor->GetPhysicalProp()->GetGlobalPosition(),m_visibilityVec);
	}
}

void Application::draw()
{
	double t0;
	core::array<AI::TacticalPoint*> tactPoints;
	t0=gTimer.getSeconds();

	CMRayApplication::draw();
	m_tacticalManager->GetNodesFromVector(m_visibilityVec,tactPoints);
	getDevice()->set3DMode();
	for(int i=0;i<tactPoints.size();++i)
	{
		getDevice()->draw3DLine(tactPoints[i]->pos,tactPoints[i]->pos+math::vector3d(0,7,0),video::SColor(255,0,0,255));
	}

	m_renderTime=gTimer.getSeconds()-t0;

	if(m_isCreating)
	{
		getDevice()->set3DMode();
		float N=20;
		float step=360.0f/N;
		math::quaternion oriStep=math::quaternion(step,math::vector3d::YAxis);
		math::quaternion lastOri;
		float Radius=m_currentMousePos.getDist(m_startCreatingPos);
		math::vector3d p1=lastOri*math::vector3d::ZAxis*Radius;
		math::vector3d p2;
		p1.y=0.1;
		for(int i=0;i<N;++i)
		{
			lastOri*=oriStep;
			p2=lastOri*math::vector3d::ZAxis*Radius;
			p2.y=0.1;
			math::line3d l3(m_startCreatingPos+p1,m_startCreatingPos+p2);
			getDevice()->draw3DLine(l3.pStart,l3.pEnd,255);
			p1=p2;
		}
		getDevice()->draw3DLine(m_startCreatingPos+math::vector3d(0,0.1,0),m_currentMousePos+math::vector3d(0,0.1,0),video::DefaultColors::White);
	}

	getDevice()->set2DMode();
	if(m_font){
		GUI::FontAttributes attr;
		attr.fontColor.Set(20,255,128,255);
		attr.fontAligment=GUI::EFA_MiddleLeft;
		attr.fontSize=12;
		attr.hasShadow=true;
		attr.shadowColor.Set(0,0,0,255);
		attr.shadowOffset=vector2d(2);
		attr.spacing=2;
		attr.wrap=0;
		attr.RightToLeft=0;
		core::string msg=mT("FPS= ");
		msg+=core::StringConverter::toString(CFPS::getInstance().getFPS());
		m_font->print(math::rectf(10,10,10,10),&attr,0,msg);
		msg=mT("Primitive drawn : ");
		msg+=core::StringConverter::toString(getDevice()->getPrimitiveDrawnCount());
		m_font->print(math::rectf(10,25,10,10),&attr,0,msg);
		msg=mT("Batches drawn : ");
		msg+=core::StringConverter::toString(getDevice()->getBatchDrawnCount());
		m_font->print(math::rectf(10,40,10,10),&attr,0,msg);
		msg=mT("AI : ");
		msg+=core::StringConverter::toString(m_aiTime);
		m_font->print(math::rectf(10,55,10,10),&attr,0,msg);
		msg=mT("Render : ");
		msg+=core::StringConverter::toString(m_renderTime);
		m_font->print(math::rectf(10,70,10,10),&attr,0,msg);
		msg=mT("PH : ");
		msg+=core::StringConverter::toString(m_phTime);
		m_font->print(math::rectf(10,85,10,10),&attr,0,msg);

		//print help
		if(m_mouseMode==EMM_SetTarget){
			msg=mT("[1]");
			attr.fontColor.Set(255,0,0,255);
		}else {
			attr.fontColor.Set(20,255,128,255);
			msg=mT("1");
		}
		m_font->print(math::rectf(10,110,10,10),&attr,0,msg+mT("- Set Target"));

		if(m_mouseMode==EMM_Create_Agent){
			msg=mT("[2]");
			attr.fontColor.Set(255,0,0,255);
		}else {
			attr.fontColor.Set(20,255,128,255);
			msg=mT("2");
		}
		m_font->print(math::rectf(10,125,10,10),&attr,0,msg+mT("- Create Agent"));

		if(m_mouseMode==EMM_Create_SphereObstacle){
			msg=mT("[3]");
			attr.fontColor.Set(255,0,0,255);
		}else {
			attr.fontColor.Set(20,255,128,255);
			msg=mT("3");
		}

		m_font->print(math::rectf(10,140,10,10),&attr,0,msg+mT("- Create Obstacle"));

		if(m_selectedActor){
			if(m_isCameraAttached)
			{
				msg=mT("[Q]- Detach Camera");
				attr.fontColor.Set(255,0,0,255);
			}else {
				attr.fontColor.Set(20,255,128,255);
				msg=mT("Q- Attach Camera");
			}
			m_font->print(math::rectf(10,155,10,10),&attr,0,msg);
		}

		if(m_selectedActor)
		{
			attr.fontColor.Set(20,255,128,255);
			float height=getDevice()->getViewportRect().getHeight();
			AI::IAIPhysics*ph= m_selectedActor->GetPhysicalProp();
			if(ph)
			{
				msg=mT("Linear Speed=");
				msg+=core::StringConverter::toString(ph->GetLinearVelocity().Length(),2);
				m_font->print(math::rectf(10,height-100,10,10),&attr,0,msg);
				msg=mT("Angular Speed=");
				msg+=core::StringConverter::toString(ph->GetAngularVelocity().y,2);
				m_font->print(math::rectf(10,height-85,10,10),&attr,0,msg);
			}
		}
	}

}
void Application::onDone()
{
	traceFunction(Application);
	array<core::string> arr;
	ResManagersListIT it= ResourceGroup::getInstance().begin();
	ResManagersListIT end= ResourceGroup::getInstance().end();

	for(;it!=end;++it){
		IResourceManager*m=(*it);
		gLogManager.startSection(m->getResourceType());
		core::string msg;
		msg=mT("Used Memory: ");
		msg+=(int)m->getUsedMemory(true);
		gLogManager.log(msg,ELL_INFO);
		m->listResources(arr);
		for(int i=0;i<arr.size();++i){
			msg=arr[i];
			gLogManager.log(msg,ELL_INFO);
		}
		arr.clear();
		gLogManager.endSection(1);

	}
}


GameEntityManager* Application::getEntityManager(){
	return m_gameEntManager;
}
scene::SCameraNode* Application::getCamera(){
	return m_camera;
}

}


