 
#include "stdafx.h"
#include "Application.h"

#include "CMRayApplication.h"
#include "GUIBatchRenderer.h"
#include "GUIManager.h"
#include "ImageSetResourceManager.h"
#include "SParticleSystemFactory.h"
#include "FontResourceManager.h"
#include "ShaderResourceManager.h"
#include "TextureResourceManager.h"
#include "MeshResourceManager.h"
#include "IFileSystem.h"
#include "GUIThemeManager.h"
#include "ViewPort.h"
#include "XMLTree.h"
#include "CFPS.h"
#include "StreamReader.h"
#include "..\common\DemoCamera.h"
#include "GUIOverlay.h"
#include "GUIPanel.h"
#include "ParticleEmitter.h"
#include "CPUParticleBillboardRenderer.h"
#include "GPUParticleBillboardRenderer.h"
#include "GuiElementRegion.h"


#include "RenderPass.h"
#include "RenderTechnique.h"
#include "MeshFileCreatorManager.h"

#include "GUIElement3D.h"

namespace mray
{


	class GUIRenderListener:public scene::IViewportListener
	{
	public:
		GUI::GUIManager* mngr;

		GUIRenderListener()
		{
		}

		virtual void onRenderDone(scene::ViewPort*vp)
		{
			mngr->DrawAll(vp);
		};
	}*g_listener;
	class StatesListener:public scene::IViewportListener
	{
	public:
		Application *app;
		virtual void onRenderDone(scene::ViewPort*vp)
		{
			app->getDevice()->set2DMode();

			// 		if(vp==m_guiViewPort)
			// 			m_guiManager->DrawAll(vp);


			GCPtr<GUI::IFont> font=gFontResourceManager.getDefaultFont();
			if(font){
				app->m_guiRender->Prepare();

				float yoffset=50;

				GUI::FontAttributes attr;
				attr.fontColor.Set(0.05,1,0.5,1);
				attr.fontAligment=GUI::EFA_MiddleLeft;
				attr.fontSize=30;
				attr.hasShadow=true;
				attr.shadowColor.Set(0,0,0,1);
				attr.shadowOffset=math::vector2d(2);
				attr.spacing=2;
				attr.wrap=0;
				attr.RightToLeft=0;
				core::string msg=mT("FPS= ");
				msg+=core::StringConverter::toString(core::CFPS::getInstance().getFPS());
				font->print(math::rectf(10,yoffset+10,10,10),&attr,0,msg,app->m_guiRender);
				yoffset+=attr.fontSize;

				msg=mT("Triangles= ");
				msg+=core::StringConverter::toString(app->getDevice()->getPrimitiveDrawnCount());
				font->print(math::rectf(10,yoffset+10,10,10),&attr,0,msg,app->m_guiRender);
				yoffset+=attr.fontSize;
				app->m_guiRender->Flush();
				app->getDevice()->useShader(0);
			}
		};
	}g_statesListener;

	class GUIOverlayElement:public GUI::IGUIPanelElement
	{
	protected:
		/*
		video::IRenderTargetPtr m_renderTarget;
		video::ITexturePtr m_rtTexture;
	public:

		GUIOverlayElement(GUI::IGUIManager* creator):GUI::IGUIPanelElement(GUID(""),creator)
		{
			m_rtTexture=creator->GetDevice()->createEmptyTexture2D(true);
			m_renderTarget=creator->GetDevice()->createRenderTarget("",m_rtTexture,0,0,0);
		}

		virtual void Draw(scene::ViewPort* vp)
		{
			_UpdateRegion(vp);

			math::vector3d rtSz=m_rtTexture->getSize();

			if(math::vector2d(rtSz.x,rtSz.y)!=GetDefaultRegion()->GetClippedRect().getSize())
			{
				rtSz.x=GetDefaultRegion()->GetClippedRect().getWidth();
				rtSz.y=GetDefaultRegion()->GetClippedRect().getHeight();
				m_rtTexture->createTexture(rtSz,video::EPixel_R8G8B8);
			}

			video::IRenderTarget* rt= video::ShaderSemanticTable::getInstance().getRenderTarget();
			GetCreator()->GetDevice()->setRenderTarget(m_renderTarget);

			video::GLDev* dev=(video::GLDev*)GetCreator()->GetDevice();
		//	dev->setViewportRect(GetDefaultRegion()->GetClippedRect());

			GUI::IGUIPanelElement::Draw(vp);
			GetCreator()->GetDevice()->setRenderTarget(rt);
			video::TextureUnit tu;
			tu.SetTexture(m_rtTexture);
			GetCreator()->GetDevice()->useTexture(0,&tu);
			GetCreator()->GetDevice()->draw2DImage(GetDefaultRegion()->GetClippedRect(),1);

		}*/

