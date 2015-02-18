
#include "stdafx.h"
#include "LeapMotionImageRetrival.h"
#include "TextureRTWrap.h"
#include "IThreadManager.h"


namespace mray
{

LeapMotionImageRetrival::LeapMotionImageRetrival(int index) :m_index(index), undistortShader(0)
{

	distortionXTex = gEngine.getDevice()->createEmptyTexture2D(false);
	distortionXTex->setMipmapsFilter(false);
	distortionYTex = gEngine.getDevice()->createEmptyTexture2D(false);
	distortionYTex->setMipmapsFilter(false);
	mainTexture = gEngine.getDevice()->createEmptyTexture2D(false);
	mainTexture->setMipmapsFilter(false);
	mainTexture->setAnisotropicFilter(true);
	mainTexture->setBilinearFilter(true);
	//_SetMainTex(DEFAULT_TEXTURE_WIDTH, DEFAULT_TEXTURE_HEIGHT);
	//_SetDistortionTex(DEFAULT_DISTORTION_WIDTH, DEFAULT_DISTORTION_HEIGHT);

	mainTU.SetTexture(mainTexture);
	disXTU.SetTexture(distortionXTex);
	disYTU.SetTexture(distortionYTex);

	mainTU.setTextureClamp(video::ETW_WrapS, video::ETC_CLAMP);
	mainTU.setTextureClamp(video::ETW_WrapT, video::ETC_CLAMP);

	disXTU.setTextureClamp(video::ETW_WrapS, video::ETC_CLAMP);
	disXTU.setTextureClamp(video::ETW_WrapT, video::ETC_CLAMP);

	disYTU.setTextureClamp(video::ETW_WrapS, video::ETC_CLAMP);
	disYTU.setTextureClamp(video::ETW_WrapT, video::ETC_CLAMP);

	video::ParsedShaderPP* pp = new video::ParsedShaderPP(gEngine.getDevice());
	pp->LoadXML(gFileSystem.openFile("leapUndistort.peff"));
	undistortShader = pp;

	m_bufferID = 0;
	m_hasNewFrame = false;
	m_frameSize.set(640, 240);

	m_mutex = OS::IThreadManager::getInstance().createMutex();
}
LeapMotionImageRetrival::~LeapMotionImageRetrival()
{
	delete undistortShader;
	m_mutex = 0;
}



void LeapMotionImageRetrival::_SetMainTex(int width, int height)
{
	mainTexture->createTexture(math::vector3di(width, height, 1), video::EPixel_Alpha8);
	m_frameSize.set(width, height);
}

void LeapMotionImageRetrival::_SetDistortionTex(int width, int height)
{
	distortionXTex->createTexture(math::vector3di(width, height, 1), video::EPixel_R8G8B8A8);

	distortionYTex->createTexture(math::vector3di(width, height, 1), video::EPixel_R8G8B8A8);
}

void  LeapMotionImageRetrival::_LoadMainTexture(const uchar* data)
{
	//video::IHardwarePixelBuffer* buff = mainTexture->getSurface(0);
	//video::LockedPixelBox bb(buff->getWidth(), buff->getHeight(), 1, video::EPixel_Alpha8, (uchar*)data);
	m_imageInfo.setData(data, mainTexture->getSize(), video::EPixel_Alpha8);
	//buff->blitFromMemory(bb);
	const video::ImageInfo *ifo[] = { &m_imageInfo };
	mainTexture->loadSurfaces(ifo, 1);
}

void LeapMotionImageRetrival::_EncodeDisortion(Leap::Image &image)
{
	int numVals = image.distortionWidth()*image.distortionHeight();
	const float* dist = image.distortion();

	video::IHardwarePixelBuffer* dx = distortionXTex->getSurface(0);
	video::IHardwarePixelBuffer* dy = distortionYTex->getSurface(0);

	video::LockedPixelBox dxBB = dx->lock(math::box3d(0, 0, 0, dx->getWidth(), dx->getHeight(), 1), video::IHardwarePixelBuffer::ELO_Discard);
	video::LockedPixelBox dyBB = dy->lock(math::box3d(0, 0, 0, dy->getWidth(), dy->getHeight(), 1), video::IHardwarePixelBuffer::ELO_Discard);
	struct rgba
	{
		uchar v[4];
	};

	rgba* ptrX = (rgba*)dxBB.data;
	rgba* ptrY = (rgba*)dyBB.data;


	for (int i = 0; i < numVals; ++i)
	{
		float v = dist[i];

		// The distortion range is -0.6 to +1.7. Normalize to range [0..1).
		v = (v + 0.6f) / 2.3f;

		float encX = v;
		float encY = v * 255.0f;
		float encZ = v * 65025.0f;
		float encW = v * 160581375.0f;

		encX = encX - floor(encX);
		encY = encY - floor(encY);
		encZ = encZ - floor(encZ);
		encW = encW - floor(encW);

		encX -= math::i255*encY;
		encY -= math::i255*encZ;
		encZ -= math::i255*encW;

		int index = i >> 1;
		rgba* p = 0;
		if (i % 2 == 0)
			p = &ptrX[index];
		else
			p = &ptrY[index];

		p->v[0] = (uchar)(encX * 256);
		p->v[1] = (uchar)(encY * 256);
		p->v[2] = (uchar)(encZ * 256);
		p->v[3] = (uchar)(encW * 256);
	}

	dx->unlock();
	dy->unlock();

}
bool LeapMotionImageRetrival::Capture(Leap::Frame &frame)
{

	if (frame.images().isEmpty())
		return false;

	Leap::Image image = frame.images()[m_index];
	int w = image.width();
	int h = image.height();
	if (w == 0 || h == 0)
		return false;

	_SetMainTex(w, h);

	int dw = image.distortionWidth() / 2;
	int dh = image.distortionHeight();

	if (dw == 0 || dh == 0)
		return false;

	_SetDistortionTex(dw, dh);

	_LoadMainTexture(image.data());
	_EncodeDisortion(image);

	math::vector4d ray(image.rayOffsetX(), image.rayOffsetY(), image.rayScaleX(), image.rayScaleY());
	undistortShader->GetParam("Ray")->SetValue(ray);

	gEngine.getDevice()->useTexture(0, &mainTU);
	gEngine.getDevice()->useTexture(1, &disXTU);
	gEngine.getDevice()->useTexture(2, &disYTU);
	undistortShader->Setup(math::rectf(0, 0, mainTexture->getSize().x, mainTexture->getSize().y));
	undistortShader->render(&video::TextureRTWrap(mainTexture));
	finalTexture = undistortShader->getOutput()->GetColorTexture();
	//finalTexture = mainTexture;

	gEngine.getDevice()->useTexture(0, 0);
	gEngine.getDevice()->useTexture(1, 0);
	gEngine.getDevice()->useTexture(2, 0);

	m_hasNewFrame = true;

	return true;
}

void LeapMotionImageRetrival::SetFrameSize(int w, int h)
{
	//m_frameSize.set(w, h);
}

const math::vector2di& LeapMotionImageRetrival::GetFrameSize()
{
	return m_frameSize;
}


void LeapMotionImageRetrival::SetImageFormat(video::EPixelFormat fmt)
{
}

video::EPixelFormat LeapMotionImageRetrival::GetImageFormat()
{
	return m_imageInfo.format;
}


bool LeapMotionImageRetrival::GrabFrame()
{
	if (m_hasNewFrame)
	{
		m_hasNewFrame = false;
		return true;
	}
	return false;
}

bool LeapMotionImageRetrival::HasNewFrame()
{
	return m_hasNewFrame;
}

ulong LeapMotionImageRetrival::GetBufferID()
{
	return m_bufferID;
}



const video::ImageInfo* LeapMotionImageRetrival::GetLastFrame()
{
	return &m_imageInfo;
}
void LeapMotionImageRetrival::Lock()
{
	m_mutex->lock();
}
void LeapMotionImageRetrival::Unlock()
{
	m_mutex->unlock();
}

}



