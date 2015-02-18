// TestOpenCV.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "../common/DemoCamera.h"
#include "ImageSetResourceManager.h"
#include "CMRayApplication.h"
#include "GUIThemeManager.h"
#include "FontResourceManager.h"
#include "GUIBatchRenderer.h"
#include "CFPS.h"
#include "GCCollector.h"
#include "ViewPort.h"
#include "JobPool.h"
#include "IGUIElement.h"
#include "GUIManager.h"
#include "GUIElementRegion.h"
#include "IGUIPanelElement.h"

#include "IThread.h"
#include "IThreadManager.h"
#include "StereoRenderer.h"


#include "DirectShowImageDataSource.h"
#include "CameraCaptureThread.h"
#include "SingleCameraCaptureThread.h"
#include "IImageDataSource.h"
#include "ShaderResourceManager.h"

#include "GUIWebPageControl.h"

#include "AVIVideoWriter.h"
#include "DirectSoundInputStream.h"
#include "DirectSoundOutputStream.h"
#include "LPCVoiceEncoder.h"
#include "GSMVoiceEncoder.h"
#include "FSLManager.h"
#include "ISoundStreamFrame.h"
#include "TheoraManager.h"
#include "VideoGrabberTexture.h"

#include "TextureResourceManager.h"
#include "TexLoader.h"

#include "TrackIRCAmera.h"


#include "Guitar.h"
#include "FastFourierTransform.h"

#include <SineWave.h>
#include <Guitar.h>
#include <Noise.h>
#include <ADSR.h>

#define VLD_START_DISABLED

#include <vld.h>
#include <vldapi.h>

#include "mySqlManager.h"
#include "GUIManagerRT.h"
#include "GUIElement3D.h"

#include "Win32Network.h"
#include "IWebRequest.h"

/*
#include <windows.h>
#include <ole2.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// Direct2D Header Files
#include <d2d1.h>
#include <d2d1helper.h>

// DirectWrite
#include <dwrite.h>
//#include <NuiApi.h>
*/
using namespace mray;
using namespace math;
using namespace core;


namespace mray
{
namespace GUI
{
	class GUIDoubleWebPage:public IGUIPanelElement
	{
	protected:
		GCPtr<GUIWebPageControl> m_page1;
		GCPtr<GUIWebPageControl> m_page2;
	public:
		static const GUID ElementType;

		GUIDoubleWebPage(IGUIManager* creator):IGUIPanelElement(ElementType,creator)
		{
		}

		void SetPages(GCPtr<GUIWebPageControl> p1,GCPtr<GUIWebPageControl> p2)
		{
			m_page1=p1;
			m_page2=p2;

			AddElement(p1);
			AddElement(p2);
		}

		virtual void Draw(video::IRenderArea* vp)
		{
			math::rectf rc=GetDefaultRegion()->GetRect();
			math::vector2d pos,size;
			pos=0;
			size=rc.getSize();
			size.y*=0.5;
			m_page1->SetPosition(pos);
			m_page1->SetSize(size);

			pos.y+=size.y;

			m_page2->SetPosition(pos);
			m_page2->SetSize(size);

			IGUIPanelElement::Draw(vp);
		}
		virtual IGUIElement* Duplicate(){return new GUIDoubleWebPage(GetCreator());}
		virtual FontAttributes* GetFontAttributes(){return 0;}
	};
	const GUID GUIDoubleWebPage::ElementType(mT("DoubleWebPage"));

}
}

class HeadMount:public scene::ISceneNode
{
	InputManager* m_input;
	float m_speed;
public:
	HeadMount(scene::ISceneManager*smngr,float speed,InputManager* kb):scene::ISceneNode(mT("HeadMount"),0,smngr)
	{
		m_speed=speed;
		m_input=kb;
	}
	virtual void update(float dt){
		if(!this->isVisible())
			return;
		float dx=m_speed*(m_input->getKeyboard()->getKeyState(KEY_A)-
			m_input->getKeyboard()->getKeyState(KEY_D))*dt;
		float dz=m_speed*(m_input->getKeyboard()->getKeyState(KEY_W)-
			m_input->getKeyboard()->getKeyState(KEY_S))*dt;

		// 			dx*=(1+2*gKeyboardController.isLShiftPress());
		// 			dz*=(1+2*gKeyboardController.isLShiftPress());

		if(m_input->getMouse()->isPressed(controllers::EMB_Right)){
			this->rotate(-80*m_input->getMouse()->getDX()*dt,math::vector3d(0,1,0),scene::TS_World);
			this->rotate(80*m_input->getMouse()->getDY()*dt,math::vector3d(1,0,0),scene::TS_Local);
		}
		this->translate(math::vector3d(dx,0,dz),scene::TS_Local);
		ISceneNode::update(dt);
	}
};


