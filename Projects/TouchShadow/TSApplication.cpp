

#include "stdafx.h"
#include "TSApplication.h"
#include "ImageSetResourceManager.h"
#include "GUIThemeManager.h"
#include "FontResourceManager.h"
#include "ViewPort.h"
#include "OpenNIManager.h"
#include "OpenNICaptureDevice.h"
#include "OpenNIUtils.h"
#include "DebugDrawManager.h"
#include "ParticleEmitter.h"
#include "CPUParticleBillboardRenderer.h"
#include "BoxParticleEmitter.h"
#include "PAScaleInterpolater.h"
#include "PAColorInterpolater.h"

#include "SceneComponent.h"
#include "Stk.h"
#include "SineWave.h"
#include "OutputChannel.h"

#include "FileRead.h"
#include "InteractionDemo.h"
#include "XMLTree.h"

#include "DemoSharedData.h"
#include "ChannelManager.h"
#include "AudioWaveSet.h"

#include "mrayOIS.h"
#include "EffectManager.h"

#include "ParsedShaderPP.h"
#include "TextureResourceManager.h"

#include "ChannelMap.h"
#include "AudioSoundInterface.h"
#include "ChannelManager.h"
#include "EffectsScheme.h"

namespace mray
{

	void FloatToInt16(float v,char*b)
	{
		v*=0.5;
		int iv;
		if(v>1)iv=32767;
		else if(v<-1)iv=-32768;
		else iv=floor(v*32768.0);
		b[0]=((char*)&iv)[0];
		b[1]=((char*)&iv)[1];
	}

	void ConvertSamples(stk::StkFrames& src,FramesList& dst,int bps)
	{
		const float* sptr=&src[0];
		uchar* dptr=(uchar*)&dst[0];
		int wP=0;
		for(int i=0;i<src.size()*2;++i)
		{
			FloatToInt16(sptr[i],(char*)&dptr[wP]);
			wP+=bps;
			if(wP>=dst.size())
				return;
		}
	}

	TSApplication* TSApplication::Instance=0;

	TSApplication::TSApplication()
	{
		Instance=this;
		m_limitFps=true;
		m_limitFpsCount=30;
	}

	TSApplication::~TSApplication()
	{
	}


	void TSApplication::_InitResources()
	{
		CMRayApplication::loadResourceFile(mT("Resources.stg"));


		gImageSetResourceManager.loadImageSet(mT("VistaCG.imageset"));
		GCPtr<OS::IStream> themeStream=gFileSystem.createBinaryFileReader(mT("VistaCG.xml"));
		GUI::GUIThemeManager::getInstance().loadTheme(themeStream);
		GUI::GUIThemeManager::getInstance().setActiveTheme(mT("VistaCG"));

		//load font
		GCPtr<GUI::IFont>font=gFontResourceManager.loadFont(mT("TEKTONPRO_font.fnt"));
		gFontResourceManager.setDefaultFont(font);

		gLogManager.log("Resources Loaded",ELL_SUCCESS);
	}
	void TSApplication::onEvent(Event* event)
	{
		if(event->getType()==ET_Mouse)
		{
			MouseEvent* e=dynamic_cast<MouseEvent*>(event);
			if(e->event==MET_LEFTDOWN)
			{

				EffectCreatePack pack;
				pack.pos=e->pos;
				pack.pos.y=e->GetOwnerRenderWindow()->GetSize().y-pack.pos.y;
				pack.params["Span"]="0.1";
				pack.params["Scale"]="100";
				pack.params["Color"]="1,0,0,1";
				pack.params["Rate"]="1000";
				m_effects->CreateEffect("Sparkle",&pack);
			}
		}
		if(event->getType()==ET_Keyboard)
		{
			KeyboardEvent* e=dynamic_cast<KeyboardEvent*>(event);
			if(e->press)
			{
				if(e->key==KEY_C)
				{
					m_deviceController->CaptureNextUser();
				}else if(e->key==KEY_R)
				{
					m_deviceController->Reset();
				}else 
				if(e->key==KEY_A)
				{
					m_deviceController->EnableAutoUpdateFrame(!m_deviceController->IsAutoUpdateFrameEnabled());
				}else if(e->key==KEY_D)
				{
					m_deviceController->EnableDebugging(!m_deviceController->IsEnableDebugging());
				}else if(e->key==KEY_L)
				{
					if(DemoSharedData::Instance()->ActiveDemo)
					{
						
					//	for(int i=0;i<mngr->GetInterfaceCount();++i)
						{
							CAudioWave*wave= DemoSharedData::Instance()->ActiveDemo->GetAudioWave()->GetWave("loop");

							if(wave)
							{
								int i=2;
								{
									CChannel* c= DemoSharedData::Instance()->ChannelManager->GetChannel(i);
									if(!c || !c->GetChannel())
										return;
									//flush first
									c->GetChannel()->FlushScheduledFrames();
									c->GetChannel()->ScheduleFrames(&wave->waveFrames,1);
									c->GetChannel()->GetOwner()->Start();

								}
							}
						}
					}
				}else if(e->key==KEY_E)
				{
					math::vector2d pos;
					pos.x= math::Randomizer::rand01()*(float)GetRenderWindow()->GetSize().y;
					pos.y= math::Randomizer::rand01()*(float)GetRenderWindow()->GetSize().y;
					DemoSharedData::Instance()->ActiveDemo->GetEffects()->PlayRandom(pos,1);
				}

				if(e->Char>='1' && e->Char<='9')
				{
					int demo=e->Char-'1';
					if(demo<m_demos.size())
					{
						SetActiveDemo(m_demos[demo]);
					}
				}
			}
		}
	}
	void TSApplication::SetActiveDemo(InteractionDemo* d)
	{
		if(DemoSharedData::Instance()->ActiveDemo)
			DemoSharedData::Instance()->ActiveDemo->Stop();
		DemoSharedData::Instance()->ActiveDemo=d;
		m_deviceController->Clean();
		DemoSharedData::Instance()->ActiveDemo->Start();
	}


