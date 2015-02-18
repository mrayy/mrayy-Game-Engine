

#include <HeapAllocator.h>

#include <GCCollector.h>

#include <mrayEngine.h>
#include <mrayWinApp.h>
#include <mrayWin32.h>
#include <HeightMapRaw16.h>
#include "../common/DemoCamera.h"
#include "../common/GBufferListener.h"
//#include "BloomPP.h"
//#include "GlarePP.h"
//#include "BrightFilterPP.h"
//#include "ToonPP.h"
//#include "HDRPP.h"
//#include "LuminancePP.h"
//#include "ParsedShaderPP.h"
//#include "TestPP.h"

#include "TerrainUtility.h"
#include "CircleTerrainBrush.h"
#include "MathUtil.h"

#include "TextDecorator.h"
#include "TextDecorateNodeFactory.h"

#include "ReflectionRenderPass.h"
#include "NormalRenderPass.h"
#include "CPUParticleBillboardRenderer.h"
#include "ParticleEmitter.h"

#include <PAVelocityInterpolater.h>
#include <PAScaleInterpolater.h>
#include <PAColorInterpolater.h>
#include <BoxParticleEmitter.h>

#include "SceneNodeCreator.h"
#include "ImageSetResourceManager.h"

#include "GUIQuadRenderOperation.h"
#include "GUIBatchRenderer.h"
#include "GUIMultiQuadRenderOperation.h"
#include "LanguageFontSpecification.h"

#include "AnimationController.h"

#include "GUID.h"

#include "MeshAnimatorManager.h"
#include "ThreadBarrier.h"

#include "PoolMemoryAllocator.h"
#include "UTFString.h"
#include "XMLTree.h"
#include "GUIManager.h"
#include "IGUIElement.h"
#include "IGUIPanelElement.h"

#include "DirectSoundVoiceRecorder.h"
#include "FSLManager.h"
#include "ISoundStream.h"
#include "LPCVoiceEncoder.h"
#include "GSMVoiceEncoder.h"
#include "DebugDrawManager.h"

#include "WiimoteManager.h"
#include "wiiusecpp.h"
#include "WiiEvent.h"
#include "RoadNode.h"


#include "ThreadJobManager.h"
#include "IJobPiece.h"


#include "ISoundListener.h"
#include "MeshFileCreatorManager.h"
#include "SphereSoundEmitter.h"

#include "PoolString.h"
#include "FunctionProfiler.h"

#include "FunctionProfileManager.h"

using namespace mray;
using namespace math;
using namespace core;



//////////////////////////////////////////////////////////////////////////

#include <gl/gl.h>


class TestJobPiece:public IJobPiece
{
public:

	virtual bool Execute()
	{
		float t1=gTimer.getSeconds();
		float t=0;
		for(int i=0;i<40000;++i)
		{
			t=math::sind(t)+math::cosd(t);
		}
		float t2=gTimer.getSeconds();
		printf("%f\n",t2-t1);
		return true;
	}
};


class MeshResourceOrder:public JobOrder
{
public:

	core::string m_meshName;
	scene::SMeshPtr m_mesh;
public:
	MeshResourceOrder(const core::string &meshName){
		m_meshName=meshName;
	}
	virtual~MeshResourceOrder(){}
	virtual bool ExecuteJob()
	{
		printf("Start Loading Mesh %s\n",m_meshName.c_str());
		m_mesh=gMeshResourceManager.loadMesh(m_meshName,false);
		return true;
	}
};

class RefelefctionRTListener:public video::IRenderTargetListener
{
	Plane m_p;
	scene::CameraNode*m_cam;
	scene::ISceneNode*m_node;
	video::IVideoDevice*m_device;
public:
	RefelefctionRTListener(video::IVideoDevice*device,const Plane& p,scene::CameraNode*cam,scene::ISceneNode*node)
	{
		m_device=device;
		m_p=p;
		m_cam=cam;
		m_node=node;
	}
	virtual~RefelefctionRTListener(){}
	virtual void OnBind()
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		//m_cam->getDevice()->setTransformationState(video::TS_VIEW, m_cam->getViewMatrix());
		math::matrix4x4 m;
		m_device->getTransformationState(video::TS_VIEW,m);
		glLoadMatrixf(m.getTransposed().getMatPointer());
		math::Plane pp=m_p;
		//m_cam->getViewMatrix().transformNormalPre(m_p.Normal,pp.Normal);
		double p[4]=
		{pp.Normal.x,pp.Normal.y,pp.Normal.z,pp.D};
		glClipPlane(GL_CLIP_PLANE0,p);

		glPopMatrix();
		glEnable(GL_CLIP_PLANE0);

		m_node->setVisible(false);
	}
	virtual void OnUnBind()
	{
		m_node->setVisible(true);
		glDisable(GL_CLIP_PLANE0);
	}
};


math::matrix4x4 buildProjectionMatrixPerspectiveFovRH(
	float fieldOfView,float aspect,float znear,float zfar)
{
	float h=1.0f/(float)tan(fieldOfView*0.5f);
	float w=h/aspect;

	matrix4x4 mat;

	mat.mat[0]=w;
	mat.mat[1]=0;
	mat.mat[2]=0;
	mat.mat[3]=0;

	mat.mat[4]=0;
	mat.mat[5]=h;
	mat.mat[6]=0;
	mat.mat[7]=0;

	mat.mat[8]=0;
	mat.mat[9]=0;
	mat.mat[10]=zfar/(znear-zfar);
	mat.mat[11]=-1;

	mat.mat[12]=0;
	mat.mat[13]=0;
	mat.mat[14]=znear*zfar/(znear-zfar);
	mat.mat[15]=0;

	mat.flagNotIdentity();
	return mat;
}