class MapCamera:public scene::CameraNode
{
protected:
	math::vector2d m_angles;
	float m_distance;
public:
	MapCamera(scene::ISceneManager*smngr,float speed,InputManager* kb):CameraNode(mT(""),0,smngr)
	{
		this->setUseTarget(true);
		this->setTarget(math::vector3d(0,0,0));
		this->setAspect(1);
		this->setAutoUpdateAspect(false);
		this->setZFar(1000);
		m_distance=100;
	}

	void UpdatePosition()
	{
		m_angles.x=math::clamp<float>(m_angles.x,10,50);
		m_distance=math::clamp<float>(m_distance,40,100);
		math::vector3d pos;
		pos.z=0.5*m_distance*math::sind(-m_angles.x);//*math::cosd(m_angles.y);
		pos.y=m_distance*math::cosd(-m_angles.x);
		pos.x=0;//0.8*m_distance*math::sind(m_angles.x);//*math::sind(m_angles.y);
		setPosition(pos);
	}

	void SetTilt(float a)
	{
		m_angles.x=a;
		UpdatePosition();
	}
	void SetYaw(float a)
	{
		m_angles.y=a;
		UpdatePosition();
	}
	void SetDistance(float d)
	{
		m_distance=d;
		UpdatePosition();
	}
	void AddTilt(float dy)
	{
		m_angles.x+=dy;
		UpdatePosition();
	}
	void AddDist(float dy)
	{
		m_distance+=dy;
		UpdatePosition();
	}
	float getDist()
	{
		return m_distance;
	}

};
class MapController
{
	float m_distAccel;
	float m_lastDist;
public:
	MapCamera* m_camera;
	InputManager* m_input;
	video::ParsedShaderPP::MappedParams* m_blurParam;
	GUI::GUIWebPageControl* map;
	MapController()
	{
		m_blurParam=0;
		m_input=0;
		m_distAccel=0;
		m_lastDist=0;
		map=0;
	}
	void OnEvent(Event* evt)
	{
		if(evt->getType()==ET_Joystic)
		{
			JoystickEvent* e=(JoystickEvent*)evt;
			if(e->event==JET_BUTTON_PRESSED)
			{
				if(e->button==5)
				{
					map->GetWindow()->ExecuteScript(L"ZoomIn()");
				}else if(e->button==4)
				{
					map->GetWindow()->ExecuteScript(L"ZoomOut()");
				}
			}
		}
	}
	virtual void update(float dt){
		if(m_input->getJoystick(0))
		{
			float da=(m_input->getJoystick(0)->getAxisState(1).abs*50*dt);
			//	float dy=(m_input->getJoystick(0)->getAxisState(2).abs*dt);
			m_distAccel+=(-(2*m_input->getJoystick(0)->getButtonState(10)-1)*500*dt);
			m_distAccel=math::clamp<float>(m_distAccel,-100,100);

			float d=m_camera->getDist();
			float dd=d-m_lastDist;
			m_lastDist=d;

			if(m_blurParam)
			{
				m_blurParam->SetValue(fabs(400*dd));
			}
			//m_angles.y+=dy;
			m_camera->AddDist(m_distAccel*dt);
			m_camera->AddTilt(da);

			float dy=(m_input->getJoystick(0)->getAxisState(4).abs*200*dt);
			float dx=(m_input->getJoystick(0)->getAxisState(5).abs*200*dt);
			core::string str="panBy("+core::StringConverter::toString(dx)+","+core::StringConverter::toString(dy)+")";
			map->GetWindow()->ExecuteScript(core::string_to_wchar(str));
		}
	}
};

class Application:public CMRayApplication
{

protected:
	GCPtr<GUI::GUIBatchRenderer> m_guiRender;
	GCPtr<scene::StereoRenderer> m_stereoRenderer;

	scene::CameraNode* m_camera[2];

	Berkelium::Context* m_context;


	GCPtr<video::GPUShader> m_imageShader;
	GCPtr<GUI::GUIManager> m_guiManager;

	SingleCameraCaptureThread* m_LCaptureFunc;
	SingleCameraCaptureThread* m_RCaptureFunc;
	OS::IThread* m_LcaptureThread;
	OS::IThread* m_RcaptureThread;

	video::ITexturePtr m_leftTex;
	video::ITexturePtr m_rightTex;
	video::ITexturePtr m_refTex;
	video::ITexturePtr m_videoTex[2];

	bool m_fullscreen;

	OS::IMutex* m_imageMutex;

	GUI::IGUIElement* m_root;
	bool m_takeRefPic;

	GCPtr<video::AVIVideoWriter> m_writer;

	GCPtr<sound::DirectSoundInputStream> m_soundRecorder;
	GCPtr<sound::DirectSoundOutputStream> m_soundPlayer;

