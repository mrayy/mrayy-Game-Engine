
#include "stdafx.h"
#include "Application.h"
#include "GameEntity.h"
#include "GameEntityManager.h"
#include "../common/DemoCamera.h"


#include "BallsScene.h"
#include "JointsScene.h"
#include "ModelScene.h"
#include "CustomNodeScene.h"
#include "SkeletalAnimScene.h"

#include <PhysXAPI.h>


namespace mray{

	using namespace core;
	using namespace math;


Application::Application()
{
	m_activeExample=0;
}
Application::~Application()
{
}

void Application::onEvent(GCPtr<Event> event)
{
	CMRayApplication::onEvent(event);
	switch(event->getType()){
	case ET_Keyboard:
		{
			GCPtr<KeyboardEvent> e=event;
			if(e->press && e->key==KEY_ESCAPE){
				if(m_activeExample!=NULL){
					m_activeExample->destroyScene();
					m_activeExample=0;
					m_rootGUI->Visible=true;
				}else
					getWindow()->shutdown();
				return;
			}
		}break;
	case ET_GuiEvent:
		{
			GCPtr<GUIEvent> e=event;
			if(e->Event==GET_Mouse_UP){
				GUI::GUIElement *elem=e->Caller;
				int id=elem->Id;
				//check if it's example btn
				int num=id-BaseExampleButtonID;
				if(id>=BaseExampleButtonID && num<m_examples.size())
				{
					IExampleScene*es=m_examples[num];
					m_rootGUI->Visible=false;
					es->createScene();

					m_activeExample=es;
				}
				return;
			}else if(e->Event==GET_Mouse_MOVE){
				GUI::GUIElement *elem=e->Caller;
				int id=elem->Id;
				//check if it's example btn
				int num=id-BaseExampleButtonID;
				if(id>=BaseExampleButtonID && num<m_examples.size())
				{
					IExampleScene*es=m_examples[num];
					m_descriptionTxt->Caption=es->getDescription();
				}
				return;
			}
		}break;
	}
	if(m_activeExample)
		m_activeExample->onEvent(event);
}

void Application::init(core::array<SOptionElement> *extraOptions)
{
	traceFunction(Application);

	matrix4x4 projection;

	CMRayApplication::init(extraOptions);
	CMRayApplication::loadResourceFile(mT("Resources.stg"));


	{
		GCPtr<OS::IStream> themeStream=gFileSystem.createBinaryFileReader(mT("VistaCG.xml"));
		GUI::GUISkinManager::getInstance().loadTheme(themeStream)->SetDevice(getDevice());
		GUI::GUISkinManager::getInstance().setActiveSkin(mT("VistaCG"));

		//load font
		m_font=gFontResourceManager.getOrCreate(mT("solo5_font.xml"));
		gFontResourceManager.setDefaultFont(m_font);
	}

	physics::PhysicsSystemDesc phDesc;
	phDesc.gravity=math::vector3d(0,-98,0);
	m_phManager=physics::createPhysXManager(&phDesc);

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

	//Create Example Scenes

	m_examples.push_back(new BallsScene(this));
	m_examples.push_back(new JointsScene(this));
	m_examples.push_back(new CustomNodeScene(this));
	m_examples.push_back(new ModelScene(this));
	m_examples.push_back(new SkeletalAnimScene(this));


	createGUI();

}

void Application::createGUI(){


	m_rootGUI=new GUI::GUIElement(getGUIManager());
	getGUIManager()->addCustomChild(m_rootGUI);
	m_rootGUI->EnableClip=false;
	m_rootGUI->Dock=GUI::ED_FILL;

	m_exampleSelectWnd=getGUIManager()->addWindow(mT("Samples"),vector2d(100,10),vector2d(300,300),0,m_rootGUI);
	GCPtr<GUI::GUIButton> btn;
	
	float offsetY=10;
	ExampleList::iterator it=m_examples.begin();
	for (int i=0;it!=m_examples.end();++it,++i)
	{
		IExampleScene* scn=*it;
		btn=getGUIManager()->addButton(scn->getName(),vector2d(10,offsetY),vector2d(150,30),this,
			m_exampleSelectWnd,240,BaseExampleButtonID+i);
		offsetY += btn->getSize().y+10;
	}

	m_exampleSelectWnd->MainSize=vector2d(200,offsetY);
	m_exampleSelectWnd->Closable=false;

	m_exampleSelectWnd->addEffector(getGUIManager()->addAnchorEffector(GUI::EPAnchor_CENTER));

	m_descriptionTxt=getGUIManager()->addStaticText(mT(""),0,vector2d(1,40),0,m_rootGUI);
	m_descriptionTxt->getFontAttributes()->fontColor.Set(255,255,200,200);
	m_descriptionTxt->Dock=GUI::ED_BOTTOM;
}




void Application::update(float dt){
	CMRayApplication::update(dt);

	m_phManager->update(dt);
	m_gameEntManager->update(dt);

	if(m_activeExample){
		m_activeExample->update(dt);
	}
}

void Application::draw()
{
	CMRayApplication::draw();

	getDevice()->set2DMode();
	if(m_font){
		GUI::FontAttributes attr;
		attr.fontColor.Set(20,255,128,255);
		attr.fontAligment=GUI::EFA_MiddleLeft;
		attr.fontSize=24;
		attr.hasShadow=true;
		attr.shadowColor.Set(0,0,0,255);
		attr.shadowOffset=vector2d(2);
		attr.spacing=2;
		attr.wrap=0;
		attr.RightToLeft=0;
		core::string msg=mT("FPS= ");
		msg+=core::StringConverter::toString(CFPS::getInstance().getFPS());
		m_font->print(math::rectf(10,10,10,10),&attr,0,msg);
		attr.fontSize=18;
		msg=mT("Primitive drawn : ");
		msg+=core::StringConverter::toString(getDevice()->getPrimitiveDrawnCount());
		m_font->print(math::rectf(10,30,10,10),&attr,0,msg);
		msg=mT("Batches drawn : ");
		msg+=core::StringConverter::toString(getDevice()->getBatchDrawnCount());
		m_font->print(math::rectf(10,50,10,10),&attr,0,msg);
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