math::matrix4x4 buildCameraLookAtLH(vector3d pos,vector3d target,
									vector3d up)
{
	vector3d zAxis=pos-target;
	zAxis.Normalize();

	vector3d xAxis=up.crossProduct(zAxis);
	xAxis.Normalize();

	vector3d yAxis=zAxis.crossProduct(xAxis);

	matrix4x4 mat;
	mat.mat[0]=xAxis.x;
	mat.mat[1]=yAxis.x;
	mat.mat[2]=zAxis.x;
	mat.mat[3]=0;


	mat.mat[4]=xAxis.y;
	mat.mat[5]=yAxis.y;
	mat.mat[6]=zAxis.y;
	mat.mat[7]=0;

	mat.mat[8] =xAxis.z;
	mat.mat[9] =yAxis.z;
	mat.mat[10]=zAxis.z;
	mat.mat[11]=0;


	mat.mat[12]=-xAxis.dotProduct(pos);
	mat.mat[13]=-yAxis.dotProduct(pos);
	mat.mat[14]=-zAxis.dotProduct(pos);
	mat.mat[15]=1;

	mat.flagNotIdentity();
	return mat.getTransposed();

}

int totalWaits=0;

class TextureResourceOrder:public JobOrder
{
public:

	core::string m_texName;
	video::ITexturePtr m_tex;
public:

	TextureResourceOrder(const core::string &texName){
		m_texName=texName;
	}
	virtual~TextureResourceOrder(){}
	virtual bool ExecuteJob()
	{
		printf("Start Loading Texture %s\n",m_texName.c_str());
		m_tex=gTextureResourceManager.loadTexture2D(m_texName);
		return true;
	}
};



class Application:public CMRayApplication
{

	math::matrix4x4 m_projection;

	GCPtr<video::IRenderTarget> m_shadowRenderTarget;
	GCPtr<video::IRenderTarget> m_reflectionRenderTarget;

	static const int shadowMapWidthHeight=4096;

	GCPtr<scene::CameraNode> m_camera;
	GCPtr<scene::LightNode> m_light;
	GCPtr<scene::SPatchTerrain> m_terrain;

	core::array<scene::ISceneNode*> m_nodes;

	GCPtr<video::SMaterial> shadowMtrl;

	GCPtr<scene::ViewPort> m_viewPort;

/*	GCPtr<GUI::GUISlider> m_Exposure;
	GCPtr<GUI::GUISlider> m_Threshold;
*/

//	GCPtr<video::HDRPP> m_hdr;

	scene::ETerrainModificationMode m_modType;

	GCPtr<scene::TerrainUtility> m_terrainUtil;

	GCPtr<scene::ISceneNode> m_cursour;

	GCPtr<GUI::ITextDecorateNode> m_textDecorate;

	GCPtr<scene::ReflectionRenderPass> m_reflectionPass;

	GCPtr<video::TextureUnit> m_reflectionTU;


	GCPtr<scene::NormalRenderPass> m_pass;
	GCPtr<scene::ISceneNode> m_node;

	GCPtr<scene::SParticleSystem> m_ps;
	GCPtr<GUI::GUIBatchRenderer> m_guiRender;


	GCPtr<OS::ThreadBarrier> m_barrier;

	bool m_doneLoading;

	void OnTextureLoadingCompleted( JobOrder*const &o,const bool &s)
	{
		TextureResourceOrder*to=(TextureResourceOrder*)o;
		printf("Texture Loading Completed:%s\n",to->m_texName.c_str());

	}
	void OnMeshLoadingCompleted( JobOrder*const &o,const bool&s)
	{
		MeshResourceOrder*to=(MeshResourceOrder*)o;
		printf("Mesh Loading Completed:%s\n",to->m_meshName.c_str());

	}


	GCPtr<GUI::GUIManager> m_guiManager;

	GCPtr<sound::FSLManager> m_soundManager;
	GCPtr<sound::ISound> m_sound;
	GCPtr<sound::ISoundStream> m_soundStream;

	core::array<uchar> m_recorderSamples;
	core::array<uchar> m_encodedSamples;
	GCPtr<sound::DirectSoundVoiceRecorder> m_soundRec;
	sound::GSMVoiceEncoder m_soundEncoder;

	GCPtr<controllers::WiimoteManager> m_wiiMngr;

	GCPtr<scene::DebugDrawManager > m_debugManager;

	GCPtr<FunctionProfileManager> m_profileManager;

	GCPtr<ThreadJobManager> m_tb;

	std::vector<sound::SphereSoundEmitter> m_emitters;

public:

	Application():CMRayApplication()
	{
		m_modType=scene::ETMM_Add;

		HeapAllocator<char> alloc;
		alloc.allocate(10);
	}