	std::vector<uchar> m_captureBuffer;
	GCPtr<OS::IStream> m_voiceStream;
	GCPtr<sound::IVoiceEncoder> m_encoder;
	GCPtr<sound::ISound> m_sound;
	GCPtr<sound::ISoundStreamFrame> m_soundStream;
	GCPtr<sound::ISoundManager> m_soundManager;
	GCPtr<video::TheoraManager> m_theoraManager;
	video::IVideoSourcePtr m_video;
	video::IVideoSourcePtr m_video2;
	GCPtr<video::VideoGrabberTexture> m_order;

	video::IGPUShaderProgramPtr m_fragmentProgram;

	GCPtr<TrackIRCamera> m_trackIR;

	GCPtr<Guitar> m_guitar;

	GCPtr<db::ISQLManager> m_sqlManager;
	
	std::map<EKEY_CODE,float> m_pressLength;

	stk::SineWave m_sineWave;
	GCPtr<stk::Guitar> m_stkGuitar;
	stk::ADSR m_adsr;
	stk::Noise m_noise;

	GCPtr<scene::GUIManagerRT> m_guiRtManager;
	MapController m_MapController;

	GCPtr<network::INetwork> m_net;
//	GUI::GUIWebPageOrganizer* m_org;
public:


	Application():CMRayApplication()
	{
		m_fullscreen=false;
		m_LcaptureThread=0;
		m_RcaptureThread=0;
		m_takeRefPic=false;
		m_LCaptureFunc=0;
		m_RCaptureFunc=0;
		m_imageMutex=0;

	}

	virtual ~Application()
	{

		m_trackIR=0;
		if(m_LcaptureThread)
			OS::IThreadManager::getInstance().killThread(m_LcaptureThread);
		if(m_RcaptureThread)
			OS::IThreadManager::getInstance().killThread(m_RcaptureThread);
			
		delete m_LCaptureFunc;
		delete m_RCaptureFunc;

		delete m_RcaptureThread;
		delete m_LcaptureThread;

		delete m_imageMutex;

		m_theoraManager=0;
		m_soundManager=0;

		m_guiManager=0;
		Berkelium::destroy();
	}
	virtual void onEvent(Event* event)
	{
		CMRayApplication::onEvent(event);
		m_MapController.OnEvent(event);
		if(m_guiManager)
			m_guiManager->OnEvent(event);
		if(m_guiRtManager)
			m_guiRtManager->OnEvent(event);

		if(event->getType()==ET_Joystic)
		{
			JoystickEvent* e=(JoystickEvent*)event;

		}
		if(event->getType()==ET_Keyboard)
		{
			KeyboardEvent* e=(KeyboardEvent*)event;
			if(e->press && e->key==KEY_P)
			{
				core::stringw script1=L"changeCoords(20,40);";
				Berkelium::WideString str;
				str=Berkelium::WideString::point_to(script1.c_str(),script1.length());
//				m_org->GetActivePage()->GetWindow()->GetWindow()->executeJavascript(str);
			}
			if(m_guitar)
			{

				if(e->press)
				{
					if(e->key==KEY_F)
					{
						m_fullscreen=!m_fullscreen;
						m_root->SetVisible(!m_fullscreen);
					}else if(e->key==KEY_SPACE)
					{
						m_takeRefPic=true;
					}

					m_adsr.keyOn();

					m_pressLength[e->key]=gTimer.getSeconds();
				}else
				{
					float dT=gTimer.getSeconds()-m_pressLength[e->key];
					if(e->key>=KEY_1 && e->key<=KEY_9)
					{
						int i=e->key-KEY_1;
						m_guitar->Pulk(i,dT*0.0001,0.8);

					//	m_stkGuitar->noteOn(441,dT*0.0001,i);

					}
				//	m_adsr.keyOff();
				}
			}
		}
	}

