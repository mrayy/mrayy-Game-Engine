

#include "stdafx.h"
#include "IEyesRenderingBaseState.h"

#include "RemoteRobotCommunicator.h"
#include "AppData.h"
#include "OculusDevice.h"
#include "InputManager.h"
#include "FontResourceManager.h"
#include "JoystickDefinitions.h"
#include "CRobotConnector.h"

#include "GUIBatchRenderer.h"
#include "SMeshBuffer.h"
#include "RenderPass.h"

#include "OculusHeadController.h"
#include "OptiTrackHeadController.h"
#include "ICameraVideoSource.h"

#include "WiiboardInputController.h"
#include "JoystickInputController.h"
#include "KeyboardHeadController.h"
#include "NodeHeadController.h"
#include "AppData.h"
#include "CameraConfigurationManager.h"

namespace mray
{
namespace TBee
{

IEyesRenderingBaseState::IEyesRenderingBaseState(const core::string& name)
:IRenderingState(name)
{
	m_exitCode = 0;

	m_videoSource = 0;


	m_robotConnector = new CRobotConnector();

	m_lensCorrectionPP = new video::ParsedShaderPP(Engine::getInstance().getDevice());
	m_lensCorrectionPP->LoadXML(gFileSystem.openFile("LensCorrection.peff"));
// 
// 	m_correctionValue[0] = m_lensCorrectionPP->GetValue("final.HMDWrapParams1");
// 	m_correctionValue[1] = m_lensCorrectionPP->GetValue("final.HMDWrapParams2");

	switch (AppData::Instance()->headController)
	{
	case EHeadControllerType::Keyboard:
		m_robotConnector->SetHeadController(new KeyboardHeadController);
		break;;
	case EHeadControllerType::Oculus:
		m_robotConnector->SetHeadController(new OculusHeadController);
		break;;
	case EHeadControllerType::OptiTrack:
		m_robotConnector->SetHeadController(new OptiTrackHeadController(1));
		break;;
	case EHeadControllerType::SceneNode:
		m_robotConnector->SetHeadController(new NodeHeadController());
		break;;
	default:
		break;
	}

	switch (AppData::Instance()->robotController)
	{
	case ERobotControllerType::Keyboard:
		break;;
	case ERobotControllerType::Joystick:
		m_robotConnector->SetRobotController(new JoystickInputController);
		break;;
	case ERobotControllerType::Wiiboard:
		m_robotConnector->SetRobotController(new WiiboardInputController);
		break;;
	default:
		break;
	}

	GUI::GUIBatchRenderer*r = new GUI::GUIBatchRenderer();
	r->SetDevice(Engine::getInstance().getDevice());
	m_guiRenderer = r;

	m_panningScale = 1;
	m_headPan = 0;
	m_headTilt = 0;

	m_useLensCorrection = false;
	m_enablePanning = false;
	m_camConfigDirty = true;

}


IEyesRenderingBaseState::~IEyesRenderingBaseState()
{
	delete m_robotConnector;
	delete m_videoSource;
	delete m_guiRenderer;
}

void IEyesRenderingBaseState::_UpdateCameraParams()
{
	if (!m_videoSource)
		return;
	m_targetAspectRatio = m_hmdSize.x / m_hmdSize.y;
	for (int i = 0; i < 2; ++i)
	{
		math::vector2d framesize = m_videoSource->GetEyeResolution(i)*m_videoSource->GetEyeScalingFactor(i);

		if (m_cameraConfiguration->cameraRotation[i] == TelubeeCameraConfiguration::CW
			|| m_cameraConfiguration->cameraRotation[i] == TelubeeCameraConfiguration::CCW)
			math::Swap(framesize.x, framesize.y);
		if (framesize.y == 0)framesize.y = 1;
		float camRatio = framesize.x / framesize.y;
		m_eyes[i].cropping.set(framesize.x, framesize.x / m_targetAspectRatio);
		m_eyes[i].ratio = camRatio;

		float focal = 1;//in meter
		float w1 = 2 * focal*tan(math::toRad(m_hmdFov*0.5f));
		float w2 = 2 * (focal - m_cameraConfiguration->cameraOffset)*tan(math::toRad(m_cameraConfiguration->fov*0.5f));

		float ratio = w2 / w1;
		m_eyes[i].scale = m_hmdSize*ratio;
	}
}

void IEyesRenderingBaseState::SetHMDParameters(float targetAspectRatio,  float hmdFov)
{
	m_targetAspectRatio = targetAspectRatio;
	m_hmdFov = hmdFov;
	_UpdateCameraParams();
}

void IEyesRenderingBaseState::InitState()
{
	IRenderingState::InitState();
	if (m_videoSource)
		m_videoSource->Init();

}



bool IEyesRenderingBaseState::OnEvent(Event* e, const math::rectf& rc)
{
	bool ok = false;
	if (e->getType() == ET_Keyboard)
	{
		KeyboardEvent* evt = (KeyboardEvent*)e;
		/*
		if (evt->press && false) // image correction
		{
			if (evt->key == KEY_Y)
			{
				m_correctionParamsU.x += 0.01*(evt->shift ? -1 : 1);
			}
			else if (evt->key == KEY_U)
			{
				m_correctionParamsU.y += 0.01*(evt->shift ? -1 : 1);
			}
			else if (evt->key == KEY_I)
			{
				m_correctionParamsU.z += 0.01*(evt->shift ? -1 : 1);
			}
			else if (evt->key == KEY_H)
			{
				m_correctionParamsV.x += 0.01*(evt->shift ? -1 : 1);
			}
			else if (evt->key == KEY_J)
			{
				m_correctionParamsV.y += 0.01*(evt->shift ? -1 : 1);
			}
			else if (evt->key == KEY_K)
			{
				m_correctionParamsV.z += 0.01*(evt->shift ? -1 : 1);
			}
		}*/
	}
	
	if (e->getType() == ET_Joystick)
	{
		JoystickEvent* evt = (JoystickEvent*)e;
		if (evt->event == JET_BUTTON_PRESSED)
		{
			if (evt->button == JOYSTICK_SelectButton)
			{
				m_exitCode = STATE_EXIT_CODE;
				m_robotConnector->EndUpdate();
				ok = true;
			}
			else if (evt->button == JOYSTICK_StartButton)
			{
				if (m_robotConnector->IsRobotConnected())
					m_robotConnector->EndUpdate();
				else
					m_robotConnector->StartUpdate();
				ok = true;
			}
		}
	}
	return ok;
}


void IEyesRenderingBaseState::OnEnter(IRenderingState*prev)
{
	//VCameraType* cam=(VCameraType*)m_camera;//m_video->GetGrabber().pointer();

	_UpdateCameraParams();

	m_robotConnector->ConnectRobot();
	m_robotConnector->EndUpdate();
}


void IEyesRenderingBaseState::OnExit()
{
	IRenderingState::OnExit();
	//VCameraType* cam=(VCameraType*)m_video->GetGrabber().pointer();
	m_robotConnector->DisconnectRobot();

}

class TextureRenderTarget :public video::IRenderArea
{
protected:
	video::ITexturePtr m_tex;
public:
	TextureRenderTarget(video::ITexturePtr tex){ m_tex = tex; }
	virtual~TextureRenderTarget()
	{
	}
	virtual const video::ITexturePtr& GetColorTexture(int i = 0) { return m_tex; }
	virtual int GetColorTextureCount() { return 1; }
	virtual void Resize(int x, int y) {}
	virtual math::vector2di GetSize()
	{
		return math::vector2di(m_tex->getSize().x, m_tex->getSize().y);
	}

};
void
qtomatrix(math::matrix4x4& m,const math::quaternion& q)
/*
 * Convert quaterion to rotation sub-matrix of 'm'.
 * The left column of 'm' gets zeroed, and m[3][3]=1.0, but the 
 * translation part is left unmodified.
 * 
 * m = q
 */
{
#define X q.x
#define Y q.y
#define Z q.z
#define W q.w
    float    x2 = X * X;
    float    y2 = Y * Y;
    float    z2 = Z * Z;
    
    m[0][0] = 1 - 2 * (y2 +  z2);
    m[0][1] = 2 * (X * Y + W * Z);
    m[0][2] = 2 * (X * Z - W * Y);
    m[0][3] = 0.0;
    
    m[1][0] = 2 * (X * Y - W * Z);
    m[1][1] = 1 - 2 * (x2 + z2);
    m[1][2] = 2 * (Y * Z + W * X);
    m[1][3] = 0.0;
    
    m[2][0] = 2 * (X * Z + W * Y);
    m[2][1] = 2 * (Y * Z - W * X);
    m[2][2] = 1 - 2 * (x2 + y2);
    m[2][3] = 0.0;

    m[3][3] = 1.0;
}


void IEyesRenderingBaseState::_RenderUI(const math::rectf& rc, math::vector2d& pos)
{
	if (!AppData::Instance()->IsDebugging)
		return;
	GUI::IFont* font = gFontResourceManager.getDefaultFont();
	GUI::FontAttributes attr;
	video::IVideoDevice* dev = Engine::getInstance().getDevice();
	AppData* app = AppData::Instance();
	if (font)
	{
		attr.fontColor.Set(1, 1, 1, 1);
		attr.fontAligment = GUI::EFA_MiddleLeft;
		attr.fontSize = 18;
		attr.hasShadow = true;
		attr.shadowColor.Set(0, 0, 0, 1);
		attr.shadowOffset = math::vector2d(2);
		attr.spacing = 2;
		attr.wrap = 0;
		attr.RightToLeft = 0;

		math::rectf r = rc;

		if (m_robotConnector->GetHeadController())
		{
			math::vector3d head;
			math::quaternion q,q2(m_robotConnector->GetHeadRotation());
			q = q2;
			q.x = q2.z;
			q.y = q2.x;
			q.z = q2.y;
			q.toEulerAngles(head);
			/*
			math::matrix4x4 m;
			qtomatrix(m, q);

			char buff[512];
			sprintf(buff, "%0.2f, %0.2f, %0.2f, %0.2f",)


			core::string msg = mT(" ") + core::StringConverter::toString(head);
			font->print(r, &attr, 0, msg, m_guiRenderer);
			r.ULPoint.y += attr.fontSize + 5;
			*/

			core::string msg = mT("Head Rotation: ")+core::StringConverter::toString(head);
			font->print(r, &attr, 0, msg, m_guiRenderer);
			r.ULPoint.y += attr.fontSize + 5;

			head = m_robotConnector->GetHeadPosition();
			msg = mT("Head Position: ") + core::StringConverter::toString(head);
			font->print(r, &attr, 0, msg, m_guiRenderer);
			r.ULPoint.y += attr.fontSize + 5;
			
		}
		else
		{
			core::string msg = mT("Head: Non") ;
			font->print(r, &attr, 0, msg, m_guiRenderer);
			r.ULPoint.y += attr.fontSize + 5;
		}
		if (m_robotConnector->GetRobotController())
		{

			math::vector2d speed;
			float rot;
			speed = m_robotConnector->GetSpeed();
			rot = m_robotConnector->GetRotation();
			core::string msg = mT("Robot Speed: ") + core::StringConverter::toString(speed);
			font->print(r, &attr, 0, msg, m_guiRenderer);
			r.ULPoint.y += attr.fontSize + 5;
			msg = mT("Robot Rotation: ") + core::StringConverter::toString(rot);
			font->print(r, &attr, 0, msg, m_guiRenderer);
			r.ULPoint.y += attr.fontSize + 5;
		}
		pos = r.ULPoint;
		/*
		math::vector3d correctionX(m_correctionValue[0]->floatParam[0], m_correctionValue[0]->floatParam[1], m_correctionValue[0]->floatParam[2]);
		math::vector3d correctionY(m_correctionValue[1]->floatParam[0], m_correctionValue[1]->floatParam[1], m_correctionValue[1]->floatParam[2]);
		core::string msg = mT("Correction X:") + core::StringConverter::toString(correctionX);
		font->print(r, &attr, 0, msg, m_guiRenderer);
		r.ULPoint.y += attr.fontSize + 5;
		msg = mT("Correction Y:") + core::StringConverter::toString(correctionY);
		font->print(r, &attr, 0, msg, m_guiRenderer);
		r.ULPoint.y += attr.fontSize + 5;*/
		m_guiRenderer->Flush();
	}

}
video::IRenderTarget* IEyesRenderingBaseState::Render(const math::rectf& rc, ETargetEye eye)
{
	IRenderingState::Render(rc, eye);
	int index = GetEyeIndex(eye);
	if (!m_videoSource)
		return m_renderTarget[index].pointer();


	video::IVideoDevice* dev = Engine::getInstance().getDevice();

	video::TextureUnit tex;
	tex.SetEdgeColor(video::DefaultColors::Black);
	tex.setTextureClamp(video::ETW_WrapS, video::ETC_CLAMP_TO_BORDER);
	tex.setTextureClamp(video::ETW_WrapT, video::ETC_CLAMP_TO_BORDER);

	if (m_videoSource)
		m_videoSource->Blit(index);

	video::ITexturePtr cameraTex = m_videoSource->GetEyeTexture(index);

	if (m_useLensCorrection)
	{
		math::vector2d size(cameraTex->getSize().x, cameraTex->getSize().y);
		video::ParsedShaderPP::MappedParams* texRect = m_lensCorrectionPP->GetParam("texRect");

		if (texRect)
		{
			math::rectf r=m_videoSource->GetEyeTexCoords(index);
			texRect->SetValue(math::vector4d(r.ULPoint.x, r.ULPoint.y, r.BRPoint.x, r.BRPoint.y));
		}
		if (m_camConfigDirty)
		{
			video::ParsedShaderPP::MappedParams* OpticalCenter = m_lensCorrectionPP->GetParam("OpticalCenter");
			video::ParsedShaderPP::MappedParams* FocalCoeff = m_lensCorrectionPP->GetParam("FCoff");
			video::ParsedShaderPP::MappedParams* KPCoeff = m_lensCorrectionPP->GetParam("KCoff");

			if (OpticalCenter)
				OpticalCenter->SetValue(m_cameraConfiguration->OpticalCenter);
			if (FocalCoeff)
				FocalCoeff->SetValue(m_cameraConfiguration->FocalCoeff);
			if (KPCoeff)
				KPCoeff->SetValue(m_cameraConfiguration->KPCoeff);
		}

		m_lensCorrectionPP->Setup(math::rectf(0, size));
		m_lensCorrectionPP->render(&TextureRenderTarget(cameraTex));
		cameraTex = m_lensCorrectionPP->getOutput()->GetColorTexture();
	}
	dev->setRenderTarget(m_renderTarget[index],1,1,video::DefaultColors::Black);
	dev->set2DMode();

	//	gTextureResourceManager.writeResourceToDist(m_video->GetTexture(),"screens\\image#"+core::StringConverter::toString(s_id++)+".jp2");
	if (rc.getSize() != m_hmdSize)
	{
		m_hmdSize = rc.getSize();
		_UpdateCameraParams();
	}
	tex.SetTexture(cameraTex);
	//float croppingHeight=1-m_targetAspectRatio/m_cameraSource[index].ratio;
	math::vector2d frameRes = m_videoSource->GetEyeResolution(index)*m_videoSource->GetEyeScalingFactor(index);
	if (m_cameraConfiguration->cameraRotation[index]==TelubeeCameraConfiguration::CW
		|| m_cameraConfiguration->cameraRotation[index] == TelubeeCameraConfiguration::CCW)
		math::Swap(frameRes.x, frameRes.y);

	if (frameRes.y == 0)
		frameRes.y = 1;
	float targetHeight = frameRes.x / m_targetAspectRatio;
	float heightDiff = (targetHeight - frameRes.y);
	//float croppingHeight = 1-1.0f/(targetHeight / rc.getHeight());
	float croppingHeight = (heightDiff / frameRes.y);
	croppingHeight *= 0.5f;
	math::rectf tc;
	tc=math::rectf(0, -croppingHeight, 1, 1 + croppingHeight);
	math::rectf eyeTc = m_videoSource->GetEyeTexCoords(index);
	//now calculate the actual rendering rectangle
	math::rectf targetRect;
	math::vector2d margin = (rc.getSize() - m_eyes[index].scale) / 2;
	targetRect.ULPoint = rc.ULPoint + margin;
	targetRect.BRPoint = rc.BRPoint - margin;
	math::vector2d tcSz = tc.getSize();
	tc.ULPoint += eyeTc.ULPoint;
	tc.BRPoint = tc.ULPoint + tcSz*eyeTc.getSize();


	//tc.ULPoint=targetRect.ULPoint/rc.getSize();
	//tc.BRPoint=targetRect.BRPoint/rc.getSize();
	
	tc.ULPoint.y = 1 - tc.ULPoint.y;
	tc.BRPoint.y = 1 - tc.BRPoint.y;

// 	tc.ULPoint.y = 1 - tc.ULPoint.y;
// 	tc.BRPoint.y = 1 - tc.BRPoint.y;
	//tc = math::rectf(0, 0, 1, 1);
	if (m_enablePanning)
	{
		math::vector2d center = tc.getCenter();
		math::vector2d halfSz = tc.getSize()*0.5f;
		if (m_panningScale > 1)
		{
			halfSz = halfSz / m_panningScale;
			tc.ULPoint = center - halfSz;
			tc.BRPoint = center + halfSz;
		}
		halfSz.x = fabs(halfSz.x);
		halfSz.y = fabs(halfSz.y);
		math::vector2d panMargin = (eyeTc.getSize()*0.5 - halfSz );
		panMargin.x = math::Max<float>(panMargin.x, 0);
		panMargin.y = math::Max<float>(panMargin.y, 0);
		math::vector2d panning(panMargin.x*m_headPan * 2, panMargin.y*m_headTilt * 2);
		tc.ULPoint += panning;
		tc.BRPoint += panning;
	}

	dev->useTexture(0, &tex);

	//dev->draw2DImage(math::rectf(targetRect.ULPoint, targetRect.BRPoint), 1, 0, &tc);

	math::vector2d points[4]=
	{
		math::vector2d(targetRect.ULPoint.x, targetRect.ULPoint.y),
		math::vector2d(targetRect.BRPoint.x, targetRect.ULPoint.y),
		math::vector2d(targetRect.BRPoint.x, targetRect.BRPoint.y),
		math::vector2d(targetRect.ULPoint.x, targetRect.BRPoint.y)
	};

	math::vector2d coords[4] =
	{
		math::vector2d(tc.ULPoint.x, tc.ULPoint.y),
		math::vector2d(tc.BRPoint.x, tc.ULPoint.y),
		math::vector2d(tc.BRPoint.x, tc.BRPoint.y),
		math::vector2d(tc.ULPoint.x, tc.BRPoint.y)
	};
	if (m_cameraConfiguration->cameraRotation[index]!=TelubeeCameraConfiguration::None)
	{
		math::matrix3x3 rotMatrix;
		if (m_cameraConfiguration->cameraRotation[index] == TelubeeCameraConfiguration::CW)
			rotMatrix.setAngle(90);
		else if (m_cameraConfiguration->cameraRotation[index] == TelubeeCameraConfiguration::CCW)
			rotMatrix.setAngle(-90);
		else if (m_cameraConfiguration->cameraRotation[index] == TelubeeCameraConfiguration::Flipped)
			rotMatrix.setAngle(180);
//     		math::Swap(tc.ULPoint.x, tc.ULPoint.y);
//     		math::Swap(tc.BRPoint.x, tc.BRPoint.y);

		for (int i = 0; i < 4; ++i)
			coords[i] = rotMatrix*(coords[i] -tc.getCenter())+tc.getCenter();
		/*
		if (!m_eyes[index].cw)
		{
			coords[0].set(tc.ULPoint.x, tc.BRPoint.y);
			coords[1].set(tc.ULPoint.x, tc.ULPoint.y);
			coords[2].set(tc.BRPoint.x, tc.ULPoint.y);
			coords[3].set(tc.BRPoint.x, tc.BRPoint.y);
		}
		else
		{
			coords[0].set(tc.BRPoint.x, tc.ULPoint.y);
			coords[1].set(tc.BRPoint.x, tc.BRPoint.y);
			coords[2].set(tc.ULPoint.x, tc.BRPoint.y);
			coords[3].set(tc.ULPoint.x, tc.ULPoint.y);
		}*/
	}
	dev->draw2DShapeTextured(points, coords, 4, 1, 1);
	//draw UI

	math::vector2d pos = rc.ULPoint;
	_RenderUI(rc,pos);

	return m_renderTarget[index].pointer();
}

void IEyesRenderingBaseState::Update(float dt)
{
	m_robotConnector->UpdateStatus();

	if (m_enablePanning)
	{
		if (AppData::Instance()->oculusDevice)
		{
			math::quaternion ori;
			ori= AppData::Instance()->oculusDevice->GetOrientation();

			math::vector3d angles;
			ori.toEulerAngles(angles);

			float maxAngle = 20 * m_panningScale;
			angles.x = math::clamp(angles.x, -maxAngle, maxAngle);
			angles.y = math::clamp(angles.y, -maxAngle, maxAngle);
			m_headPan = angles.y / maxAngle;
			m_headTilt = -angles.x / maxAngle;

			// 		m_headPan = m_headPan*0.5 + 0.5;
			// 		m_headTilt = m_headTilt*0.5 + 0.5;
		}
	}
}



void IEyesRenderingBaseState::LoadFromXML(xml::XMLElement* e)
{
	IRenderingState::LoadFromXML(e);
	xml::XMLAttribute* attr;
	attr = e->getAttribute("HMDFov");
	if (attr)
	{
		m_hmdFov = core::StringConverter::toFloat(attr->value);
	}
	attr = e->getAttribute("HMDSize");
	if (attr)
	{
		m_hmdSize = core::StringConverter::toVector2d(attr->value);
	}

	attr = e->getAttribute("EnablePanning");
	if (attr)
	{
		m_enablePanning = core::StringConverter::toBool(attr->value);
	}
	attr = e->getAttribute("PanningScale");
	if (attr)
	{
		m_panningScale = core::StringConverter::toFloat(attr->value);
	}
	core::string camConfigName = e->getValueString("CameraConfiguration");

	m_useLensCorrection = e->getValueBool("UseLensCorrection");

	m_cameraConfiguration = AppData::Instance()->camConfig->GetCameraConfiguration(camConfigName);


	m_camConfigDirty = true;
	//m_robotConnector->LoadXML(e);
}


}
}