	virtual ~Application(){
	}
	virtual void onEvent(Event*event){
		CMRayApplication::onEvent(event);
		if(m_guiManager)
			m_guiManager->OnEvent(event);
		if(event->getType()==ET_GuiEvent){
/*			GCPtr<GUIEvent>e=event;
			if(e->Caller==m_Exposure){
				m_hdr->exposureAdaptSpeed=(m_Exposure->Value()*0.01f);
			}else if(e->Caller==m_Threshold){
				m_hdr->SetThreshold(m_Threshold->Value()*0.01f);
			}*/
		}else if(event->getType()==ET_Mouse)
		{
			if(event->isRecived() || true)
				return;
			MouseEvent* e=dynamic_cast<MouseEvent*>(event);
			if(e->event==MET_LEFTDOWN)
			{
				math::line3d ray= scene::SceneHelper::getInstance().getRayFromScreenCoord(e->pos,m_camera);
				math::Plane ground(0,math::vector3d::YAxis);
				math::vector3d point;
				if(ground.getIntersectionWithLine(ray.pStart,ray.getVector(),point))
				{
					scene::CircleTerrainBrush brush;
					brush.SetOuterRadius(100);
					brush.SetInnerRadius(0);
					brush.SetFalloff(1);
					brush.Apply(m_terrainUtil,point,50,m_modType);
				}
			}else if(e->event==MET_MOVED)
			{

				math::line3d ray= scene::SceneHelper::getInstance().getRayFromScreenCoord(e->pos,m_camera);
				math::Plane ground(0,math::vector3d::YAxis);
				math::vector3d point;
				if(ground.getIntersectionWithLine(ray.pStart,ray.getVector(),point))
				{
					point.y=m_terrain->getHeight(point.x,point.z)+1;
					m_cursour->setPosition(point);
				}
			}
		}else if(event->getType()==ET_Keyboard)
		{
			KeyboardEvent* e=dynamic_cast<KeyboardEvent*>(event);
			if(e->press)
			{
				switch (e->key)
				{
				case KEY_1:
					m_modType=scene::ETMM_Add;
					break;
				case KEY_2:
					m_modType=scene::ETMM_Sub;
					break;
				case KEY_3:
					m_modType=scene::ETMM_Noise;
					break;
				case KEY_4:
					m_modType=scene::ETMM_SetValue;
					break;
				case KEY_R:
					if(m_soundRec->IsRecording())
					{
						m_soundRec->StopRecording();
						
						//m_sound->stop();
						core::array<uchar> data= m_soundEncoder.DecodeBuffer(m_encodedSamples);
						//data=m_soundEncoder.DecodeBuffer(data);
						m_soundStream->SetStreamData(data.pointer(),data.size(),8000,sound::ESF_16,1);
						m_sound->QueueSoundBuffer(m_soundStream);
						m_encodedSamples.resize(0);
						//m_sound->play();

						m_recorderSamples.resize(0);
					}
					else
						m_soundRec->StartRecording();
					break;
				case KEY_P:
					if(m_sound->isPlaying())
					{
						m_sound->stop();
					}else m_sound->play();
					break;
				case KEY_E:
					{
						m_debugManager->AddLine(0,vector3d(20,0,0),video::SColor(255,0,0,255),1,10);
						m_debugManager->AddLine(0,vector3d(0,20,0),video::SColor(0,255,0,255),1,10);
						m_debugManager->AddLine(0,vector3d(0,0,20),video::SColor(0,0,255,255),1,10);


					}break;
				case KEY_B:
					{
						m_debugManager->AddBillboard(math::vector3d(0,20,0),10,gTextureResourceManager.loadTexture2D("star_02.png"),video::DefaultColors::Red,0,10);
					}break;
				case KEY_T:
					{
						math::quaternion q;
						matrix4x4 m;
						q.fromEulerAngles(math::Randomizer::rand01()*360,math::Randomizer::rand01()*360,math::Randomizer::rand01()*360);
						q.toMatrix(m);
						m_debugManager->AddAxis(m,10,20);
						m_debugManager->AddOBB(m,vector3d(40,40,20),video::SColor(255,255,255,255),20);
					}break;
				}
			}
		}else if(event->getType()==WiiEvent::EventID)
		{
			WiiEvent* e=dynamic_cast<WiiEvent*>(event);
			switch (e->event)
			{
			case EWiiEvent_Event:
				{
					CWiimote*wm= m_wiiMngr->GetWiimotes()[e->WiimoteNumber];
					float pitch, roll, yaw, a_pitch, a_roll;
					if(wm->Buttons.isJustPressed(CButtons::BUTTON_MINUS)){
						wm->SetMotionSensingMode(CWiimote::OFF);
						wm->IR.SetMode(CIR::OFF);
					}
					if(wm->Buttons.isJustPressed(CButtons::BUTTON_PLUS)){
						wm->SetMotionSensingMode(CWiimote::ON);
						wm->IR.SetMode(CIR::ON);
					}

					if(wm->isUsingACC()){
						wm->Accelerometer.GetOrientation(pitch, roll, yaw);
						wm->Accelerometer.GetRawOrientation(a_pitch, a_roll);

						math::quaternion q;
						q.fromEulerAngles(pitch,yaw,-roll);

						m_camera->setOrintation(q);
						printf("wiimote roll = %f [%f]\n",  roll, a_roll);
						printf("wiimote pitch = %f [%f]\n",  pitch, a_pitch);
						printf("wiimote yaw = %f\n",  yaw);
					}
					if(wm->Buttons.isJustPressed(CButtons::BUTTON_A)){
						Sleep(100);
						float x,y,z;
						wm->Accelerometer.GetGravityVector(x,y,z);
						wm->Accelerometer.SetGravityCalVector(z,y,z);
					}
				}
				break;
			case EWiiEvent_Status:
				{
					CWiimote*wm= m_wiiMngr->GetWiimotes()[e->WiimoteNumber];
					printf("battery: %f %%\n", wm->GetBatteryLevel());
				}break;
			case EWiiEvent_Connect:
				printf ("Wiimote conntected [%d]\n",e->WiimoteNumber);
				break;
			case EWiiEvent_Disconnect:
				printf ("Wiimote disconntected [%d]\n",e->WiimoteNumber);
				break;
			case EWiiEvent_NunchukInserted:
				printf ("nunchak conntected [%d]\n",e->WiimoteNumber);
				break;
			case EWiiEvent_NunchukRemoved:
				printf ("nunchak removed [%d]\n",e->WiimoteNumber);
				break;
			}
		}
	}

	void TestDecorator()
	{
		PROFILE_FUNCTION();
		GUI::TextDecorator d;
		m_textDecorate=d.ParseText("Hello <color=255,0,0><size=30>World</></><image=Grass.tga,50></> <image=Bumps.bmp,30></>");
	}

	void loadSkybox()
	{
		PROFILE_FUNCTION();
		GCPtr<video::ITexture> tex= scene::SkyBoxManager::getInstance().loadSkyFromFolder(mT("skybox\\night\\"),mT("png"),mT("night"));
		GCPtr<scene::IRenderable> sky=new scene::SSkyBoxNode(tex);//scene::SceneNodeCreator::addSkyBox(getSceneManager(),tex);
		sky->SetHasCustomRenderGroup(true);
		sky->SetTargetRenderGroup(scene::RGH_Skies);
		scene::ISceneNode*node= getSceneManager()->createSceneNode();
		node->AttachNode(sky);
		node->setCullingType(scene::SCT_NONE);
	}

	void createSun()
	{
		PROFILE_FUNCTION();
		scene::IRenderable*sun=new scene::MeshRenderableNode(0);
		scene::MeshGenerator::getInstance().generateSphere(50,8,8,sun->getMesh()->addNewBuffer()->getMeshBuffer());
		scene::ISceneNode* node=getSceneManager()->createSceneNode();
		node->AttachNode(sun);
		node->setPosition(m_light->getPosition());

/*		GCPtr<video::SMaterial> mtrl=new video::SMaterial();
		mtrl->ambientColor=255;
		mtrl->emissiveColor=255;
		mtrl->diffuseColor=255;
		sun->setMaterial(mtrl);*/
	}