	video::ITexturePtr m_netTex;
	virtual void init(const OptionContainer &extraOptions){
		CMRayApplication::init(extraOptions);

		CMRayApplication::loadResourceFile(mT("Resources.stg"));

		m_net = new network::Win32Network();
		{
			MapCamera* cam;
			m_camera[0]=cam=new MapCamera(getSceneManager(),100,GetInputManager());//getSceneManager()->createCamera();//
			/*
			m_camera[1]=getSceneManager()->createCamera();
			
			HeadMount *hm=new HeadMount(getSceneManager(),100,GetInputManager());
			getSceneManager()->addSceneNode(hm);

			m_stereoRenderer=new scene::StereoRenderer(GetRenderWindow(),m_camera[0],m_camera[1],hm);
			m_stereoRenderer->SetMode(scene::EStereo_None);*/
			getSceneManager()->addSceneNode(m_camera[0]);

			scene::ViewPort* vp= GetRenderWindow()->CreateViewport("main",m_camera[0],0,math::rectf(0,0,1,1),0);

			m_MapController.m_camera=cam;
			m_MapController.m_input=GetInputManager();
			/*
			video::ITexturePtr m_rt=getDevice()->createEmptyTexture2D(true);
			m_rt->setMipmapsFilter(false);
			m_rt->createTexture(math::vector3d(1),video::EPixel_B8G8R8A8);
			video::IRenderTargetPtr m_inputRT=getDevice()->createRenderTarget("",m_rt,0,0,0);
			video::ParsedShaderPP* pp=new video::ParsedShaderPP(getDevice());
			pp->LoadXML(gFileSystem.openFile("BrowserEffect.peff"));
			m_MapController.m_blurParam= pp->GetParam("RadialBlur");
			vp->SetAutoUpdateRTSize(true);
			vp->setRenderTarget(m_inputRT);
			vp->setPostProcessing(pp);
			vp->enablePostProcessing(true);*/
		}

		m_writer=new video::AVIVideoWriter();
		m_order=new video::VideoGrabberTexture();

		m_writer->Open("d:\\test.avi",math::vector2d(320,240));
		m_leftTex=getDevice()->createEmptyTexture2D(true);
		m_rightTex=getDevice()->createEmptyTexture2D(true);
		m_refTex=getDevice()->createEmptyTexture2D(true);

		m_leftTex->setMipmapsFilter(false);
		m_rightTex->setMipmapsFilter(false);
		m_refTex->setMipmapsFilter(false);

		m_leftTex->createTexture(math::vector3d(320,240,1),video::EPixel_R8G8B8);
		m_rightTex->createTexture(math::vector3d(320,240,1),video::EPixel_R8G8B8);
		m_refTex->createTexture(math::vector3d(320,240,1),video::EPixel_R8G8B8);

		m_imageMutex=OS::IThreadManager::getInstance().createMutex();

		m_imageShader=new video::GPUShader();
		m_imageShader->SetVertexShader(gShaderResourceManager.loadShader(mT("backgroundRemove.cg"),video::EShader_VertexProgram,"main_vp","cg"));
		m_imageShader->SetFragmentShader(gShaderResourceManager.loadShader(mT("backgroundRemove.cg"),video::EShader_FragmentProgram,"main_fp","cg"));

		{
			m_sqlManager=new db::mySqlManager();
			GCPtr<db::ISQLConnection> connection= m_sqlManager->CreateConnection();
			connection->Connect("www.freesql.org","mrayy","yamenslayer","mrayy",3306,0);
			connection->ExecNonBlockingCmdWithRet("Select p.name,i.name from PlayerData as p,ItemData as i,(Select * from PlayerItem) pi where pi.playerID=p.playerID AND pi.itemID=i.itemID ",0,0,0);
		}

		network::IWebRequest* req= m_net->CreateWebRequest();
		req->Connect("http://farm9.staticflickr.com/8251/8594833174_dccca65c91_z.jpg");

		OS::IStream* stream= req->GetStream();
		m_netTex=gTextureResourceManager.loadTexture2D("netTexture",stream);

		core::stringw path=L".\\berk";
		Berkelium::FileString str;
		str=Berkelium::FileString::point_to(path.c_str(),path.length());

		if(false)
		{
			IImageDataSource* leftDS=new DirectShowImageDataSource(1,320,240,40);
			m_LCaptureFunc=new SingleCameraCaptureThread( leftDS);

// 			m_captureFunc->GetLeftDataSource()->AttachTexture(m_leftTex);
// 			m_captureFunc->GetRightDataSource()->AttachTexture(m_rightTex);

			m_LcaptureThread=OS::IThreadManager::getInstance().createThread(m_LCaptureFunc);
			m_LcaptureThread->start(0);
			m_RCaptureFunc=0;

 			IImageDataSource* rightDS=new DirectShowImageDataSource(0,320,240,40);
 			m_RCaptureFunc=new SingleCameraCaptureThread( rightDS);
 			m_RcaptureThread=OS::IThreadManager::getInstance().createThread(m_RCaptureFunc);
 			m_RcaptureThread->start(0);
		}

		{
			gImageSetResourceManager.loadImageSet(mT("VistaCG.imageset"));
			GCPtr<OS::IStream> themeStream=gFileSystem.createBinaryFileReader(mT("VistaCG.xml"));
			GUI::GUIThemeManager::getInstance().loadTheme(themeStream);
			GUI::GUIThemeManager::getInstance().setActiveTheme(mT("VistaCG"));
			//load font
			GCPtr<GUI::IFont>font=gFontResourceManager.getOrCreate(mT("solo5_font.xml"));
			gFontResourceManager.setDefaultFont(font);
		}

		m_guiManager=new GUI::GUIManager(getDevice());
		Berkelium::init(str);
		core::string urls[]=
		{
			//"https://maps.google.com/",
			"file:///C:/Development/mrayEngine/Bin/Data/HTML/maps.html",
			"http://maps.google.com",
			"file:///D:/Development/Tachilab/tests/ge.html?lat=0&lng=20&offsetX=0&offsetY=0.1",
			"file:///D:/Development/Tachilab/tests/ge.html?lat=20&lng=40",
		};
// 
 		GUI::GUIDoubleWebPage* root=new GUI::GUIDoubleWebPage(m_guiManager);
 		root->SetSize(GetRenderWindow()->GetSize());

		if(true)
		{
			GCPtr<BrowserWindow> wnd;
			m_context=Berkelium::Context::create();
			wnd=new BrowserWindow(GetRenderWindow()->GetSize().x,GetRenderWindow()->GetSize().y,m_context);

			core::string url;//=urls[0];//"http://alfagiulietta.com/virtualownership/";//"https://developers.google.com/maps/customize";
			url=core::string("file:///")+gFileSystem.getAppPath()+"../Data/HTML/maps.html";
			url=urls[0];
			bool res=wnd->GetWindow()->navigateTo(url.c_str(),url.length());

			wnd->GetWindow()->focus();

			GUI::GUIWebPageControl* ctrl[2];
			ctrl[0]=new GUI::GUIWebPageControl(m_guiManager);
			float scale=1;//3.0f/2.0f;
			ctrl[0]->SetWindow(wnd);
			ctrl[0]->SetSize(((math::vector2d)GetRenderWindow()->GetSize())*scale);///math::vector2d(1,2));
			ctrl[0]->SetPosition(((math::vector2d)GetRenderWindow()->GetSize())*(-(scale-1.0f)/2.0f));
	// 		ctrl[1]=new GUI::GUIWebPageControl(m_guiManager);
	// 		ctrl[1]->SetWindow(wnd);

			m_root=ctrl[0];
			m_MapController.map=ctrl[0];
			m_guiManager->SetRootElement(m_root);
		
		//root->SetPages(ctrl[0],ctrl[1]);
			m_guiRtManager=new scene::GUIManagerRT();
			scene::ISceneNode* snode=getSceneManager()->createSceneNode();
			scene::IGUIElementRT* guiRT= m_guiRtManager->CreateFromManager(m_guiManager);//m_guiRtManager->CreateFromElement(m_root);
			scene::GUIElement3D* rendNode=new scene::GUIElement3D();
			rendNode->SetElement(guiRT);
			rendNode->setOwner(snode);
			snode->AttachNode(rendNode);
			snode->setScale(100);
		}
		/*
		m_org=new GUI::GUIWebPageOrganizer(m_guiManager);
		m_org->SetSize(GetRenderWindow()->GetSize());
		for(int i=0;i<1;++i)
		{
			GCPtr<BrowserWindow> m_bwindow;

			m_context=Berkelium::Context::create();
			m_bwindow=new BrowserWindow(1024,512,m_context);
			core::string url=urls[i];//"http://alfagiulietta.com/virtualownership/";//"https://developers.google.com/maps/customize";
			bool res=m_bwindow->GetWindow()->navigateTo(url.c_str(),url.length());

			m_bwindow->GetWindow()->focus();

			GUI::GUIWebPageControl* ctrl=new GUI::GUIWebPageControl(m_guiManager);
			ctrl->SetPosition(math::vector2d(100+i*50,100+i*50));
			ctrl->SetSize(math::vector2d(500,500));
			ctrl->SetWindow(m_bwindow);


			m_org->AddNewTab(ctrl);;
		}

		m_guiManager->SetRootElement(m_org);*/
//		VLDEnable();

		m_guiRender=new GUI::GUIBatchRenderer();
		m_guiRender->SetDevice(getDevice());
		m_theoraManager=new video::TheoraManager();

		m_soundRecorder=new sound::DirectSoundInputStream();
		m_encoder=new sound::GSMVoiceEncoder();
		m_soundManager=new sound::FSLManager();

		m_theoraManager->setSoundManager(m_soundManager);
		

		m_video=m_theoraManager->CreateVideoFromFile("MRAYY HD 1280x720.ogv");
		m_video2=m_theoraManager->CreateVideoFromFile("MRAYY HD 1280x720.ogv");
// 		m_video->play();
 //		m_video2->play();

		m_videoTex[0]=getDevice()->createEmptyTexture2D(true);
		m_videoTex[0]->setMipmapsFilter(false);
		m_videoTex[0]->createTexture(math::vector3d(m_video->GetFrameSize().x,m_video->GetFrameSize().y,1),m_video->GetImageFormat());

		m_videoTex[1]=getDevice()->createEmptyTexture2D(true);
		m_videoTex[1]->setMipmapsFilter(false);
		m_videoTex[1]->createTexture(math::vector3d(m_video2->GetFrameSize().x,m_video2->GetFrameSize().y,1),m_video2->GetImageFormat());

		
		m_fragmentProgram=gShaderResourceManager.loadShader("edgeDetect_simple.cg",video::EShader_FragmentProgram,"main_fp","cg");

		if(false)
		{
			m_voiceStream=gFileSystem.openFile("Voice.bin",OS::BIN_READ);
			std::vector<uchar> samplesIn;
			samplesIn.resize(m_voiceStream->length());
			m_voiceStream->read(&samplesIn[0],samplesIn.size());
			m_voiceStream->close();

			//std::vector<uchar> samplesOut=m_encoder->DecodeBuffer(samplesIn);

			m_soundStream=m_soundManager->CreateManualStream();
			m_soundStream->SetStreamFrameData(&samplesIn[0],samplesIn.size(),8000,sound::ESF_8,1);
			m_sound=m_soundManager->CreateManualSound("");
			m_sound->setSoundBuffer(m_soundStream);
			m_sound->play();
		}
		if(false)
		{
			if(0)
			{
			int freq=44100;
			int buffSize=freq*2;
			int n=buffSize*2;
			std::vector<float> samples;
			m_soundPlayer=new sound::DirectSoundOutputStream();
			m_soundPlayer->CreateOutputStream(0,buffSize,2,freq);
			samples.resize(n);
			float time=0;
			float step=1.0f/44100.0f;
			int length=(44100*10)/64;
			math::FastFourierTransform fft(256);

			for(int j=0;j<length;++j)
			{

				for(int i=0;i<n;i+=2)
				{
					samples[i]=samples[i+1]=sin(time*PI32*2.f)*sin(time*PI32)*sin(time*PI32*2.f/3.0f);
					time+=1000*step;
				}
				float* freqs= fft.Calculate(&samples[0],samples.size());
				m_soundPlayer->PlaySamples(samples);
			//	OS::IThreadManager::getInstance().sleep(n*1000/44100.0f);
			}
			}
		}

		if(false)
		{
			m_sound=m_soundManager->CreateManualSound("test");

			m_guitar=new Guitar();

			std::vector<int> times;
			for(int i=0;i<10;++i)
			{
				times.push_back(0);
				m_guitar->AddString(440*pow(2,i/12.0));;
			}

			if(false)
			{
				sound::ISoundStreamFrame* stream=m_soundManager->CreateManualStream();



				int freq=44100;
				int len=freq*10;

				short* data=new short[len];
				float angle=0;
				math::Randomizer r;
				m_guitar->Pulk(0,r.rand01(),r.rand01()*0.7+0.3);;


				for(int i=0;i<len;++i)
				{
					if(r.randRange(0,10000)==0 && 0)
					{
						int t=0;
						int ranked=0;
						for(int j=0;j<times.size();++j)
						{
						}
						m_guitar->Pulk(r.randRange(0,m_guitar->GetStringsCount()),r.rand01(),r.rand01()*0.7+0.3);;
					}
					float v=m_guitar->Sample();
					data[i]=v*32000;
					m_guitar->Tick();;
				}

				delete [] data;

				stream->SetStreamFrameData(data,len,freq,sound::ESampleFormat::ESF_16,1);
				m_sound->QueueSoundBuffer(stream);
			}
			m_sound->play();
		}

// 		m_soundRecorder->CreateInputStream(0,1,8000,0,8);
// 		m_soundRecorder->StartInputStream();


		if(false)
		{

			m_voiceStream=gFileSystem.openFile("Voice.bin",OS::BIN_WRITE);

			m_trackIR=new TrackIRCamera();
			m_trackIR->Init(9501);


		
			m_sineWave.setFrequency(441);
			m_sineWave.setRate(1);

			m_stkGuitar=new stk::Guitar();
			m_stkGuitar->setPluckPosition(0.5);
			m_stkGuitar->noteOn(441,1);

			m_adsr.setDecayTime(0);
			m_adsr.setSustainLevel(1);
			m_adsr.setReleaseTime(0.001);

			m_adsr.setSampleRate(44100);
		}


	}

