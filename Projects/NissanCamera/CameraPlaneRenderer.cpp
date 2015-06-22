

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

	m_sphericalPlane = true;

	m_cameraOffsets.z = 1.5f;		//camera offset from user eye
//	m_cameraOffsets.z = 1.0f;		//camera offset from user eye
	m_cameraOffsets.x = -0.6;		// camera offset from center of screen
	m_cameraOffsets.y = -0.1;		// camera offset from center of screen

	m_distortionParams.distortionVal = 0.8;
	m_distortionParams.cylindricalRatio = 0.4;
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

void CameraPlaneRenderer::Init(scene::ISceneNode* headNode, scene::CameraNode* leftCam, scene::CameraNode* rightCam)
{
	m_headNode = headNode;
	m_camera[0] = leftCam;
	m_camera[1] = rightCam;

	{
//		((TBee::LocalCameraVideoSource*)m_videoSource)->SetCameraResolution(m_cameraResolution, m_cameraFPS);
		m_videoSource->Init();
	}

	m_lensCorrectionPP = new video::ParsedShaderPP(Engine::getInstance().getDevice());
	m_lensCorrectionPP->LoadXML(gFileSystem.openFile("NissanLensCorrection.peff"));

	GenerateSurface(!m_sphericalPlane, m_cameraConfiguration->fov, m_videoSource->GetEyeResolution(0).x / m_videoSource->GetEyeResolution(0).y, 20, m_cameraOffsets.z);

	{

		controllers::InputKeyMap& keys = NCAppGlobals::Instance()->keyMap;

		keys.RegisterKey((uint)ECameraPlaneRendererKeys::EnableCorrection, KEY_C, true, false, "Enable/Disable Camera Correction");
		keys.RegisterKey((uint)ECameraPlaneRendererKeys::ShowWireframe, KEY_W, true, false, "Show/Hide Projection Wireframe");
	}
	m_camera[0]->setFovY(m_displayFov);
	m_camera[1]->setFovY(m_displayFov);

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


void CameraPlaneRenderer::GenerateSurface(bool plane, float hfov, float aspectRatio, int segments, float cameraScreenDistance)
{
	float radius = cameraScreenDistance;
	m_surfaceParams.plane = plane;
	m_surfaceParams.hfov = hfov;
	m_surfaceParams.aspect = aspectRatio;
	m_surfaceParams.segments = segments;
	m_surfaceParams.radius = radius;
	m_surfaceParams.scale[0] = m_surfaceParams.scale[1] = 1;
	int vertCount = (segments + 1)*(segments + 1);
	int indCount = 6 * segments*segments;

	for (int i = 0; i < 2; ++i)
	{
		if (m_surface[i])
		{
			m_screenNode[i]->RemoveNode(m_surface[i]);
			m_surface[i] = 0;
		}
	}

	scene::ISceneManager* smngr= m_headNode->getSceneManager();
	scene::MeshBufferData* meshdata[2];
	if (plane)
	{
		//Create Screen Node
		for (int i = 0; i < 2; ++i)
		{
			m_screenNode[i] = smngr->createSceneNode("ScreenNode");
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
				math::vector2d(0, 1),
				math::vector2d(1, 1),
				math::vector2d(1, 0),
				math::vector2d(0, 0),
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


			m_screenNode[i]->AttachNode(rnode);
			m_headNode->addChild(m_screenNode[i]);
			//m_headNode->addChild(m_screenNode[i]);
			m_screenNode[i]->setPosition(math::vector3d(0, 0, cameraScreenDistance));
			m_screenNode[i]->setVisible(false);
			m_screenNode[i]->setScale(1);
			m_screenNode[i]->setCullingType(scene::SCT_NONE);
		}

	}
	else
	{
		float vfov = hfov / aspectRatio;
		float w = 2 * math::PI32*radius*hfov / 360.0f;
		float h = 2 * math::PI32*radius*vfov / 360.0f;
		//Create Screen Node
		for (int i = 0; i < 2; ++i)
		{
			m_screenNode[i] = smngr->createSceneNode("ScreenNode");
			scene::MeshRenderableNode* rnode = new scene::MeshRenderableNode(new scene::SMesh());

			scene::MeshBufferData* bdata = rnode->getMesh()->addNewBuffer();
			scene::IMeshBuffer* buffer = bdata->getMeshBuffer();
			meshdata[i] = bdata;

			m_surface[i] = rnode;

			video::IHardwareStreamBuffer* pos = buffer->createStream(0, video::EMST_Position, video::ESDT_Point3f, vertCount, video::IHardwareBuffer::EUT_WriteOnly, false, false);
			video::IHardwareStreamBuffer* tc = buffer->createStream(0, video::EMST_Texcoord, video::ESDT_Point2f, vertCount, video::IHardwareBuffer::EUT_WriteOnly, false, false);
			video::IHardwareIndexBuffer* idx = buffer->createIndexBuffer(video::IHardwareIndexBuffer::EIT_16Bit, indCount, video::IHardwareBuffer::EUT_WriteOnly, false, false);

			math::vector3d* posPtr = (math::vector3d*) pos->lock(0, vertCount, video::IHardwareBuffer::ELO_NoOverwrite);
			math::vector2d* uvPtr = (math::vector2d*)tc->lock(0, vertCount, video::IHardwareBuffer::ELO_NoOverwrite);
			ushort* indPtr = (ushort*)idx->lock(0, indCount, video::IHardwareBuffer::ELO_NoOverwrite);


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
			for (int x = 0; x <= segments; ++x)
			{
				float u = ((float)x) / (float)segments;;
				float a1 = hfov*(x - segments / 2.0f) / (float)segments;
				for (int y = 0; y <= segments; ++y)
				{
					float v = ((float)y) / (float)segments;;
					float a2 = vfov*(y - segments / 2.0f) / (float)segments;
					float xp = radius*math::sind(a1) *math::sind(a2);
					float yp = radius* radius*math::cosd(a2); //((float)y - segments / 2.0f) / (float)segments;;;//
					float zp = radius*math::cosd(a1) *math::sind(a2);



					(*posPtr++).set(xp, yp, zp);
					(*uvPtr).set(u, v);
					*uvPtr = (rotMat*(*uvPtr * 2 - 1))*0.5 - 0.5f;

					uvPtr++;

				}
			}

			for (int x = 0; x < segments; ++x)
			{
				for (int y = 0; y < segments; ++y)
				{
					*indPtr++ = y*(segments + 1) + x;
					*indPtr++ = y*(segments + 1) + x + 1;
					*indPtr++ = (y + 1)*(segments + 1) + (x + 1);

					*indPtr++ = y*(segments + 1) + x;
					*indPtr++ = (y + 1)*(segments + 1) + (x + 1);
					*indPtr++ = (y + 1)*(segments + 1) + x;
				}
			}

			pos->unlock();
			tc->unlock();
			idx->unlock();

			rnode->SetHasCustomRenderGroup(true);
			rnode->SetTargetRenderGroup(scene::RGH_Solid - 5);

			m_screenNode[i]->AttachNode(rnode);
			m_headNode->addChild(m_screenNode[i]);
			//m_screenNode[i]->setPosition(math::vector3d(0, 0, cameraScreenDistance));
			m_screenNode[i]->setVisible(false);
		}
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

	m_screenNode[0]->setOrintation(math::quaternion(180, math::vector3d::ZAxis));
	m_screenNode[1]->setOrintation(math::quaternion(180, math::vector3d::ZAxis));


}

void CameraPlaneRenderer::RescaleMesh(int index, const math::vector3d &scaleFactor)
{
	m_surfaceParams.scale[index] = scaleFactor;
	if (m_surfaceParams.plane)
	{
		//m_screenNode[index]->setScale(scaleFactor);
		//m_scaleFactor = scaleFactor;
	}
	else
	{
		m_surfaceParams.aspect = m_surfaceParams.scale[0].x / m_surfaceParams.scale[0].y;

		float radius = m_surfaceParams.radius;
		float hfov = m_surfaceParams.hfov;// *m_surfaceParams.scale[0].x;
		float vfov = m_surfaceParams.hfov/m_surfaceParams.aspect;// *m_surfaceParams.scale[1].y;
		int segments = m_surfaceParams.segments;
		int vertCount = (segments + 1)*(segments + 1);
		int indCount = 6 * segments*segments;

		printf("VFoV=%f\n", vfov);

		float w = 2 * math::PI32*radius*hfov / 360.0f;
		float h = 2 * math::PI32*radius*vfov / 360.0f;
		//Create Screen Node
		for (int i = 0; i < 2; ++i)
		{


			scene::IMeshBuffer* buffer = m_surface[i]->getMesh()->getBuffer(0);

			video::IHardwareStreamBuffer* pos = buffer->getStream(0, video::EMST_Position);

			math::vector3d* posPtr = (math::vector3d*) pos->lock(0, vertCount, video::IHardwareBuffer::ELO_NoOverwrite);
			for (int x = 0; x <= segments; ++x)
			{
				float a1 = hfov*(x - (segments + 1) / 2.0f) / (float)(segments + 1);
				for (int y = 0; y <= segments; ++y)
				{
					float a2 = vfov*(y - (segments + 1) / 2.0f) / (float)(segments + 1) + 90;
					float xp = radius*math::sind(a1)*math::sind(a2);
					float yp = radius*math::cosd(a2);
					float zp = radius*math::cosd(a1)*math::sind(a2);

					(*posPtr++).set(xp, yp, zp);

				}
			}
			pos->unlock();
		}
	}
}

float CameraPlaneRenderer::CalcDisplayFoV(float headDistance)
{
	//calculate display field of view based on head position and physical screen width
	float halfW = m_displayWidth*0.5f;
	float fov= math::toDeg(atan2(halfW, headDistance) * 2.0f);
	//m_cameraOffsets.z = headDistance;
// 	m_camera[0]->setFovY(m_displayFov);
// 	m_camera[1]->setFovY(m_displayFov);

	return fov;
}
void CameraPlaneRenderer::_UpdateCameraProj(const math::vector3d& eyePos)
{
	m_offAxisProj[0].SetEyePosition(eyePos);

	//m_camera[0]->setProjectionMatrix(m_offAxisProj[0].GetProjectionMatrix());
	//m_camera[0]->setWorldViewMatrix(m_offAxisProj[0].GetViewMatrix());
}

void CameraPlaneRenderer::SetTransformation(const math::vector3d& pos, const math::vector3d &angles)
{
	math::vector3d p;
	//float fov=CalcDisplayFoV(pos.z + m_screenDistance);
	//_UpdateCameraProj(pos);
	//m_camera[0]->setFovY(fov);
	//m_camera[1]->setFovY(fov);

	for (int i = 0; i < 2; ++i)
	{
	//	m_camera[i]->setPosition(math::vector3d(-m_cameraOffsets.x, -m_cameraOffsets.y, 0));
		//m_camera[i]->setOrie(math::vector3d(-m_cameraOffsets.x, -m_cameraOffsets.y, 0));
	}
	if (m_surfaceParams.plane)
	{
		if (true)
		{
			//math::quaternion q(angles.x, angles.y, angles.z);
			math::quaternion q(0, 0, -angles.z);
			p.z = m_cameraOffsets.z;
			p.x = m_cameraOffsets.x + math::sind(-angles.y)*m_cameraOffsets.z;
			p.y = m_cameraOffsets.y + math::sind(angles.x)*m_cameraOffsets.z;
			//	m_headNode->setPosition(pos);

			math::vector3d scale = 1;
			//	scale.x = math::cosd(abs(angles.y));
			//	scale.y = math::cosd(abs(angles.x));
			scale.z = 1;
			for (int i = 0; i < 2; ++i)
			{
				m_screenNode[i]->setPosition(p);
				m_screenNode[i]->setOrintation(q);
				m_screenNode[i]->setScale(scale*m_surfaceParams.scale[i]);
			}

			// 		m_screenNode[0]->setScale(scale);
			// 		m_screenNode[1]->setScale(scale);

		}
		else
		{
			//math::quaternion q(angles.x, angles.y, angles.z);
			math::quaternion q(0, 0, angles.z);
			p.z = m_cameraOffsets.z;//
			p.x =  math::sind(angles.y)*m_cameraOffsets.z;
			p.y =  math::sind(angles.x)*m_cameraOffsets.z;
			//	m_headNode->setPosition(pos);
			math::vector3d scale = 1;
// 			scale.x = math::cosd(abs(angles.y));
// 			scale.y = math::cosd(abs(angles.x));
			for (int i = 0; i < 2; ++i)
			{
				m_screenNode[i]->setPosition(p);
				m_screenNode[i]->setOrintation(q);
				m_screenNode[i]->setScale(scale*m_surfaceParams.scale[i]);
			}

			// 		m_screenNode[0]->updateAbsoluteTransformation();
			// 		m_screenNode[1]->updateAbsoluteTransformation();

		}
	}
	else
	{
		math::quaternion q(-angles.x,-angles.y, -angles.z);
		p = m_cameraOffsets;
		p.z = 0;
		//	m_headNode->setPosition(pos);
		math::vector3d scale = 1;
		for (int i = 0; i < 2; ++i)
		{
			m_screenNode[i]->setPosition(p);
			m_screenNode[i]->setOrintation(q);
			m_screenNode[i]->setScale(1);// scale*m_surfaceParams.scale[i]);
		}

	}

#if 0
	else
	{
		p.z = m_cameraOffsets.z;
		p.x = m_cameraOffsets.x + math::sind(angles.y)*m_cameraOffsets.z;
		p.y = m_cameraOffsets.y + math::sind(-angles.x)*m_cameraOffsets.z;

		p = m_camera[0]->getAbsolutePosition() + m_camera[0]->getAbsoluteOrintation()*p;

		math::vector3d proj1 = m_camera[0]->WorldToScreen(p);

		SetContentsPosition(proj1.x, proj1.y);
		SetContentsRotation(-angles.z);
	}
#endif
}
void CameraPlaneRenderer::Update(float dt)
{
	InputManager* mngr = NCAppGlobals::Instance()->App->GetInputManager();
	controllers::IMouseController* m = mngr->getMouse();
	controllers::IKeyboardController* kb = mngr->getKeyboard();

	if (kb->isLCtrlPress())
	{
		m_displayFov += m->getDZ()*dt*10;
		m_camera[0]->setFovY(m_displayFov);
		m_camera[1]->setFovY(m_displayFov);
	}	
	
	if (kb->isRShiftPress())
	{
		m_distortionParams.distortionVal = math::clamp<float>(m_distortionParams.distortionVal + (kb->getKeyState(KEY_LEFT) - kb->getKeyState(KEY_RIGHT))*dt, 0, 1.0f);
		m_distortionParams.cylindricalRatio = math::clamp<float>(m_distortionParams.cylindricalRatio + (kb->getKeyState(KEY_UP) - kb->getKeyState(KEY_DOWN))*dt, 0, 1.0f);
	}
}

void CameraPlaneRenderer::PreRender(const math::rectf& rc, int index)
{

#if 1
	m_videoSource->Blit();
	video::ITexture* camTex = m_videoSource->GetEyeTexture(index);;//gTextureResourceManager.loadTexture2D("Checker.png");// //
	video::ITexture* camTex1 = camTex;
	video::ShaderSemanticTable::getInstance().setRenderPass(0);

	math::vector3d scale;
	if (camTex->getSize().y!=0)
		scale.x = (float)camTex->getSize().x / (float)camTex->getSize().y;
	else scale.x = 1;
	scale.y = 1;
	scale.z = 1;
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
			video::ParsedShaderPP::MappedParams* OpticalCenter = m_lensCorrectionPP->GetParam("OpticalCenter");
			video::ParsedShaderPP::MappedParams* FocalCoeff = m_lensCorrectionPP->GetParam("FCoff");
			video::ParsedShaderPP::MappedParams* KPCoeff = m_lensCorrectionPP->GetParam("KCoff");
			video::ParsedShaderPP::MappedParams* uvScalesP = m_lensCorrectionPP->GetParam("uvScales");

			float fovDegrees=m_camera[0]->getFovY();
			float aspect = scale.x;

			LensDistortion::AspectRatio aspectRatio(aspect);
			LensDistortion::AspectRatio cylindricalAspectRatio(aspect * m_distortionParams.cylindricalRatio);

			float tan = LensDistortion::fovDegreesToFOVTan(fovDegrees);
			float diagonalFOVTan;

			{
				// Below, pointOnNonNormalizedRectangleToDiagonalFOVTan() is used to calculate the diagonal fov required to get exactly the horizontal fov 
				// at a given horizontal line on the screen for which y=1/3 (where y = 0 is mid-screen and y = 1 is at the top of the screen). 
				// In the article, this procedure is called 'pinning the horizontal fov'.
				float yAtWhichFOVIsSpecified = m_distortionParams.yOffset;// 0.333f;
				float horizontalFOVTan = LensDistortion::fovDegreesToFOVTan(fovDegrees);
				diagonalFOVTan = LensDistortion::pointOnNonNormalizedRectangleToDiagonalFOVTan(m_distortionParams.distortionVal, cylindricalAspectRatio,
					horizontalFOVTan, yAtWhichFOVIsSpecified * cylindricalAspectRatio.horizontalToVerticalTan(horizontalFOVTan));

				if (diagonalFOVTan < 0)
				{
					diagonalFOVTan = 100.0f;
				}
			}
			math::vector4d uvScales;
			LensDistortion::NormalizedDistortion distortion(aspectRatio, m_distortionParams.cylindricalRatio, m_distortionParams.distortionVal, diagonalFOVTan);

			uvScales.x = 2.0f * distortion.getCylindricalAspectRatio().getRatio() * sqrt(distortion.getNormCoeff());
			uvScales.y = 2.0f * sqrt(distortion.getNormCoeff());
			uvScales.z = distortion.getZoomCoeff();
			uvScales.w = 0.5f * distortion.getZoomCoeff() - 0.5f;

			if (OpticalCenter)
				OpticalCenter->SetValue(m_cameraConfiguration->OpticalCenter);
			if (FocalCoeff)
				FocalCoeff->SetValue(m_cameraConfiguration->FocalCoeff);
			if (KPCoeff)
				KPCoeff->SetValue(m_cameraConfiguration->KPCoeff);
			if (uvScalesP)
				uvScalesP->SetValue(uvScales);
		}


		m_lensCorrectionPP->Setup(math::rectf(0, size));
		m_lensCorrectionPP->render(&video::TextureRTWrap(camTex));
		camTex = m_lensCorrectionPP->getOutput()->GetColorTexture();
	}
		//	scale.x *= m_cameraConfiguration->fov / m_displayFov;
//	scale.y *= m_cameraConfiguration->fov / m_displayFov;
	if (scale != m_surfaceParams.scale[index])
	{
		RescaleMesh(index, scale);
	}
#endif
	m_screenNode[index]->setVisible(true);
	//m_screenNode[index]->setScale(scale);
	m_screenMtrl[index]->setTexture(camTex, 0);
}
void CameraPlaneRenderer::PostRender(int index)
{
	m_screenNode[index]->setVisible(false);

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
		if (m_useLensCorrection)
		{
			PRINT_LOG("Distortion Strength : " + core::StringConverter::toString(m_distortionParams.distortionVal));
			PRINT_LOG("Cylindrical Ratio   : " + core::StringConverter::toString(m_distortionParams.cylindricalRatio));
		}
		PRINT_LOG_COLORED(core::string("Sceen Parameters"), video::SColor(1, 0.7, 0.7, 1));
		PRINT_LOG("Screen Distance: " + core::StringConverter::toString(m_cameraOffsets.z));
		PRINT_LOG("Screen FoV: " + core::StringConverter::toString(m_offAxisProj[0].GetFoV()));


		msg = ("Plane FoV: " + core::StringConverter::toString(m_surfaceParams.hfov) + " / " + core::StringConverter::toString(m_surfaceParams.hfov / m_surfaceParams.aspect));
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
	m_screenDistance = e->getValueFloat("ScreenDistance");
	m_sphericalPlane = e->getValueBool("SphericalPlane");

	m_displayWidth=e->getValueFloat("DisplayWidth");

	m_cameraConfiguration = NCAppGlobals::Instance()->camConfig->GetCameraConfiguration(camConfigName);
	
	m_displayFov = e->getValueFloat("DisplayFoV");
	math::vector3d params = core::StringConverter::toVector3d(e->getValueString("DistortionParams"));
	m_distortionParams.distortionVal = params.x;
	m_distortionParams.cylindricalRatio = params.y;
	m_distortionParams.yOffset = params.z;
	m_cameraOffsets.z = m_screenDistance;

	m_camConfigDirty = true;
}

}
}

