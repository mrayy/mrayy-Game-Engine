
#include "stdafx.h"

#include "../common/DemoCamera.h"
#include "../common/GBufferListener.h"
#include "../common/GBufferListener.h"


#include "GUIXMLElementVisitor.h"
#include "SGShaderManager.h"
#include "SGFunctionPrototype.h"
#include "BaseFont.h"
#include "BasicTypesGUID.h"

#include "IGUIListBox.h"
#include "IGUIComboList.h"
#include "IGUIPropertyGrid.h"
#include "IGUIStackPanel.h"
#include "IGUICatagoryPanel.h"
#include "IGUISliderBar.h"
#include "RenderWIndow.h"

#include "CanvasManager.h"
#include "CanvasUtilities.h"
#include "ICanvasObject.h"
#include "FilledShape.h"
#include "ColoredShape.h"
#include "TexturedShape.h"
#include "TextShape.h"
#include "GUICanvasObject.h"

#include "IConnector.h"
#include "ConnectionCurve.h"
#include "BHCanvasManager.h"

#include "BHSelectorObject.h"
#include "BHSelectorNode.h"

#include "BHSequenceObject.h"
#include "BHSequenceNode.h"

#include "IBHActionNode.h"
#include "BHActionObject.h"

#include "BHTreeObject.h"
#include "BHDecoratorObject.h"

#include "mrayLua.h"

#include "PhysicalVehicle.h"
#include "VehicleDesc.h"
#include "VehicleGearDesc.h"
#include "VehicleMotorDesc.h"
#include "VehicleWheelDesc.h"
#include "VehicleManager.h"
#include "VehicleMotor.h"
#include "VehicleGear.h"
#include "VehicleXMLParser.h"
#include "IVehicleWheel.h"

#include "ColorCurve.h"
#include "LUTGenerator.h"
#include "Noise3D.h"
#include "FractalTypes.h"

#include <PolyVoxCore/LargeVolume.h>
#include <PolyVoxCore/MaterialDensityPair.h>
#include <PolyVoxCore/SurfaceExtractor.h>
#include "PolyVoxCore/Log.h"
#include "PolyVoxCore/SurfaceMesh.h"
#include "PolyVoxCore/MeshDecimator.h"
#include "PolyVoxCore/LowPassFilter.h"
#include "PolyVoxCore/AmbientOcclusionCalculator.h"
#include "Perlin.h"

#include "LanguageFontSpecification.h"
#include "RayNodeSelector.h"
#include "BoxNodeSelector.h"
#include "FunctionProfileManager.h"
#include "FunctionProfiler.h"

#include "StereoRenderer.h"
#include "AtmosphericCubeMap.h"
#include "TextureResourceManager.h"
#include "ShaderResourceManager.h"
#include "ImageSetResourceManager.h"
#include "FontResourceManager.h"
#include "ResourceGroup.h"
#include "TraceManager.h"
#include "GUIThemeManager.h"
#include "ISound.h"
#include "PhysicsComponent.h"
#include "XMLSceneLoader.h"
#include <mrayGLPlugin.h>

using namespace mray;
using namespace math;
using namespace core;
using namespace PolyVox;
/*
class SimpleGameAction:public game::IGameAction
{
	physics::IPhysicalNode* m_phNode;
	game::GameEntity* m_ent;
public:
	SimpleGameAction(physics::IPhysicalNode* phNode):game::IGameAction(mT("SimpleGA"))
	{
		m_phNode=phNode;
		m_ent=0;
	}
	virtual ~SimpleGameAction()
	{
	}

	virtual void AttachToEntity(game::GameEntity*ent)
	{
		m_ent=ent;
	}

	virtual void Update(float dt)
	{
		if(m_ent && m_phNode)
		{
			m_ent->GetShape()->setPosition(m_phNode->getGlobalPosition());
			m_ent->GetShape()->setOrintation(m_phNode->getGlobalOrintation());
		}
	}

};
*/
class WheelComponent:public game::IGameComponent
{
	physics::IVehicleWheel* m_phNode;
	physics::PhysicalVehicle* m_car;

	scene::IMovable* m_bone;

	float m_angle;
public:
	WheelComponent(scene::IMovable* bone,physics::IVehicleWheel* phNode,physics::PhysicalVehicle*v)
	{
		m_angle=0;
		m_phNode=phNode;
		m_bone=bone;

		m_car=v;
	}
	virtual ~WheelComponent()
	{
	}

	virtual void Update(float dt)
	{
		if(m_phNode)
		{
			math::matrix4x4 parent;
			m_car->GetPhysicalNode()->getGlobalOrintation().toMatrix(parent);
			parent.setTranslation(m_car->GetPhysicalNode()->getGlobalPosition());

			m_angle+=math::toDeg(m_phNode->GetRPM()*dt*math::TwoPI32);

			//m_bone->setPosition(parent*m_phNode->GetWheelPos());
			/*
			math::quaternion q(m_phNode->GetGlobalOrintation());
			q*=math::quaternion(m_phNode->GetSteeringAngle(),vector3d::YAxis);
			q*=math::quaternion(m_angle,vector3d::XAxis);*/
			math::quaternion q=math::quaternion(m_phNode->GetSteeringAngle(),vector3d::YAxis)*
				math::quaternion(m_angle,vector3d::XAxis);
			m_bone->setOrintation(q);
		}
	}

};
class RotaterGameAction:public game::IGameComponent
{
	float m_angle;
public:
	RotaterGameAction():game::IGameComponent()
	{
		m_angle=0;
	}
	virtual ~RotaterGameAction()
	{
	}


	virtual void Update(float dt)
	{
		if(GetOwnerEntity())
		{
			GetOwnerEntity()->GetShape()->setOrintation(math::quaternion(m_angle,vector3d::YAxis)*
				math::quaternion(0.1*m_angle,vector3d::XAxis));
			m_angle+=1000*dt;
		}
	}

};

class CustomProfilerListener:public IFunctionProfileListener
{
public:
	virtual void OnProfileEnd(FunctionProfiler*p)
	{
		printf(("method[%s] execution time:%f\n"),p->GetName().c_str(),p->GetLastExecutionTime());
	}
}g_functionListener;


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


class Application:public CMRayApplication
{

protected:

	class PhysicsReporter:public physics::IPhysicUserContactReport
	{
	public:

		scene::DebugDrawManager* m_debugger;

		PhysicsReporter():m_debugger(0)
		{
		}

		virtual void OnPairTouch(physics::PhysicContactPair* pair)
		{
			while (pair->contactStream->NextPair())
			{
				while (pair->contactStream->NextPatch())
				{
					while (pair->contactStream->NextPoint())
					{
						m_debugger->AddLine(pair->contactStream->GetPoint(),
							pair->contactStream->GetPoint()+pair->contactStream->GetPatchNormal()*5
							,video::SColor(1,0,0,1),1,0);
					}
				}
			}
		}
	};

	class PhyiscsSceneQueryReporter:public physics::IPhysicSceneQueryReport
	{
	public:
		scene::DebugDrawManager* m_debugger;

		PhyiscsSceneQueryReporter():m_debugger(0)
		{
		}

		virtual physics::EQueryReportResult OnRaycastQuery(void*userData,const std::vector<physics::PhysicsRaycaseHit>& hits)
		{
			for(int i=0;i<hits.size();++i)
			{
				m_debugger->AddLine(hits[i].worldImpact,
					hits[i].worldImpact+hits[i].worldNormal*5
					,video::SColor(0,1,0,1),1,10);
			}
			return physics::EQResult_Continue;
		}

		virtual physics::EQueryReportResult OnShapeQuery(void*userData,const std::vector<physics::IPhysicalShape*>& hits)
		{
			return physics::EQResult_Continue;
		}

		virtual physics::EQueryReportResult OnSweepQuery(void*userData,const std::vector<physics::PhysicsSweepQueryHit>& hits)
		{
			return physics::EQResult_Continue;
		}
	}m_SQReporter;

	math::matrix4x4 m_projection;

	GCPtr<video::IRenderTarget> m_shadowRenderTarget;

	static const int shadowMapWidthHeight=2048;

	GCPtr<GUI::IGUISliderBar> m_offsetGUI;
	GCPtr<scene::StereoRenderer> m_stereoRenderer;
	GCPtr<scene::CameraNode> m_cameraL;
	GCPtr<scene::CameraNode> m_cameraR;

	GCPtr<scene::CameraNode> m_secCamera;
	GCPtr<scene::LightNode> m_light;
	GCPtr<animation::IterativeIK> m_ik;


	math::vector3d m_targetVel;
	math::vector3d m_target;
	scene::BoneNode* m_ef;

	bool m_freeCam;

	GCPtr<scene::DebugDrawManager > m_debugManager;

	GCPtr<physics::IPhysicManager> m_phManager;

	core::array<GCPtr<scene::MeshRenderableNode>> m_nodes;

	GCPtr<video::SMaterial> shadowMtrl;

	GCPtr<video::TextureUnit> m_renderUnit[4];


	GCPtr<scene::VegetationBuilder> m_vegetation;

	std::vector<video::TextureUnit> m_objectTex;
	std::vector<GCPtr<video::IRenderTarget>> m_objectRT;

	float m_distance;
	math::vector3d m_center;

	GCPtr<GUI::GUIBatchRenderer> m_guiRender;
	scene::SPatchTerrain*m_terrain;

	scene::ISceneNode* m_billboardChainNode;

	video::ITexturePtr m_screenShot;

	scene::ISceneNode* m_modelNode;

	GCPtr<GUI::GUIManager> m_guiManager;
	GCPtr<game::GameEntityManager> m_gameManager;

	GCPtr<physics::IPhysicSceneQuery> m_squery;

	GCPtr<scene::GBufferListener> m_gbuffer;

	GCPtr<video::IRenderTarget> m_renderTarget;
	GCPtr<video::GPUShader> m_clearRTShader;

	PhysicsReporter m_reporter;
	GCPtr<video::RTPickBuffer> m_pickBuffer;

	physics::IPhysicalCloth*cloth;
	bool m_clothSelected;
	uint m_clothVerID;
	float m_pickDepth;

	GCPtr<sound::ISoundManager> m_soundManager;

	GCPtr<game::AtmosphericCubeMap> m_atmosphericCubeMap;

	uint m_pickedID;

	video::ITexturePtr renderTargetTex;

	video::GPUShader* m_screenEffShader;
	canvas::CanvasManager* m_cmanager;
	GUI::IGUIPropertyGrid *propGrid;

	GCPtr<script::IScriptManager> m_scriptManager;
	GCPtr<physics::VehicleManager> m_vehicleManager;
	physics::PhysicalVehicle* m_carVehicle;
	math::vector3d m_vehControling;

	video::ParsedShaderPP* m_motionBlurPP;
	video::ParsedShaderPP* m_DofPP;
	video::ParsedShaderPP* m_antiAliasPP;
	video::ParsedShaderPP* m_LutPP;
	video::ParsedShaderPP* m_HDRPP;
	float m_hdrExposure;
	float m_hdrThreshold;

	GUI::IGUISliderBar* m_MieSlider;
	GUI::IGUISliderBar* m_RaleighSlider;
	GUI::IGUISliderBar* m_HGSlider;

	GUI::LanguageFontSpecification* m_langSpec;

	GCPtr<ThreadJobManager> m_appThreadManager;

	int m_activeCar;
	float m_focalDistance;
public:

	Application():CMRayApplication(),m_clothSelected(0)
	{
		m_billboardChainNode=0;
		m_pickedID=0;
		m_terrain=0;
		m_focalDistance=10;
		m_activeCar=0;
		m_langSpec=0;
		m_hdrExposure=0.1;
		m_hdrThreshold=0.1;
		m_freeCam=false;
	}