	struct STime
	{
		float t;
		STime(){t=0;}
	};
	STime m_time;
	virtual void draw(scene::ViewPort* vp)
	{
		CMRayApplication::draw(vp);
		//m_guiManager->DrawAll(vp);

		video::TextureUnit tex,bg;
		getDevice()->set2DMode();
		tex.SetTexture(m_netTex);
		getDevice()->useTexture(0,&tex);
		getDevice()->draw2DImage(vp->getAbsViewPort(),1);

		if(false)
		{
			tex.SetTexture(m_leftTex);
			bg.SetTexture(m_refTex);

			//getDevice()->useShader(m_imageShader);
			getDevice()->useTexture(0,&tex);
			//		getDevice()->useTexture(1,&bg);
			math::vector2d imgSize(vp->getAbsViewPort().getSize());
			imgSize.x*=0.5;
			// 		if(!m_fullscreen)
			// 			imgSize*=0.2;
			// 		m_imageShader->GetFragmentShader()->setTexture("baseTex",&tex);
			// 		m_imageShader->GetFragmentShader()->setTexture("backTex",&bg);
			getDevice()->draw2DImage(math::rectf(0,imgSize),1);

			tex.SetTexture(m_rightTex);
			getDevice()->useTexture(0,&tex);
			getDevice()->draw2DImage(math::rectf(math::vector2d(imgSize.x,0),math::vector2d(imgSize.x*2,imgSize.y)),1);
			getDevice()->useShader(0);
			tex.SetTexture(m_refTex);

			//getDevice()->setFragmentShader(m_fragmentProgram);
			tex.SetTexture(m_videoTex[0]);
			getDevice()->useTexture(0,&tex);
			getDevice()->draw2DImage(math::rectf(0,m_video->GetFrameSize()),1);

			tex.SetTexture(m_videoTex[1]);
			getDevice()->useTexture(0,&tex);
			getDevice()->draw2DImage(math::rectf(math::vector2d(0,m_video->GetFrameSize().y),math::vector2d(0,m_video->GetFrameSize().y)+m_video2->GetFrameSize()),1);
			getDevice()->setFragmentShader(0);


		}

		if(false && m_captureBuffer.size()/2>0)
		{
			std::vector<math::vector2d> points;
			ushort* ptr=(ushort*)m_captureBuffer[0];
			points.resize(m_captureBuffer.size()/2);
			for(int i=0;i<m_captureBuffer.size();i+=2)
			{
				int idx=i/2;
				points[idx].x=idx*1.f;
				points[idx].y=200+200*m_captureBuffer[i]/128.0f;
			}
			getDevice()->draw2DLine(&points[0],points.size(),video::SColor(1));
		}
		
		GCPtr<GUI::IFont> font=gFontResourceManager.getDefaultFont();
		if(font && 1){
			m_guiRender->Prepare();

			float yoffset=50;

			GUI::FontAttributes attr;
			attr.fontColor.Set(0.05,1,0.5,1);
			attr.fontAligment=GUI::EFA_MiddleLeft;
			attr.fontSize=24;
			attr.hasShadow=true;
			attr.shadowColor.Set(0,0,0,1);
			attr.shadowOffset=vector2d(2);
			attr.spacing=2;
			attr.wrap=0;
			attr.RightToLeft=0;
			core::string msg=mT("FPS= ");
			msg+=core::StringConverter::toString(CFPS::getInstance().getFPS());
			font->print(math::rectf(10,yoffset+10,10,10),&attr,0,msg,m_guiRender);
			yoffset+=30;
			if(m_LCaptureFunc)
			{
				msg=mT("L Capture FPS= ");
				msg+=core::StringConverter::toString(m_LCaptureFunc->GetFPS());
				font->print(math::rectf(10,yoffset+10,10,10),&attr,0,msg,m_guiRender);
				yoffset+=30;
			}
			if(m_RCaptureFunc)
			{
				msg=mT("R Capture FPS= ");
				msg+=core::StringConverter::toString(m_RCaptureFunc->GetFPS());
				font->print(math::rectf(10,yoffset+10,10,10),&attr,0,msg,m_guiRender);
				yoffset+=30;
			}
			yoffset+=30;
			msg=mT("Time= ");
			msg+=core::StringConverter::toString(m_video->getTime());
			msg+="/";
			msg+=core::StringConverter::toString(m_time.t);
			font->print(math::rectf(10,yoffset+10,10,10),&attr,0,msg,m_guiRender);
			/*
			msg=mT("Primitives= ");
			msg+=core::StringConverter::toString(getDevice()->getPrimitiveDrawnCount());
			font->print(math::rectf(10,yoffset+10,10,10),&attr,0,msg,m_guiRender);
			yoffset+=30;
*/
			m_guiRender->Flush();
			getDevice()->useShader(0);
		}
	}

