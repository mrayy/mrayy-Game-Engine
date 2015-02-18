
#include "stdafx.h"
#include "Application.h"
#include "StreamLogger.h"
#include "SkyBoxManager.h"
#include "SSkyBoxNode.h"
#include "FontAttributes.h"
#include "HeightMapRaw16.h"
#include "ThreadJobManager.h"
#include "MeshAnimatorManager.h"
#include "MeshFileCreatorManager.h"
#include "TangentCalculater.h"
#include "TextureResourceManager.h"
#include "FontResourceManager.h"
#include "GUIThemeManager.h"
#include "ImageSetResourceManager.h"

#include "PhysicalModel/Segment.h"
#include "..\common\DemoCamera.h"
#include "PhysXAPI.h"


Application::Application()
{
}
Application::~Application()
{
}

void Application::onEvent(Event*event)
{
	CMRayApplication::onEvent(event);
}
scene::SPatchTerrain* Application::loadTerrain()
{
	scene::SPatchTerrain* m_terrain=0;

	GCPtr<loaders::HeightMapRaw16> l=new loaders::HeightMapRaw16();
	l->loadHeightMap(gFileSystem.openFile(mT("terrain.raw")));
	m_terrain=new scene::SPatchTerrain(0,math::vector3d(2500,30,2500));
	m_terrain->loadTerrain(l,7,0.05,false);
	m_terrain->getMaterial()->setTexture(gTextureResourceManager.loadTexture2D(mT("terrain.jpg")),0);
	m_terrain->getMaterial()->setTexture(gTextureResourceManager.loadTexture2D(mT("detailmap.bmp")),2);

	scene::ISceneNode*node=getSceneManager()->createSceneNode();
	node->AttachNode(m_terrain);
	//node->setVisible(false);
	/*
	m_terrain->setBaseTexture(gTextureResourceManager.loadTexture2D(mT("terrain.jpg")));
	m_terrain->setDetailTexture(gTextureResourceManager.loadTexture2D(mT("detail_mud.jpg")));
	m_terrain->setDetailNormal(gTextureResourceManager.loadTexture2D(mT("detail_mud.jpg")));*/

	//	m_terrainUtil=new scene::TerrainUtility(m_terrain);

	//	m_cursour=scene::SceneNodeCreator::addSphere(getSceneManager(),1,5,5);

	physics::TerrainShapeDesc terrDesc(l->getWidth(),l->getHeight(),l->getHeightData(),m_terrain->GetScale());
	physics::PhysicalNodeDesc phNode;
	phNode.actorGroupID=1;
	phNode.shapes.push_back(&terrDesc);
	m_physicManager->createNode(&phNode);

	return m_terrain;
}
void Application::init(const OptionContainer &extraOptions)
{
	CMRayApplication::init(extraOptions);
	CMRayApplication::loadResourceFile(mT("Resources.stg"));

	GCPtr<StreamLogger> logger=new StreamLogger(1);
	logger->setStream(gFileSystem.createTextFileWriter(gFileSystem.getAppPath()+ mT("log.txt")));

	gLogManager.addLogDevice(logger);

	{

		gImageSetResourceManager.loadImageSet("VistaCG.imageset");
		GCPtr<OS::IStream> themeStream=gFileSystem.createBinaryFileReader(mT("VistaCG.xml"));
		GUI::GUIThemeManager::getInstance().loadTheme(themeStream);
		GUI::GUIThemeManager::getInstance().setActiveTheme(mT("VistaCG"));

		//m_guiManager->SetActiveTheme(GUI::GUIThemeManager::getInstance().getActiveTheme());

		//load font
		GCPtr<GUI::IFont>font=gFontResourceManager.getOrCreate(mT("Tahoma_font.xml"));
		gFontResourceManager.setDefaultFont(font);
	}

	physics::PhysicsSystemDesc desc;
	desc.gravity.set(0,-9.8,0);
	m_physicManager=physics::createPhysXManager(&desc);
	m_physicManager->ConnectToRemoteDebugger();


	m_camera=new DemoCamera(getSceneManager(),100,GetInputManager());
	getSceneManager()->addSceneNode(m_camera);

	m_viewPort=GetRenderWindow()->CreateViewport(mT("MainVP"),m_camera,0,math::rectf(0,0,1,1),0);


	m_guiRender=new GUI::GUIBatchRenderer();
	m_guiRender->SetDevice(getDevice());

	loadTerrain();

	scene::LightNode* light=getSceneManager()->createLightNode();
	light->setPosition(math::vector3d(30,400,40));
	light->setRadius(100);
	light->setAmbient(100);

	{

		GCPtr<video::ITexture> tex= scene::SkyBoxManager::getInstance().loadSkyFromFolder(mT("skybox\\night\\"),mT("png"),mT("night"));
		GCPtr<scene::IRenderable> sky=new scene::SSkyBoxNode(tex);//scene::SceneNodeCreator::addSkyBox(getSceneManager(),tex);
		sky->SetHasCustomRenderGroup(true);
		sky->SetTargetRenderGroup(scene::RGH_Skies);
		scene::ISceneNode*node= getSceneManager()->createSceneNode();
		node->AttachNode(sky);
		node->setCullingType(scene::SCT_NONE);
	}
	{
		//load character model
		//scene::MeshAnimatorManager::getInstance().SetGPUSkeletonAnimatorParams(3,4);
		scene::MeshRenderableNode* modelNode= scene::MeshFileCreatorManager::getInstance().LoadFromFile("sportive03_m_mediumpoly.mdx",true);

		scene::ISceneNode*m_modelNode;

		m_modelNode=getSceneManager()->createSceneNode();
		m_modelNode->AttachNode(modelNode);
		m_modelNode->setScale(10);
		m_modelNode->setPosition(math::vector3d(0,5,0));

// 		scene::TangentCalculater t(1,2);
// 		t.calculateTangents(modelNode->getMesh());

		m_characterManager=new PhysicsBasedCharacter::CharacterManager();
		m_characterManager->SetPhysicsManager(m_physicManager);

		PhysicsBasedCharacter::Character* charc= m_characterManager->CreateCharacter(mT("Character\\Human2.xml"),modelNode->getMesh()->getSkeleton(),m_modelNode->getScale().x,m_modelNode->getPosition());

		PhysicsBasedCharacter::Segment*seg= charc->getSegment(mT("Head"));
		if(seg)
		{
			seg->getActor()->SetBodyFlag(physics::EBF_Kinematic,true);
		}
	}

}
void Application::update(float dt)
{
	CMRayApplication::update(dt);
	m_physicManager->update(dt);
	m_characterManager->Update(dt);

	((scene::SceneManager*)getSceneManager())->GetJobsManager()->EndProcessing();

}
void Application::draw(scene::ViewPort* vp)
{
	CMRayApplication::draw(vp);
	GUI::IFont* font=gFontResourceManager.getDefaultFont();

	if(font)
	{
		getDevice()->set2DMode();
		m_guiRender->Prepare();

		GUI::FontAttributes attr;
		attr.fontColor.Set(20,255,128,255);
		attr.fontAligment=GUI::EFA_MiddleLeft;
		attr.fontSize=20;
		attr.hasShadow=true;
		attr.shadowColor.Set(0,0,0,255);
		attr.shadowOffset=math::vector2d(2);
		attr.spacing=2;
		attr.wrap=0;
		attr.RightToLeft=0;



		core::string msg=mT("FPS= ");
		msg+=core::StringConverter::toString(core::CFPS::getInstance().getFPS());
		font->print(math::rectf(10,10,10,10),&attr,0,(utf8*)msg.c_str(),m_guiRender);
		attr.fontSize=14;
		msg=mT("Primitive drawn : ");
		msg+=core::StringConverter::toString(getDevice()->getPrimitiveDrawnCount());
		font->print(math::rectf(10,30,10,10),&attr,0,(utf8*)msg.c_str(),m_guiRender);
		msg=mT("Batches drawn : ");
		msg+=core::StringConverter::toString(getDevice()->getBatchDrawnCount());
		attr.spacing=0;
		font->print(math::rectf(10,50,10,10),&attr,0,(utf8*)msg.c_str(),m_guiRender);

		m_guiRender->Flush();

	}
}
void Application::onDone()
{
}