	virtual ~Application(){
	}
	virtual void onEvent(Event* event)
	{
		static math::rectf s_selRC;
		static bool s_selecting=false;
		CMRayApplication::onEvent(event);
		if(m_guiManager)
			m_guiManager->OnEvent(event);
		if(event->getType()==ET_Mouse)
		{
			MouseEvent*e=(MouseEvent*)event;
			if(m_cmanager)
				m_cmanager->ProcessMouseEvent(e);
			if(e->event==MET_LEFTDOWN)
			{
				s_selRC.ULPoint=e->pos;
				s_selecting=true;
//				m_pickBuffer->SetProjectionMatrix(m_camera->getProjectionMatrix());
	//			m_pickBuffer->SetViewMatrix(m_camera->getViewMatrix());
				//m_pickBuffer->Render();

				//m_pickedID= m_pickBuffer->GetID(e->pos.x,e->pos.y);

				if(cloth)
				{
					math::Ray3d ray;
					ray.Start=m_cameraL->getPosition();
					math::vector3d ppos=m_cameraL->ScreenToWorld(e->pos,1);
//					printf("%f,%f,%f,%f,%f\n",ppos.x,ppos.y,ppos.z,m_cameraL->getWindowDim().x,m_cameraL->getWindowDim().y);
					ray.Dir=(ppos-ray.Start).Normalize();
					m_debugManager->AddLine(ray.Start,ray.Dir*100.0f,video::DefaultColors::Red,2,10);
					math::vector3d hit;
					if(cloth->rayCast(ray,hit,m_clothVerID))
					{
						m_pickDepth=m_cameraL->WorldToScreen(hit).z;
						printf("selected %f\n",m_pickDepth);
						m_clothSelected=true;
					}
				}
			}else if(e->event==MET_MOVED)
			{
				s_selRC.BRPoint=e->pos;
				//if(s_selecting)
				//	TestSelection(0,&s_selRC);
				if(cloth && m_clothSelected)
				{
					math::vector3d pos=m_cameraL->ScreenToWorld(e->pos,m_pickDepth);
					cloth->attachVertexToGlobalPos(m_clothVerID,pos);
				}
			}else if(e->event==MET_LEFTUP)
			{
				s_selecting=false;
				if(m_clothSelected)
				{
					m_clothSelected=false;
					cloth->freeVertex(m_clothVerID);
				}
			}
		}
		if(event->getType()==ET_ResizeEvent)
		{
		}
		if(event->getType()==ET_Keyboard)
		{
			KeyboardEvent* e=(KeyboardEvent*)event;
			if(e->press && e->key==KEY_F5)
			{
				//m_screenShot= GetRenderWindow()->TakeScreenShot();
				//gTextureResourceManager.writeResourceToDist(m_screenShot,mT("screenShot.bmp"));
			}
			if(e->press && e->key==KEY_3)
			{
				m_DofPP->setEnable(!m_DofPP->isEnabled());
			}
			if(e->press && e->key==KEY_4)
			{
				m_motionBlurPP->setEnable(!m_motionBlurPP->isEnabled());
			}
			if(e->press && e->key==KEY_5)
			{
				m_antiAliasPP->setEnable(!m_antiAliasPP->isEnabled());
			}
			if(e->press && e->key==KEY_6)
			{
				m_LutPP->setEnable(!m_LutPP->isEnabled());
			}
			if(e->press && e->key==KEY_7)
			{
				m_HDRPP->setEnable(!m_HDRPP->isEnabled());
			}
			if(e->press && e->key==KEY_1)
			{
				m_freeCam=false;
			}
			if(e->press && e->key==KEY_2)
			{
				m_freeCam=true;
			}

/*
			if(e->press && e->key==KEY_1)
			{
				m_viewPort->setCamera(m_camera);
				m_camera->setVisible(true);
				m_secCamera->setVisible(false);
				getSceneManager()->setActiveCamera(m_camera);
			}
			if(e->press && e->key==KEY_2)
			{
				m_viewPort->setCamera(m_secCamera);
				m_camera->setVisible(false);
				m_secCamera->setVisible(true);
				getSceneManager()->setActiveCamera(m_camera);
			}*/
			if(e->press && e->key==KEY_SPACE)
			{
				m_squery->RaycaseAllShapes(math::Ray3d(m_cameraL->getPosition(),(m_cameraL->getOrintation()*math::vector3d::ZAxis).Normalize()),physics::EShape_All);
				m_squery->Execute();
				m_squery->Finish(true);

			}
			if(e->press && e->key==KEY_SPACE)
			{
				int mode=(m_stereoRenderer->GetMode()+1) % scene::EStereo_ModesCount;
				m_stereoRenderer->SetMode((scene::EStereoMode)mode);
			}
			if(e->key==KEY_P)
			{
				m_hdrExposure+=gFPS.dt();
				m_HDRPP->GetParam(mT("Exposure"))->SetValue(m_hdrExposure);
			}
			if(e->key==KEY_O)
			{
				m_hdrExposure-=gFPS.dt();
				m_hdrExposure=math::Max<float>(m_hdrExposure,0.0f);
				m_HDRPP->GetParam(mT("Exposure"))->SetValue(m_hdrExposure);
			}
			if(e->key==KEY_L)
			{
				m_hdrThreshold+=gFPS.dt();
				m_hdrThreshold=math::Min<float>(m_hdrThreshold,1.0f);
				m_HDRPP->GetParam(mT("Threshold"))->SetValue(m_hdrThreshold);
			}
			if(e->key==KEY_K)
			{
				m_hdrThreshold-=gFPS.dt();
				m_hdrThreshold=math::Max<float>(m_hdrThreshold,0.0f);
				m_HDRPP->GetParam(mT("Threshold"))->SetValue(m_hdrThreshold);
			}
			if(e->key==KEY_SPACE)
			{
				m_vehControling.z=e->press;
			}
			if(e->key==KEY_UP )
			{
				m_vehControling.x=e->press;
				m_targetVel.z=e->press;
			}else if(e->key==KEY_DOWN)
			{
				m_vehControling.x=-e->press;
				m_targetVel.z=-e->press;
			}else if(e->key==KEY_LEFT)
			{
				m_vehControling.y=-0.001*e->press;
				m_targetVel.x=e->press;
			}else if(e->key==KEY_RIGHT)
			{
				m_vehControling.y=0.001*e->press;
				m_targetVel.x=-e->press;
			}else if(e->key==KEY_HOME)
			{
				m_targetVel.y=e->press;
			}else if(e->key==KEY_END)
			{
				m_targetVel.y=-e->press;
			}else if(e->key==KEY_N && e->press)
			{
				m_activeCar++;
				if(m_activeCar>=m_vehicleManager->GetVehicles().size())
					m_activeCar=0;
				m_carVehicle=m_vehicleManager->GetVehicles()[m_activeCar];
			}
		}
	}

	
	void TestSelection(math::vector2d *pos,math::rectf* rc)
	{
		static scene::ISceneNode* s_lastSelected=0;
		static scene::NodesQueryList s_lastList;
		
		scene::MovableNodeList nodes;
		scene::NodesQueryList result;
		scene::QueryResultNode best;

		const scene::ISceneManager::SceneNodeMap& lst= getSceneManager()->GetChildren();
		scene::ISceneManager::SceneNodeMap::const_iterator nit=lst.begin();
		for(;nit!=lst.end();++nit)
			nodes.push_back(nit->second);

		if(pos)
		{
			scene::RayNodeSelector selector;
			math::line3d ray;
			ray.pStart= m_secCamera->ScreenToWorld(*pos,0);
			ray.pEnd= m_secCamera->ScreenToWorld(*pos,1);

			selector.setup(ray,nodes,0);
			best=selector.query(result);
		}
		if(rc)
		{
			scene::BoxNodeSelector selector;
			math::box3d box;
			box.MinP= m_secCamera->ScreenToWorld(rc->ULPoint,0);
			box.MaxP= m_secCamera->ScreenToWorld(rc->BRPoint,1);
			box.repair();

			selector.setup(box,nodes,0);
			best=selector.query(result);
		}


		scene::NodesQueryList::iterator it= s_lastList.begin();
 		for(;it!=s_lastList.end();++it)
		{
			(dynamic_cast<scene::ISceneNode*>(it->node))->setDrawBoundingBox(false);
		}
		if(best.node)
		{
			//printf(mT("node selected: %s\n"),best.node->getNodeName().c_str());

		}
		s_lastSelected=dynamic_cast<scene::ISceneNode*>(best.node);
		s_lastList=result;
		it= s_lastList.begin();
		for(;it!=s_lastList.end();++it)
		{
			(dynamic_cast<scene::ISceneNode*>(it->node))->setDrawBoundingBox(true);
		}
	}

	void TestShaderGeneration()
	{
		GCPtr<shader::SGProgramGeneratorCG> generator=new shader::SGProgramGeneratorCG();
		GCPtr<shader::SGProgram> program=new shader::SGProgram(shader::EPT_VERTEX);
		program->AddUniform(new shader::SGVariable(mT("weight"),video::EUT_Sampler1D,shader::EVU_NORMAL,shader::EUS_UNKOWN,0,mT("Weight of the vertex")));
		program->AddDependency(mT("shadow"));
		program->AddDependency(mT("math"));
		program->AddDependency(mT("operators"));

		shader::SGFunction* mainVP= program->CreateFunction(mT("Main"),shader::EFT_MAIN_VERTEX);
		program->SetEntryPoint(mainVP);

		shader::SGVariablePtr Pos= mainVP->AddInputVariable(mT("Pos"),video::EUT_Float4,false,shader::EUS_POSITION);
		shader::SGVariablePtr normal= mainVP->AddInputVariable(mT("normal"),video::EUT_Float3,false,shader::EUS_NORMAL);
		shader::SGVariablePtr texCoord= mainVP->AddInputVariable(mT("texCoord"),video::EUT_Float2,false,shader::EUS_TEXCOORD,0);
		shader::SGVariablePtr WorldMat= mainVP->AddInputVariable(mT("WorldMat"),video::EUT_Matrix4x4,true,shader::EUS_UNKOWN);
		shader::SGVariablePtr WorldViewProjMat= mainVP->AddInputVariable(mT("WorldViewProjMat"),video::EUT_Matrix4x4,true,shader::EUS_UNKOWN);
		shader::SGVariablePtr LightViewProj= mainVP->AddInputVariable(mT("LightViewProj"),video::EUT_Matrix4x4,true,shader::EUS_UNKOWN);
		shader::SGVariablePtr oPos= mainVP->AddOutputVariable(mT("oPos"),video::EUT_Float4,shader::EUS_POSITION);
		shader::SGVariablePtr oTexCoord=  mainVP->AddOutputVariable(mT("oTexCoord"),video::EUT_Float2,shader::EUS_TEXCOORD,0);
		shader::SGVariablePtr oNormal= mainVP->AddOutputVariable(mT("oNormal"),video::EUT_Float3,shader::EUS_TEXCOORD,1);
		shader::SGVariablePtr viewDir= mainVP->AddOutputVariable(mT("viewDir"),video::EUT_Float3,shader::EUS_TEXCOORD,2);
		shader::SGVariablePtr lightPos= mainVP->AddOutputVariable(mT("lightPos"),video::EUT_Float4,shader::EUS_TEXCOORD,3);

		shader::SGVariablePtr worldPos= new shader::SGVariable(mT("worldPos"),video::EUT_Float4,shader::EVU_NORMAL,shader::EUS_UNKOWN,0,mT("World position of the vertex"));
		mainVP->AddVariable(worldPos);


		shader::SGFunctionCall*mulCall=new shader::SGFunctionCall(mT("mul"));

		mulCall->AddInputValue(WorldViewProjMat);
		mulCall->AddInputValue(Pos);

		mainVP->AddStatement(new shader::SGOperation(worldPos,mulCall));
		mainVP->AddStatement(new shader::SGOperation(worldPos,viewDir));


		shader::SGFunctionCall*call=new shader::SGFunctionCall(mT("diffuse"));

		call->AddInputValue(normal);
		call->AddInputValue(Pos);
		mainVP->AddStatement(call);

		mainVP->SetReturnValue(new shader::SGMultiValue(video::EUT_Float4,mT("0"),mT("0"),mT("1"),mT("1")));
		core::string shader=generator->GenerateShader(program);

//		printf(shader.c_str());
		{

			shader::SGFunctionPrototype*proto= mainVP->CreatePrototype(0);
			xml::XMLElement*e=program->Save(0);//proto->Save(0);
			xml::XMLWriter writer;
			writer.addElement(e);
			core::string str=writer.flush();

			OS::IStreamPtr stream=gFileSystem.createBinaryFileWriter(gFileSystem.getAppPath()+mT("\\shader.xml"));
			OS::StreamWriterPtr streamWr=new OS::StreamWriter(stream);
			//streamWr->writeString(shader);
			streamWr->writeString(str);

			stream->close();
		}
		{
			xml::XMLTree tree;
			OS::IStreamPtr stream=gFileSystem.createBinaryFileReader(gFileSystem.getAppPath()+mT("\\shader.xml"));
			tree.load(stream);
			xml::XMLElement*e=tree.getSubElement(mT("Program"));
			stream->close();

			shader::SGProgram* p=new shader::SGProgram(shader::EPT_VERTEX);
			p->Load(e);
			shader=generator->GenerateShader(p);
			stream=gFileSystem.createBinaryFileWriter(gFileSystem.getAppPath()+mT("\\shader.cg"));
			OS::StreamWriter w(stream);
			w.writeString(shader);
			stream->close();
		}
		{

			shader::SGXMLVisitor xmlVis;
			xml::XMLElement*e=xmlVis.Traverse(program);
			OS::IStreamPtr stream=gFileSystem.createTextFileWriter(mT("generated_shader_vp.xml"));
			xml::XMLWriter w;
			w.addElement(e);

			OS::StreamWriter ww(stream);

			ww.writeString(w.flush());
			stream->close();
		}

	}

	scene::SPatchTerrain* loadTerrain(){
		GCPtr<loaders::HeightMapRaw16> l=new loaders::HeightMapRaw16();
		l->loadHeightMap(gFileSystem.openFile(mT("terrain.raw")));
		m_terrain=new scene::SPatchTerrain(0,math::vector3d(10000,200,10000));
		m_terrain->loadTerrain(l,7,0.05,false);
		m_terrain->getMaterial()->setTexture(gTextureResourceManager.loadTexture2D(mT("terrain.jpg")),0);
		m_terrain->getMaterial()->setTexture(gTextureResourceManager.loadTexture2D(mT("detailmap.bmp")),1);

		video::GPUShader*shader= new video::GPUShader();

		shader->SetVertexShader(gShaderResourceManager.loadShader(mT("phong2.cg"),video::EShader_VertexProgram,"main_vp",mT("cg")));
		shader->SetFragmentShader(gShaderResourceManager.loadShader(mT("phong2.cg"),video::EShader_FragmentProgram,"main_fp",mT("cg")));
		m_terrain->getMaterial()->setRenderShader(shader);

		scene::ISceneNode*node=getSceneManager()->createSceneNode(mT("Terrain"));
		node->AttachNode(m_terrain);
		//node->setVisible(false);
		/*
		m_terrain->setBaseTexture(gTextureResourceManager.loadTexture2D(mT("terrain.jpg")));
		m_terrain->setDetailTexture(gTextureResourceManager.loadTexture2D(mT("detail_mud.jpg")));
		m_terrain->setDetailNormal(gTextureResourceManager.loadTexture2D(mT("detail_mud.jpg")));*/

		//	m_terrainUtil=new scene::TerrainUtility(m_terrain);

		//	m_cursour=scene::SceneNodeCreator::addSphere(getSceneManager(),1,5,5);

		physics::PhysicMaterialDesc matDesc;
		matDesc.dynamicFriction=0.5;
		matDesc.staticFriction=0.7;
		physics::TerrainShapeDesc terrDesc(l->getWidth(),l->getHeight(),l->getHeightData(),m_terrain->GetScale());
		terrDesc.material=m_phManager->createMaterial(&matDesc);
		physics::PhysicalNodeDesc phNode;
		phNode.actorGroupID=1;
		phNode.shapes.push_back(&terrDesc);
		m_phManager->createNode(&phNode);

		//m_terrain->setShadowCaster(true);

		return m_terrain;
	}