	std::vector<uchar> m_frame;
	virtual void update(float dt)
	{
		m_time.t+=dt;
		m_MapController.update(dt);
		CMRayApplication::update(dt);
		m_theoraManager->Update(dt);
		m_order->Set(m_video,m_videoTex[0]);
		m_order->Blit();
		m_order->Set(m_video2,m_videoTex[1]);
		m_order->Blit();

		m_soundManager->runSounds(dt);
		Berkelium::update();
		m_guiManager->Update(dt);
		//TakeCameraShot();
		ProcessCapturedImages();

		m_captureBuffer.clear();
	//	m_soundRecorder->CaptureSamples(m_captureBuffer);

		//JobPool::getInstance().AddRequest(m_order,0,0);

		if(0)
		{
			std::vector<uchar> comp= m_encoder->EncodeBuffer(m_frame);
			if(m_captureBuffer.size() )
				m_voiceStream->write(&m_captureBuffer[0],m_captureBuffer.size());

			if(comp.size()&& 0)
				m_voiceStream->write(&comp[0],comp.size());

		}
		if(m_trackIR)
		{
			math::vector3d pos,rot;
			m_trackIR->ReceiveData(pos,rot);
			printf("<%s><%s>\n",core::StringConverter::toString(pos).c_str(),core::StringConverter::toString(rot).c_str());
		}

		if(false)
		{
			
			float freq=44100;
			int N=freq*math::Min(1.0f,dt);
			printf("%d\n",N);
			short *data=new short[N];
			sound::ISoundStreamFrame* stream=m_soundManager->CreateManualStream();
			for(int i=0;i<N;++i)
			{
				m_guitar->Tick();
				data[i]=m_guitar->Sample()*32000;
			//	data[i]=m_sineWave.tick()*32000;
			//	data[i]=m_stkGuitar->tick()*32000;
			//	data[i]=m_noise.tick()* m_adsr.tick()*10000;
				if(i<100)
					printf("%d ",data[i]);
			}
			stream->SetStreamFrameData(data,N,freq,sound::ESF_16,1);
			
			delete [] data;
			m_sound->QueueSoundBuffer(stream);
			if(!m_sound->isPlaying())
				m_sound->play();
		}

	//	OS::IThreadManager::getInstance().sleep(1000.0/60.0f);
	}