	void loadTerrain(){
		PROFILE_FUNCTION();
		GCPtr<loaders::HeightMapRaw16> l=new loaders::HeightMapRaw16();
		l->loadHeightMap(gFileSystem.openFile(mT("terrain.raw")));
		m_terrain=new scene::SPatchTerrain(0,math::vector3d(25000,300,25000));
		m_terrain->loadTerrain(l,7,0.05,false);
		m_terrain->getMaterial()->setTexture(gTextureResourceManager.loadTexture2D(mT("terrain.jpg")),0);

		scene::ISceneNode*node=getSceneManager()->createSceneNode();
		node->AttachNode(m_terrain);
		//node->setVisible(false);
		/*
		m_terrain->setBaseTexture(gTextureResourceManager.loadTexture2D(mT("terrain.jpg")));
		m_terrain->setDetailTexture(gTextureResourceManager.loadTexture2D(mT("detail_mud.jpg")));
		m_terrain->setDetailNormal(gTextureResourceManager.loadTexture2D(mT("detail_mud.jpg")));*/

	//	m_terrainUtil=new scene::TerrainUtility(m_terrain);

	//	m_cursour=scene::SceneNodeCreator::addSphere(getSceneManager(),1,5,5);
	}

	UTFString m_arStr;
	math::matrix4x4 m_matProj;
	virtual void init(core::array<SOptionElement> *extraOptions){
		PROFILE_FUNCTION();
		traceFunction(Application);
		CMRayApplication::init(extraOptions);
		CMRayApplication::loadResourceFile(mT("Resources.stg"));
/*
		{
			PROFILE_BLOCK("HeapString");
			{
				std::list<core::string> items;
				for(int i=0;i<100000;++i)
				{
					core::string a("Hello");
					items.push_back(a);
				}
			}
		}
		{
			PROFILE_BLOCK("PoolString");
			{
				std::list<PoolString<char>> items;
				for(int i=0;i<100000;++i)
				{
					PoolString<char> a("Hello");
					items.push_back(a);
				}
			}
		}

		TestJobPiece pieces[50];
		m_tb=new ThreadJobManager();
		for(int i=0;i<50;++i)
		{
			m_tb->AddPiece(&pieces[i]);
		}
		m_tb->BeginProcessing();

		m_tb->EndProcessing();

		float startT=gTimer.getSeconds();
		float maxTime=0;
		for(int i=0;i<50;++i)
		{
			float t1=gTimer.getSeconds();
			pieces[i].Execute();
			float t2=gTimer.getSeconds();
			maxTime=math::Max(maxTime,t2-t1);
		}
		float endT=gTimer.getSeconds();
		printf("exec=%f,maxTime=%f\n",endT-startT,maxTime);
*/
		GCPtr<StreamLogger> logger=new StreamLogger(1);
		logger->setStream(gFileSystem.createTextFileWriter(gFileSystem.getAppPath()+ mT("log.txt")));

		gLogManager.addLogDevice(logger);
		{
			m_guiManager=new GUI::GUIManager(getDevice());
			GUI::IGUIPanelElement* panel=new GUI::IGUIPanelElement(mray::GUID("Panel"),m_guiManager);
			panel->SetSize(math::vector2d(800,600));

			GUI::IGUIElement* elem;
			
			elem=m_guiManager->CreateElement(mray::GUID("Button"));
			elem->SetPosition(math::vector2d(20,20));
			elem->SetSize(math::vector2d(200,50));
			elem->SetText(core::string("Button"));
			panel->AddElement(elem);

			elem=m_guiManager->CreateElement(mray::GUID("CheckBox"));
			elem->SetPosition(math::vector2d(20,170));
			elem->SetSize(math::vector2d(200,50));
			elem->SetText(core::string("CheckBox"));
			panel->AddElement(elem);

			m_guiManager->SetRootElement(panel);
		}

		xml::XMLTree tree;
		{
			tree.load(gFileSystem.openFile("xmlFile.xml"));
			xml::XMLElement* elem= tree.getSubElement("Tree");
			xml::XMLAttribute*attr= elem->getAttribute("Attrib");
			m_arStr=(utf8*)attr->value.c_str();
		}
	
		m_wiiMngr=new controllers::WiimoteManager();

		if(0)
		{
			m_wiiMngr->Find(5);
			m_wiiMngr->ConnectWithAllMotes();
			for(int i=0;i<m_wiiMngr->GetNumConnectedWiimotes();++i)
			{
				CWiimote* mote=m_wiiMngr->GetWiimotes(false)[i];
				mote->SetLEDs(CWiimote::LED_1);
				//mote->SetRumbleMode(CWiimote::ON);

				//Sleep(200);
				mote->SetRumbleMode(CWiimote::OFF);

			}
		}

		m_debugManager=new scene::DebugDrawManager(getDevice());
//		getGUIManager()->GetLanguageSpecification()->Load("Arabic.spec");
//		m_arStr=getGUIManager()->GetLanguageSpecification()->ProcessText(m_arStr);

		m_guiRender=new GUI::GUIBatchRenderer();
		m_guiRender->SetDevice(getDevice());
		scene::MeshAnimatorManager::getInstance().SetGPUSkeletonAnimatorParams(3,4);

		TestDecorator();

		m_soundRec=new sound::DirectSoundVoiceRecorder();
		m_soundRec->CreateBuffer(1,8000,16000,16);


		m_soundManager=new sound::FSLManager();
		m_sound=m_soundManager->loadSound("afterburn.ogg",true,sound::ESNDT_2D);
		m_soundStream=m_soundManager->CreateManualStream();
	//	m_sound->setSoundBuffer(m_soundStream);

		mray::GUID str1,str2;

		{
			PROFILE_BLOCK("Themes");
			gImageSetResourceManager.loadImageSet("VistaCG.imageset");
			GCPtr<OS::IStream> themeStream=gFileSystem.createBinaryFileReader(mT("VistaCG.xml"));
			GUI::GUIThemeManager::getInstance().loadTheme(themeStream);
			GUI::GUIThemeManager::getInstance().setActiveTheme(mT("VistaCG"));

			m_guiManager->SetActiveTheme(GUI::GUIThemeManager::getInstance().getActiveTheme());

			//load font
			GCPtr<GUI::IFont>font=gFontResourceManager.getOrCreate(mT("Tahoma_font.xml"));
			gFontResourceManager.setDefaultFont(font);
		}

		float aspect=(float)getWindow()->getSize().x/(float)getWindow()->getSize().y;

		GCPtr<video::ITexture> texture=getDevice()->createTexture2D(shadowMapWidthHeight,video::EPixel_Float16_R);
		m_shadowRenderTarget=getDevice()->createRenderTarget(mT("shadowRenderTarget"),texture,0,0,0);



		m_camera=new DemoCamera(getSceneManager(),100);
		m_projection=math::MathUtil::CreateProjectionMatrixPerspectiveFov(m_camera->getFovY(),
			m_camera->getAspect(),1,500);
		getSceneManager()->addSceneNode(m_camera);

		m_viewPort=getSceneManager()->createViewport(mT("MainVP"),0);
		m_viewPort->setCamera(m_camera);


		m_soundManager->getListener()->attachToListener(m_camera);

		scene::ISceneNode* node;


		if(0)
		{
			math::Sphere sp;
			sp.radius=20;
			scene::SMesh* mesh=new scene::SMesh();
			scene::MeshGenerator::getInstance().generateSphere(sp.radius,8,8,mesh->addNewBuffer()->getMeshBuffer());

			sound::SphereSoundEmitter emitter;
			m_soundManager->setMetersPerUnit(10);

			for(int i=0;i<4;++i)
			{

				sp.center.x=math::Randomizer::randRange(-300,300);
				sp.center.y=math::Randomizer::randRange(-300,300);
				sp.center.z=math::Randomizer::randRange(-300,300);

				scene::IRenderable*sun=new scene::MeshRenderableNode(mesh);
				sun->getMesh()->calcCurrBoundingBox();

				node=getSceneManager()->createSceneNode();
				node->AttachNode(sun);
				node->setPosition(sp.center);
				node->setDrawBoundingBox(true);

				emitter.SetSphere(sp);

				sound::ISound* snd= m_soundManager->loadSound("Wind Light.wav",false,sound::ESNDT_3D);
				snd->setVolume(10);
				snd->play();
				snd->setLooping(true);
				snd->setMinDistance(100);
				snd->setMaxDistance(10000);
				emitter.AttachSound(snd);

				m_emitters.push_back(emitter);
			}
		}

		loadTerrain();

		

		if(1)
		{
			PROFILE_BLOCK("Road Building")
			core::array<math::vector3d> points;
			points.resize(9);

			points[0].set(0,0,0);
			points[1].set(10,0,0);
			points[2].set(15,0,5);
			points[3].set(10,0,10);
			points[4].set(0,0,10);
			points[5].set(-10,0,10);
			points[6].set(-15,0,5);
			points[7].set(-10,0,0);
			points[8].set(0,0,0);
			for (int i=0;i<points.size();++i)
			{
				points[i]*=10;
			}

			math::CCatmullCurve<math::vector3d> curve(points.pointer(),points.size(),20);

			points.resize(curve.getPointsCount());
			for (int i=0;i<points.size();++i)
			{
				points[i]=curve.getCurvePoints()[i];
				points[i].y=m_terrain->getHeight(points[i].x,points[i].z)+4;
			}
			


			scene::IRenderable*r=new scene::MeshRenderableNode(0);

			scene::IMeshBuffer* buff=r->getMesh()->addNewBuffer()->getMeshBuffer();


			scene::RoadNode road;
			road.SetRoadCVs(points);
			road.Build(20,buff);
			scene::TangentCalculater calc(1,2);
			calc.calculateTangents(buff);

			r->getMesh()->calcCurrBoundingBox();

			video::SMaterial* mtrl=r->getMesh()->getBufferData(0)->getMaterial();
			mtrl->setTexture(gTextureResourceManager.loadTexture2D("PlaneRoad.tga"),0);
			mtrl->setTexture(gTextureResourceManager.loadTexture2D("PlaneRoadNormal.tga"),1);
			//r->getMesh()->getBufferData(0)->getMaterial()->setRenderState(video::RS_Wireframe,video::ES_Use);

			mtrl->setMaterialRenderer(video::MRT_TRANSPARENT);
			mtrl->setRenderShader(gShaderResourceManager.getResource("Simple/Phong"));

			node=getSceneManager()->createSceneNode();
			node->AttachNode(r);
		}
		//sun->getMaterial()->setTexture(gTextureResourceManager.loadTexture2D("terrain.jpg"),0);

		/*	node=scene::SceneNodeCreator::addAnimatedMesh(getSceneManager(),"model.mdl",math::vector3d(0,-20,0));;//getSceneManager()->addSphere(10,8,8,math::vector3d(0,10,0),math::quaternion::Identity,1);
		node->ShadowCaster=true;
		node->ShadowReciver=true;*/


		//		m_node=node;
		m_pass=new scene::NormalRenderPass();
		//		getSceneManager()->addAnimatedMesh("model.mdl",math::vector3d(0,-20,0));

		texture=getDevice()->createTexture2D(256,video::EPixel_R8G8B8A8);
		m_reflectionTU=new video::TextureUnit();
		m_reflectionTU->SetTexture(texture);
		m_reflectionRenderTarget=getDevice()->createRenderTarget(mT("ReflectionRenderTarget"),texture,0,0,0);


		m_light=getSceneManager()->createLightNode();
		m_light->setPosition(math::vector3d(30,400,40));
		m_light->setRadius(100);
		m_light->setAmbient(100);
		/*		m_light->setProjection(m_projection);
		m_light->setShadowMap(m_shadowRenderTarget);
		*/
		{
			PROFILE_BLOCK("Test");
		}
		{
			PROFILE_BLOCK("Models");
			for(int i=0;i<5;++i)
			{
				scene::TangentCalculater t(1,2);
				scene::MeshRenderableNode* modelNode= scene::MeshFileCreatorManager::getInstance().LoadFromFile("male.mdx",true);
				t.calculateTangents(modelNode->getMesh());
				
				animation::AnimationController *controller=modelNode->getMesh()->getSkeleton()->getAnimationMixer()->getAnimationController("Run");
				controller->play(20,animation::EAM_Cycle);
				controller->getGroup()->playAll();
				node=getSceneManager()->createSceneNode();
				node->AttachNode(modelNode);
				node->setScale(10);
				node->setPosition(math::vector3d(5*i,0,0));
				node->setDrawBoundingBox(true);
			}
		}
		//modelNode->getMesh()->getSkeleton()->getBone("Bip01_L_Thigh")->setOrintation(quaternion(45,math::vector3d(1,0,0)));


//		getGUIManager()->addButton("Hello World",math::vector2d(100,200),math::vector2d(200,50),0);
//		getGUIManager()->addEditBox("«·”·«ÅE⁄·ÌﬂÅE,math::vector2d(100,400),math::vector2d(200,50),0);


		loadSkybox();

		m_barrier=new OS::ThreadBarrier();

/*		JobPool::getInstance().AddRequest(1,new TextureResourceOrder("sportive04_m_25.png"),newClassDelegate2<Application,void,JobOrder*,bool>("",this,&Application::OnTextureLoadingCompleted),m_barrier);
		JobPool::getInstance().AddRequest(1,new TextureResourceOrder("sportive03_m_25_normal.tga"),newClassDelegate2<Application,void,JobOrder*,bool>("",this,&Application::OnTextureLoadingCompleted),m_barrier);
		JobPool::getInstance().AddRequest(1,new TextureResourceOrder("sportive03_m_25.tga"),newClassDelegate2<Application,void,JobOrder*,bool>("",this,&Application::OnTextureLoadingCompleted),m_barrier);
		JobPool::getInstance().AddRequest(1,new TextureResourceOrder("sportive03_m_25_spec.tga"),newClassDelegate2<Application,void,JobOrder*,bool>("",this,&Application::OnTextureLoadingCompleted),m_barrier);
		JobPool::getInstance().AddRequest(1,new TextureResourceOrder("casual01_f_20.png"),newClassDelegate2<Application,void,JobOrder*,bool>("",this,&Application::OnTextureLoadingCompleted),m_barrier);
		JobPool::getInstance().AddRequest(2,new MeshResourceOrder("school.mdl"),newClassDelegate2<Application,void,JobOrder*,bool>("",this,&Application::OnMeshLoadingCompleted),m_barrier);
		JobPool::getInstance().AddRequest(2,new MeshResourceOrder("grid.mdl"),newClassDelegate2<Application,void,JobOrder*,bool>("",this,&Application::OnMeshLoadingCompleted),m_barrier);
*/		JobPool::getInstance().AddRequest(new MeshResourceOrder("SimpleMap12.mdl"),newClassDelegate2<Application,void,JobOrder*,bool>("",this,&Application::OnMeshLoadingCompleted),m_barrier);

		m_doneLoading=false;

		//m_barrier->Wait();


		/*
		{
		m_ps=scene::SceneNodeCreator::addParticleSystem(getSceneManager(),0,0,0);
		{
		scene::BoxParticleEmitter* e=new scene::BoxParticleEmitter(m_ps);
		m_ps->AddEmitter(e);
		e->setSize(1);
		e->setParticleVelocity(math::vector3d(0,30,0));
		e->particlesLifeSpawn.addKey(0,2);
		e->particlesRate.addKey(0,400);
		e->particlesCount.addKey(0,8000);
		e->SetRenderer(new scene::CPUParticleBillboardRenderer(10000));
		e->getMaterial()->setRenderState(video::RS_CullFace,video::ES_DontUse);
		e->getMaterial()->setRenderState(video::RS_Lighting,video::ES_DontUse);
		e->getMaterial()->setRenderState(video::RS_ZWrite,video::ES_DontUse);
		e->getMaterial()->diffuseColor.Set(255,0,0,255);

		e->getMaterial()->setMaterialRenderer(video::MRT_TRANSPARENT);

		e->getMaterial()->setTexture(gTextureResourceManager.loadTexture2D("star_02.png"),0);

		scene::PAColorInterpolater*clrAff=new scene::PAColorInterpolater();
		clrAff->addKey(0,video::SColor(250,100,100,200));
		clrAff->addKey(0.5,video::SColor(100,50,0,200));
		clrAff->addKey(1,video::SColor(20,20,20,0));
		e->addAffector(clrAff);

		scene::PAVelocityInterpolater*velAff=new scene::PAVelocityInterpolater();
		velAff->addKey(0,math::vector3d(0,10,0),math::vector3d(0,15,0));
		velAff->addKey(0,math::vector3d(-1,10,-1),math::vector3d(1,15,1));
		velAff->addKey(0,math::vector3d(-8,10,-8),math::vector3d(8,15,8));
		e->addAffector(velAff);
		}
		{
		scene::BoxParticleEmitter* e=new scene::BoxParticleEmitter(m_ps);
		m_ps->AddEmitter(e);
		e->setSize(4);
		e->setParticleVelocity(math::vector3d(0,5,0));
		e->particlesLifeSpawn.addKey(0,4);
		e->particlesRate.addKey(0,200);
		e->particlesCount.addKey(0,8000);
		e->SetRenderer(new scene::CPUParticleBillboardRenderer(10000));
		e->getMaterial()->setRenderState(video::RS_CullFace,video::ES_DontUse);
		e->getMaterial()->setRenderState(video::RS_Lighting,video::ES_DontUse);
		e->getMaterial()->setRenderState(video::RS_ZWrite,video::ES_DontUse);
		e->getMaterial()->diffuseColor.Set(255,0,0,255);

		e->getMaterial()->setMaterialRenderer(video::MRT_TRANSPARENT);

		e->getMaterial()->setTexture(gTextureResourceManager.loadTexture2D("star_02.png"),0);
		scene::PAScaleInterpolater*scaleAff=new scene::PAScaleInterpolater();
		scaleAff->addKey(0,2);
		scaleAff->addKey(0.5,4);
		scaleAff->addKey(1,10);
		e->addAffector(scaleAff);

		scene::PAColorInterpolater*clrAff=new scene::PAColorInterpolater();
		clrAff->addKey(0,video::SColor(250,100,100,30));
		clrAff->addKey(0.5,video::SColor(100,50,0,20));
		clrAff->addKey(1,video::SColor(20,20,20,0));
		e->addAffector(clrAff);
		}

		m_ps->addAnimator(new scene::SAnimatorCircle(0,math::vector3d::YAxis,40,50));
		}*/


		//createSun();
		/*
		if(false){
		math::Plane plane;
		plane.D=0;
		plane.Normal=math::vector3d::YAxis;
		node=scene::SceneNodeCreator::addPlane(getSceneManager(),1,1,0,math::quaternion::Identity,1000);
		node->setCullingType(scene::ENodeCullingType::SCT_NONE);

		m_reflectionPass=new scene::ReflectionRenderPass(m_camera);

		//m_enableGUIManager=false;

		m_reflectionRenderTarget->AddListener(new RefelefctionRTListener(plane,m_camera,node));

		m_reflectionPass->SetReflectionPlane(plane);
		m_reflectionPass->SetRenderTarget(m_reflectionRenderTarget);
		getSceneManager()->addRenderPass(m_reflectionPass);

		//m_camera->enableReflection(math::Plane(0,math::vector3d::YAxis));

		m_reflectionTU->setTextureClamp(video::ETW_WrapR, video::ETC_REPEAT);
		m_reflectionTU->setTextureClamp(video::ETW_WrapS, video::ETC_REPEAT);
		m_reflectionTU->setTextureClamp(video::ETW_WrapT, video::ETC_REPEAT);

		GCPtr<video::SMaterial>mtrl= gMaterialResourceManager.getMaterial("WaterMtrl");


		if(mtrl){
		mtrl=mtrl->duplicate();
		mtrl->setTextureUnit(m_reflectionTU,0);
		//mtrl->setTexture(gTextureResourceManager.loadTexture2D("F15Diff.png"),0);
		node->setMaterial(mtrl,0);
		}

		m_nodes.push_back(node);



		GCPtr<scene::SMesh> mesh=new scene::SMesh();
		gMeshResourceManager.addResource(mesh,mT("sphere"));

		GCPtr<scene::MeshBufferData> bd= mesh->addNewBuffer();

		scene::MeshGenerator::getInstance().generateSphere(1,8,8,bd->getMeshBuffer());

		float angle=0;
		int n=0;
		for(int i=0;i<n;i++){
		for(int j=0;j<n;j++){
		node=new scene::MeshSceneNode(mesh,mT(""),0,
		math::vector3d((i-n/2)*10,80+abs(sind(angle))*30,(j-n/2)*10),
		quaternion::Identity,1,0);
		node->ShadowCaster=true;
		node->ShadowReciver=true;
		getSceneManager()->addSceneNode(node);

		math::vector3d pos=node->getPosition();
		pos.y=m_terrain->getHeight(pos.x,pos.z)+0;
		node->position=pos;

		//getSceneManager()->Animators.addCircleAnimator(node,node->getPosition(),math::vector3d(3,1,1),50,50*(math::Randomizer::rand01()*2-1));

		angle=i+10*j;

		m_nodes.push_back(node);
		}
		}

		node=scene::SceneNodeCreator::addTorus(getSceneManager(),10,3,10,10,math::vector3d(0,0,0));
		node->ShadowCaster=true;
		node->ShadowReciver=true;

		//getSceneManager()->Animators.addRotateAnimator(node,math::vector3d(3,1,1),300);

		m_nodes.push_back(node);
		}*/


		//getSceneManager()->Animators.addCircleAnimator(m_light,math::vector3d(0,5000,0),math::vector3d::YAxis,5000,100);



		//m_camera->ZFar=1000;
		//m_camera->fovY=(math::OneFourthPI32);

		return;
#if 0
		math::vector2di vpSize=getDevice()->getViewportRect().getSize();
		GCPtr<video::ITexture> rtTex=getDevice()->createTexture2D(vpSize,video::EPixel_Float16_RGB);
		m_viewPort->setRenderTarget(getDevice()->createRenderTarget(mT(""),rtTex,0,0,0));

		GCPtr<video::CMultiPassPP> pp=new video::CMultiPassPP(getDevice());
		m_hdr=new video::HDRPP(getDevice(),vpSize);
		pp->addPostProcessor(m_hdr);

		GCPtr<video::ParsedShaderPP> pShader=new video::ParsedShaderPP(getDevice());
		pShader->LoadXML(gFileSystem.openFile("Streaks.peff"));
		//pp->addPostProcessor(pShader);

		m_viewPort->setPostProcessing(pp);
		m_viewPort->enablePostProcessing(false);
#endif
		//m_Exposure=getGUIManager()->addSlider(0,100,math::vector2d(10,10),math::vector2d(200,30),this);
//		m_Threshold=getGUIManager()->addSlider(0,100,math::vector2d(10,50),math::vector2d(200,30),this);
		return;

		shadowMtrl=gMaterialResourceManager.getMaterial(mT("GBufferMaterial"));
		/*
		for (int i=0;i<m_nodes.size();i++)
		{
		m_nodes[i]->setMaterial(shadowMtrl,0);
		}*/

		//////////////////////////////////////////////////////////////////////////

		m_viewPort->enablePostProcessing(1);




//		GCPtr<GUI::GUIImage> img;

//		GCPtr<video::IPostProcessing> g=new GlarePP(getDevice(),vpSize);
		/*
		img=getGUIManager()->addImage(vector2d(10,300),100);
		img->setTexture(g->m_lumRT[GlarePP::maxLuminance-1]->getColorTexture());
		img=getGUIManager()->addImage(vector2d(10,400),200);
		img->setTexture(g->m_blurTU[0]->texture);

		return;

		GCPtr<scene::GBufferListener> gbuff=new scene::GBufferListener(getDevice(),m_viewPort);
		m_viewPort->addListener(gbuff);

		GCPtr<video::ToonPP> g=new video::ToonPP(getDevice(),vpSize);
		g->setColorTexture(gbuff->getColorRT());
		g->setNormalTexture(gbuff->getNormalRT());
		g->setDepthTexture(gbuff->getDepthRT());
		*/
	//	pp->addPostProcessor(g);
		/*
		m_hdr=new video::HDRPP(getDevice(),vpSize);
		pp->addPostProcessor(m_hdr);
		*/


//		m_viewPort->setPostProcessing(pp);

	}