	void TSApplication::CreatePhysicsSystem()
	{
		physics::PhysicsSystemDesc desc;
		desc.gravity.set(0,-9.,0);
		desc.maxIter=32;

		desc.maxTimestep=1.0f/(16.0f*60.0f);
		desc.useFixedTimeStep=true;
		m_phManager=new physics::PhysXManager(&desc);


	}
	class CircleAnimator:public game::IGameComponent
	{
		scene::IMovable* m_snode;
		math::vector3d m_velocity;
		math::vector3d m_pos;
		float m_velLen;
		float m_angle;


		scene::IMovable* m_targetNode;
	public:
		CircleAnimator()
		{
			m_velocity.x=math::Randomizer::rand01()-0.5;
			m_velocity.y=math::Randomizer::rand01()-0.5;
			m_velocity.Normalize();
			m_velLen=math::Randomizer::rand01()*100+10;

			m_angle=math::Randomizer::rand01()*math::PI32;

		}
		bool InitComponent()
		{
			game::IGameComponent::InitComponent();
			m_snode=0;
			game::SceneComponent* comp=(game::SceneComponent*)GetOwnerEntity()->GetFirstComponent(game::SceneComponent::getClassRTTI());
			if(!comp)
				return false;
			m_snode=comp->GetMovableObject();
			return true;
		}
		void Update(float dt)
		{
			m_pos+=m_velocity*dt*m_velLen;

			math::vector3d dir;
			dir.x=cos(m_angle);
			dir.y=sin(m_angle);
			m_velocity+=dir;
			m_velocity.Normalize();

			m_angle+=math::Randomizer::rand01()*math::PI32*dt*0.05;
			m_snode->setPosition(m_pos);
		}
	};

	class FollowAnimator:public game::IGameComponent
	{
		scene::IMovable* m_snode;
		math::vector3d m_velocity;