	void rgbToHSL(const video::SColor&c,float &h,float&s,float&l)
	{
		video::SColor ret;
		float maxV=math::Max(math::Max(c.R,c.G),c.B);
		float minV=math::Min(math::Min(c.R,c.G),c.B);
		l=(maxV+minV)*0.5;
		if(maxV==minV)
		{
			h=s=0;
		}else
		{
			float d=maxV-minV;
			s=l>0.5? d/(2-maxV-minV):d/(maxV+minV);
			if (maxV==c.R)
			{
				h=(c.G-c.B)/d+(c.G<c.B ? 0.6:0);
			}else if (maxV==c.G)
			{
				h=(c.B-c.R)/d+2;
			}else 
			{
				h=(c.R-c.G)/d+4;
			}
			h/=6.0f;
		}
	}
	video::ImageInfo* ProcessFrame(const video::ImageInfo*ifo)
	{

		video::ImageInfo* ret=new video::ImageInfo();
		ret->createData(ifo->Size,video::EPixel_B8G8R8A8);
		uchar* data=ret->imageData;
		uchar*ptr=data;
		video::SColor clr;
		float h,s,l;
		
		const uchar*ptr2=ifo->imageData;
		for(int x=0;x<ifo->Size.x;++x)
		{
			for(int y=0;y<ifo->Size.y;++y)
			{
				ptr[2]=ptr2[0];
				ptr[1]=ptr2[1];
				ptr[0]=ptr2[2];
				clr.Set(ptr[0]*math::i255,ptr[1]*math::i255,ptr[2]*math::i255,1);
				rgbToHSL(clr,h,s,l);
				if(abs(h-0)<0.2)
				{
					float v=1-abs(h-0.5)/0.2f;
					ptr[3]=v*255;
				}else
					ptr[3]=0;
				ptr+=4;
				ptr2+=3;
			}
		}
		return ret;
	}

