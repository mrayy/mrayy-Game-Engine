

#include "stdafx.h"
#include "PIDApplication.h"
#include "ImageSetResourceManager.h"
#include "GUIThemeManager.h"
#include "FontResourceManager.h"
#include "ViewPort.h"

#include "LuaScriptManager.h"
#include "FluidSolver.h"
#include "Fluid2DTexture.h"
#include "CameraOrbitComp.h"

namespace mray
{

PIDApplication* PIDApplication::Instance=0;

PIDApplication::PIDApplication()
{
	Instance=this;
	m_fluidTexture=0;
	m_animating=true;
}

PIDApplication::~PIDApplication()
{
}


void PIDApplication::_InitResources()
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
void PIDApplication::onEvent(Event* event)
{
	if(event->getType()==ET_Mouse)
	{
		if(m_fluidSolver)
		{
			static float h=0;
			MouseEvent* e=dynamic_cast<MouseEvent*>(event);
			static math::vector2d oldPos;
			static float t=0;
			h+=0.001f;
			h=math::cycle(h,0.f,1.0f);
			math::vector2d vec=(e->pos-oldPos)*10;
			video::SColor clr(1,0,0,1);
			m_fluidSolver->Lock();
			m_fluidSolver->AddForceAtCell(e->pos.y/10,e->pos.x/10,vec);
			float brightness=0;
			for(int a1=0;a1<360;a1+=45)
			{
				clr.setHSL(h,1,a1/360.0f);
				for(int r=0;r<3;++r)
				{
					for(int a=0;a<360;a+=10)
					{
						float x=r*math::cosd(a)+30*math::cosd(a1+t);
						float y=r*math::sind(a)+30*math::sind(a1+t);
						m_fluidSolver->AddColorAtCell(y+e->pos.y/10,x+e->pos.x/10,clr);
					}
				}
			}
			t+=1;
			m_fluidSolver->Unlock();
			oldPos=e->pos;
		}
	}
	if(event->getType()==ET_Keyboard)
	{
		KeyboardEvent* e=dynamic_cast<KeyboardEvent*>(event);
		if(e->press)
		{
			if(e->key==KEY_1)
			{
				m_fluidTexture->SetSourceType(FX::FluidSource_Color);
			}else if(e->key==KEY_2)
			{
				m_fluidTexture->SetSourceType(FX::FluidSource_Motion);
			}else if(e->key==KEY_3)
			{
				m_fluidTexture->SetSourceType(FX::FluidSource_Velocity);
			}
			if(e->key==KEY_UP || e->key==KEY_DOWN)
			{
				m_fluidSolver->GetEditDesc().solverIterations+=(e->key==KEY_UP?1:-1);
			}
			if(e->key==KEY_SPACE)
				m_animating=!m_animating;
		}
	}
}


void PIDApplication::CreatePhysicsSystem()
{
	physics::PhysicsSystemDesc desc;
	desc.gravity.set(0,-9.,0);
	desc.maxIter=32;

	desc.maxTimestep=1.0f/(16.0f*60.0f);
	desc.useFixedTimeStep=true;
	m_phManager=new physics::PhysXManager(&desc);


}

void PIDApplication::CreateFluid()
{
	FX::FluidSolverDesc desc;

	desc.resolution.set(256,64);
	desc.doRGB=1;
	desc.solverIterations=5;
	desc.fadeSpeed=0.05;
	desc.colorDiffusion=0.00;
	desc.wrapX=false;
	desc.wrapY=false;
	desc.viscocity=0.00001;

	m_fluidSolver=new FX::FluidSolver();
	m_fluidTexture=new FX::Fluid2DTexture();
	m_fluidSolver->SetupFromDesc(desc);
	m_fluidTexUnit.SetTexture(getDevice()->createEmptyTexture2D(false));
	m_fluidTexture->SetFluidSolver(m_fluidSolver);
	m_fluidTexture->SetTexture(m_fluidTexUnit.GetTexture());
	m_fluidTexture->SetSourceType(FX::FluidSource_Color);


	for(int i=5;i<20;++i)
	{
		for(int j=5;j<20;++j)
			m_fluidSolver->AddColorAtCell(i,j,video::SColor(1,0,0,1));
		for(int j=30;j<40;++j)
			m_fluidSolver->AddColorAtCell(i,j,video::SColor(0,0,1,1));
		for(int j=30;j<40;++j)
			m_fluidSolver->AddColorAtCell(j,i,video::SColor(01,1,0,1));
	}
	for(int j=30;j<40;++j)
		m_fluidSolver->AddForceAtCell(j,50,math::vector2d(0,-j*20));
}

void ExportApplicationToLua(lua_State* L);

class ProjectionPlane
{
public:

	math::Plane m_plane;
	math::vector3d m_origin;
	scene::CameraNode* m_camera;
	math::vector2d size;
	math::vector2d position;
	float len[4];
	float clen[4];

	math::Ray3d reflectedRays[4];

	ProjectionPlane()
	{
		size=2;
	//	m_plane.set(math::quaternion(15,math::vector3d(1,0,0))*math::vector3d::YAxis,0);
	}
	void ProjectCamera(scene::CameraNode* cam)
	{
		cam=m_camera;
		m_origin=cam->getAbsolutePosition();
		//form 4 rays from camera position
		math::vector3d rays[4];
		float hfov=cam->getFovY()/2;
		float vfov=hfov;
		rays[0]=cam->getOrintation()*math::quaternion(-hfov,math::vector3d::YAxis)*math::quaternion(-vfov,math::vector3d::XAxis)*math::vector3d::ZAxis;
		rays[1]=cam->getOrintation()*math::quaternion( hfov,math::vector3d::YAxis)*math::quaternion(-vfov,math::vector3d::XAxis)*math::vector3d::ZAxis;	
		rays[2]=cam->getOrintation()*math::quaternion( hfov,math::vector3d::YAxis)*math::quaternion( vfov,math::vector3d::XAxis)*math::vector3d::ZAxis;
		rays[3]=cam->getOrintation()*math::quaternion(-hfov,math::vector3d::YAxis)*math::quaternion( vfov,math::vector3d::XAxis)*math::vector3d::ZAxis;	

		for(int i=0;i<4;++i)
		{
			//m_plane.getIntersectionWithLine(cam->getAbsolutePosition(),rays[i],m_points[i]);
			ReflectRay(math::Ray3d(cam->getAbsolutePosition(),rays[i]),reflectedRays[i]);
			clen[i]=(cam->getAbsolutePosition()-reflectedRays[i].Start).Length();
		}
		len[0]=(reflectedRays[0].Start-reflectedRays[1].Start).Length();
		len[1]=(reflectedRays[1].Start-reflectedRays[2].Start).Length();
		len[2]=(reflectedRays[2].Start-reflectedRays[3].Start).Length();
		len[3]=(reflectedRays[3].Start-reflectedRays[0].Start).Length();
	}

	bool ReflectRay(const math::Ray3d& r,math::Ray3d& ret)
	{
		if(!m_plane.getIntersectionWithLine(r.Start,r.Dir,ret.Start))
		{
			return false;
		}
		ret.Dir=r.Dir-m_plane.Normal*2*m_plane.Normal.dotProduct(r.Dir);
		ret.Dir.Normalize();
		ret.Dir=-ret.Dir;
		return true;

	}

