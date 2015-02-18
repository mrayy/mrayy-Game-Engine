

#include "stdafx.h"
#include "CameraCorrectionGrabber.h"

#include "VTAppGlobals.h"
#include "OculusCameraComponent.h"
#include "OculusDevice.h"

namespace mray
{
	

CameraCorrectionGrabber::CameraCorrectionGrabber(int idx)
{
		m_index = idx;
	m_rotation = ERotation::None;
	m_distance = 0;
	m_fov = 90;
	m_target.set(640, 800);
	m_source = new video::CameraTextureSource();


	m_lensCorrectionPP = new video::ParsedShaderPP(Engine::getInstance().getDevice());
	m_lensCorrectionPP->LoadXML(gFileSystem.openFile("LensCorrection.peff"));

	m_correctionValue[0] = m_lensCorrectionPP->GetValue("final.HMDWrapParams1");
	m_correctionValue[1] = m_lensCorrectionPP->GetValue("final.HMDWrapParams2");
}
CameraCorrectionGrabber::~CameraCorrectionGrabber()
{
}
bool CameraCorrectionGrabber::_Load()
{
	if (!m_source->Load())
		return 0;
	if (m_texture.isNull())
	{
		m_texture = Engine::getInstance().getDevice()->createEmptyTexture2D(false);
		m_texture->setMipmapsFilter(false);
		m_rt = Engine::getInstance().getDevice()->createRenderTarget("", m_texture, 0, 0, 0);
	}


	return true;
}

void CameraCorrectionGrabber::_UpdateParams()
{
	m_targetAspectRatio = m_target.x / m_target.y;

	math::vector2d framesize(m_source->GetTexture()->getSize().x, m_source->GetTexture()->getSize().y);
	if (m_rotation!=ERotation::None)
		math::Swap(framesize.x, framesize.y);
	float camRatio = framesize.x / framesize.y;
	m_cropping.set(framesize.x, framesize.x / m_targetAspectRatio);
	m_ratio = camRatio;

	float focal = 1;//in meter
	float w1 = 2 * focal*tan(math::toRad(m_targetFov*0.5f));
	float w2 = 2 * (focal - m_distance)*tan(math::toRad(m_fov*0.5f));

	float ratio = w2 / w1;
	m_scale = m_target*ratio;

}

void CameraCorrectionGrabber::SetTargetRect(const math::vector2d& target)
{
	if (m_target.x != m_texture->getSize().x || m_target.y != m_texture->getSize().y)
	{
		m_texture->createTexture(math::vector3d(m_target.x, m_target.y, 1), m_texture->getImageFormat());
		_UpdateParams();
	}

}
bool CameraCorrectionGrabber::_Unload()
{
	return m_source->Unload();
}

const core::string& CameraCorrectionGrabber::GetSourceType()
{
	return "CameraCorrection";
}

class TextureRenderTarget :public video::IRenderTarget
{
protected:
	video::ITexturePtr m_tex;
public:
	TextureRenderTarget(video::ITexturePtr tex){ m_tex = tex; }
	virtual~TextureRenderTarget()
	{
	}

	virtual void clear(const video::SColor&c, bool clearBackbuffer, bool clearDepthBuffer)
	{
	}

	virtual void bind() {}
	virtual void unbind() {}

	virtual void attachRenderTarget(const video::ITexturePtr& tex, uint index = 0) {}
	virtual void deattachRenderTarget(const video::ITexturePtr& tex, uint index = 0) {}

	virtual const video::ITexturePtr& GetColorTexture(int i = 0) { return m_tex; }
	virtual const video::IHardwarePixelBufferPtr& GetDepthBuffer() { return video::IHardwarePixelBufferPtr::Null; }
	virtual const video::IHardwarePixelBufferPtr& GetStencilBuffer() { return video::IHardwarePixelBufferPtr::Null; }