	void ProcessCapturedImages()
	{
		
		const video::ImageInfo* ifo;
		if(m_LCaptureFunc && (ifo=m_LCaptureFunc->GetImage())!=0)
		{
			m_imageMutex->lock();
			//ifo=ProcessFrame(ifo);
			video::LockedPixelBox box(math::box3d(0,ifo->Size),ifo->format,ifo->imageData);
			m_leftTex->getSurface(0)->blitFromMemory(box);
			//m_rightTex->getSurface(0)->blitFromMemory(box);

			if(m_takeRefPic)
			{
				m_takeRefPic=false;
				m_refTex->getSurface(0)->blitFromMemory(box);
			}

			m_imageMutex->unlock();
			//delete ifo;
		} 

		if(m_RCaptureFunc && (ifo=m_RCaptureFunc->GetDataSource()->PopImage())!=0)
		{
			//ifo=ProcessFrame(ifo);
			video::LockedPixelBox box(math::box3d(0,ifo->Size),ifo->format,ifo->imageData);
			m_rightTex->getSurface(0)->blitFromMemory(box);
			//delete ifo;
		}
	}


	virtual void onDone(){
	}
};


APPLICATION_ENTRY_POINT
{
#if defined(DEBUG) | defined(_DEBUG)
	//	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );//_crtBreakAlloc
	long temp=-1;
	_crtBreakAlloc=temp;
#endif



	GCPtr<Application> app=new Application();

	core::string resFileName=mT("plugins.stg");

#ifdef UNICODE
	resFileName=mT("pluginsU.stg");
#endif

	std::vector<SOptionElement> extraOptions;
	app->loadResourceFile(mT("dataPath.stg"));
	if(app->startup(mT("MRaY App"),vector2di(800,600),false,extraOptions,resFileName,0,true,false))
	{
		app->run();
	}


	//	VLDDisable();
	app=0;

	GCCollector::shutdown();
}