	void Draw()
	{
		video::IVideoDevice* dev= Engine::getInstance().getDevice();

		int edges[]=
		{
			0,1,
			1,2,
			2,3,
			3,0
		};
		math::vector3d points[4];
		points[0]=points[3]=m_origin;
		for(int i=0;i<8;i+=2)
		{
			points[1]=reflectedRays[edges[i]].Start;
			points[2]=reflectedRays[edges[i+1]].Start;
			dev->drawLine(&points[0],4,1);
		}
		for(int i=0;i<4;++i)
		{
			points[0]=reflectedRays[i].Start;
			points[1]=reflectedRays[i].Dir*10;
			dev->drawLine(&points[0],2,1);
		}
	}
}ptPlane;

void PIDApplication::init(const OptionContainer &extraOptions)
{
	CMRayApplication::init(extraOptions);
	_InitResources();

	m_limitFps=true;

	script::LuaScriptManager *scriptManager=new script::LuaScriptManager ();
	ExportApplicationToLua(scriptManager->GetLuaState());

	//scriptManager->BindObject("App",this);
	luabind::globals(scriptManager->GetLuaState())["App"]=getInstance();
	script::IScript* s= scriptManager->createScript("");
		

	m_guiRender=new GUI::GUIBatchRenderer();
	m_guiRender->SetDevice(getDevice());

	CreatePhysicsSystem();

	m_gameManager=new game::GameEntityManager();
	m_gameManager->SetPhysicsManager(m_phManager);
	m_gameManager->SetSceneManager(getSceneManager());

	s->SetScript(gFileSystem.openFile("Script.lua"));
	s->ExecuteScript();
	//m_gameManager->loadFromFile("tachilab.xml");



	m_camera=getSceneManager()->createCamera();
	scene::ViewPort*vp= GetRenderWindow()->CreateViewport("Camera",m_camera,0,math::rectf(0,0,1,1),0);
	vp->SetClearColor(video::DefaultColors::Blue);

	m_camera->setPosition(math::vector3d(0,0,10));

	ptPlane.m_camera=getSceneManager()->createCamera();
	ptPlane.m_camera->setFovY(60);
	ptPlane.m_camera->setPosition(math::vector3d(0,1,0));
	ptPlane.m_camera->setOrintation(math::quaternion(135,math::vector3d(1,0,0)));


	game::GameEntity* ent= m_gameManager->CreateGameEntity("CameraEnt");
	game::CameraOrbitComp* camOrbit=new game::CameraOrbitComp();
	camOrbit->SetCamera(m_camera,m_inputManager);
	ent->AddComponent(camOrbit);

	CreateFluid();
}

void PIDApplication::CreateScene()
{

}

void PIDApplication::WindowPostViweportUpdate(video::RenderWindow* wnd,scene::ViewPort* vp)
{
	getDevice()->set3DMode();
	if(vp->getCamera())
	{
		ptPlane.ProjectCamera(vp->getCamera());
		ptPlane.Draw();
	}
	m_gameManager->DebugRender(0);
	getDevice()->set2DMode();
	if(m_fluidTexture)
	{
		static ulong lastT=gTimer.getActualTime();
		ulong currT=gTimer.getActualTime();
		if(currT-lastT>1000/60)
		{
			m_fluidTexture->UpdateTexture();

			lastT=currT;
		}
		getDevice()->useTexture(0,&m_fluidTexUnit);
		getDevice()->draw2DImage(math::rectf(0,0,1000,1000),1);
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

		attr.fontSize=18;

		float l[4];
		font->print(math::rectf(50,70,50,50),&attr,0,core::StringConverter::toString(ptPlane.len[0]),m_guiRender);
		font->print(math::rectf(50,90,50,50),&attr,0,core::StringConverter::toString(ptPlane.len[1]),m_guiRender);
		font->print(math::rectf(50,110,50,50),&attr,0,core::StringConverter::toString(ptPlane.len[2]),m_guiRender);
		font->print(math::rectf(50,130,50,50),&attr,0,core::StringConverter::toString(ptPlane.len[3]),m_guiRender);


		font->print(math::rectf(250,70,50,50),&attr,0,core::StringConverter::toString(ptPlane.clen[0]),m_guiRender);
		font->print(math::rectf(250,90,50,50),&attr,0,core::StringConverter::toString(ptPlane.clen[1]),m_guiRender);
		font->print(math::rectf(250,110,50,50),&attr,0,core::StringConverter::toString(ptPlane.clen[2]),m_guiRender);
		font->print(math::rectf(250,130,50,50),&attr,0,core::StringConverter::toString(ptPlane.clen[3]),m_guiRender);
		m_guiRender->Flush();
	}
}

void PIDApplication::update(float dt)
{
	CMRayApplication::update(dt);

	if(m_animating)
		ptPlane.m_camera->rotate(math::quaternion(10*gFPS.dt(),math::vector3d(1,0,0)),scene::TS_Local);
	m_fluidSolver->Update(dt);
	m_phManager->update(dt);
	m_gameManager->Update(dt);
}

void PIDApplication::onDone()
{
}

}