	void CreateBalls()
	{

		GCPtr<scene::ISceneNode> node;
		GCPtr<video::SMaterial> mtrl=new video::SMaterial();
		mtrl->SetAmbient(1);
		mtrl->SetDiffuse(1);
		mtrl->SetEmissive(0);
		mtrl->setRenderState(video::RS_CullFace,video::ES_DontUse);
		mtrl->setTexture(gTextureResourceManager.loadTexture2D(mT("wood16.jpg")),0);
		mtrl->setTexture(gTextureResourceManager.loadTexture2D(mT("FieldstoneBumpDOT3.tga")),1);
		video::GPUShader*shader= new video::GPUShader();

		//shader->SetGeometryShader(gShaderResourceManager.loadShader(mT("phong2.cg"),video::EShader_GeometryProgram,"main_gp",mT("cg")));
		shader->SetVertexShader(gShaderResourceManager.loadShader(mT("phong2.cg"),video::EShader_VertexProgram,"main_vp",mT("cg")));
		shader->SetFragmentShader(gShaderResourceManager.loadShader(mT("phong2.cg"),video::EShader_FragmentProgram,"main_fp",mT("cg")));
		mtrl->setRenderShader(shader);

		math::vector3d ext(5);

		GCPtr<scene::SMesh> NodeMesh=new scene::SMesh();
		NodeMesh->addNewBuffer();
		scene::MeshGenerator::getInstance().generateBox(ext,NodeMesh->getBuffer(0));
		NodeMesh->getBufferData(0)->setMaterial(mtrl);
		NodeMesh->calcCurrBoundingBox();


		if(1)
		{
			for(int j=0;j<10;++j)
			{
				GCPtr<scene::MeshRenderableNode> Node=new scene::MeshRenderableNode(NodeMesh);
				Node->setShadowCaster(true);

				node=new scene::ISceneNode(mT("PhysicalBox_")+core::StringConverter::toString(j),0,0);
				node->setPosition(math::vector3d(math::Randomizer::rand01()*10,10+(2*ext.y+1)*j,0));
				node->setOrintation(math::quaternion(math::Randomizer::rand(360),math::vector3d::YAxis));
				//node->setScale(math::Randomizer::rand01()*2+1);
				node->updateAbsoluteTransformation();
				node->AttachNode(Node);


				getSceneManager()->addSceneNode(node);
				//m_vegetation->AddObject(node);
				physics::BoxShapeDesc boxDesc(ext);
				physics::PhysicalNodeDesc phNodeDesc;
				physics::PhysicalBodyDesc bodyDesc;
				bodyDesc.mass=10;
				if(j==9)
					bodyDesc.mass=100;

				phNodeDesc.shapes.push_back(&boxDesc);
				node->updateAbsoluteTransformation();
				phNodeDesc.globalPos=node->getAbsoluteTransformation();
				phNodeDesc.bodyDesc=&bodyDesc;
				GCPtr<physics::IPhysicalNode> phNode= m_phManager->createNode(&phNodeDesc);

				game::GameEntity* ent=new game::GameEntity(mT("box")+core::StringConverter::toString(j),m_gameManager);
				m_gameManager->AddGameEntity(ent);
				ent->SetShape(node);
				game::PhysicsComponent* phComp=new game::PhysicsComponent(m_phManager);
				ent->AddComponent(phComp);
				//ent->SetGameAction(new SimpleGameAction(phNode));
				//node->ShadowCaster=false;

				m_pickBuffer->PushPickID(j+1);
				m_pickBuffer->AddRenderObject(NodeMesh->getBufferData(0),Node);

				if(j==9)
				{
					cloth->attachVertexToShape(0,phNode->GetShape(0),math::vector3d(0,10,0),physics::ECAF_TwoWay);
				}

			}
			if(0)
			{
				NodeMesh=new scene::SMesh();
				NodeMesh->addNewBuffer();
				scene::MeshGenerator::getInstance().generatePlane(1,1,NodeMesh->getBuffer(0));
				scene::SMeshManipulator::getInstance().scaleMesh(NodeMesh->getBuffer(0),10000);
				NodeMesh->getBufferData(0)->setMaterial(mtrl);

				GCPtr<scene::MeshRenderableNode> Node=new scene::MeshRenderableNode(NodeMesh);
			//	Node->setShadowCaster(true);

				node=new scene::ISceneNode(mT("Node_Plane"),0,0);
				node->setPosition(math::vector3d(0,0,0));
				//node->setScale(math::Randomizer::rand01()*2+1);
				node->updateAbsoluteTransformation();
				node->AttachNode(Node);


				getSceneManager()->addSceneNode(node);
				//m_vegetation->AddObject(node);
				physics::PlaneShapeDesc planeDesc(math::Plane::XZPlane);
				physics::PhysicalNodeDesc phNodeDesc;
				physics::PhysicalBodyDesc bodyDesc;

				phNodeDesc.shapes.push_back(&planeDesc);
				node->updateAbsoluteTransformation();
				phNodeDesc.globalPos=node->getAbsoluteTransformation();
				//phNodeDesc.bodyDesc=&bodyDesc;
				GCPtr<physics::IPhysicalNode> phNode= m_phManager->createNode(&phNodeDesc);

			}
		}
	}


	physics::PhysicalVehicle* CreateVehicle(const math::vector3d& pos)
	{
		math::vector3d scale=0.1;
		scene::ISceneNode*node;
		scene::SMesh* NodeMesh=0;

		physics::VehicleXMLParser parser;
		physics::VehicleDesc *vehicleDesc=parser.LoadVehicleDesc(gFileSystem.openFile(mT("VehicleCar.xml")));
		if(!vehicleDesc)
			return 0;
		vehicleDesc->position=pos;
		vehicleDesc->position.y*=scale.y;

		core::string bones[]=
		{
			mT("Wheel_FR"),
			mT("Wheel_FL"),
			mT("Wheel_BR"),
			mT("Wheel_BL")
		};
		core::string wheelNames[]=
		{
			mT("FR"),
			mT("FL"),
			mT("BR"),
			mT("BL")
		};

		for(int i=0;i<vehicleDesc->wheels.size();++i)
			vehicleDesc->wheels[i]->radius*=scale.y*100;


		{

			GCPtr<video::SMaterial> mtrl=new video::SMaterial();
			mtrl->SetAmbient(1);
			mtrl->SetDiffuse(1);
			mtrl->SetEmissive(0);
			mtrl->setRenderState(video::RS_CullFace,video::ES_DontUse);
			mtrl->setTexture(gTextureResourceManager.loadTexture2D(mT("buggy001.tga")),0);
			video::GPUShader*shader= new video::GPUShader();

			shader->SetVertexShader(gShaderResourceManager.loadShader(mT("Skinned.cg"),video::EShader_VertexProgram,"main_vp",mT("cg")));
			shader->SetFragmentShader(gShaderResourceManager.loadShader(mT("Skinned.cg"),video::EShader_FragmentProgram,"main_fp",mT("cg")));
			mtrl->setRenderShader(shader);

			
			GCPtr<scene::MeshRenderableNode> CarNode=scene::MeshFileCreatorManager::getInstance().LoadFromFile(mT("buggy.mdx"),true);
			//new scene::MeshRenderableNode(NodeMesh);
			NodeMesh=CarNode->getMesh();
			//	NodeMesh->getBufferData(0)->setMaterial(mtrl);
			CarNode->setShadowCaster(true);

			node=new scene::ISceneNode(mT("Car"),0,0);
			node->AttachNode(CarNode);
			node->setScale(scale);
			node->setPosition(vehicleDesc->position);
		}
		for(int i=0;i<4;++i)
		{
			physics::VehicleWheelDesc* wdesc= vehicleDesc->GetWheel(wheelNames[i]);
			scene::BoneNode*b= NodeMesh->getSkeleton()->getBone(bones[i]);
			if(wdesc && b)
			{
				wdesc->position=b->getAbsoluteBasePosition()*scale;
			}
		}

		GCPtr<scene::MeshRenderableNode> CollNode=scene::MeshFileCreatorManager::getInstance().LoadFromFile(mT("buggy_collision.mdl"),false);
		
		//create Node
		physics::PhysicalNodeDesc nodeDesc;
		physics::PhysicalBodyDesc bodyDesc;
		{
			physics::ConvexShapeDesc* shapes=new physics::ConvexShapeDesc();
			shapes->mesh=CollNode->getMesh()->getBuffer(0);
			scene::SMeshManipulator::getInstance().scaleMesh(shapes->mesh,scale);
			nodeDesc.shapes.push_back(shapes);
			//physics::BoxShapeDesc* shapes;
			//shapes=new physics::BoxShapeDesc();
			//vehicleDesc->shapes.push_back(shapes);
			//shapes->extent.set(20,1,10);
			//shapes->localSpace.setTranslation(math::vector3d(0,1,0));
		}
		bodyDesc.mass=1000;
		nodeDesc.bodyDesc=&bodyDesc;
		nodeDesc.globalPos.setTranslation(pos);

		physics::IPhysicalNode* phnode=m_phManager->createNode(&nodeDesc);

		physics::PhysicalVehicle* ret=m_vehicleManager->CreateVehicle(*vehicleDesc,phnode,1);
		delete vehicleDesc;

		{

			getSceneManager()->addSceneNode(node);
			game::GameEntity* ent=new game::GameEntity(mT("Car"),m_gameManager);
			m_gameManager->AddGameEntity(ent);
			ent->SetShape(node);
			ent->AddComponent(new game::PhysicsComponent(m_phManager));
		//	ent->SetGameAction(new SimpleGameAction(ret->GetPhysicalNode()));


			for(int i=0;i<ret->GetWheels().size();++i)
			{
				physics::IVehicleWheel*wheel=ret->GetWheels()[i];
				scene::BoneNode*b= NodeMesh->getSkeleton()->getBone(bones[i]);

				ent->AddComponent(new WheelComponent(b,wheel,ret));
				/*
				NodeMesh=new scene::SMesh();
				NodeMesh->addNewBuffer();
				scene::MeshGenerator::getInstance().generateCylinder(
				wheel->GetRadius(),2,8,NodeMesh->getBuffer(0));
				NodeMesh->getBufferData(0)->setMaterial(mtrl);

				scene::SMeshManipulator::getInstance().rotateMesh(NodeMesh->getBuffer(0),
				quaternion(90,vector3d::ZAxis));


				GCPtr<scene::MeshRenderableNode> Node=new scene::MeshRenderableNode(NodeMesh);

				scene::ISceneNode*node=new scene::ISceneNode("Wheel"+StringConverter::toString(i),0,0);
				node->AttachNode(Node);
				getSceneManager()->addSceneNode(node);
				*/
				//game::GameEntity* ent=new game::GameEntity(mT("Wheel")+StringConverter::toString(i));
				//m_gameManager->AddGameEntity(ent);
				//ent->SetShape(node);
				//ent->SetGameAction(new WheelGameAction(b,wheel,ret));

			}

		}

		return ret;
	}

	scene::SParticleSystem* CreateParticleSystem()
	{
		scene::SParticleSystem* pSys=new scene::SParticleSystem();
		scene::BoxParticleEmitter* emitter;
		scene::PAColorInterpolater* colorIntp=new scene::PAColorInterpolater();
		scene::PAScaleInterpolater* scaleIntp=new scene::PAScaleInterpolater();
		scene::PAVelocityInterpolater* velIntp=new scene::PAVelocityInterpolater();
		colorIntp->addKey(0,video::SColor(1,1,0.4,0));
		colorIntp->addKey(0.1,video::SColor(1,1,0.4,1));
		colorIntp->addKey(0.8,video::SColor(1,1,0.4,1));
		colorIntp->addKey(1,video::SColor(1,1,0.4,0));

		scaleIntp->addKey(0,0);
		scaleIntp->addKey(0,0.1);
		scaleIntp->addKey(0.8,0.5);
		scaleIntp->addKey(1,0);

		velIntp->addKey(0,math::vector3d(0.4,0,0.4),math::vector3d(1,1,1));

	//	pSys->setDevice(getDevice());
		//emitter=pSys->CreateEmitter();
		emitter=new scene::BoxParticleEmitter(pSys);
		//emitter->setOutterRadius(100);
		emitter->setSize(vector3d(500,20,500));
		pSys->AddEmitter(emitter);
		emitter->setParticleAcceleration(math::vector3d(0,-0.5,0));
		emitter->particlesLifeSpawn.addKey(0,10);
		emitter->particlesRate.addKey(0,500);
		emitter->addAffector(colorIntp);
		emitter->addAffector(scaleIntp);
		emitter->addAffector(velIntp);
		scene::CPUParticleBillboardRenderer* renderer=new scene::CPUParticleBillboardRenderer(5000);
		emitter->SetRenderer(renderer);
		emitter->getMaterial()->setTexture(gTextureResourceManager.loadTexture2D(mT("star_02.png")),0);
		emitter->getMaterial()->SetDiffuse(video::SColor( 1,0,0,1));
		emitter->getMaterial()->SetAmbient(video::SColor(1,1,1,1));
		emitter->getMaterial()->setMaterialRenderer(video::MRT_TRANSPARENT);

		if(1)
		{

			scene::SphereParticleEmitter* semitter=new scene::SphereParticleEmitter(pSys);
			semitter->setOutterRadius(30);
			
			pSys->AddEmitter(semitter);
			semitter->setParticleAcceleration(math::vector3d(0,-0.5,0));
			semitter->particlesLifeSpawn.addKey(0,10);
			semitter->particlesRate.addKey(0,200);
			semitter->addAffector(colorIntp);
			semitter->addAffector(scaleIntp);
			//semitter->addAffector(velIntp);
			renderer=new scene::CPUParticleBillboardRenderer(5000);
			semitter->SetRenderer(renderer);
			semitter->getMaterial()->setTexture(gTextureResourceManager.loadTexture2D(mT("smoke_trail04.tga")),0);
			semitter->getMaterial()->SetDiffuse(video::SColor( 0,1,0,1));
			semitter->getMaterial()->SetAmbient(video::SColor(1,1,1,1));
			//semitter->getMaterial()->setMaterialRenderer(video::MRT_TRANSPARENT);
		}
		
		scene::ISceneNode* node=getSceneManager()->createSceneNode(mT(""));
		node->AttachNode(pSys);
		node->setDrawBoundingBox(true);
		return pSys;
	}

	void OnTextBoxLostFocus(const mray::PVOID& obj,const mray::PVOID& param)
	{
		/*
		GUI::IGUIElement* elem=static_cast<GUI::IGUIElement*>(obj);

		regex::Pattern*p2=regex::Pattern::compile("([a-zA-Z]+)@([a-zA-Z]+).([a-zA-Z.]+)");
		regex::Matcher*mm= p2->createMatcher(elem->GetText().GetAsString().c_str());
		bool res=mm->matches();
		std::vector<std::string> groups=mm->getGroups();

		printf("match result=%s\n",res?"success":"failed");
		if(!res)
			m_guiManager->SetFocus(elem);*/
	}

