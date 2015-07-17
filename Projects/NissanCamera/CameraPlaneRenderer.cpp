

#include "stdafx.h"
#include "CameraPlaneRenderer.h"
#include "CameraConfigurationManager.h"
#include "RenderMaterial.h"
#include "RenderTechnique.h"
#include "RenderPass.h"
#include "RenderManager.h"
#include "MeshBufferData.h"
#include "LocalCameraVideoSource.h"
#include "ICameraVideoGrabber.h"
#include "TextureRTWrap.h"
#include "NCAppGlobals.h"
#include "LensDistortion.h"
#include "Application.h"
#include "NCAppGlobals.h"
#include "FontResourceManager.h"
#include "RobotCameraState.h"


namespace mray
{
namespace NCam
{

	// important read about lens-distortion 
	// http://www.decarpentier.nl/lens-distortion
	// http://mrl.nyu.edu/~dzorin/papers/zorin1995cgp.pdf

	enum class ECameraPlaneRendererKeys
	{
		EnableCorrection=200,
		ShowWireframe
	};

CameraPlaneRenderer::CameraPlaneRenderer()
{
	m_surface[0] = m_surface[1] = 0;
	m_videoSource = new TBee::LocalCameraVideoSource();
	m_camConfigDirty = true;
	m_useLensCorrection = false;
	m_cameraConfiguration = 0;
	m_displayFov = 80;

	m_surfaceParams.distance=1.0;

//	m_cameraOffsets.z = 1.5f;		//camera offset from user eye
//	m_cameraOffsets.z = 1.0f;		//camera offset from user eye
	m_cameraOffsets.x = -0.6;		// camera offset from center of screen
	m_cameraOffsets.y = -0.1;		// camera offset from center of screen

}
CameraPlaneRenderer::~CameraPlaneRenderer()
{
	m_videoSource->Close();
	delete m_videoSource;
}
bool CameraPlaneRenderer::OnEvent(Event* e)
{
	bool ok = false;
	if (e->getType() == ET_Keyboard)
	{
		KeyboardEvent* evt = (KeyboardEvent*)e;
		if (evt->press)
		{
			ECameraPlaneRendererKeys cmdCode = (ECameraPlaneRendererKeys)NCAppGlobals::Instance()->keyMap.GetCommand(evt);
			if (cmdCode==ECameraPlaneRendererKeys::EnableCorrection)
			{
				m_useLensCorrection = !m_useLensCorrection;
			}
			else if (cmdCode == ECameraPlaneRendererKeys::ShowWireframe)
			{
				bool enabled = !m_wireframePass[0]->IsEnabled();
				m_wireframePass[0]->SetEnabled(enabled);
				m_wireframePass[1]->SetEnabled(enabled);
			}
		}
	}

	return ok;
}

void CameraPlaneRenderer::Init(CarObjects* car)
{
	m_car = car;

	{
//		((TBee::LocalCameraVideoSource*)m_videoSource)->SetCameraResolution(m_cameraResolution, m_cameraFPS);
		m_videoSource->Init();
	}

	m_lensCorrectionPP = new video::ParsedShaderPP(Engine::getInstance().getDevice());
	m_lensCorrectionPP->LoadXML(gFileSystem.openFile("LensCorrection.peff"));

	GenerateSurface(m_videoSource->GetEyeResolution(0).x / m_videoSource->GetEyeResolution(0).y, m_surfaceParams.distance);

	{

		controllers::InputKeyMap& keys = NCAppGlobals::Instance()->keyMap;

		keys.RegisterKey((uint)ECameraPlaneRendererKeys::EnableCorrection, KEY_C, true, false, "Enable/Disable Camera Correction");
		keys.RegisterKey((uint)ECameraPlaneRendererKeys::ShowWireframe, KEY_W, true, false, "Show/Hide Projection Wireframe");
	}
	m_car->Camera[0]->setFovY(m_displayFov);
	m_car->Camera[1]->setFovY(m_displayFov);

	//set display screen params
	m_offAxisProj[0].SetScreenParams(math::vector3d(0.550, 0.050, -0.500), math::vector2d(0.27, 0.14), math::quaternion::Identity);
	m_offAxisProj[1].SetScreenParams(math::vector3d(0.550, 0.050, -0.500), math::vector2d(0.27, 0.14), math::quaternion::Identity);
}

void CameraPlaneRenderer::Start()
{
	m_videoSource->Open();
	((TBee::LocalCameraVideoSource*)m_videoSource)->SetCameraParameterValue(video::ICameraVideoGrabber::Param_Focus, "0.0");
}

void CameraPlaneRenderer::Stop()
{
	m_videoSource->Close();
}


void CameraPlaneRenderer::GenerateSurface(  float aspectRatio, float cameraScreenDistance)
{
	float radius = cameraScreenDistance;
	m_surfaceParams.aspect = aspectRatio;
	m_surfaceParams.fovScaler = 1;
	m_surfaceParams.distance = cameraScreenDistance;

	for (int i = 0; i < 2; ++i)
	{
		if (m_surface[i])
		{
			m_car->CameraPlane[i]->RemoveNode(m_surface[i]);
			m_surface[i] = 0;
		}
	}

	scene::ISceneManager* smngr = m_car->Head->getSceneManager();
	scene::MeshBufferData* meshdata[2];

	//Create Screen Node
	for (int i = 0; i < 2; ++i)
	{
		m_car->CameraPlane[i] = smngr->createSceneNode("ScreenNode");
		scene::MeshRenderableNode* rnode = new scene::MeshRenderableNode(new scene::SMesh());
		m_surface[i] = rnode;

		scene::MeshBufferData* bdata = rnode->getMesh()->addNewBuffer();
		scene::IMeshBuffer* buffer = bdata->getMeshBuffer();
		meshdata[i] = bdata;

		video::IHardwareStreamBuffer* pos = buffer->createStream(0, video::EMST_Position, video::ESDT_Point3f, 4, video::IHardwareBuffer::EUT_WriteOnly, false, false);
		video::IHardwareStreamBuffer* tc = buffer->createStream(0, video::EMST_Texcoord, video::ESDT_Point2f, 4, video::IHardwareBuffer::EUT_WriteOnly, false, false);
		video::IHardwareIndexBuffer* idx = buffer->createIndexBuffer(video::IHardwareIndexBuffer::EIT_16Bit, 6, video::IHardwareBuffer::EUT_WriteOnly, false, false);

		math::vector3d posPtr[4] =
		{
			math::vector3d(-1, -1, 0),
			math::vector3d(+1, -1, 0),
			math::vector3d(+1, +1, 0),
			math::vector3d(-1, +1, 0)
		};
		math::vector2d tcPtr[4] = {
			math::vector2d(0, 0),
			math::vector2d(1, 0),
			math::vector2d(1, 1),
			math::vector2d(0, 1),
		};

		math::matrix3x3 rotMat;


		if (m_cameraConfiguration->cameraRotation[i] != TBee::TelubeeCameraConfiguration::None)
		{
			if (m_cameraConfiguration->cameraRotation[i] == TBee::TelubeeCameraConfiguration::CW)
				rotMat.setAngle(90);
			else if (m_cameraConfiguration->cameraRotation[i] == TBee::TelubeeCameraConfiguration::CCW)
				rotMat.setAngle(-90);
			else if (m_cameraConfiguration->cameraRotation[i] == TBee::TelubeeCameraConfiguration::Flipped)
				rotMat.setAngle(180);
			//     		math::Swap(tc.ULPoint.x, tc.ULPoint.y);
			//     		math::Swap(tc.BRPoint.x, tc.BRPoint.y);

		}
		/*
		if (i == 0)
		rotMat.setAngle(90);
		else rotMat.setAngle(-90);
		*/
		for (int j = 0; j < 4; ++j)
			tcPtr[j] = (rotMat*(tcPtr[j] * 2 - 1))*0.5 + 0.5f;
		ushort idxPtr[6] = { 0, 1, 2, 0, 2, 3 };

		pos->writeData(0, 4 * sizeof(math::vector3d), posPtr, true);
		tc->writeData(0, 4 * sizeof(math::vector2d), tcPtr, true);
		idx->writeData(0, 6 * sizeof(ushort), idxPtr, true);


		m_car->CameraPlane[i]->AttachNode(rnode);
		m_car->Eyes[i]->addChild(m_car->CameraPlane[i]);
		//m_headNode->addChild(m_screenNode[i]);
		m_car->CameraPlane[i]->setVisible(false);
		m_car->CameraPlane[i]->setScale(1);
		m_car->CameraPlane[i]->setCullingType(scene::SCT_NONE);
	}

	for (int i = 0; i < 2; ++i)
	{

		video::RenderMaterialPtr mtrl = new video::RenderMaterial();
		m_screenMtrl[i] = mtrl->CreateTechnique("Default")->CreatePass("ScreenPass");
		m_wireframePass[i] = mtrl->GetTechniqueByName("Default")->CreatePass("WireframePass");
		meshdata[i]->setMaterial(mtrl);

		m_screenMtrl[i]->setRenderState(video::RS_Lighting, video::ES_DontUse);
		m_screenMtrl[i]->setRenderState(video::RS_ZWrite, video::ES_DontUse);
		m_screenMtrl[i]->setRenderState(video::RS_ZTest, video::ES_DontUse);
		m_screenMtrl[i]->setRenderState(video::RS_CullFace, video::ES_DontUse);


		m_wireframePass[i]->setRenderState(video::RS_Wireframe, video::ES_Use);
		m_wireframePass[i]->setRenderState(video::RS_CullFace, video::ES_DontUse);
		m_wireframePass[i]->setRenderState(video::RS_Lighting, video::ES_DontUse);
		m_wireframePass[i]->setRenderState(video::RS_ZTest, video::ES_DontUse);
		m_wireframePass[i]->SetEnabled(false);

	}

 	m_car->CameraPlane[0]->setOrintation(math::quaternion(180, math::vector3d::ZAxis));
 	m_car->CameraPlane[1]->setOrintation(math::quaternion(180, math::vector3d::ZAxis));
//	m_car->ProjectionPlane->setScale(math::vector3d(m_displaySize.x, 1, m_displaySize.y) );
	_SetCameraPlaneDistance(cameraScreenDistance);

}

void CameraPlaneRenderer::_UpdateCameraPlaneScaler()
{
	m_surfaceParams.fovScaler = fabs(tan(math::toRad(m_cameraConfiguration->fov / 2.0f))*m_surfaceParams.distance);
}
void CameraPlaneRenderer::_SetCameraPlaneDistance(float d)
{
	m_surfaceParams.distance = d;
	m_car->CameraPlane[0]->setPosition(math::vector3d(0, 0, m_surfaceParams.distance));
	m_car->CameraPlane[1]->setPosition(math::vector3d(0, 0, m_surfaceParams.distance));

	_UpdateCameraPlaneScaler();
}
float CameraPlaneRenderer::CalcDisplayFoV(float headDistance)
{
	//calculate display field of view based on head position and physical screen width
	float halfW = m_displaySize.x*0.5f;
	float fov= math::toDeg(atan2(halfW, headDistance) * 2.0f);

	return fov;
}
void CameraPlaneRenderer::_UpdateCameraProj()
{
	math::vector3d projPlane;// = m_car->ProjectionPlane->getPosition();
	projPlane = m_car->Head->getPosition();
	projPlane.x += m_cameraOffsets.x;
	projPlane.y += m_cameraOffsets.y;
	projPlane.z += m_surfaceParams.distance;
	m_car->ProjectionPlane->setPosition(projPlane);
	m_car->ProjectionPlane->setScale(math::vector3d(m_car->Camera[0]->getAspect()*m_displaySize.x, m_displaySize.x, 1));

	for (int i = 0; i < 2; ++i)
	{
		m_offAxisProj[i].SetEyePosition(m_car->Camera[i]->getAbsolutePosition());
		math::vector3d sz= m_car->ProjectionPlane->getScale(); 
		m_offAxisProj[i].SetScreenParams(m_car->ProjectionPlane->getAbsolutePosition(), math::vector2d(sz.x,sz.y), m_car->ProjectionPlane->getAbsoluteOrintation());

		m_car->Camera[i]->setProjectionMatrix(m_offAxisProj[i].GetProjectionMatrix());
		m_car->Camera[i]->setWorldViewMatrix(m_offAxisProj[i].GetViewMatrix());
		m_car->Camera[i]->setFovY(m_offAxisProj[i].GetFoV());
 		m_car->Camera[i]->setOrintation(m_offAxisProj[i].GetRotation());
	}

}

void CameraPlaneRenderer::_UpdateHead(const math::vector3d& pos, const math::vector3d &angles)
{
	m_car->Head->setOrintation(angles);
	m_car->Head->setPosition(pos);
	_UpdateCameraProj();
}
void CameraPlaneRenderer::_UpdateCameraPlane()
{
	math::vector3d s (m_surfaceParams.fovScaler*m_surfaceParams.aspect, m_surfaceParams.fovScaler, 1);
	m_car->CameraPlane[0]->setScale(s);
	m_car->CameraPlane[1]->setScale(s);
}
void CameraPlaneRenderer::SetTransformation(const math::vector3d& pos, const math::vector3d &angles)
{
	_UpdateHead(pos, angles); 
	_UpdateCameraPlane();
}
void CameraPlaneRenderer::Update(float dt)
{
	InputManager* mngr = NCAppGlobals::Instance()->App->GetInputManager();
	controllers::IMouseController* m = mngr->getMouse();
	controllers::IKeyboardController* kb = mngr->getKeyboard();

	if (kb->isLCtrlPress())
	{
//		m_displayFov += m->getDZ()*dt*1;
// 		m_cameraConfiguration->fov += m->getDZ()*dt * 10;
// 		_UpdateCameraPlaneScaler();

		m_surfaceParams.distance += m->getDZ()*dt * 0.1f;

		//m_car->Camera[0]->setFovY(m_displayFov);
		//m_car->Camera[1]->setFovY(m_displayFov);
	}	
	
}

void CameraPlaneRenderer::PreRender(const math::rectf& rc, int index)
{

	m_videoSource->Blit();
	video::ITexture* camTex = m_videoSource->GetEyeTexture(index);;//gTextureResourceManager.loadTexture2D("Checker.png");// //
	video::ITexture* camTex1 = camTex;
	video::ShaderSemanticTable::getInstance().setRenderPass(0);

	float aspect;
	if (camTex->getSize().y!=0)
		aspect = (float)camTex->getSize().x / (float)camTex->getSize().y;
	else aspect = 1;
	if (m_useLensCorrection)
	{
		math::vector2d size(rc.getSize());
		video::ParsedShaderPP::MappedParams* texRect = m_lensCorrectionPP->GetParam("texRect");

		if (texRect)
		{
			math::rectf r = m_videoSource->GetEyeTexCoords(index);
			texRect->SetValue(math::vector4d(r.ULPoint.x, r.ULPoint.y, r.BRPoint.x, r.BRPoint.y));
		}
		if (m_camConfigDirty)
		{
			m_camConfigDirty = false;
			_UpdateCameraPlaneScaler();

			video::ParsedShaderPP::MappedParams* OpticalCenter = m_lensCorrectionPP->GetParam("OpticalCenter");
			video::ParsedShaderPP::MappedParams* FocalCoeff = m_lensCorrectionPP->GetParam("FCoff");
			video::ParsedShaderPP::MappedParams* KPCoeff = m_lensCorrectionPP->GetParam("KCoff");
			video::ParsedShaderPP::MappedParams* uvScalesP = m_lensCorrectionPP->GetParam("uvScales");


			if (OpticalCenter)
				OpticalCenter->SetValue(m_cameraConfiguration->OpticalCenter);
			if (FocalCoeff)
				FocalCoeff->SetValue(m_cameraConfiguration->FocalCoeff);
			if (KPCoeff)
				KPCoeff->SetValue(m_cameraConfiguration->KPCoeff);
		}


		m_lensCorrectionPP->Setup(math::rectf(0, size));
		m_lensCorrectionPP->render(&video::TextureRTWrap(camTex));
		camTex = m_lensCorrectionPP->getOutput()->GetColorTexture();
	}
	m_surfaceParams.aspect = aspect;
	m_car->CameraPlane[index]->setVisible(true);
	//m_screenNode[index]->setScale(scale);
	m_screenMtrl[index]->setTexture(camTex, 0);
}
void CameraPlaneRenderer::PostRender(int index)
{
	m_car->CameraPlane[index]->setVisible(false);

	if (NCAppGlobals::Instance()->IsDebugging)
	{
		video::TextureUnit tex;
		gEngine.getDevice()->set2DMode();
		//debug render camera views
		tex.SetTexture(m_screenMtrl[index]->getTexture(0));
		gEngine.getDevice()->useTexture(0, &tex);
		gEngine.getDevice()->draw2DImage(math::rectf(0, 0, 200, 200), 1);
	}
}
void CameraPlaneRenderer::DebugRender(GUI::IGUIRenderer* renderer)
{

	GUI::IFont* font = gFontResourceManager.getDefaultFont();
	GUI::FontAttributes attr;


	video::IVideoDevice* dev = Engine::getInstance().getDevice();
	dev->set2DMode();
	TBee::AppData* app = TBee::AppData::Instance();

	if (!TBee::AppData::Instance()->IsDebugging)
		return;
	math::rectf r(100,200,0,0);
	if (font)
	{
#define PRINT_LOG(msg)\
	attr.fontColor.Set(1,1,1,1); \
	font->print(r, &attr, 0, msg, renderer); \
	r.ULPoint.y += attr.fontSize;
#define PRINT_LOG_COLORED(msg,clr)\
	attr.fontColor = clr;\
	font->print(r, &attr, 0, msg, renderer); \
	r.ULPoint.y += attr.fontSize;

		core::string msg;

		attr.fontColor.Set(1, 1, 1, 1);
		attr.fontAligment = GUI::EFA_MiddleLeft;
		attr.fontSize = 18;
		attr.hasShadow = true;
		attr.shadowColor.Set(0, 0, 0, 1);
		attr.shadowOffset = math::vector2d(2);
		attr.spacing = 2;
		attr.wrap = 0;
		attr.RightToLeft = 0;

		msg = core::string("Distortion Correction: ") + core::string(m_useLensCorrection ? "Enabled" : "Disabled");
		PRINT_LOG_COLORED(msg,video::SColor(1,0.7,0.7,1));
		PRINT_LOG_COLORED(core::string("Sceen Parameters"), video::SColor(1, 0.7, 0.7, 1));
		PRINT_LOG("Screen Distance: " + core::StringConverter::toString(m_surfaceParams.distance));
		PRINT_LOG("Screen FoV: " + core::StringConverter::toString(m_offAxisProj[0].GetFoV()));


		PRINT_LOG(msg);

	}
	renderer->Flush();

}

void CameraPlaneRenderer::LoadFromXML(xml::XMLElement* e)
{

	xml::XMLAttribute* attr;


	xml::XMLElement* c = e->getSubElement("CameraSetup");
	if (c)
		m_videoSource->LoadFromXML(c);


	core::string camConfigName = e->getValueString("CameraConfiguration");

	m_useLensCorrection = e->getValueBool("UseLensCorrection");
	math::vector2d proj = core::StringConverter::toVector2d(e->getValueString("CameraProjection"));
	m_cameraOffsets.x = proj.x;
	m_cameraOffsets.y = proj.y;
	m_surfaceParams.distance = e->getValueFloat("ScreenDistance");

	m_displaySize = core::StringConverter::toVector2d(e->getValueString("DisplaySize"));

	m_cameraConfiguration = NCAppGlobals::Instance()->camConfig->GetCameraConfiguration(camConfigName);
	
	m_displayFov = e->getValueFloat("DisplayFoV");

	m_camConfigDirty = true;
}

}
}