	virtual int GetColorTextureCount() { return 1; }
	virtual void Resize(int x, int y) {}
	virtual math::vector2di GetSize()
	{
		return math::vector2di(m_tex->getSize().x, m_tex->getSize().y);
	}

};
void CameraCorrectionGrabber::Update(float dt)
{
	m_source->Update(dt);

	video::IVideoDevice* dev = Engine::getInstance().getDevice();

	video::ITexturePtr cameraTex = m_source->GetTexture();
	{

		math::vector2d size(cameraTex->getSize().x, cameraTex->getSize().y);
		m_lensCorrectionPP->Setup(math::rectf(0, size));
		m_lensCorrectionPP->render(&TextureRenderTarget(cameraTex));
		cameraTex = m_lensCorrectionPP->getOutput()->GetColorTexture();
	}
	dev->setRenderTarget(m_rt,1,1,video::SColor(0,0,0,1));
	dev->set2DMode();

	if (VT::VTAppGlobals::Instance()->oculusDevice->IsConnected())
	{
		math::matrix4x4 m, pm;
		dev->getTransformationState(video::TS_PROJECTION, m);
		pm.f14 = VT::VTAppGlobals::Instance()->oculusComponents[m_index]->GetPerspectiveOffset();
		pm = pm*m;
		dev->setTransformationState(video::TS_PROJECTION, pm);
	}

	//	gTextureResourceManager.writeResourceToDist(m_video->GetTexture(),"screens\\image#"+core::StringConverter::toString(s_id++)+".jp2");
	/*if (rc.getSize() != m_hmdSize)
	{
		m_hmdSize = rc.getSize();
		_UpdateCameraParams();
		}*/
	_UpdateParams();
	video::TextureUnit tex;
	tex.SetEdgeColor(video::DefaultColors::Black);
	tex.setTextureClamp(video::ETW_WrapS, video::ETC_CLAMP_TO_BORDER);
	tex.setTextureClamp(video::ETW_WrapT, video::ETC_CLAMP_TO_BORDER);

	tex.SetTexture(cameraTex);
	//float croppingHeight=1-m_targetAspectRatio/m_cameraSource[index].ratio;
	math::vector2d frameRes(m_source->GetTexture()->getSize().x, m_source->GetTexture()->getSize().y);
	if (m_rotation!=ERotation::None)
		math::Swap(frameRes.x, frameRes.y);
	float targetHeight = frameRes.x / m_targetAspectRatio;
	float heightDiff = (targetHeight - frameRes.y);
	//float croppingHeight = 1-1.0f/(targetHeight / rc.getHeight());
	float croppingHeight = (heightDiff / frameRes.y);
	croppingHeight *= 0.5f;
	math::rectf tc;
	tc = math::rectf(0, -croppingHeight, 1, 1 + croppingHeight);
	math::rectf eyeTc(0,0,1,1);
	//now calculate the actual rendering rectangle
	math::rectf targetRect;
	math::vector2d margin = (m_target- m_scale) / 2;
	targetRect.ULPoint =  margin;
	targetRect.BRPoint = m_rt->GetSize() - margin;
	math::vector2d tcSz = tc.getSize();
	tc.ULPoint += eyeTc.ULPoint;
	tc.BRPoint = tc.ULPoint + tcSz*eyeTc.getSize();


	//tc.ULPoint=targetRect.ULPoint/rc.getSize();
	//tc.BRPoint=targetRect.BRPoint/rc.getSize();
	tc.ULPoint.y = 1 - tc.ULPoint.y;
	tc.BRPoint.y = 1 - tc.BRPoint.y;


	dev->useTexture(0, &tex);

	math::vector2d points[4] =
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
	if (m_rotation!=ERotation::None)
	{
 		math::Swap(tc.ULPoint.x, tc.ULPoint.y);
 		math::Swap(tc.BRPoint.x, tc.BRPoint.y);
		if (m_rotation==ERotation::CW)
		{
			coords[0].set(tc.BRPoint.x, tc.BRPoint.y);
			coords[1].set(tc.BRPoint.x, tc.ULPoint.y);
			coords[2].set(tc.ULPoint.x, tc.ULPoint.y);
			coords[3].set(tc.ULPoint.x, tc.BRPoint.y);
		}
		else
		{
			coords[0].set(tc.ULPoint.x, tc.ULPoint.y);
			coords[1].set(tc.ULPoint.x, tc.BRPoint.y);
			coords[2].set(tc.BRPoint.x, tc.BRPoint.y);
			coords[3].set(tc.BRPoint.x, tc.ULPoint.y);
		}

	}
	dev->draw2DShapeTextured(points, coords, 4, 1, 1);
	//	dev->draw2DImage(math::rectf(targetRect.ULPoint, targetRect.BRPoint), 1, 0, &tc);

	dev->setRenderTarget(0);
}
video::ITextureCRef CameraCorrectionGrabber::GetTexture()
{
	return m_texture;
}
xml::XMLElement* CameraCorrectionGrabber::LoadFromXML(xml::XMLElement* e)
{
	m_source->LoadFromXML(e);
	m_distance = e->getValueFloat("Distance");
	m_fov = e->getValueFloat("FOV");
	m_targetFov = e->getValueFloat("TargetFOV");
	core::string rotation= e->getValueString("Rotation");
	rotation.make_lower();
	if (rotation == "none")m_rotation = ERotation::None;
	if (rotation == "cw")m_rotation = ERotation::CW;
	if (rotation == "ccw")m_rotation = ERotation::CCW;
	return e;
}


}