	void printXML(xml::IXMLNode*node)
	{
		switch(node->GetType())
		{
		case xml::ENT_Comment:
			printf("Comment:%s\n",((xml::XMLComment*)node)->GetValue().c_str());
			break;
		case xml::ENT_Text:
			printf("Text:%s\n",((xml::XMLTextNode*)node)->GetValue().c_str());
			break;
		case xml::ENT_Element:
			{
			printf("[+]Element:%s\n",((xml::XMLElement*)node)->getName().c_str());
			xml::xmlSubElementsMap::iterator it=((xml::XMLElement*)node)->getElementsBegin();
			xml::xmlSubElementsMap::iterator end=((xml::XMLElement*)node)->getElementsEnd();
			for(;it!=end;++it)
			{
				printXML(*it);
			}
			}
			break;
		}
	}

	static void exampleLog(std::string message, int severity)
	{
		using namespace std;
		//Identify how severe the mesage is
		switch(severity)
		{
		case LS_DEBUG:
			cout << "DEBUG: ";
			break;
		case LS_INFO:
			cout << "INFO: ";
			break;
		case LS_WARN:
			cout << "WARN: ";
			break;
		case LS_ERROR:
			cout << "ERROR: ";
			break;
		}

		//Print the message
		cout << message << endl;
	}

	class CPUTexture
	{
	protected:
		math::vector3di m_size;
		std::vector<float> m_values;
	public:
		void CreateTexture(const math::vector3d& size)
		{
			m_size=size;
			m_values.resize(m_size.x*m_size.y*m_size.z);
		}
		inline int getIndex(const math::vector3di& coords)
		{
			return coords.z*m_size.x*m_size.y+coords.x*m_size.y+coords.y;
		}

		float Sample(math::vector3d coords)
		{
			coords.x=fmodf(coords.x,1);
			coords.y=fmodf(coords.y,1);
			coords.z=fmodf(coords.z,1);
			math::vector3d pixel=coords*(m_size-1);
			math::vector3di pixelA=pixel;
			float diff=(pixel-pixelA).Length();
			math::vector3di pixelB=pixelA+1;
			pixelB.x=math::Min<int>(pixelB.x,m_size.x-1);
			pixelB.y=math::Min<int>(pixelB.y,m_size.y-1);
			pixelB.z=math::Min<int>(pixelB.z,m_size.z-1);

			int pixelAIdx=getIndex(pixelA);
			int pixelBIdx=getIndex(pixelB);

			float ret=math::lerp<float>(m_values[pixelAIdx],m_values[pixelBIdx],diff);
			return ret;
		}

		void SetValue(const math::vector3di& coords,float v)
		{
			int pixelIdx=getIndex(coords);
			m_values[pixelIdx]=v;
		}
	}m_cputexture;


	float DensityFunction(float x,float y,float z)
	{
		static Perlin perlin(2,2,1,234);
		
		static Noise3D noise1(1234567);
		float ret= y-10;

		x/=256.0f;
		y/=32.0f;
		z/=256.0f;

		for(int i=0;i<8;++i)
		{
			float w=i/8.0f;
			ret+=noise1.Noise(math::vector3d(x*4,y,z*4)*(i+1))*(1-w)*16;
			ret+=perlin.Get3D(x*(i+1),y*(i+1),z*(i+1))*(1-w)*8;
		}

		return -ret;
		
	}

	void createTerrainInVolume(LargeVolume<MaterialDensityPair44>& volData, Vector3DInt32 lowerCorner, Vector3DInt32 upperCorner)
	{/*
		 Noise3D noise1;
		math::vector3d sz=vector3d(32,32,32);//vector3d(upperCorner.getX(),upperCorner.getY(),upperCorner.getZ());
		m_cputexture.CreateTexture(sz);
		for(int x=0;x<sz.x;++x)
		{
			for(int y=0;y<sz.y;++y)
			{
				for(int z=0;z<sz.z;++z)
				{
					float v=noise1.Noise(vector3d(x,y,z)/sz);
					m_cputexture.SetValue(vector3di(x,y,z),v);
				}
			}
		}*/
		//This three-level for loop iterates over every voxel between the specified corners
		for (int z = lowerCorner.getZ(); z <= upperCorner.getZ(); z++)
		{
			for (int y = lowerCorner.getY(); y <= upperCorner.getY(); y++)
			{
				for (int x = lowerCorner.getX() ; x <= upperCorner.getX(); x++)
				{
					float uValue=DensityFunction(x,y,z);
					volData.setVoxelAt(x,y,z, MaterialDensityPair44(uValue, uValue > 0 ? MaterialDensityPair44::getMaxDensity() : MaterialDensityPair44::getMinDensity()));
				}
			}
		}
	}
	void createPerlinVolumeSlow(LargeVolume<MaterialDensityPair44>& volData)
	{
		Perlin perlin(2,8,1,234);

		for(int z = 1; z < 256-1; z++)
		{
			std::cout << z << std::endl;
			for(int y = 1; y < 256-1; y++)
			{
				for(int x = 1; x < 256-1; x++)
				{							
					float perlinVal = perlin.Get3D(x /static_cast<float>(256-1), (y) / static_cast<float>(256-1), z / static_cast<float>(256-1));

					perlinVal += 1.0f;
					perlinVal *= 0.5f;
					perlinVal *= MaterialDensityPair44::getMaxDensity();

					MaterialDensityPair44 voxel;

					voxel.setMaterial(245);
					voxel.setDensity(perlinVal);

					/*if(perlinVal < 0.0f)
					{
					voxel.setMaterial(245);
					voxel.setDensity(MaterialDensityPair44::getMaxDensity());
					}
					else
					{
					voxel.setMaterial(0);
					voxel.setDensity(MaterialDensityPair44::getMinDensity());
					}*/

					volData.setVoxelAt(x, y, z, voxel);
				}
			}
		}
	}


	void createCubeInVolume(LargeVolume<MaterialDensityPair44>& volData, Vector3DInt32 lowerCorner, Vector3DInt32 upperCorner, uint8_t uValue)
	{
		//This three-level for loop iterates over every voxel between the specified corners
		for (int z = lowerCorner.getZ(); z <= upperCorner.getZ(); z++)
		{
			for (int y = lowerCorner.getY(); y <= upperCorner.getY(); y++)
			{
				for (int x = lowerCorner.getX() ; x <= upperCorner.getX(); x++)
				{
					volData.setVoxelAt(x,y,z, MaterialDensityPair44(uValue, uValue > 0 ? MaterialDensityPair44::getMaxDensity() : MaterialDensityPair44::getMinDensity()));
				}
			}
		}
	}
	void createSphereInVolume(LargeVolume<MaterialDensityPair44>& volData, float fRadius, uint8_t uValue)
	{
		//This vector hold the position of the center of the volume
		Vector3DInt32 v3dVolCenter = (volData.getEnclosingRegion().getUpperCorner() - volData.getEnclosingRegion().getLowerCorner()) / 2;

		//This three-level for loop iterates over every voxel in the volume
		for (int z = 0; z < volData.getWidth(); z++)
		{
			for (int y = 0; y < volData.getHeight(); y++)
			{
				for (int x = 0; x < volData.getDepth(); x++)
				{
					//Store our current position as a vector...
					Vector3DInt32 v3dCurrentPos(x,y,z);	
					//And compute how far the current position is from the center of the volume
					double fDistToCenter = (v3dCurrentPos - v3dVolCenter).length();

					//If the current voxel is less than 'radius' units from the center
					//then we make it solid, otherwise we make it empty space.
					if(fDistToCenter <= fRadius)
					{
						volData.setVoxelAt(x,y,z, MaterialDensityPair44(uValue, uValue > 0 ? MaterialDensityPair44::getMaxDensity() : MaterialDensityPair44::getMinDensity()));
					}
				}
			}
		}
	}

	scene::IMeshBufferPtr ConvertToMesh(SurfaceMesh<PositionMaterialNormal> &  mesh,const math::vector3d &offset
		,PolyVox::Array<3,uint8_t>& occ)
	{
		const std::vector<PositionMaterialNormal>& vecVertices = mesh.getVertices();
		const std::vector<uint32_t>& vecIndices = mesh.getIndices();

		if(vecIndices.size()==0 || vecVertices.size()==0)
			return scene::IMeshBufferPtr::Null;

		scene::IMeshBufferPtr  res=new scene::SMeshBuffer();
		video::IHardwareStreamBuffer* posStream=  res->createStream(0,video::EMST_Position,video::ESDT_Point3f,vecVertices.size(),video::IHardwareBuffer::EUT_StaticWriteOnly,false,false);
		video::IHardwareStreamBuffer* normStream=  res->createStream(0,video::EMST_Normal,video::ESDT_Point3f,vecVertices.size(),video::IHardwareBuffer::EUT_StaticWriteOnly,false,false);
		video::IHardwareStreamBuffer* clrStream=  res->createStream(0,video::EMST_Color,video::ESDT_Point3f,vecVertices.size(),video::IHardwareBuffer::EUT_StaticWriteOnly,false,false);
		video::IHardwareStreamBuffer* tcStream=  res->createStream(0,video::EMST_Texcoord,video::ESDT_Point2f,vecVertices.size(),video::IHardwareBuffer::EUT_StaticWriteOnly,false,false);
		video::IHardwareIndexBuffer* indStream=  res->createIndexBuffer(video::IHardwareIndexBuffer::EIT_32Bit,vecIndices.size(),video::IHardwareBuffer::EUT_StaticWriteOnly,false,false);
		indStream->writeData(0,vecIndices.size()*sizeof(uint),&vecIndices[0]);

		vector3d* pos=(vector3d*) posStream->lock(0,vecVertices.size(),video::IHardwareStreamBuffer::ELO_Discard);
		vector3d* norm=(vector3d*)normStream->lock(0,vecVertices.size(),video::IHardwareStreamBuffer::ELO_Discard);
		vector3d* clr=(vector3d*)clrStream->lock(0,vecVertices.size(),video::IHardwareStreamBuffer::ELO_Discard);
		vector2d* tc=(vector2d*)tcStream->lock(0,vecVertices.size(),video::IHardwareStreamBuffer::ELO_Discard);

		box3d bb;
		uint8_t* occPtr=occ.getRawData();

		for(int i=0;i<vecVertices.size();++i)
		{
			const PositionMaterialNormal& ver=vecVertices[i];
			pos[i].set(ver.position.getX()+offset.x,
				ver.position.getY()+offset.y,
				ver.position.getZ()+offset.z);
			norm[i].set(ver.normal.getX(),ver.normal.getY(),ver.normal.getZ());
			clr[i]=ver.material;//(float)occPtr[i]/255.0f;//
			if(i==0)
				bb.reset(pos[i]);
			else
				bb.AddPoint(pos[i]);
		}
		for(int i=0;i<vecVertices.size();++i)
		{
			tc[i].x=pos[i].x/bb.getWidth();;
			tc[i].y=pos[i].z/bb.getHeight();
		}
		posStream->unlock();
		normStream->unlock();
		clrStream->unlock();
		tcStream->unlock();
		res->setBoundingBox(bb);
		return res;
	}