		scene::IMovable* m_targetNode;
	public:
		FollowAnimator(scene::IMovable* target)
		{
			m_targetNode=target;
		}
		bool InitComponent()
		{
			game::IGameComponent::InitComponent();
			m_snode=0;
			game::SceneComponent* comp=(game::SceneComponent*)GetOwnerEntity()->GetFirstComponent(game::SceneComponent::getClassRTTI());
			if(!comp)
				return false;
			m_snode=comp->GetMovableObject();
			return true;
		}
		void Update(float dt)
		{
			math::vector3d vel=m_targetNode->getAbsolutePosition()-m_snode->getAbsolutePosition();
			m_velocity+=vel*dt*0.1;
			m_velocity-=m_velocity*dt*0.05;
			math::vector3d pos=m_snode->getAbsolutePosition();
			pos+=m_velocity*dt;
			m_snode->setPosition(pos);
		}
	};
	void TSApplication::LoadDemos(const core::string& path)
	{
		xml::XMLTree t;
		if(!t.load(path))
			return;
		xml::XMLElement* e=t.getSubElement("Demos");
		if(!e)
			return;
		e=e->getSubElement("Demo");
		while(e)
		{
			InteractionDemo *demo=new InteractionDemo;
			if(demo->LoadFromXML(e->getValueString("Path")))
			{
				m_demos.push_back(demo);
				printf("Demo Loaded: %s\n",demo->GetName().c_str());
			}
			else 
				delete demo;
			e=e->nextSiblingElement("Demo");
		}
	}
	void TSApplication::init(const OptionContainer &extraOptions)
	{
		CMRayApplication::init(extraOptions);
		_InitResources();

		m_guiRender=new GUI::GUIBatchRenderer();
		m_guiRender->SetDevice(getDevice());

		CreatePhysicsSystem();

		DemoSharedData::Instance()->App=this;
		DemoSharedData::Instance()->ChannelManager=new ChannelManager();

		m_effects=new EffectManager();
		m_effects->CreateEffect("IdleShapes",0);

		DemoSharedData::Instance()->EffectManager=m_effects;
		if(true)
		{
			AudioSoundInterfaceDesc desc;
			desc.bps=16;
			desc.isFloat=false;
			desc.samplesPerSec=11025;
			desc.stereo=true;

			stk::FileRead file;
			core::string path;
			DemoSharedData::Instance()->AudioManager=new AudioInterfaceManager();
			DemoSharedData::Instance()->AudioManager->ReloadInterfaces(desc);
		}

		{

			OptionContainer opt;
			opt["title"].value="Preview";
			opt["VSync"].value="false";
			m_previewWnd=getDevice()->CreateRenderWindow("Preview",math::vector2di(640,480),false,opt,0);
			m_previewWnd->CreateViewport(mT("Main"),0,0,math::rectf(0,0,1,1),0);
			AddRenderWindow(m_previewWnd);
			InputCreationPack pack(m_previewWnd);
			pack.WinSize=m_previewWnd->GetSize();
			pack.exclusiveMouse=false;
			m_inputManager=0;
			m_inputManager=CreateOISInputManager(pack);
			m_previewVP=m_previewWnd->CreateViewport("Preview",0,0,math::rectf(0,0,1,1),0);
			m_previewVP->SetClearColor(video::DefaultColors::Blue);

		}
		{
			m_logo=gTextureResourceManager.loadTexture2D("InteractTeam.png");
			m_title=gTextureResourceManager.loadTexture2D("tickleShadow.png");
		}
		DemoSharedData::Instance()->ChannelManager->LoadFromXML("ChannelMap.xml");
		LoadDemos("IVRCDemos.xml");
		m_dbgMngr=new scene::DebugDrawManager(m_device);
		m_gameManager=new game::GameEntityManager();
		m_gameManager->SetPhysicsManager(m_phManager);
		m_gameManager->SetSceneManager(getSceneManager());
		
		m_openNiManager=new OpenNIManager();
		m_openNiManager->Init(0,0);

		//Create Kinect device controller
		m_deviceController=new DeviceController();
		//Attach any available device
		m_deviceController->SetDevice(m_openNiManager->CreateDeviceAny());
		//m_deviceController->GetDevice()->Open();
		DemoSharedData::Instance()->DeviceController=m_deviceController;

		m_camera=getSceneManager()->createCamera();
		{
			m_vp= GetRenderWindow()->CreateViewport("Camera",m_camera,0,math::rectf(0,0,1,1),0);
			m_vp->SetClearColor(video::DefaultColors::White);
			m_vp->AddListener(this);
			m_vp->setOnlyToRenderTarget(true);

			video::ITexturePtr renderTargetTex=getDevice()->createTexture2D(math::vector2d(1,1),video::EPixel_R8G8B8,true);
			renderTargetTex->setBilinearFilter(false);
			renderTargetTex->setTrilinearFilter(false);
			renderTargetTex->setMipmapsFilter(false);

			video::IRenderTargetPtr rt=getDevice()->createRenderTarget(mT(""),renderTargetTex,video::IHardwareBufferPtr::Null,video::IHardwareBufferPtr::Null,false);
			m_vp->setRenderTarget(rt);


			video::ParsedShaderPP* pp=new video::ParsedShaderPP(getDevice());
			pp->LoadXML(gFileSystem.openFile("IVRC.peff"));
			m_vp->setPostProcessing(pp);
			m_vp->enablePostProcessing(true);
			m_vp->SetAutoUpdateRTSize(true);


			m_rgbShiftValue=pp->GetValue("RGBShift.final.offset");
		}
		{
			video::ITexturePtr tex=getDevice()->createTexture2D(math::vector2d(1,1),video::EPixel_R8G8B8,true);
			tex->setBilinearFilter(false);
			tex->setTrilinearFilter(false);
			tex->setMipmapsFilter(false);

			m_finalRT=getDevice()->createRenderTarget(mT(""),tex,video::IHardwareBufferPtr::Null,video::IHardwareBufferPtr::Null,false);

		}


		m_camera->setUseTarget(true);
		m_camera->setTarget(0);
		m_camera->setPosition(500);


		scene::ISceneNode* followNode=getSceneManager()->createSceneNode();
		followNode->setPosition(math::vector3d(0,50,0));
		{

			game::GameEntity* ent=m_gameManager->CreateGameEntity("");
			game::SceneComponent* c=new game::SceneComponent(m_gameManager);
			c->SetSceneNode(followNode);
			ent->AddComponent(c);
			ent->AddComponent(new CircleAnimator());

			ent->Initialize();
		}
		for(int i=0;i<5;++i)
		{
			scene::ISceneNode* rainNode=getSceneManager()->createSceneNode();
			rainNode->setPosition(math::vector3d((math::Randomizer::rand01()-0.5)*5000,100,(math::Randomizer::rand01()-0.5)*5000));

			rainNode->setCullingType(scene::SCT_NONE);
			m_rainPS=new scene::SParticleSystem();
			//scene::ParticleEmitter* emitter= m_rainPS->CreateEmitter();
			scene::BoxParticleEmitter* emitter= new scene::BoxParticleEmitter();
			m_rainPS->AddEmitter(emitter);
			emitter->setSize(math::vector3d(20,20,20));
		//	emitter->setParticleAcceleration(math::vector3d(0,-9.8,0));
	//		emitter->setParticleVelocity(math::vector3d(0,10,0));
			emitter->setParticleShapeType(scene::EPS_Billboard);
			emitter->setMaterial(gMaterialResourceManager.getMaterial("IVRC/Particle"));
			emitter->particlesLifeSpawn.addKey(0,10);
			emitter->particlesRate.addKey(0,100);
			emitter->setGenerateMode(scene::EPGM_Infinite);
			emitter->SetRenderer(new scene::CPUParticleBillboardRenderer(1000));

			scene::PAScaleInterpolater* scaleInt=new scene::PAScaleInterpolater();
			scaleInt->addKey(0,math::vector3d(20),math::vector3d(20));
			scaleInt->addKey(1,math::vector3d(0),math::vector3d(0));
			emitter->addAffector(scaleInt);

			video::SColor clr;
			clr.setHSL(math::Randomizer::rand01(),1,1);
			scene::PAColorInterpolater* clrInt=new scene::PAColorInterpolater();
			clrInt->addKey(0,video::SColor(clr.R,clr.G,clr.B,0));
			clrInt->addKey(0.5,video::SColor(clr.R,clr.G,clr.B,1));
			clrInt->addKey(1,video::SColor(clr.R,clr.G,clr.B,0));
			emitter->addAffector(clrInt);
			rainNode->AttachNode(m_rainPS);

			game::GameEntity* ent=m_gameManager->CreateGameEntity("");
			game::SceneComponent* c=new game::SceneComponent(m_gameManager);
			c->SetSceneNode(rainNode);
			ent->AddComponent(c);
			ent->AddComponent(new FollowAnimator(followNode));

			ent->Initialize();

		}
	}