	double m_updateTime;

	void update(float dt)
	{
		//PROFILE_FUNCTION();
		double t=gTimer.getSeconds();
		CMRayApplication::update(dt);
		m_updateTime=gTimer.getSeconds()-t;

		m_soundRec->CaptureSamples(m_recorderSamples);
		core::array<uchar>& data= m_soundEncoder.EncodeBuffer(m_recorderSamples);
		int sz=m_encodedSamples.size();
		m_encodedSamples.increaseUsed(data.size());
		memcpy(m_encodedSamples.pointer()+sz,data.pointer(),data.size());
	//	m_recorderSamples.resize(0);

		for (int i=0;i<m_emitters.size();++i)
		{
			m_emitters[i].Update(m_soundManager->getListener());
			m_debugManager->AddCross(m_emitters[i].GetAttachedSound()->getPosition(),1,video::SColor(255,0,0,255),0);
		}

		m_soundManager->runSounds(dt);


		m_wiiMngr->PollEvents();
		((scene::SceneManager*)getSceneManager())->GetJobsManager()->EndProcessing();
	}
	virtual void draw()
	{
		m_debugManager->StartDraw(m_camera);
		GUI::IFont* font=gFontResourceManager.getDefaultFont();
		if(m_barrier->GetBlockedThreads()>1)
		{
			//if(!getGUIManager())return;
			getDevice()->set2DMode();
			m_guiRender->Prepare();
			if(font){
				static float angle=0;
				GUI::FontAttributes attr;
				attr.fontColor.Set(20,255,128,255);
				attr.fontAligment=GUI::EFA_MiddleLeft;
				attr.fontSize=20;
				attr.hasShadow=true;
				attr.shadowColor.Set(0,0,0,255);
				attr.shadowOffset=vector2d(2);
				attr.spacing=2;
				attr.wrap=0;
				attr.RightToLeft=0;



				core::string msg=mT("Loading..");
				font->print(math::rectf(10,10,10,10),&attr,0,(utf8*)msg.c_str(),m_guiRender);

				getDevice()->draw2DImageRot(math::rectf(100,100,200,200),0,0,255,angle);

				angle+=150*gFPS.dt();

				m_guiRender->Flush();

			}
		}else
		{

			CMRayApplication::draw();
			m_debugManager->EndDraw();
			//		m_camFrustrum.set(m_projection*vm);

			getDevice()->set3DMode();


			getDevice()->drawBox(m_camera->getViewFrustrum()->boundingBox,math::matrix4x4::Identity,video::MR_LINES);


			m_matProj=buildCameraLookAtLH(m_camera->getPosition(),m_camera->getTarget(),m_camera->getUpVector());

			getDevice()->set2DMode();
			m_guiManager->DrawAll();

			//	tu.texture=m_hdr->m_luminance->getOutput()->getColorTexture();
			//	getDevice()->draw2DImage(rectf(10,10,100,100),&tu,255);
			//	getDevice()->draw2DImage(rectf(0,0,256,256),m_reflectionTU,255);


			float y=0;
			//	for(int i=0;i<m_hdr->m_luminance->m_lumCount;++i){
			int i=1;
			//tu.texture=m_hdr->m_luminance->m_lumRT[i]->getColorTexture();
			//	getDevice()->draw2DImage(rectf(10,10+y,100,y+100),&tu,255);
			//	y+=110;
			//	}

			GUI::TextContextAttributes context;
			context.device=getDevice();
			context.pos=math::vector2d(100,40);
			context.font=font;

			m_textDecorate->CalculateSize(&context);
//			m_textDecorate->Draw(&context,getGUIManager()->GetRenderQueue());

			m_guiRender->Prepare();

			if(font){
				GUI::FontAttributes attr;
				attr.fontColor.Set(20,255,128,255);
				attr.fontAligment=GUI::EFA_MiddleLeft;
				attr.fontSize=20;
				attr.hasShadow=true;
				attr.shadowColor.Set(0,0,0,255);
				attr.shadowOffset=vector2d(2);
				attr.spacing=2;
				attr.wrap=0;
				attr.RightToLeft=0;

				core::string msg=mT("FPS= ");
				msg+=core::StringConverter::toString(CFPS::getInstance().getFPS());
				msg+=" , ";
				msg+=core::StringConverter::toString(m_updateTime);
				font->print(math::rectf(10,10,10,10),&attr,0,(utf8*)msg.c_str(),m_guiRender);
				attr.fontSize=14;
				msg=mT("Primitive drawn : ");
				msg+=core::StringConverter::toString(getDevice()->getPrimitiveDrawnCount());
				font->print(math::rectf(10,30,10,10),&attr,0,(utf8*)msg.c_str(),m_guiRender);
				msg=mT("Batches drawn : ");
				msg+=core::StringConverter::toString(getDevice()->getBatchDrawnCount());
				attr.spacing=0;
				font->print(math::rectf(10,50,10,10),&attr,0,(utf8*)msg.c_str(),m_guiRender);



				msg=mT("camera : ")+core::StringConverter::toString(m_camera->getPosition());
				//	msg+=core::StringConverter::toString(m_hdr->GetExposure());
				font->print(math::rectf(10,70,10,10),&attr,0,m_arStr,m_guiRender);
				attr.wrap=true;
				//	font->print(math::rectf(10,90,240,260),&attr,&math::rectf(10,90,240,260),"Hello World\nmy name is\n Muhammad Yamen Saraiji,I'm from Syria,!@#%^&*()_+|\\=-;,.';\"I hope if I can complete my study in Japan,Thank you!",getGUIManager()->GetRenderQueue());

				if(m_soundRec->IsRecording())
				{
					msg=mT("Recording");
					//	msg+=core::StringConverter::toString(m_hdr->GetExposure());
					font->print(math::rectf(10,90,10,10),&attr,0,msg,m_guiRender);
				}

				m_guiRender->Flush();

			}
		}


	}

	virtual void onDone(){
		traceFunction(Application);
		m_soundManager.dispose();

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
};



APPLICATION_ENTRY_POINT{

	GCPtr<Application> app=new Application();

	core::string resFileName;
	resFileName=mT("plugins.stg");
	app->loadResourceFile(mT("dataPath.stg"));
	if(app->startup(mT("MRaY App"),vector2di(800,600),false,resFileName,0,true,false))
		app->run();
	app=0;
}