	public:

		scene::ISceneNode* m_owner;
		scene::CameraNode* m_cam;

		math::vector3d p1;

		GUIOverlayElement(GUI::IGUIManager* creator):GUI::IGUIPanelElement(GUID(""),creator)
		{
		}
		virtual void Draw(video::IRenderArea*vp)
		{
			_UpdateRegion(vp);
			GUI::IGUIPanelElement::Draw(vp);


			GUI::IGUITheme* skin=GetCreator()->GetActiveTheme();
			GUI::FontAttributes fAttrs;

			GUI::IFont*font=gFontResourceManager.getFontByName(fAttrs.fontName);

			if(font)
			{
				core::string text;
				text=core::StringConverter::toString(p1);
				font->print(GetDefaultRegion()->GetClippedRect(),&fAttrs,0,text,GetCreator()->GetRenderQueue());
			}
		}
	};

	class GUIDelegateListener:public IDelegateContainer
	{
	public:
		GUIDelegateListener()
		{
			ADD_DELEGATE(GUIDelegateListener,ButtonDown);
		}
		DECLARE_OBJECT_DELEGATE( ButtonDown)
		{
			math::vector2d p=*(math::vector2d*)p2;
			printf("Button Down : %s,%f,%f\n",dynamic_cast<GUI::IGUIElement*>(p1)->GetName().c_str(),p.x,p.y);
		}
	}g_guiListener;

	Application::Application()
	{
		ADD_DELEGATE(Application,PrintHello);
	}
	Application::~Application(){}

	void Application::PrintHello( IObjectPtr p1, mray::PVOID p2)
	{
		const char *msg=(const char*)p2;
		printf("%s\n",msg);
	}

	void Application::onEvent(Event* event)
	{
		CMRayApplication::onEvent(event);
		if(m_guiManager)
			m_guiManager->OnEvent(event);

		if(m_guiRtManager)
			m_guiRtManager->OnEvent(event);

		if(event->getType()==ET_ResizeEvent)
		{
			ResizeEvent* e=(ResizeEvent*)event;
		}
		if(event->getType()==ET_Mouse)
		{
			MouseEvent* e=(MouseEvent*)event;
		}
		if(event->getType()==ET_Keyboard)
		{
			KeyboardEvent*e=(KeyboardEvent*) event;
			CallDelegate("PrintHello",0,"this is a test");
		}
	}