	void TSApplication::WindowPreViweportUpdate(video::RenderWindow* wnd,scene::ViewPort* vp)
	{
		//getDevice()->set2DMode();
		//m_deviceController->Display(m_dbgMngr,vp);
	//	getDevice()->set3DMode();

	}

	void TSApplication::DrawDebug(scene::ViewPort* vp)
	{

		getDevice()->set2DMode();


		math::matrix4x4 m;
		math::matrix4x4 m2;
		getDevice()->getTransformationState(video::TS_VIEW,m);
		m2.setScale(math::vector3d(-1,1,1));
		m=m2*m;
		getDevice()->setTransformationState(video::TS_VIEW,m);
		m_deviceController->Display(m_dbgMngr,vp);
		m_dbgMngr->StartDraw(vp->getCamera());
		m_dbgMngr->EndDraw();
		getDevice()->set2DMode();
		math::rectf rc;
		rc.ULPoint.set(50,50);
		rc.BRPoint=rc.ULPoint+math::vector2d(100,100);
		for(int i=0;i<m_demos.size();++i)
		{
			if(m_demos[i]==DemoSharedData::Instance()->ActiveDemo)
			{
				getDevice()->draw2DRectangle(rc,video::SColor(0.8,0.8,0.8,1));
				getDevice()->setLineWidth(5);
				getDevice()->draw2DRectangle(rc,video::SColor(1,0,0,1),false);
			}else
			{
				getDevice()->draw2DRectangle(rc,video::SColor(0.4,0.4,0.4,1));
			}

			video::TextureUnit tex;
			tex.SetTexture(m_demos[i]->GetImage());
			getDevice()->useTexture(0,&tex);

			if(m_demos[i]==DemoSharedData::Instance()->ActiveDemo)
				getDevice()->draw2DImage(rc,video::SColor(1,1,1,1));
			else
				getDevice()->draw2DImage(rc,video::SColor(1,1,1,0.5));
			rc.ULPoint.y+=100;
			rc.BRPoint.y+=100;
		}
		GUI::IFont*font=gFontResourceManager.getDefaultFont();
		if(font)
		{

			GUI::FontAttributes attr;
			attr.fontSize=24;
			attr.fontColor.Set(1,1,1,1);
			core::string msg=vp->getName()+"=";
			msg+=core::StringConverter::toString(gFPS.getFPS());
			font->print(math::rectf(50,50,50,50),&attr,0,msg,m_guiRender);

			msg="Is Captured="+core::StringConverter::toString(m_deviceController->IsUserCaptured());
			font->print(math::rectf(50,100,50,50),&attr,0,msg,m_guiRender);
			msg="Auto Update="+core::StringConverter::toString(m_deviceController->IsAutoUpdateFrameEnabled());
			font->print(math::rectf(50,150,50,50),&attr,0,msg,m_guiRender);
			m_guiRender->Flush();
		}
	}