	std::vector<scene::IMeshBufferPtr> CreateVoxels()
	{
		std::vector<scene::IMeshBufferPtr> res;
		logHandler=&exampleLog;
		vector3di g_uVolumeSideLength(128,128,128);
		LargeVolume<MaterialDensityPair44> volData(PolyVox::Region(Vector3DInt32(0,0,0), Vector3DInt32(g_uVolumeSideLength.x-1, g_uVolumeSideLength.y-1, g_uVolumeSideLength.z-1)));

		vector3di minPos = 0;
		vector3di midPos = g_uVolumeSideLength / 2;
		vector3di maxPos = g_uVolumeSideLength - 1;

		//createTerrainInVolume(volData,Vector3DInt32(minPos.x, minPos.y, minPos.z), Vector3DInt32(maxPos.x-1, maxPos.y-1, maxPos.z-1));
		
		createSphereInVolume(volData, 60.0f, 5);
		if(0)
		{
			createSphereInVolume(volData, 50.0f, 0);
			createSphereInVolume(volData, 40.0f, 3);
			createSphereInVolume(volData, 30.0f, 0);
			createSphereInVolume(volData, 20.0f, 1);
			createCubeInVolume(volData, Vector3DInt32(minPos.x, minPos.y, minPos.z), Vector3DInt32(midPos.x-1, midPos.y-1, midPos.z-1), 0);
			createCubeInVolume(volData, Vector3DInt32(midPos.x+1, midPos.y+1, minPos.z), Vector3DInt32(maxPos.x, maxPos.y, midPos.z-1), 0);
			createCubeInVolume(volData, Vector3DInt32(midPos.x+1, minPos.y, midPos.z+1), Vector3DInt32(maxPos.x, midPos.y-1, maxPos.z), 0);
			createCubeInVolume(volData, Vector3DInt32(minPos.x, midPos.y+1, midPos.z+1), Vector3DInt32(midPos.x-1, maxPos.y, maxPos.z), 0);

			createCubeInVolume(volData, Vector3DInt32(1, midPos.y-10, midPos.z-10), Vector3DInt32(maxPos.x-1, midPos.y+10, midPos.z+10), MaterialDensityPair44::getMaxDensity());
			createCubeInVolume(volData, Vector3DInt32(midPos.x-10, 1, midPos.z-10), Vector3DInt32(midPos.x+10, maxPos.y-1, midPos.z+10), MaterialDensityPair44::getMaxDensity());
			createCubeInVolume(volData, Vector3DInt32(midPos.x-10, midPos.y-10 ,1), Vector3DInt32(midPos.x+10, midPos.y+10, maxPos.z-1), MaterialDensityPair44::getMaxDensity());
			
		}
		
		RawVolume<MaterialDensityPair44> tempVolume(PolyVox::Region(0,0,0,128, 128, 128));
		LowPassFilter<LargeVolume, RawVolume, MaterialDensityPair44> pass1(&volData, PolyVox::Region(Vector3DInt32(62, 62, 62), Vector3DInt32(126, 126, 126)), &tempVolume, PolyVox::Region(Vector3DInt32(62, 62, 62), Vector3DInt32(126, 126, 126)), 3);
		pass1.executeSAT();
		LowPassFilter<RawVolume, LargeVolume, MaterialDensityPair44> pass2(&tempVolume, PolyVox::Region(Vector3DInt32(62, 62, 62), Vector3DInt32(126, 126, 126)), &volData, PolyVox::Region(Vector3DInt32(62, 62, 62), Vector3DInt32(126, 126, 126)), 3);
		pass2.executeSAT();
		
		vector3di m_uRegionSideLength=g_uVolumeSideLength;//(g_uVolumeSideLength.x,32,g_uVolumeSideLength.z);
		uint m_uVolumeWidthInRegions = volData.getWidth() / m_uRegionSideLength.x;
		uint m_uVolumeHeightInRegions = volData.getHeight() / m_uRegionSideLength.y;
		uint m_uVolumeDepthInRegions = volData.getDepth() / m_uRegionSideLength.z;

		math::vector3d offset;
		for(int uRegionZ = 0; uRegionZ < m_uVolumeDepthInRegions; ++uRegionZ)
		{
			offset.z=uRegionZ * m_uRegionSideLength.z;
			std::cout << "uRegionZ = " << uRegionZ << " of " << m_uVolumeDepthInRegions << std::endl;
			for(int uRegionY = 0; uRegionY < m_uVolumeHeightInRegions; ++uRegionY)
			{
				offset.y=uRegionY * m_uRegionSideLength.y;
				for(int uRegionX = 0; uRegionX < m_uVolumeWidthInRegions; ++uRegionX)
				{
					offset.x=uRegionX * m_uRegionSideLength.x;

					int regionStartX = uRegionX * m_uRegionSideLength.x;
					int regionStartY = uRegionY * m_uRegionSideLength.y;
					int regionStartZ = uRegionZ * m_uRegionSideLength.z;

					int regionEndX = regionStartX + m_uRegionSideLength.x;
					int regionEndY = regionStartY + m_uRegionSideLength.y;
					int regionEndZ = regionStartZ + m_uRegionSideLength.z;


					Vector3DInt32 regLowerCorner(regionStartX, regionStartY, regionStartZ);
					Vector3DInt32 regUpperCorner(regionEndX, regionEndY, regionEndZ);


					SurfaceMesh<PositionMaterialNormal> * mesh=new SurfaceMesh<PositionMaterialNormal>();
					SurfaceMesh<PositionMaterialNormal> * decimatedMesh(new SurfaceMesh<PositionMaterialNormal>);
					SurfaceExtractor<LargeVolume, MaterialDensityPair44> surfaceExtractor(&volData, PolyVox::Region(regLowerCorner, regUpperCorner), mesh);
					surfaceExtractor.execute();

					MeshDecimator<PositionMaterialNormal> decimator(mesh, decimatedMesh, 0.95f);
					decimator.execute();

					PolyVox::Array<3,uint8_t> occResult;
					//AmbientOcclusionCalculator<LargeVolume, MaterialDensityPair44> occCalc(&volData,&occResult,PolyVox::Region(regLowerCorner, regUpperCorner),100,4);
//					occCalc.execute();

					std::cout<<mesh->getNoOfVertices()<<" now:"<<decimatedMesh->getNoOfVertices()<<std::endl;

					scene::IMeshBufferPtr m=ConvertToMesh(*decimatedMesh,offset,occResult);
					if(!m.isNull())
						res.push_back(m);

					delete mesh;
					delete decimatedMesh;
				}
			}
		}

		return res;
	}

