#include "StdAfx.h"
#include "CEngineWindow.h"

#include "demoCamera.h"
#include "Resource.h"
#include "MeshAnimatorManager.h"
#include "EditorGlobals.h"
#include "EditorEventMessages.h"

#include "GameActionFactory.h"
#include "SLensEffect.h"

#include "ParsedShaderPP.h"
#include "PhysxAPI.h"

#include "AIRenderDevice.h"

#include "AIIntializing.h"

#include <Sun.h>

namespace mray
{

using namespace math;
using namespace core;


math::quaternion NLerp(const math::quaternion&rkP,const math::quaternion&rkQ,float t,bool shortestPath=true)
{

	math::quaternion result;
	float fCos = rkP.dotProduct(rkQ);
	if (fCos < 0.0f && shortestPath)
	{
		result = rkP +  ((-rkQ) - rkP) * t;
	}
	else
	{
		result = rkP + (rkQ - rkP) * t;
	}
	result.Normalize();
	return result;
}

float getPitch(const math::quaternion&q)
{

	float fTx  = 2.0*q.x;
	float fTy  = 2.0*q.y;
	float fTz  = 2.0*q.z;
	float fTwx = fTx*q.w;
	float fTxx = fTx*q.x;
	float fTyz = fTz*q.y;
	float fTzz = fTz*q.z;

	// Vector3(fTxy-fTwz, 1.0-(fTxx+fTzz), fTyz+fTwx);
	return atan2((float)(fTyz+fTwx), 1.0f-(fTxx+fTzz));
}

float getYaw(const math::quaternion&q)
{

	// yaw = atan2(localz.x, localz.z)
	// pick parts of zAxis() implementation that we need
	float fTx  = 2.0*q.x;
	float fTy  = 2.0*q.y;
	float fTz  = 2.0*q.z;
	float fTwy = fTy*q.w;
	float fTxx = fTx*q.x;
	float fTxz = fTz*q.x;
	float fTyy = fTy*q.y;

	// Vector3(fTxz+fTwy, fTyz-fTwx, 1.0-(fTxx+fTyy));

	return atan2(fTxz+fTwy, 1.0f-(fTxx+fTyy));
}
float getRoll(const math::quaternion&q)
{

	float fTx  = 2.0*q.x;
	float fTy  = 2.0*q.y;
	float fTz  = 2.0*q.z;
	float fTwz = fTz*q.w;
	float fTxy = fTy*q.x;
	float fTyy = fTy*q.y;
	float fTzz = fTz*q.z;

	// Vector3(1.0-(fTyy+fTzz), fTxy+fTwz, fTxz-fTwy);

	return atan2(fTxy+fTwz, 1.0f-(fTyy+fTzz));
}
CEngineWindow::CEngineWindow(void)
{
	m_selectedNode=0;
	m_eventManager=new EditorEventManager();
	m_pickBuffer=0;
	m_pickBufferTU=new video::TextureUnit();

	m_billboardTU=new video::TextureUnit();

	m_manipulator=new scene::TransformManipulator();


	m_textLogger=new TextLogDevice();


	m_requestClean=false;
	m_requestLoad=false;

	math::matrix4x4 m1,m2;
	m1.setRotationDegrees(math::vector3d(-180,-3,-180));
	m2.setRotationDegrees(math::vector3d(-180,-3,-220));

	math::quaternion q1(m1);
	math::quaternion q2(m2);

	math::vector3d a1,a2;
	q1.toEulerAngles(a1);
	q2.toEulerAngles(a2);
	for(int i=0;i<100;++i){
		float w1=i*0.01f;
		float w2=1-w1;

		math::quaternion q11,q21;
		q11=NLerp(math::quaternion::Identity,q1,w1,false);
		q21=NLerp(math::quaternion::Identity,q2,w2,false);

		a1.x=math::toDeg(q11.getPitch());
		a1.y=math::toDeg(q11.getYaw());
		a1.z=math::toDeg(q11.getRoll());

		a2.x=math::toDeg(q21.getPitch());
		a2.y=math::toDeg(q21.getYaw());
		a2.z=math::toDeg(q21.getRoll());


		math::quaternion qq(a1+a2);

		math::vector3d aa;
		qq.toEulerAngles(aa);

		math::quaternion q;
		q=q21*q11;
		math::vector3d angle;
		q.toEulerAngles(angle);
	}

}

CEngineWindow::~CEngineWindow(void)
{
	m_gmLevel=0;
	m_phManager=0;
	m_soundManager=0;
	AI::AIIntializing::ShutdownAI();
}



void CEngineWindow::InitApp()
{
	m_engine->loadResourceFile(mT("datapath.stg"));
	m_engine->loadPlugins(mT("plugins.stg"));
	m_engine->loadResourceFile(mT("Resources.stg"));

	gLogManager.addLogDevice(m_textLogger);

	{
		GCPtr<OS::IStream> themeStream=gFileSystem.createBinaryFileReader(mT("Default.xml"));
		GUI::GUISkinManager::getInstance().loadTheme(themeStream)->SetDevice(m_device);
		GUI::GUISkinManager::getInstance().setActiveSkin(mT("Default"));

		//load font
		GCPtr<GUI::IFont>font=gFontResourceManager.getOrCreate(mT("Tahoma_font.xml"));
		gFontResourceManager.setDefaultFont(font);
	}
	m_gmLevel=new game::GameLevel();
	m_gmLevel->SetSceneManager(m_smngr);
	m_gmLevel->SetEditorMode(true);

	m_soundManager=sound::SoundManagerFactory::getInstance().createSoundManager(mT("fmod"));
	m_gmLevel->SetSoundManager(m_soundManager);




	m_camera=new DemoCamera(m_smngr,10);
	m_camera->ZFar=3000;
	m_smngr->addSceneNode(m_camera);

	m_viewPort=m_smngr->createViewport(mT(""),0);
	m_viewPort->setCamera(m_camera);

	if(m_soundManager){
		m_soundManager->getListener()->attachToListener(m_camera);
	}
/*
	GCPtr<video::SMaterial> mat;
	mat=gMaterialResourceManager.getMaterial(mT("ShadowedMaterial"));
	GCPtr<scene::ISceneNode>node=m_smngr->addPlane(1,1,math::vector3d(0,0,0),math::quaternion::Identity,2000);
	node->ShadowCaster=false;
	node->ShadowReciver=true;
	node->setMaterial(mat,0);*/

	scene::MeshAnimatorManager::getInstance().SetGPUSkeletonAnimatorParams(3,4);

	m_pickBuffer=new video::RTPickBuffer(m_device,m_device->getViewportRect().getSize()/4,m_device->getViewportRect().getSize());


	scene::SkyBoxManager::getInstance().loadSkyFromFolder(mT("SkyBox\\boat\\"),mT("png"),mT("boat"));
	{
		GCPtr<video::ITexture> rtTex=m_device->createTexture2D(m_viewPort->getAbsViewPort().getSize(),video::EPixel_R8G8B8A8);
		GCPtr<video::IRenderTarget> rt=m_device->createRenderTarget(mT(""),rtTex,0,0,0);
		m_viewPort->enablePostProcessing(true);
/*

		GCPtr<video::ParsedShaderPP> pp=new video::ParsedShaderPP(m_device);

		GCPtr<OS::IStream> stream=gFileSystem.openFile(mT("BloomSky.xml"),OS::TXT_READ);
		pp->LoadXML(stream);
*/
/*
		GCPtr<video::SkyBloomPP> pp=new video::SkyBloomPP(m_device,128);
*/		m_gmLevel->GetEnvironment()->SetPostProcessingName(mT("BloomSky.peff"));

		m_viewPort->setPostProcessing(m_gmLevel->GetEnvironment()->GetPostProcessingEffect());
		m_viewPort->setRenderTarget(rt);
	}
	m_manipulator->Init(mT("TranslateM.mdl"),mT("RotateM.mdl"),mT("ScaleM.mdl"),m_smngr,m_camera);

	EventQueue::getInstance().addEventHandler(this,BIT(ET_Mouse) | BIT(ET_Keyboard)| BIT(ET_SystemEvent));


	physics::PhysicsSystemDesc phDesc;
	phDesc.gravity.set(0,-9.8,0);
	m_phManager=physics::createPhysXManager(&phDesc);
	m_gmLevel->SetPhysicsManager(m_phManager);
	m_gmLevel->SetGUIManager(m_gmngr);
	m_gmLevel->SetCamera(m_camera);

	m_gmLevel->InitManagers();




	
	m_gmLevel->GetAISystem()->EnableVisualize(true);
	m_gmLevel->GetAISystem()->GetNavigationWorld()->EnableVisualize(true);


	EditorGlobals::IsEngineInitiated=true;
	EditorEventManager::getInstance().FireEvent(EEE_OnEngineInitiated);


	scene::SLensEffect* eff=new scene::SLensEffect(m_smngr,math::vector3d(100));
	m_smngr->addSceneNode(eff);

	scene::ISceneNode* sunNode=m_smngr->addSphere(100,20,20);
	sunNode->addChild(eff);
	sunNode->setMaterial(gMaterialResourceManager.getMaterial(mT("SunMaterial")));

	m_gmLevel->GetEnvironment()->GetSun()->AttachNode(sunNode);

	SetSelectedNode(0);

}

void CEngineWindow::OnRender()
{
	MFCEngineControl::OnRender();

	std::list<IToolBox*>::iterator it= EditorGlobals::Toolboxes.begin();
	for (;it!= EditorGlobals::Toolboxes.end();++it)
	{
		(*it)->OnRender();
	}

	m_device->set3DMode();
	if(m_selectedNode)
	{
		m_device->drawBox(m_selectedNode->getTransformedBoundingBox(),math::matrix4x4::Identity,video::MR_LINES);
	}

	m_device->set2DMode();

	GCPtr<GUI::IFont> font=gFontResourceManager.getDefaultFont();
	if(font){
		GUI::FontAttributes attr;
		attr.fontColor.Set(230,230,230,255);
		attr.fontAligment=GUI::EFA_MiddleLeft;
		attr.fontSize=14;
		attr.hasShadow=true;
		attr.shadowColor.Set(0,0,0,255);
		attr.shadowOffset=vector2d(2);
		attr.spacing=2;
		attr.wrap=0;
		attr.RightToLeft=0;
		core::string msg;
		msg=mT("Batch Count=");
		msg+=core::StringConverter::toString(m_device->getBatchDrawnCount());
		msg+=mT("- Primitive Count=");
		msg+=core::StringConverter::toString(m_device->getPrimitiveDrawnCount());

		msg+=mT("- FPS=");
		msg+=core::StringConverter::toString(gFPS.getFPS());
		font->print(math::rectf(10,10,10,10),&attr,0,msg);
	}/**/
}

void CEngineWindow::OnUpdate(float dt)
{
	MFCEngineControl::OnUpdate(dt);

	m_manipulator->Update();
	m_gmLevel->Update(dt);
	m_soundManager->runSounds(dt);

	if(m_requestClean)
	{
		m_requestClean=false;
		_OnNewScene();
	}
	if(m_requestLoad)
	{
		m_requestLoad=false;
		_OnLoadScene(m_requestLevelName);
	}
}

void CEngineWindow::onEvent(GCPtr<Event> event)
{
	m_gmngr->onEvent(event);
	switch (event->getType())
	{
	case ET_SystemEvent:
		{
			GCPtr<SystemEvent> e=event;
			if(e->m_eventType==ESET_Resized){
				m_viewPort->setAbsViewPort(GetDevice()->getViewportRect());
				m_manipulator->OnViewportResize();
				m_pickBuffer->SetViewportSize(GetDevice()->getViewportRect().getSize());
				m_pickBuffer->SetRenderTargetSize(GetDevice()->getViewportRect().getSize()/2);
			}
			break;
		}
	case ET_Mouse:
		{
			if(!m_gmLevel->IsEditorMode())
				break;
			math::vector2d mousePos;
			MouseEvent*e=(MouseEvent*)event.pointer();
			mousePos.x=e->x;
			mousePos.y=e->y;
			if(e->Event==MET_LEFTDOWN)
			{
				OnLMouseDown(mousePos.x,mousePos.y);
			}else if(e->Event==MET_MOVED)
			{
				m_manipulator->OnMouseMoved(mousePos.x,mousePos.y);
			}else if(e->Event==MET_LEFTUP)
			{
				m_manipulator->OnMouseReleased();
				EditorEventManager::getInstance().FireEvent(EEE_OnNodeTrasnformingDone);
			}else if(e->Event==MET_RIGHTDOWN)
			{
				m_manipulator->OnRMousePressed();
			}else if(e->Event==MET_MIDDLEDOWN)
			{
				m_manipulator->OnMouseMiddlePressed(mousePos.x,mousePos.y);
			}else if(e->Event==MET_MIDDLEUP)
			{
				m_manipulator->OnMouseReleased();
				EditorEventManager::getInstance().FireEvent(EEE_OnNodeTrasnformingDone);
			}
		}break;
	case ET_Keyboard:
		{
			if(!m_gmLevel->IsEditorMode())
				break;
			KeyboardEvent*e=(KeyboardEvent*)event.pointer();
			if(e->press)
			{
				if(e->key==KEY_Q)
				{
					m_manipulator->SetActiveManipulator(scene::TransformManipulator::EMT_None);
					EditorEventManager::getInstance().FireEvent(EEE_OnNodeTrasnformingDone);
				}else if(e->key==KEY_1)
				{
					m_manipulator->SetActiveManipulator(scene::TransformManipulator::EMT_Translate);
					EditorEventManager::getInstance().FireEvent(EEE_OnNodeTrasnformingDone);
				}else if(e->key==KEY_2)
				{
					m_manipulator->SetActiveManipulator(scene::TransformManipulator::EMT_Rotate);
					EditorEventManager::getInstance().FireEvent(EEE_OnNodeTrasnformingDone);
				}else if(e->key==KEY_3)
				{
					m_manipulator->SetActiveManipulator(scene::TransformManipulator::EMT_Scale);
					EditorEventManager::getInstance().FireEvent(EEE_OnNodeTrasnformingDone);
				}else if(e->key==KEY_DELETE)
				{
					if(!m_selectedNode)
						break;
					if(MessageBox(mT("Are your sure to remove the node?"),mT("Delete"),MB_YESNO)==IDYES)
					{
						RemoveObject(m_selectedNode);
						SetSelectedNode(0);
					}
				}
			}
		}break;
	}
}

void CEngineWindow::RemoveObject(scene::ISceneNode*node)
{
	int id=m_gmLevel->GetEntityManager()->GetSceneNodeEntID(node->getID());
	if(id!=0)
	{
		m_gmLevel->GetEntityManager()->RemoveGameEntity(id);
	}
	m_smngr->removeChild(node);
	m_gmLevel->GetRootSceneNode()->removeChild(node);
}

void CEngineWindow::SetSelectedNode(scene::ISceneNode*node)
{
	m_selectedNode=node;
	OnNodeModel();
}
game::GameLevel* CEngineWindow::GetLevel()
{
	return m_gmLevel;
}
TextLogDevice* CEngineWindow::getTextLogger()
{
	return m_textLogger;
}
scene::ISceneNode* CEngineWindow::GetSelectedNode()
{
	return m_selectedNode;
}

void CEngineWindow::OnLMouseDown(int x,int y)
{
	if(m_manipulator->OnMousePressed(x,y))
		return;

	m_pickBuffer->Clear();
	m_pickBuffer->SetViewMatrix(m_camera->getViewMatrix());
	m_pickBuffer->SetProjectionMatrix(m_camera->getProjectionMatrix());

	core::IteratorPair<scene::NodeList> it= m_gmLevel->GetRootSceneNode()->getChildren();
	for(;!it.done();++it)
	{
		GCPtr<scene::ISceneNode> node=*it;
		if(!node)
			continue;
		scene::SMesh*mesh=node->getMesh();
		if(!mesh)
			continue;
		m_pickBuffer->PushPickID(node->getID());
		for(int i=0;i<mesh->getBuffersCount();++i){
			m_pickBuffer->AddRenderObject(mesh->getBufferData(i),node);
		}
	}

	m_pickBuffer->Render();

	uint id= m_pickBuffer->GetID(x,y);
	if(id==0){
		SetSelectedNode(0);
	}else
	{
		it= m_gmLevel->GetRootSceneNode()->getChildren();
		for(;!it.done();++it)
		{
			GCPtr<scene::ISceneNode> node=*it;
			if(!node)
				continue;
			if(node->getID()==id)
			{
				SetSelectedNode(node);
				break;
			}
		}
	}

	/*
	math::line3d ray=scene::SceneHelper::getInstance().getRayFromScreenCoord(mousePos,m_camera);
	GCPtr<scene::ISceneNode> node= scene::SceneHelper::getInstance().getNodeFromRay(m_root->getChildren(),ray,0,~(scene::EST_SKY | scene::EST_EDITOR));
	if(m_selectedNode)
	{
		if(node!=m_selectedNode)
		{
		}
	}
	SetSelectedNode(node);*/
}

GCPtr<scene::TransformManipulator>  CEngineWindow::GetManipulator()
{
	return m_manipulator;
}

void CEngineWindow::ClearScene()
{
	SetSelectedNode(0);
	//m_smngr->removeChild(m_root);

}

void CEngineWindow::_OnNewScene()
{
	ClearScene();
	m_gmLevel->ClearLevel();
	m_gmLevel->SetEditorMode(true);
	EditorEventManager::getInstance().FireEvent(EEE_OnCleanLevel);

}
void CEngineWindow::OnNewScene()
{
	m_requestClean=true;
}
void CEngineWindow::OnLoadModel(const core::string&name,const core::string& ext)
{
	/*
	if(m_selectedNode){
		GetSceneManager()->removeChild(m_selectedNode);
		m_selectedNode=0;
	}*/
	//if(ext==mT("mdl")){
		core::string app=gFileSystem.getAppPath();
		app.replace('/','\\');
		core::string fileName=gFileSystem.makePathRelative(name,app);

		m_selectedNode=GetSceneManager()->addAnimatedMesh(fileName.c_str());
		if(m_selectedNode){
			GetSceneManager()->UnParent(m_selectedNode);
			m_selectedNode->position=m_camera->position()+m_camera->getOrintation()*math::vector3d::ZAxis*2;
		}
	/*	
	}else if(ext==mT("mdx"))
	{
		GCPtr<OS::IStream> stream= gFileSystem.openFile(name,OS::BIN_READ);
		xml::XMLTree tree;
		if(tree.load(stream))
		{
			xml::XMLElement*e=tree.getSubElement(mT("ISceneNode"));
			if(e){
				xml::XMLAttribute*attr=e->getAttribute(mT("Type"));
				if(attr){
					m_selectedNode=m_smngr->getFactory()->createNode(attr->value.c_str());
					m_selectedNode->loadXMLSettings(e);
					if(m_selectedNode){
						GCPtr<scene::SLODAnimatedMesh> mesh=m_selectedNode;
						if(mesh){
						}
						GetSceneManager()->addSceneNode(m_selectedNode);
						GetSceneManager()->UnParent(m_selectedNode);
					}
				}
			}
		}
		
			//GetSceneManager()->addSceneNode(m_selectedNode);
		stream->close();
	}*/
	if(m_selectedNode){
		int i=name.findlast('\\');
		core::string nodeName;
		if(i!=-1)
			nodeName=name.substr(i+1,name.length()-i);
		else
			nodeName=name;
		m_selectedNode->setNodeName(nodeName);
		m_gmLevel->GetRootSceneNode()->addChild(m_selectedNode);
		EditorEventManager::getInstance().FireEvent(EEE_OnNodeAdded);
	}
	OnNodeModel();
}


void CEngineWindow::GenerateTerrain(GCPtr<loaders::IHeightMapLoader> hm,const math::vector3d &scale,GCPtr<video::ITexture> base,GCPtr<video::ITexture> baseNormal,
									GCPtr<video::ITexture> detail,GCPtr<video::ITexture> detailNormal,float lod,float lodDist)
{
	m_gmLevel->GenerateTerrain(hm,scale,lod,lodDist,base,baseNormal,detail,detailNormal);
}

void CEngineWindow::OnNodeModel()
{
	if(m_selectedNode)
	{
		m_selectedNode->DrawDebugObjects=true;
		m_selectedNode->DrawBoundingBox=true;
		CreateBillboardRT();
	}

	m_manipulator->SetNode(m_selectedNode);
	EditorEventManager::getInstance().FireEvent(EEE_OnModelSelected);

	ClearSettings();
}


void CEngineWindow::ClearSettings()
{

}


scene::ISceneNode* CEngineWindow::GetRootNode()
{
	return m_gmLevel->GetRootSceneNode();
}



void CEngineWindow::EnableShadows(bool e)
{
	if(!m_selectedNode)
		return;
	m_selectedNode->ShadowCaster=e;
	m_selectedNode->ShadowReciver=e;

}

void CEngineWindow::_OnLoadScene(const core::string& path)
{
	ClearScene();

	m_gmLevel->LoadLevel(path);

	EditorEventManager::getInstance().FireEvent(EEE_OnLevelLoaded);
	EditorEventManager::getInstance().FireEvent(EEE_OnNodeAdded);
}
void CEngineWindow::OnLoadScene(const core::string& path)
{
	m_requestLoad=true;
	m_requestLevelName=path;
}

void CEngineWindow::OnExportScene(const core::string& path)
{
	m_gmLevel->SetEditorMode(true);
	m_gmLevel->ExportLevel(path);
}


void CEngineWindow::CreateBillboardRT()
{
	if(!m_selectedNode)
		return;
	return;
	m_device->setRenderTarget(m_billboardRT);

	math::matrix4x4 viewmat;
	viewmat.buildCameraLookAtRH(m_selectedNode->getPosition()+math::vector3d(0,0,30),m_selectedNode->getPosition(),math::vector3d::YAxis);
	m_device->set3DMode();
	m_device->setTransformationState(video::TS_PROJECTION,m_camera->getProjectionMatrix());
	m_device->setTransformationState(video::TS_VIEW,viewmat);
	m_selectedNode->directRender();

	m_device->setRenderTarget(0);
}

}