	void TSApplication::onRenderDone(scene::ViewPort*vp)
	{
		getDevice()->set2DMode();
		m_effects->Draw(vp->getAbsRenderingViewPort());
	}
	void TSApplication::WindowPostViweportUpdate(video::RenderWindow* wnd,scene::ViewPort* vp)
	{
		if(wnd==m_previewWnd)
		{
			DrawDebug(vp);
		}else
		{
			video::TextureUnit tex;

			math::rectf rc=vp->getAbsRenderingViewPort();
			if(rc.getWidth()!=m_finalRT->getSize().x || rc.getHeight()!=m_finalRT->getSize().y)
			{
				m_finalRT->getColorTexture(0)->createTexture(math::vector3di(rc.getWidth(),rc.getHeight(),1),video::EPixel_R8G8B8);
			}
			getDevice()->set2DMode();
			getDevice()->setRenderTarget(m_finalRT);

			//DrawDebug(vp);
			

			tex.SetTexture(m_vp->getRenderTarget()->getColorTexture(0));
			getDevice()->useTexture(0,&tex);
			getDevice()->draw2DImage(rc,1);


			tex.SetTexture(m_title);
			getDevice()->useTexture(0,&tex);
			getDevice()->draw2DImage(math::rectf(10,10,10+2*130,10+2*40),1);

			tex.SetTexture(m_logo);
			getDevice()->useTexture(0,&tex);

			getDevice()->draw2DImage(math::rectf(10,rc.BRPoint.y-2*18,2*102.4,rc.BRPoint.y),1);
			
			getDevice()->setRenderTarget(0);
			getDevice()->set2DMode();

			tex.SetTexture(m_finalRT->getColorTexture(0));
			getDevice()->useTexture(0,&tex);
			getDevice()->draw2DImage(rc,1,0,&math::rectf(0,0,1,1));
//			getDevice()->draw2DImage(rc,1,0,&math::rectf(1,0,0,1));

		}
	}

	void TSApplication::update(float dt)
	{
		CMRayApplication::update(dt);

		m_phManager->update(dt);
		m_gameManager->Update(dt);
		m_dbgMngr->Update(dt);
		m_effects->Update(dt);

		if(DemoSharedData::Instance()->ActiveDemo)
			DemoSharedData::Instance()->ActiveDemo->Run();

		if(m_rgbShiftValue)
		{
			if(math::Randomizer::rand01()>0.9)
				m_rgbShiftValue->floatParam[0]=0.005;
			else
				m_rgbShiftValue->floatParam[0]=0.002;
		}

		//Sleep(1000/60);
	}

	void TSApplication::onDone()
	{
	}

}