	void Application::init(const OptionContainer &extraOptions)
	{
		CMRayApplication::init(extraOptions);

		CMRayApplication::loadResourceFile(mT("Resources.stg"));

		g_listener=new GUIRenderListener();
		g_statesListener.app=this;



		{
			gImageSetResourceManager.loadImageSet(mT("VistaCG.imageset"));
			GCPtr<OS::IStream> themeStream=gFileSystem.createBinaryFileReader(mT("VistaCG.xml"));
			GUI::GUIThemeManager::getInstance().loadTheme(themeStream);
			GUI::GUIThemeManager::getInstance().setActiveTheme(mT("VistaCG"));
		}
		{
			//load font
			GCPtr<GUI::IFont>font=gFontResourceManager.getOrCreate(mT("solo5_font.xml"));
			gFontResourceManager.setDefaultFont(font);
		}


		m_guiRender=new GUI::GUIBatchRenderer();
		m_guiRender->SetDevice(getDevice());

		m_guiManager=new GUI::GUIManager(getDevice());
		m_guiManager->SetActiveTheme(GUI::GUIThemeManager::getInstance().getActiveTheme());

		//////////////////////////////////////////////////////////////////////////
		// Create Scene

		m_guiViewPort=GetRenderWindow()->CreateViewport(mT("GUIVP"),0,0,math::rectf(0,0,1,1),0);

		m_camera=new DemoCamera(getSceneManager(),10,GetInputManager());
		m_viewPort=GetRenderWindow()->CreateViewport(mT("MainVP"),m_camera,0,math::rectf(0,0,1,1),0);
		getSceneManager()->addSceneNode(m_camera);
		
		m_viewPort->AddListener(&g_statesListener);

		GUIOverlayElement* parent=new GUIOverlayElement(m_guiManager);
		m_guiManager->SetRootElement(parent);

		parent->SetPosition(0);
		parent->SetSize(math::vector2d(800,600));

		GUI::IGUIPanelElement* panel=(GUI::IGUIPanelElement*)m_guiManager->CreateElement(GUI::GUIPanel::ElementType);//new GUI::IGUIPanelElement(core::string(""),m_guiManager);
		parent->AddElement(panel);
		panel->SetSize(math::vector2d(300,400));
		parent->SetPosition(0);
		GUI::GUIOverlay* overlay=GUI::GUIOverlayManager::getInstance().LoadOverlay("Overlays/overlay1.gui");
		GUI::ElementList elems= overlay->CreateElements(m_guiManager,panel,&g_guiListener,0/*&m_overlayElement*/);



		m_guiRtManager=new scene::GUIManagerRT();
		for(int i=0;i<3;++i)
		{
			scene::ISceneNode* snode=getSceneManager()->createSceneNode();
			scene::IGUIElementRT* guiRT= m_guiRtManager->CreateFromManager(m_guiManager);
			scene::GUIElement3D* rendNode=new scene::GUIElement3D();
			rendNode->SetElement(guiRT);
			rendNode->setOwner(snode);
			snode->AttachNode(rendNode);
			snode->setScale(10);
			guiRT->SetResolution(i*0.3f);
			snode->setPosition(math::vector3d(0,i*10,0));
		}
#if 0
		video::ITexturePtr rtTexture;
		{
			g_listener->mngr=m_guiManager;

			rtTexture=getDevice()->createEmptyTexture2D(true);
			rtTexture->createTexture(math::vector3d(GetRenderWindow()->GetSize().x,GetRenderWindow()->GetSize().y,1),video::EPixel_R8G8B8A8);
			m_guiViewPort->setRenderTarget(getDevice()->createRenderTarget("",rtTexture,0,0,0));
			//m_guiViewPort->setOnlyToRenderTarget(true);
			m_guiViewPort->AddListener(g_listener);
		}
		scene::MeshRenderableNode* rnode=new scene::MeshRenderableNode(new scene::SMesh());
		snode->AttachNode(scene::MeshFileCreatorManager::getInstance().LoadFromFile(mT("RoboGround.mdx"),true));

		{

			math::vector3d vpos[]={
				math::vector3d(-0.5,0,-0.5),
				math::vector3d( 0.5,0,-0.5),
				math::vector3d( 0.5,0, 0.5),
				math::vector3d(-0.5,0, 0.5)
			};
			math::vector2d vtc[]={
				math::vector2d(1,0),
				math::vector2d( 0,0),
				math::vector2d( 0, 1),
				math::vector2d(1, 1)
			};

			ushort ind[]={0,1,2,0,2,3};
			scene::IMeshBuffer* buf= rnode->getMesh()->addNewBuffer()->getMeshBuffer();
			video::IHardwareStreamBuffer* vStream=buf->createStream(0,video::EMST_Position,video::ESDT_Point3f,4,video::IHardwareStreamBuffer::EUT_Static,true);
			video::IHardwareStreamBuffer* uvStream= buf->createStream(0,video::EMST_Texcoord,video::ESDT_Point2f,4,video::IHardwareStreamBuffer::EUT_Static,true);
			vStream->writeData(0,vStream->getSizeInBytes(),vpos);
			uvStream->writeData(0,uvStream->getSizeInBytes(),vtc);


			video::IHardwareIndexBuffer*indBuff=buf->createIndexBuffer(video::IHardwareIndexBuffer::EIT_16Bit,6,video::IHardwareIndexBuffer::EUT_StaticWriteOnly,false);
			indBuff->writeData(0,indBuff->getSizeInBytes(),ind);
			math::box3d b;
			b.MinP.set(-0.5,0,-0.5);
			b.MaxP.set(0.5,0,0.5);
			buf->setBoundingBox(b);

			buf->setRenderType(video::MR_TRIANGLES);

		}
		snode->AttachNode(rnode);

		parent->m_owner=snode;
		parent->m_cam=m_camera;

		video::RenderMaterial* mtrl=new video::RenderMaterial();
		video::RenderPass* pass= mtrl->CreateTechnique("")->CreatePass("");
		pass->setTexture(rtTexture,0);
		pass->setRenderState(video::RS_Lighting,video::ES_DontUse);
		pass->setRenderState(video::RS_CullFace,video::ES_DontUse);

		rnode->setMaterial(mtrl,0);


#endif
		if(1)
		{
			GCPtr<scene::LightNode> m_light=getSceneManager()->createLightNode(mT(""));

			m_light->setPosition(math::vector3d(100,100,100));
			m_light->setOrintation(math::quaternion(-45,math::vector3d::XAxis));
		}

	}

	void Application::draw(scene::ViewPort* vp)
	{
		m_guiRtManager->DrawRT();
		CMRayApplication::draw(vp);

		m_guiManager->DrawAll(vp);
	}
	void Application::WindowPostRender(video::RenderWindow* wnd)
	{
	}

	void Application::update(float dt)
	{
		CMRayApplication::update(dt);
		m_guiManager->Update(dt);
	}

	void Application::onDone()
	{
		CMRayApplication::onDone();
	}

}
