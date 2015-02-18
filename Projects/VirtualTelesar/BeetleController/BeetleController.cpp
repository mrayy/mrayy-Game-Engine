// BeetleController.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "OptiTrackClient.h"
#include "ITHreadManager.h"
#include "CarController.h"
#include "ImageSetResourceManager.h"
#include "GUIThemeManager.h"
#include "FontResourceManager.h"
#include "GUIBatchRenderer.h"

#include "win32NetInterface.h"

namespace mray
{
class Application:public CMRayApplication,public animation::IOptiTrackClientListener
{
private:
protected:
	GCPtr<animation::OptiTrackClient> m_optiTrack;
	CarController m_carController;
	scene::ViewPort* m_vp;

	GCPtr<GUI::GUIBatchRenderer> m_guiRender;
public:
	Application(){}
	virtual~Application(){}

	virtual void onEvent(Event*event )
	{
		if(event->getType()==ET_Keyboard)
		{
			KeyboardEvent* e=(KeyboardEvent*)event;
			if(e->press)
			{
				switch(e->key)
				{
				case KEY_C:
					m_carController.Init();
					m_carController.StartCalibrating(3);
					break;
				case KEY_S:
					m_carController.Stop();
					break;
				case KEY_A:
					m_carController.OnBodyDataReceived(0,math::quaternion(45,0,0));
					break;
				case KEY_D:
					m_carController.OnBodyDataReceived(0,math::quaternion(-45,0,0));
					break;
				case KEY_W:
					m_carController.OnBodyDataReceived(0,math::quaternion(0,45,0));
					break;
				case KEY_Z:
					m_carController.OnBodyDataReceived(0,math::quaternion(0,-45,0));
					break;
				case KEY_Q:
					m_carController.OnBodyDataReceived(0,math::quaternion(0,0,45));
					break;
				case KEY_E:
					m_carController.OnBodyDataReceived(0,math::quaternion(0,0,-45));
					break;
				}
			}
		}
	}

	virtual void init(const OptionContainer &extraOptions)
	{
		CMRayApplication::init(extraOptions);

		CMRayApplication::loadResourceFile(mT("Resources.stg"));


		gImageSetResourceManager.loadImageSet(mT("VistaCG.imageset"));
		GCPtr<OS::IStream> themeStream=gFileSystem.createBinaryFileReader(mT("VistaCG.xml"));
		GUI::GUIThemeManager::getInstance().loadTheme(themeStream);
		GUI::GUIThemeManager::getInstance().setActiveTheme(mT("VistaCG"));

		//load font
		GCPtr<GUI::IFont>font=gFontResourceManager.loadFont(mT("TEKTONPRO_font.fnt"));
		gFontResourceManager.setDefaultFont(font);

		network::createWin32Network();

		m_vp=GetRenderWindow()->CreateViewport("vp",0,0,math::rectf(0,0,1,1),0);

		m_optiTrack=new animation::OptiTrackClient();
		//m_optiTrack->Connect(mT("192.168.10.129"),mT("192.168.10.129"),mT("239.255.42.99"));

		m_optiTrack->AddListener(this);


		m_guiRender=new GUI::GUIBatchRenderer();
		m_guiRender->SetDevice(getDevice());

	}

	virtual void draw(scene::ViewPort* vp)
	{
	}
	virtual void WindowPostRender(video::RenderWindow* wnd)
	{
		GCPtr<GUI::IFont> font=gFontResourceManager.getDefaultFont();
		if(font){
			m_guiRender->Prepare();

			float yoffset=50;


			GUI::FontAttributes attr;
			attr.fontColor.Set(0.05,1,0.5,1);
			attr.fontAligment=GUI::EFA_MiddleLeft;
			attr.fontSize=24;
			attr.hasShadow=true;
			attr.shadowColor.Set(0,0,0,1);
			attr.shadowOffset=math::vector2d(2);
			attr.spacing=2;
			attr.wrap=0;
			attr.RightToLeft=0;
			core::string msg=mT("FPS= ");
			msg+=core::StringConverter::toString((int)core::CFPS::getInstance().getFPS());
			font->print(math::rectf(20,20,10,10),&attr,0,msg,m_guiRender);
			yoffset+=attr.fontSize;


			if(m_carController.IsIdle())
			{
				msg="Idle: Press 'C' to Calibrate";
				font->print(math::rectf(20,20+yoffset,10,10),&attr,0,msg,m_guiRender);
				yoffset+=attr.fontSize;
			}else if(m_carController.IsCalibrating())
			{
				msg="Calibrating: Please Move your head to detect movement area";
				font->print(math::rectf(20,20+yoffset,10,10),&attr,0,msg,m_guiRender);
				yoffset+=attr.fontSize;
			}else if(m_carController.IsControlling())
			{
				msg="Controlling!";
				font->print(math::rectf(20,20+yoffset,10,10),&attr,0,msg,m_guiRender);
				yoffset+=attr.fontSize;
			}

		}

		m_guiRender->Flush();
		getDevice()->useShader(0);
	}
	virtual void update(float dt)
	{
		m_carController.Update(dt);
		OS::IThreadManager::getInstance().sleep(30);
	}
	virtual void onDone()
	{

	}

	void OnOptiTrackData(animation::OptiTrackClient*client,const animation::OptiTrackRigidBody& body)
	{
		m_carController.OnBodyDataReceived(body.GetPosition(),body.GetOrintation());
	}

};


}

using namespace mray;
using namespace core;
using namespace math;


int _tmain(int argc, _TCHAR* argv[])
{
	GCPtr<Application> app=new Application();
	GCPtr<Engine> e=Engine::getInstancePtr();

	core::string resFileName=mT("plugins.stg");

#ifdef UNICODE
	resFileName=mT("pluginsU.stg");
#endif

	std::vector<SOptionElement> extraOptions;
	SOptionElement op;
	op.name="RemoteIP";
	op.value="192.168.10.215";
	extraOptions.push_back(op);
	op.name="LocalIP";
	op.value="192.168.10.119";
	extraOptions.push_back(op);
	//VLDEnable();
	app->loadResourceFile(mT("dataPath.stg"));
	if(app->startup(mT("MRaY App"),vector2di(800,600),false,extraOptions,resFileName,0,true,false))
	{
		app->run();
	}

	//	VLDDisable();
	app=0;
	e=0;

	return 0;
}