	virtual void init(const OptionContainer&extraOptions){
		traceFunction(Application);
		CMRayApplication::init(extraOptions);
		CMRayApplication::loadResourceFile(mT("Resources.stg"));

		{
			shader::SGShaderManager manager;
			manager.LoadShaderLibraries(gFileSystem.getAppPath()+
				mT("..\\Data\\shaders\\shaderLib\\"));
		}
		TestShaderGeneration();
		{
			gImageSetResourceManager.loadImageSet(mT("VistaCG.imageset"));
			GCPtr<OS::IStream> themeStream=gFileSystem.createBinaryFileReader(mT("VistaCG.xml"));
			GUI::GUIThemeManager::getInstance().loadTheme(themeStream);
			GUI::GUIThemeManager::getInstance().setActiveTheme(mT("VistaCG"));
			//load font
			GCPtr<GUI::BaseFont>font=gFontResourceManager.getOrCreate(mT("solo5_font.xml"));
			gFontResourceManager.setDefaultFont(font);
			gTextureResourceManager.writeResourceToDist(font->getTexture(0)->GetTexture(),
				gFileSystem.getAppPath()+ mT("TektonPro.png"));

			m_langSpec=new GUI::LanguageFontSpecification();
			m_langSpec->Load(mT("Arabic.spec"));
		}
		if(0)
		{
			xml::XMLTree tree;
			OS::IStreamPtr stream=gFileSystem.createTextFileReader(mT("streets.mdl"));
			OS::IStreamPtr ostream=gFileSystem.createBinaryFileWriter(mT("SOAP.zip"));

			Compress::GZipCompress c;
			c.compress(stream,ostream);
			ostream->close();
		}
		if(0)
		{
			xml::XMLTree tree;
			OS::IStreamPtr stream=gFileSystem.createTextFileReader(mT("SOAP.zip"));
			OS::IStreamPtr ostream=gFileSystem.createBinaryFileWriter(mT("SOAP2.xml"));

			Compress::GZipCompress c;
			c.decompress(stream,ostream);
			ostream->close();
			stream->close();
		}
		FunctionProfileManager::getInstance().AddListener(&g_functionListener);
		m_appThreadManager=new ThreadJobManager();
		m_appThreadManager->SetMultiThreaded(true);


		scene::MeshAnimatorManager::getInstance().SetGPUSkeletonAnimatorParams(3,4);

		m_cameraL=new scene::CameraNode(mT("CameraL"),0,getSceneManager());//100,GetInputManager());
		m_cameraR=new scene::CameraNode(mT("CameraR"),0,getSceneManager());//100,GetInputManager());
		
		getSceneManager()->addSceneNode(m_cameraL);
		getSceneManager()->addSceneNode(m_cameraR);

		if(0)
		{
			video::GPUShader*shader= new video::GPUShader();

			shader->SetVertexShader(gShaderResourceManager.loadShader(mT("phong2.cg"),video::EShader_VertexProgram,"main_vp",mT("cg")));
			shader->SetFragmentShader(gShaderResourceManager.loadShader(mT("phong2.cg"),video::EShader_FragmentProgram,"main_fp",mT("cg")));
			XMLSceneLoader loader(getSceneManager(),m_phManager);
			std::vector<scene::ISceneNode*> nodes;
			std::vector<physics::IPhysicalNode*> phnodes;
			loader.Load(gFileSystem.openFile(mT("testScene.xml")),0,nodes,phnodes);
			for(int i=0;i<nodes.size();++i)
			{
				scene::IRenderable* r= nodes[i]->GetAttachedNode(0);
				int cnt=r->getMesh()->getBuffersCount();
				for(int j=0;j<cnt;++j)
				{
					r->getMesh()->getBufferData(j)->getMaterial()->setRenderShader(shader);
				}
			}
		}

		HeadMount* hm=new HeadMount(getSceneManager(),100,GetInputManager());


		m_stereoRenderer=new scene::StereoRenderer(GetRenderWindow(),m_cameraL,m_cameraR,hm);
		getSceneManager()->addSceneNode(hm);

		m_secCamera=new DemoCamera(getSceneManager(),100,GetInputManager());
		m_secCamera->setNodeName(mT("SecCam"));
		getSceneManager()->addSceneNode(m_secCamera);
	//	m_secCamera->SetCustomViewNode(m_camera);
		m_secCamera->setVisible(false);
		m_secCamera->setPosition(math::vector3d(100,500,100));
		m_secCamera->setTarget(math::vector3d(0,0,0));

		if(1)
		{
			m_light=getSceneManager()->createLightNode(mT(""));

			m_light->setPosition(math::vector3d(10,100,10));



			if(1)
			{
				GCPtr<video::ITexture> texture=getDevice()->createEmptyTexture2D(true);
				texture->setMipmapsFilter(false);
				texture->SetNumberOfMipmaps(0);
				texture->createTexture(shadowMapWidthHeight,video::EPixel_Float16_R);
				m_shadowRenderTarget=getDevice()->createRenderTarget(mT("shadowRenderTarget"),texture,0,0,0);

				m_light->setCastShadows(true);
				m_light->setShadowMap(m_shadowRenderTarget);

				m_projection=math::MathUtil::CreateProjectionMatrixPerspectiveFov(m_cameraL->getFovY(),
 					m_cameraL->getAspect(),1,100);
				m_light->setProjection(m_projection);

			}
		}

		{
			m_scriptManager=script::CreateLuaScriptManager();
			GCPtr<script::IScript> script1=m_scriptManager->createScript(mT("test"));
			script1->setScript(gFileSystem.openFile(mT("test.lua")));
			script1->runScript();

// 			script1->getFunctionHandler()->beginCall("SetCamera");
// 			script1->getFunctionHandler()->pushParam(m_camera.pointer(),"CameraNode");
// 			script1->getFunctionHandler()->endFunctionCall(1);
		}

		if(0)
		{
			GCPtr<video::SMaterial> mtrl=new video::SMaterial();
			mtrl->SetAmbient(1);
			mtrl->SetDiffuse(1);
			mtrl->SetEmissive(0);
			mtrl->setRenderState(video::RS_CullFace,video::ES_DontUse);
			mtrl->setRenderState(video::RS_Lighting,video::ES_DontUse);
			mtrl->setTexture(gTextureResourceManager.loadTexture2D(mT("wood16.jpg")),0);
			mtrl->setTexture(gTextureResourceManager.loadTexture2D(mT("FieldstoneBumpDOT3.tga")),1);
			mtrl->setRenderState(video::RS_Wireframe,video::ES_Use);
			video::GPUShader*shader= new video::GPUShader();

			shader->SetVertexShader(gShaderResourceManager.loadShader(mT("phong2.cg"),video::EShader_VertexProgram,"main_vp",mT("cg")));
			shader->SetFragmentShader(gShaderResourceManager.loadShader(mT("phong2.cg"),video::EShader_FragmentProgram,"main_fp",mT("cg")));
			mtrl->setRenderShader(shader);

			std::vector<scene::IMeshBufferPtr> meshList=CreateVoxels();

			scene::SMesh* mesh=new scene::SMesh();
			for(int i=0;i<meshList.size();++i)
			{
				mesh->addBuffer(meshList[i]);
				mesh->getBufferData(i)->setMaterial(mtrl);
			}


			GCPtr<scene::ISceneNode> node;
			GCPtr<scene::MeshRenderableNode> Node=new scene::MeshRenderableNode(mesh);
			Node->setShadowCaster(false);

			node=new scene::ISceneNode(mT("Voxeld"),0,0);
			node->setPosition(math::vector3d(0,20,0));
			//node->setScale(math::Randomizer::rand01()*2+1);
			node->updateAbsoluteTransformation();
			node->AttachNode(Node);

			getSceneManager()->addSceneNode(node);


		}
#if 0
		if(0)
		{
			math::vector2d size=m_viewPort->getAbsViewPort().getSize();
			GCPtr<video::ITexture> tex=getDevice()->createTexture2D(size,video::EPixel_R8G8B8A8,true);
			m_renderTarget=getDevice()->createRenderTarget(mT("RenderTarget"),tex,0,0,0);

			tex=getDevice()->createTexture2D(size,video::EPixel_R8G8B8A8,true);

			m_renderTarget->attachRenderTarget(tex,1);

			m_clearRTShader=new video::GPUShader();
			m_clearRTShader->SetVertexShader(gShaderResourceManager.loadShader(mT("clearGeoBuffer.cg"),video::EShader_VertexProgram,"main_vp",mT("cg")));
			m_clearRTShader->SetFragmentShader(gShaderResourceManager.loadShader(mT("clearGeoBuffer.cg"),video::EShader_FragmentProgram,"main_fp",mT("cg")));

		}
#endif
		if(1)
		{
			m_soundManager= sound::SoundManagerFactory::getInstance().CreateObject(mT("FMod"));
			
			if(0)
			{
				sound::ISound*snd=m_soundManager->loadSound(mT("sound\\afterburn.ogg"),true);
				snd->play();
			}
		}

		m_debugManager=new scene::DebugDrawManager(getDevice());
		m_reporter.m_debugger=m_debugManager.pointer();

		m_guiRender=new GUI::GUIBatchRenderer();
		m_guiRender->SetDevice(getDevice());

		m_pickBuffer=new video::RTPickBuffer(getDevice(),GetRenderWindow()->GetSize()/2,GetRenderWindow()->GetSize());

		if(1)
		{
			m_cmanager=new canvas::BHCanvasManager(getDevice());
			m_cmanager->SetBackgroundTexture(gTextureResourceManager.loadTexture2D(mT("Japanese_Butterfly_and_Bonsai.jpg")));

			canvas::PointList opoints;
			canvas::CanvasUtilities::GenerateChamferedRectangle(math::vector2d(100,70),30,opoints);

			video::SColor clrs[]=
			{
				video::SColor(1,1,1,0.5),video::SColor(1,1,1,0.5),
				video::SColor(0,0,0,0),video::SColor(0,0,0,0)
			};
			if(0)
			{
				for(int i=0;i<5;++i)
				{
					canvas::ColoredShape* shape1=new canvas::ColoredShape();
					canvas::ICanvasObject* object=new canvas::ICanvasObject();
					shape1->SetColors(clrs);
					shape1->SetShapePoints(opoints);
					canvas::TexturedShape* shape2=new canvas::TexturedShape();

					shape2->SetShapePoints(opoints);
					shape2->SetTexture(gTextureResourceManager.loadTexture2D(mT("wood16.jpg")),math::rectf(0,0,1,1));

					canvas::TextShape* shape3=new canvas::TextShape();
					shape3->SetString(core::string(mT("this is a Test String:"))+core::StringConverter::toString(i));
					shape3->SetRect(math::rectf(-50,-30,50,30));

					object->AddShape(shape2);
					object->AddShape(shape3);
					object->AddShape(shape1);
					object->SetPos(math::vector2d(150 + i*100,180));


					canvas::IConnector* conn=new canvas::IConnector();
					object->AddConnector(conn);
					conn->SetPlacement((canvas::EConnectorPlacement)(i%4));

					m_cmanager->AddObject(object);
				}
			}
			for(int i=0;i<10;++i)
			{
				canvas::IBHCanvasObject* obj;
				int r=rand()%5;
				if(r==0)
				{
					canvas::BHActionObject* object=new canvas::BHActionObject();
					AI::IBHActionNode* n=new AI::IBHActionNode();
					m_cmanager->AddObject(object);
					n->SetActionName(mT("Action"));
					object->SetNode(n);
					obj=object;
				}else if(r==1)
				{
					canvas::BHSelectorObject* object=new canvas::BHSelectorObject();
					m_cmanager->AddObject(object);
					object->SetNode(new AI::BHSelectorNode());
					obj=object;
				}else if(r==2)
				{
					canvas::BHSequenceObject* object=new canvas::BHSequenceObject();
					m_cmanager->AddObject(object);
					object->SetNode(new AI::BHSequenceNode());
					obj=object;
				}else if(r==3)
				{
					canvas::BHDecoratorObject* object=new canvas::BHDecoratorObject();
					m_cmanager->AddObject(object);
					object->SetNode(new AI::IBHDecorator());
					obj=object;
				}else
				{
					canvas::BHTreeObject* object=new canvas::BHTreeObject();
					m_cmanager->AddObject(object);
					object->SetTree(new AI::IBehaviorTree());
					obj=object;
				}
				obj->SetPos(math::vector2d(150 + i*100,180));
			}
		}
		{
			mray::GUID panelID("Panel");

			m_guiManager=new GUI::GUIManager(getDevice());

			GUI::IGUIPanelElement* rootElem=new GUI::IGUIPanelElement(mray::GUID(""),m_guiManager);
			rootElem->SetSize(GetRenderWindow()->GetSize());
			GUI::IGUIPanelElement* window=(GUI::IGUIPanelElement*)m_guiManager->CreateElement(mray::GUID("Window"));

			window->SetPosition(math::vector2d(50,100));
			window->SetSize(math::vector2d(400,400));
			window->SetText(core::UTFString(mT("Window Panel")));

			GUI::IGUIStackPanel* panel=(GUI::IGUIStackPanel*)m_guiManager->CreateElement(mray::GUID("StackPanel"));
			panel->SetDocking(GUI::EED_Fill);
			panel->SetOffset(10);
			window->AddElement(panel);
			panel->SetStackDirection(GUI::IGUIStackPanel::EVertical);


			propGrid=(GUI::IGUIPropertyGrid *)m_guiManager->CreateElement(mray::GUID("PropertyGrid"));
			propGrid->SetPosition(math::vector2d(0,0));
			propGrid->SetSize(math::vector2d(200,300));
			panel->AddElement(propGrid);


			GUI::IGUICatagoryPanel* catagory=(GUI::IGUICatagoryPanel *)m_guiManager->CreateElement(mray::GUID("CatagoryPanel"));
			catagory->SetSize(math::vector2d(200,50));
			catagory->SetText(core::string(mT("Catagory2")));
			panel->AddElement(catagory);
			GUI::GUICanvasObject*canvasObj=new GUI::GUICanvasObject(m_guiManager);
			canvasObj->SetCanvasManager(m_cmanager);
			canvasObj->SetSize(math::vector2d(200,200));
			canvasObj->SetPosition(math::vector2d(0,0));
			panel->AddElement(canvasObj);
			
			propGrid->SetPropertyObject(m_cameraL);
/*
			elem=m_guiManager->CreateElement(mray::GUID("Button"));
			elem->SetPosition(math::vector2d(20,120));
			elem->SetSize(math::vector2d(200,50));
			elem->SetText(core::string("Exit"));

			panel->AddElement(elem);

			elem=m_guiManager->CreateElement(mray::GUID("Editbox"));
			elem->SetPosition(math::vector2d(220,120));
			elem->SetSize(math::vector2d(200,50));
		//	elem->GetDictionary()->getPropertie("RegularExpression")->parse(elem,mT("([a-zA-Z]*)"));
			elem->SetText(core::string("Text"));
			panel->AddElement(elem);

		//	elem->OnLostFocus+=newClassDelegate2<Application,void,mray::PVOID,mray::PVOID>("",this,&Application::OnTextBoxLostFocus);
*/


			GUI::IGUIComboList* lstBox=(GUI::IGUIComboList*)m_guiManager->CreateElement(mray::GUID("ComboList"));
			//GUI::IGUIListBox* lstBox=(GUI::IGUIListBox*)m_guiManager->CreateElement(mray::GUID("ListBox"));
			lstBox->SetPosition(math::vector2d(20,170));
			lstBox->SetSize(math::vector2d(200,30));

			for(int i=0;i<20;++i)
				lstBox->AddItem(new GUI::StringListItem(mT("Item")+core::StringConverter::toString(i)));

			lstBox->SetSelectedItem(2);

			lstBox->GetFontAttributes()->fontColor=video::DefaultColors::Black;

			panel->AddElement(lstBox);

			rootElem->AddElement(window);

			m_MieSlider=(GUI::IGUISliderBar*)m_guiManager->CreateElement(mray::GUID("SliderBar"));
			m_MieSlider->SetPosition(math::vector2d(50,150));
			m_MieSlider->SetSize(math::vector2d(200,20));
			m_MieSlider->SetDirection(GUI::ESBDir_Horizontal);
			m_MieSlider->SetMinValue(0.0001);
			m_MieSlider->SetMaxValue(1);

			m_RaleighSlider=(GUI::IGUISliderBar*)m_guiManager->CreateElement(mray::GUID("SliderBar"));
			m_RaleighSlider->SetPosition(math::vector2d(50,170));
			m_RaleighSlider->SetSize(math::vector2d(200,20));
			m_RaleighSlider->SetDirection(GUI::ESBDir_Horizontal);
			m_RaleighSlider->SetMinValue(0.0001);
			m_RaleighSlider->SetMaxValue(1);

			m_HGSlider=(GUI::IGUISliderBar*)m_guiManager->CreateElement(mray::GUID("SliderBar"));
			m_HGSlider->SetPosition(math::vector2d(50,190));
			m_HGSlider->SetSize(math::vector2d(200,20));
			m_HGSlider->SetDirection(GUI::ESBDir_Horizontal);
			m_HGSlider->SetMinValue(0.0001);
			m_HGSlider->SetMaxValue(1);


			rootElem->AddElement(m_MieSlider);
			rootElem->AddElement(m_RaleighSlider);
			rootElem->AddElement(m_HGSlider);

			m_offsetGUI=(GUI::IGUISliderBar*)m_guiManager->CreateElement(mray::GUID("SliderBar"));
			m_offsetGUI->SetPosition(math::vector2d(50,250));
			m_offsetGUI->SetSize(math::vector2d(200,20));
			m_offsetGUI->SetDirection(GUI::ESBDir_Horizontal);
			m_offsetGUI->SetMinValue(0);
			m_offsetGUI->SetMaxValue(1);
			rootElem->AddElement(m_offsetGUI);

			m_guiManager->SetRootElement(rootElem);

			m_guiManager->SetActiveTheme(GUI::GUIThemeManager::getInstance().getActiveTheme());

			GUI::GUIXMLElementVisitor v;

			xml::XMLWriter writer;
			writer.addElement(v.Visit(panel,mT("test")));

			OS::IStreamPtr file=gFileSystem.createTextFileWriter(mT("overlay.gui"));
			OS::StreamWriter ww(file);
			ww.writeString(writer.flush());


			file->close();
		}

		{
			m_gameManager=new game::GameEntityManager();
			m_gameManager->SetSceneManager(getSceneManager());
		}
		if(1)
		{
			physics::PhysicsSystemDesc desc;
			desc.gravity.set(0,-98,0);
			m_phManager=new physics::PhysXManager(&desc);

			m_phManager->AddContactReport(&m_reporter);
			m_phManager->SetActorGroupPairFlag(0,1,physics::ECRF_Touch,true);
			m_phManager->registerBenchmark();
			m_phManager->ConnectToRemoteDebugger();

			m_SQReporter.m_debugger=m_debugManager;

			physics::PhysicSceneQueryDesc sqdesc;
			sqdesc.report=&m_SQReporter;
			sqdesc.execMode=physics::ESQExec_Syncronous;

			m_squery=m_phManager->CreateSceneQuery(sqdesc);

			m_vehicleManager=new physics::VehicleManager(m_phManager);
			for(int i=0;i<10;++i)
				m_carVehicle=CreateVehicle(math::vector3d((i-15)*50,10,0));
		}

		if(0)
		{

			scene::ISceneNode* node=getSceneManager()->createSceneNode();
			node->setPosition(0);
			node->setScale(1);
			scene::MeshRenderableNode* modelNode= scene::MeshFileCreatorManager::getInstance().LoadFromFile(mT("street.mdx"),true);
			node->AttachNode(modelNode);

			physics::TriangleShapeDesc desc;
			desc.mesh=modelNode->getMesh()->getBuffer(0);

			physics::PhysicalNodeDesc phDesc;
			phDesc.shapes.push_back(&desc);
			m_phManager->createNode(&phDesc);

		}
		{

			parsers::LParser p;
			p.LoadFromFile(mT("test1.l"));
			std::vector<int>atoms,result;
			atoms.push_back(p.GetAtomID(mT("T")));
			atoms.push_back(p.GetAtomID(mT("B")));
			for(int i=0;i<5;++i)
				atoms=p.ApplyRules(atoms);

			result=atoms;

		}

		if(1)
		{
			physics::PhysicalClothDesc desc;
			desc.globalPos.setTranslation(math::vector3d(0,400,0));

			desc.bendingStiffness=0.5;
			desc.stretchingStiffness=0.5;
			desc.thickness=0.1;
			desc.friction=0.5;
			desc.tearFactor=1.5;
			desc.pressure=1.5;
			desc.meshData=new scene::SMeshBuffer();
		//	desc.flags[physics::ECF_Bending]=true;
		//	desc.flags[physics::ECF_Terrable]=true;
			desc.flags[physics::ECF_Pressure]=true;
			desc.flags[physics::ECF_CollisionTwoWay]=true;
		//	desc.flags[physics::ECF_Hardware]=true;
			scene::MeshGenerator::getInstance().generateSphere(20,10,10,desc.meshData);
			//scene::MeshGenerator::getInstance().generatePlane(10,10,desc.meshData);
			//scene::SMeshManipulator::getInstance().scaleMesh(desc.meshData,100);
/*
			desc.tearableFlags.resize(20*20);
			for(int i=0;i<20;++i)
			{
				int idx=20*10+i;
				desc.tearableFlags[idx]=true;
			}*/


			cloth= m_phManager->createCloth(&desc);
			scene::SMesh* mesh=new scene::SMesh();
			mesh->addBuffer(cloth->GetRenderBuffer());

			GCPtr<video::SMaterial> mtrl=new video::SMaterial();
			mtrl->SetAmbient(1);
			mtrl->SetDiffuse(1);
			mtrl->SetEmissive(0);
			mtrl->setRenderState(video::RS_CullFace,video::ES_DontUse);
			mtrl->setTexture(gTextureResourceManager.loadTexture2D(mT("wood16.jpg")),0);
			mtrl->setTexture(gTextureResourceManager.loadTexture2D(mT("FieldstoneBumpDOT3.tga")),1);
			video::GPUShader*shader= new video::GPUShader();

			shader->SetVertexShader(gShaderResourceManager.loadShader(mT("phong2.cg"),video::EShader_VertexProgram,"main_vp",mT("cg")));
			shader->SetFragmentShader(gShaderResourceManager.loadShader(mT("phong2.cg"),video::EShader_FragmentProgram,"main_fp",mT("cg")));
			mtrl->setRenderShader(shader);
			mesh->getBufferData(0)->setMaterial(mtrl);


			scene::MeshRenderableNode*r=new scene::MeshRenderableNode(mesh);
			scene::ISceneNode* node=getSceneManager()->createSceneNode();
			node->AttachNode(r);
			r->setShadowCaster(true);

			cloth->AttachNode(r,0);
		//	cloth->setWindAcceleration(math::vector3d(50,10,0));

			propGrid->SetPropertyObject(cloth);

		}


		//scene::SParticleSystem* particleSystem=CreateParticleSystem();
		scene::SPatchTerrain* terrain=loadTerrain();



		GCPtr<scene::ISceneNode> node;
		GCPtr<video::SMaterial> mtrl;

		CreateBalls();

		int nCnt=0;
		for(int i=0;i<nCnt;++i){
			for(int j=0;j<nCnt;++j){
				GCPtr<scene::MeshRenderableNode> snode=new scene::MeshRenderableNode(m_vegetation->GetBillboardObjects());
				scene::ISceneNode* node=getSceneManager()->createSceneNode(mT(""));
				
				node->AttachNode(snode);
				node->setDrawBoundingBox(true);
				/*
				m_nodes.push_back(snode);
				snode->setCullingType(scene::SCT_NONE);
				snode=new scene::MeshSceneNode(m_vegetation->GetDynamicObjects(),mT(""),0,0,math::quaternion::Identity,1,0);
				getSceneManager()->addSceneNode(snode);
				snode->setCullingType(scene::SCT_NONE);
				m_nodes.push_back(snode);*/
			//	snode->position=math::vector3d((i-nCnt*0.5)*500,0,(j-nCnt*0.5)*500);
				node->setPosition(math::vector3d((math::Randomizer::rand01()-0.5)*100,0,(math::Randomizer::rand01()-0.5)*100));
			}
		}
		{
			m_light->update(0);
			m_atmosphericCubeMap=new game::AtmosphericCubeMap();
			m_atmosphericCubeMap->SetThreadManager(m_appThreadManager);
			m_atmosphericCubeMap->SetLightDirection(m_light->getDir());
			m_atmosphericCubeMap->Create(640000.0,95.0213,-0.985148,638640,637800,
				math::vector3d(2.9427e-006, 9.4954e-006, 1.40597e-005),math::vector3d(9.43648e-005, 3.36762e-005, 6.59358e-006),
				256,256,0.005f,0.02f);

			GCPtr<video::ITexture> tex= m_atmosphericCubeMap->GetSkyBoxTexture();//scene::SkyBoxManager::getInstance().loadSkyFromFolder(mT("skybox\\night\\"),mT("png"),mT("night"));
			GCPtr<scene::IRenderable> sky=new scene::SSkyBoxNode(tex);//scene::SceneNodeCreator::addSkyBox(getSceneManager(),tex);
			sky->SetHasCustomRenderGroup(true);
			sky->SetTargetRenderGroup(scene::RGH_Skies);
			scene::ISceneNode*node= getSceneManager()->createSceneNode();
			node->AttachNode(sky);
			node->setCullingType(scene::SCT_NONE);
		}

		//GCPtr<video::SMaterial> mtrl=new video::SMaterial();
		mtrl=new video::SMaterial();
		mtrl->setRenderState(video::RS_CullFace,video::ES_DontUse);
		mtrl->setRenderState(video::RS_Lighting,video::ES_DontUse);
		mtrl->SetAlphaFunction(video::EAF_Greater);
		mtrl->SetAlphaReferenceValue(0.01);
		video::GPUShader* shader=new video::GPUShader();
		shader->SetVertexShader(gShaderResourceManager.loadShader(mT("GrassObject.cg"),video::EShader_VertexProgram,"main_vp",mT("cg")));
		shader->SetFragmentShader(gShaderResourceManager.loadShader(mT("GrassObject.cg"),video::EShader_FragmentProgram,"main_fp",mT("cg")));
		mtrl->setRenderShader(shader);
		//mtrl->setRenderState(video::RS_ZWrite,video::ES_DontUse);
		//mtrl->setRenderState(video::RS_ZTest,video::ES_Use);
		//mtrl->setRenderState(video::RS_Blend,video::ES_Use);
		mtrl->setTexture(gTextureResourceManager.loadTexture2D(mT("marsh_grass.png")),0);
		mtrl->setTexture(gTextureResourceManager.loadTexture2D(mT("Grass_Colors.png")),1);
		mtrl->getTexture(0)->SetNumberOfMipmaps(3);
		//mtrl->getTexture(0)->setMipmapsFilter(false);
		mtrl->getTexture(0)->setTrilinearFilter(false);
		mtrl->getTexture(0)->setBilinearFilter(1);
		//mtrl->getTexture(0)->setAnisotropicFilter(true);
		
		if(0)
		{
			GCPtr<scene::GrassRenderNode> grassBuilder=new scene::GrassRenderNode();
			grassBuilder->AddUVRect(math::rectf(0.5,0.5,0,0),1);
			grassBuilder->AddUVRect(math::rectf(1,0.5,0.5,0),1);
			grassBuilder->AddUVRect(math::rectf(0.5,1,0,0.5),1);
			grassBuilder->AddUVRect(math::rectf(1,1,0.5,0.5),1);/**/
			for(int i=0;i<10000;++i)
			{
				math::vector3d pos=math::vector3d((math::Randomizer::rand01()-0.5)*100,0,(math::Randomizer::rand01()-0.5)*100);
				grassBuilder->AddObject(0,pos,0.2,0);
			}
			grassBuilder->Build(3);
			grassBuilder->getMesh()->getBufferData(0)->setMaterial(mtrl);
		//	for(int i=0;i<4;++i)
			{
			//	for(int j=0;j<4;++j)
				{
				//	snode->position=math::vector3d((i-2)*25,0,(j-2)*25);
					scene::ISceneNode* node= getSceneManager()->createSceneNode(mT(""));
					node->AttachNode(grassBuilder);
				}
			}
		}
		if(0)
		{
			GCPtr<video::ITexture> grassTex=gTextureResourceManager.loadTexture2D(mT("Grass.tga"));
			grassTex->load(false);//make sure it is loaded

			std::vector<scene::GrassRenderNode*> result=scene::GrassRenderNode::GenerateFromPixels(grassTex->getSurface(0),2000,0.4,10,5);
			for(int i=0;i<result.size();++i)
			{
				if(result[i]->GetGrassCount()>0){
					result[i]->AddUVRect(math::rectf(0.5,0.5,0,0),1);
					result[i]->AddUVRect(math::rectf(1,0.5,0.5,0),1);
					result[i]->AddUVRect(math::rectf(0.5,1,0,0.5),1);
					result[i]->AddUVRect(math::rectf(1,1,0.5,0.5),1);
					if(m_terrain)
					{
						for(int j=0;j<result[i]->GetGrassCount();++j)
						{
							vector3d p=result[i]->GetGrassData(j)->pos;
							p.y=m_terrain->getHeight(p.x,p.z);
							result[i]->GetGrassData(j)->pos.y=p.y;
						}
					}
					result[i]->Build(2);
					result[i]->ClearGrasses();
					result[i]->getMesh()->getBufferData(0)->setMaterial(mtrl);

					//	snode->position=math::vector3d((i-2)*25,0,(j-2)*25);
					scene::ISceneNode* node= getSceneManager()->createSceneNode(mT(""));
					node->AttachNode(result[i]);
				//	node->setDrawBoundingBox(true);
					//node->setPosition(vector3d(-200,0,-200));
					/*
					node->DrawDebugObjects=true;
					node->DrawBoundingBox=true;*/
					//	snode->position=math::vector3d((i-2)*25,0,(j-2)*25);
				}
			}
		}


		if(0)
		{
			scene::SBillboardChain* chain=new scene::SBillboardChain(100,4,2);
			m_billboardChainNode=getSceneManager()->createSceneNode();
			m_billboardChainNode->AttachNode(chain);
			m_billboardChainNode->setCullingType(scene::ENodeCullingType::SCT_NONE);
			chain->setRecordInterval(0.01);

			video::GPUShader* shader=new video::GPUShader();
			shader->SetVertexShader(gShaderResourceManager.loadShader(mT("phong.cg"),video::EShader_VertexProgram,"main_vp",mT("cg")));
			shader->SetFragmentShader(gShaderResourceManager.loadShader(mT("phong.cg"),video::EShader_FragmentProgram,"main_fp",mT("cg")));
			chain->getMaterial(0)->setRenderShader(shader);

			chain->getMaterial(0)->setTexture(gTextureResourceManager.loadTexture2D(mT("smokeTrail03.tga")),0);;
			chain->getMaterial(0)->setTexture(gTextureResourceManager.loadTexture2D(mT("smoke_trail04_line_local.tga")),1);;
			chain->getMaterial(0)->setMaterialRenderer(video::MRT_TRANSPARENT);


		}
		if(0)
		{
			scene::MeshRenderableNode* modelNode= scene::MeshFileCreatorManager::getInstance().LoadFromFile(mT("male.mdx"),true);
/*
			scene::TangentCalculater t(1,2);
			t.calculateTangents(modelNode->getMesh());
			m_ik=new animation::IterativeIK();

			modelNode->setShadowCaster(true);
*/
/*
			animation::BonesJointQuery* jQuery=new animation::BonesJointQuery();

			scene::Skeleton* skeleton= modelNode->getMesh()->getSkeleton();
			jQuery->addBone(skeleton->getBone(mT("Bip01_R_UpperArm")));
			jQuery->addBone(skeleton->getBone(mT("Bip01_R_Forearm")));
			m_ef=skeleton->getBone(mT("Bip01_R_Hand"));
			jQuery->addBone(m_ef,true);

			m_target=m_ef->getAbsolutePosition();

			m_ik=new animation::IterativeIK();
			m_ik->setJointsChain(jQuery);
			m_ik->setBeta(0.01);
			m_ik->setAllowedError(0.005);
			m_ik->setMaxIterations(100);
			m_ik->calcOrintation(false);
			//m_ik->setTarget(math::vector3d(-42.624 ,137.684   ,24.88   ));
			m_ik->setJacobian(new math::TJacobian());

			animation::AnimationController *controller=modelNode->getMesh()->getSkeleton()->getAnimationMixer()->getAnimationController("Run");
			controller->play(20,animation::EAM_Cycle);
			controller->getGroup()->playAll();
			skeleton->getBone("Bip01_R_UpperArm")->SetManualAnimation(true);
			skeleton->getBone("Bip01_R_Forearm")->SetManualAnimation(true);
*/

			m_modelNode=getSceneManager()->createSceneNode();
			m_modelNode->AttachNode(modelNode);
			m_modelNode->setScale(1);
			m_modelNode->setPosition(vector3d(0,5,0));
		}

		if(0)
		{
			video::ColorCurve curve;
			curve.AddPoint(0.1,0.5);
			curve.AddPoint(0.9,0.5);

			int n=100;
			for(int i=0;i<n;++i)
			{
				float t=(float)i/(float)n;
				math::vector2d v=curve.GetValue(t);

				printf("(%f,%f),",v.x,v.y);
			}
		}

		if(1)
		{

			GCPtr<scene::ISceneNode> node;
			GCPtr<video::SMaterial> mtrl=new video::SMaterial();
			mtrl->SetAmbient(1);
			mtrl->SetDiffuse(1);
			mtrl->SetEmissive(0);
			mtrl->setRenderState(video::RS_CullFace,video::ES_DontUse);
			mtrl->setTexture(gTextureResourceManager.loadTexture2D(mT("wood16.jpg")),0);
			mtrl->setTexture(gTextureResourceManager.loadTexture2D(mT("FieldstoneBumpDOT3.tga")),1);
			video::GPUShader*shader= new video::GPUShader();

			shader->SetVertexShader(gShaderResourceManager.loadShader(mT("phong2.cg"),video::EShader_VertexProgram,"main_vp",mT("cg")));
			shader->SetFragmentShader(gShaderResourceManager.loadShader(mT("phong2.cg"),video::EShader_FragmentProgram,"main_fp",mT("cg")));
			mtrl->setRenderShader(shader);

			math::vector3d ext(5,5,30);

			GCPtr<scene::SMesh> NodeMesh=new scene::SMesh();
			NodeMesh->addNewBuffer();
			scene::MeshGenerator::getInstance().generateBox(ext,NodeMesh->getBuffer(0));
//			scene::MeshGenerator::getInstance().generateSphere(10,10,10,NodeMesh->getBuffer(0));
//			scene::SMeshManipulator::getInstance().translateMesh(NodeMesh->getBuffer(0),math::vector3d(30,30,30));
			NodeMesh->getBufferData(0)->setMaterial(mtrl);
			NodeMesh->calcCurrBoundingBox();

			GCPtr<scene::MeshRenderableNode> Node=new scene::MeshRenderableNode(NodeMesh);
			Node->setShadowCaster(true);

			node=new scene::ISceneNode(mT("Node_Rotater"),0,0);
			node->setPosition(math::vector3d(0,20,0));
			//node->setScale(math::Randomizer::rand01()*2+1);
			node->updateAbsoluteTransformation();
			node->AttachNode(Node);

			getSceneManager()->addSceneNode(node);

			game::GameEntity* ent=new game::GameEntity(mT("Rotater"),m_gameManager);
			m_gameManager->AddGameEntity(ent);
			ent->SetShape(node);
			ent->AddComponent(new RotaterGameAction());
		}
#if 0
		if(0)
		{
			m_gbuffer=new scene::GBufferListener(getDevice(),m_viewPort);
			m_viewPort->addListener(m_gbuffer);
		}
#endif
		m_screenEffShader=0;
		m_motionBlurPP=0;
		m_antiAliasPP=0;
		m_LutPP=0;


		scene::ViewPort* vp[]={m_stereoRenderer->GetLeftVP(),m_stereoRenderer->GetRightVP()};
			video::CMultiPassPP* shaderPipe=new video::CMultiPassPP(getDevice());
			if(1)
			{
				video::ParsedShaderPP* pp=new video::ParsedShaderPP(getDevice());
				pp->LoadXML(gFileSystem.openFile(mT("HDR.peff")));
				shaderPipe->addPostProcessor(pp);

				m_HDRPP=pp;;
			}
			if(1)
			{
				video::ParsedShaderPP* pp=new video::ParsedShaderPP(getDevice());
				pp->LoadXML(gFileSystem.openFile(mT("DoF.peff")));
				shaderPipe->addPostProcessor(pp);

				m_DofPP=pp;;
			}
			if(1)
			{
				video::ParsedShaderPP* pp=new video::ParsedShaderPP(getDevice());
				pp->LoadXML(gFileSystem.openFile(mT("MotionBlur.peff")));
				shaderPipe->addPostProcessor(pp);

				m_motionBlurPP=pp;;
			}
			if(1)
			{
				video::ParsedShaderPP* pp=new video::ParsedShaderPP(getDevice());
				pp->LoadXML(gFileSystem.openFile(mT("FXAA.peff")));
				shaderPipe->addPostProcessor(pp);
				m_antiAliasPP=pp;
			}
			if(1)
			{
				video::ParsedShaderPP* pp=new video::ParsedShaderPP(getDevice());
				pp->LoadXML(gFileSystem.openFile(mT("LUT.peff")));
				m_LutPP=pp;
				
				int id=pp->GetPassID(mT("final"));
				video::ParsedShaderPP::ProcessingPass*pass=pp->GetPass(id);
				if(pass)
					m_screenEffShader=pass->GetShader();
				

				video::ParsedShaderPP::MappedParams*val= m_LutPP->GetParam(mT("LUT"));
				video::ITexturePtr tex=gTextureResourceManager.loadTexture3D(mT("LUT-yellow.mtex"));
				tex->load(false);
				tex->setMipmapsFilter(false);
				
				val->SetTexture(tex.pointer());
				if(0)
				{
					video::ColorCurve rCurve;
					video::ColorCurve gCurve;
					video::ColorCurve bCurve;

					video::LUTGenerator generator;

					id=pp->GetPassID(mT("pass1"));
					pass=pp->GetPass(id);
					if(pass)
					{
						video::ParsedShaderPP::PassValue*val= pass->GetValue(mT("LUT"));

						if(val)
						{
							val->texParam->SetTexture(generator.GenerateLUT(getDevice(),&rCurve,&gCurve,&bCurve));
						}
					}
				}
				shaderPipe->addPostProcessor(pp);
			}
			for(int i=0;i<2;++i)
			{
			if(1)
			{
				math::vector2d sz=vp[i]->getAbsViewPort().getSize();
				renderTargetTex=getDevice()->createTexture2D(sz,video::EPixel_Float16_RGB,true);

				video::IRenderTargetPtr rt=getDevice()->createRenderTarget(mT(""),renderTargetTex,video::IHardwareBufferPtr::Null,video::IHardwareBufferPtr::Null,false);
				renderTargetTex=getDevice()->createTexture2D(sz,video::EPixel_Float16_RGB,true);
				rt->attachRenderTarget(renderTargetTex,1);
			//	video::IRenderTargetPtr rt=getDevice()->createMRenderTarget("",sz,video::EPixel_R8G8B8,1,true,true);
				//renderTargetTex=rt->getColorTexture(0);

			
				vp[i]->setRenderTarget(rt);

				vp[i]->enablePostProcessing(true);
				vp[i]->setPostProcessing(shaderPipe);
			}
		}

#if 0
		//return;
		node=getSceneManager()->addAnimatedMesh(mT("male.mdx"));
		scene::MeshSerializer ser;
		GCPtr<OS::IStream> stream;/*=gFileSystem.openFile("tree10.mesh",OS::BIN_WRITE);
		ser.Serialize(node->getMesh(),stream);
		stream->close();
		stream=gFileSystem.openFile("tree10.mesh",OS::BIN_READ);
		GCPtr<scene::SMesh>loadedMesh=ser.Deserialize(stream);
		stream->close();
		node=new scene::MeshSceneNode(loadedMesh,"",0,0,math::quaternion::Identity,1,0);
		getSceneManager()->addSceneNode(node);
*/
		//getSceneManager()->Animators.addRotateAnimator(node,math::vector3d(3,1,1),300);

		node->getMesh()->calcCurrBoundingBox();
		node->update(0);
		/**/for(int i=0;i<node->getMaterialCount();++i){
			node->getMaterial(i)->AlphaFunction=video::EAF_Greater;
			node->getMaterial(i)->AlphaRef=0.5;
			node->getMaterial(i)->setRenderState(video::RS_CullFace,video::ES_DontUse);
			node->getMaterial(i)->setRenderState(video::RS_Lighting,video::ES_DontUse);
			node->getMaterial(i)->setRenderState(video::RS_Blend,video::ES_DontUse);
			node->getMaterial(i)->setRenderState(video::RS_ZTest,video::ES_Use);
			node->getMaterial(i)->setRenderState(video::RS_ZWrite,video::ES_Use);
			node->getMaterial(i)->shader=gShaderResourceManager.loadShader(mT("GlowObject.cg","GlowObject.cg",mT("cg"));
		}

		//		getSceneManager()->setShadowMap(m_shadowRenderTarget);

		float fov=m_camera->getFovY();
		math::box3d box=node->getBoundingBox();
		float distance=0.6f*box.getHeight()/(tan(fov*0.5));

		int n=12;
		m_objectTex.resize(n);
		m_objectRT.resize(n);

		float step=360.0f/(float)n;
		float angle=0;
		getDevice()->set3DMode();

		m_distance=distance;

		m_camera->setTarget(box.getCenter());
		m_camera->useTarget=true;
		m_center=box.getCenter();
		box.repair();
		float maxSide=math::Max(box.getWidth(),box.getDepth());
		float maxLength=math::Max(maxSide,box.getHeight());
		float ratio=maxSide/maxLength;
		float oldAspect=m_camera->Aspect;
		m_camera->Aspect=ratio;
		m_light->render();
		for(int i=0;i<n;++i)
		{
			m_objectTex[i].texture=getDevice()->createTexture2D(vector2di(128,128),video::EPixel_R8G8B8);
			m_objectRT[i]=getDevice()->createRenderTarget("",m_objectTex[i].texture,0,0,0);

			math::vector3d pos;
			pos.x=distance*cosd(angle);
			pos.z=distance*sind(angle);

			angle+=step;

			pos+=box.getCenter();
			m_camera->position=pos;
			m_camera->updateAbsoluteTransformation();
			m_camera->updateView();
			getDevice()->setRenderTarget(m_objectRT[i]);
			getDevice()->clearBuffer(video::EDB_COLOR);
			getDevice()->clearBuffer(video::EDB_DEPTH);
			getDevice()->setTransformationState(video::TS_VIEW,m_camera->getViewMatrix());
			getDevice()->setTransformationState(video::TS_PROJECTION,m_camera->getProjectionMatrix());
			//node->directRender();
			getDevice()->setRenderTarget(0);
		}
		m_camera->Aspect=oldAspect;
		m_camera->useTarget=false;

		math::vector2di vpSize=getDevice()->getViewportRect().getSize();
		float aspect=(float)vpSize.x/(float)vpSize.y;

		GCPtr<video::ITexture> texture=getDevice()->createTexture2D(shadowMapWidthHeight,video::EPixel_Float16_R);
		m_shadowRenderTarget=getDevice()->createRenderTarget(mT("shadowRenderTarget"),texture,0,0,0);

		//m_light->setShadowMap(m_shadowRenderTarget);

		m_projection.buildProjectionMatrixPerspectiveFovLH(m_camera->fovY,
			m_camera->Aspect,1,100);

#endif

		//m_light->setProjection(m_projection);

	}
	virtual void update(float dt)
	{
		static float angle=0;
		CMRayApplication::update(dt);
		if(m_vegetation)
			m_vegetation->Update(m_cameraL);

		math::vector3d pos;
		pos.x=cosd(angle);
		pos.z=sind(angle);
		angle+=100*dt;
		((scene::SceneManager*)getSceneManager())->GetJobsManager()->EndProcessing();

		//m_light->setPosition(math::vector3d(pos.x*20,20,pos.z*20));
		

		if(m_billboardChainNode)
			m_billboardChainNode->setPosition(m_cameraL->getPosition()+ pos);

		if(m_terrain)
		{
			vector3d p=m_secCamera->getPosition();
			float y=m_terrain->getHeight(p.x,p.z)+4;
			if(y>p.y)
				p.y=y;
			m_secCamera->setPosition(p);
		}

		if(m_soundManager)m_soundManager->runSounds(dt);

		if(m_phManager)m_phManager->update(dt);
		if(m_gameManager)m_gameManager->Update(dt);

		if(m_screenEffShader)
		{
			float v=abs(sin(gTimer.getSeconds()*0.0005));
			m_screenEffShader->GetFragmentShader()->setConstant(mT("lerpVal"),&v,1);
		}

		m_target+=m_targetVel*dt;
//		m_modelNode->setOrintation(math::quaternion(angle,math::vector3d(0,1,0)));
		if(m_ik)
		{
			m_ik->setTarget(m_target-math::vector3d(0,5,0),math::quaternion(90,math::vector3d(1,0,0)));

			m_debugManager->AddCross(m_target,0.2,video::SColor(0,0,1,1),1);

			m_ik->update();
		}
		if(m_scriptManager)
			m_scriptManager->update(dt);

		if(m_vehicleManager)
			m_vehicleManager->Update(dt);

		if(m_carVehicle)
		{
			m_carVehicle->Control(m_vehControling.y,false,m_vehControling.x,false,m_vehControling.z!=0);
			math::vector3d carPos=m_carVehicle->GetPhysicalNode()->getGlobalPosition();
			pos=carPos+m_carVehicle->GetPhysicalNode()->getGlobalOrintation()*math::vector3d(-5,0,0);
			pos.y+=4;

			if(!m_freeCam)
			{
				math::matrix4x4 m=math::MathUtil::CreateLookAtMatrix(pos,carPos,math::vector3d::YAxis);
				 
				m_stereoRenderer->GetHeadMount()->setPosition(pos);
			//	m_stereoRenderer->GetHeadMount()->setOrintation(m);
			}
		}

#if 0
		if(m_DofPP)
		{
			video::ParsedShaderPP::MappedParams* p= m_DofPP->GetParam(mT("FocalPoint"));
			vector3d point=m_carVehicle->GetPosition();
			p->SetValue(point);
		}
#endif
		{
			float mie=m_MieSlider->GetValue();
			float ral=m_RaleighSlider->GetValue();
			float hg=m_HGSlider->GetValue();
			m_atmosphericCubeMap->SetCoefficients(mie,ral,hg);
			m_atmosphericCubeMap->Invalidate(false);
			m_atmosphericCubeMap->Update();
		}

		m_stereoRenderer->SetOffset(m_offsetGUI->GetValue());
		//IGCCollector::getInstance().collect();
	}


	virtual void draw(scene::ViewPort* vp)
	{
		CMRayApplication::draw(vp);
		if(!vp->getCamera())
			return;

		if(0)
		{
			getDevice()->set2DMode();
			//Engine::getInstance().getDevice()->set2DMode();
			getDevice()->useShader(m_clearRTShader);
			getDevice()->setRenderTarget(m_renderTarget,0,0);
			//m_callback.setConstants(m_clearRTShader);
			getDevice()->useTexture(0,0);
			getDevice()->draw2DImage(math::rectf(0,0,1,1),video::DefaultColors::White);
			getDevice()->setRenderTarget(0);
			getDevice()->unuseShader();
			getDevice()->set3DMode();
		}

		double t1=gTimer.getSeconds();
		{



			if(0)
			{
				m_debugManager->StartDraw(m_secCamera);
				m_debugManager->EndDraw();
			}
		}
		t1=gTimer.getSeconds()-t1;
		//printf("%f\n",t1);
/*
		getDevice()->set3DMode();
		getDevice()->useMaterial(0);
		getDevice()->drawBox(m_secCamera->getViewFrustrum()->boundingBox,math::matrix4x4::Identity,video::MR_LINES);
		getDevice()->set2DMode();*/
		//for(int i=0;i<m_objectTex.size();++i)
		

	}
	virtual void WindowPostRender(video::RenderWindow* wnd)
	{

		GCPtr<GUI::BaseFont> font=gFontResourceManager.getDefaultFont();
		getDevice()->set2DMode();

		m_stereoRenderer->Render();
		if(font){
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
			attr.RightToLeft=1;
			core::string msg=mT("ãÚÏEÇáÑÓE ");
			msg+=core::StringConverter::toString(CFPS::getInstance().getFPS());
			msg+=mT("  ")+core::StringConverter::toString(m_pickedID);

			font->print(math::rectf(10,yoffset+10,10,10),&attr,0,msg,m_guiRender);
			attr.fontSize=18;
			msg=mT("Primitive drawn : ");
			msg+=core::StringConverter::toString(getDevice()->getPrimitiveDrawnCount());
			font->print(math::rectf(10,yoffset+30,10,10),&attr,0,msg,m_guiRender);
			msg=mT("Batches drawn : ");
			msg+=core::StringConverter::toString(getDevice()->getBatchDrawnCount());
			font->print(math::rectf(10,yoffset+50,10,10),&attr,0,msg,m_guiRender);
			msg=mT("Exposure : ");
			msg+=core::StringConverter::toString(m_hdrExposure);
			font->print(math::rectf(120,yoffset+50,10,10),&attr,0,msg,m_guiRender);

			/*
			msg=mT("Physics : ");
			BenchmarkItem* bitem= Engine::getInstance().getRootBenchmarking()->getSubItem("Physics")->getSubItem("Simulate");
			msg+=core::StringConverter::toString(bitem->getAverageTime());
			font->print(math::rectf(10,yoffset+70,10,10),&attr,0,msg,m_guiRender);
			*/

			if(m_carVehicle)
			{
				msg=mT("RPM:");
				msg+=core::StringConverter::toString(m_carVehicle->GetMotor()->GetRPM());
				font->print(math::rectf(10,yoffset+80,10,10),&attr,0,msg,m_guiRender);
				msg=mT("Gear:");
				msg+=core::StringConverter::toString(m_carVehicle->GetGears()->GetCurrGear());
				font->print(math::rectf(300,yoffset+80,10,10),&attr,0,msg,m_guiRender);
				msg=mT("Velocity:");
				msg+=core::StringConverter::toString(m_carVehicle->GetDriveVelocity());
				font->print(math::rectf(10,yoffset+100,10,10),&attr,0,msg,m_guiRender);
				msg=mT("Acc:");
				msg+=core::StringConverter::toString(m_vehControling.x);
				font->print(math::rectf(300,yoffset+100,10,10),&attr,0,msg,m_guiRender);
			}

			m_guiRender->Flush();

		}

		m_guiManager->DrawAll();
		static math::rectf cmanagerRect=math::rectf(100,42,400,400);

		if(1)
		{
			getDevice()->setRenderState(video::RS_Blend,false);
			video::TextureUnit tex;
			tex.SetTexture(m_atmosphericCubeMap->GetExtinctionTexture());
			//	tex.SetTexture(m_shadowRenderTarget->getColorTexture());
			getDevice()->useTexture(0,&tex);
			getDevice()->draw2DImage(math::rectf(10,10,200,30),1);
			getDevice()->useTexture(0,0);
		}
	}

	virtual void onDone(){
		traceFunction(Application);
		std::vector<core::string> arr;
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



APPLICATION_ENTRY_POINT
{
	GCPtr<Application> app=new Application();

	core::string resFileName=mT("plugins.stg");
#ifdef UNICODE
	resFileName=mT("pluginsU.stg");
#endif
	/*
	math::matrix4x4 m1,m2;
	m1=math::MathUtil::CreateLookAtMatrix(math::vector3d(10,0,5),math::vector3d(0,5,0),math::vector3d::ZAxis);
	m2=math::MathUtil::CreateProjectionMatrixPerspectiveFov(1,1,0.1,100);
	math::matrix4x4 m1Inv,m2Inv;
	m1Inv=*/

//	plugins::mrayGLPlugin plugin;
//	plugin.install();
	std::vector<SOptionElement> extraOptions;
	app->loadResourceFile(mT("dataPath.stg"));
	if(app->startup(mT("MRaY App"),vector2di(800,600),false,extraOptions,resFileName,0,true,false))
	{
		/*
		GCPtr<OS::ISystemProcess> process=OS::IOSystem::getInstance().CreateSystemProcess();
		process->SetApplicationName("c:\\windows\\notepad.exe");
		process->GetProcessStartupInfo().showWindow=true;
		process->Run();
		*/

		app->run();
	}
	app=0;
}

