
#include "stdafx.h"
#include "StereoRenderer.h"
#include "ViewPort.h"
#include "RenderWindow.h"
#include "CameraNode.h"
#include "Engine.h"
#include "ShaderResourceManager.h"


namespace mray
{
namespace scene
{


StereoRenderer::StereoRenderer(video::RenderWindow* rw,CameraNode*leftCam,CameraNode* rightCam,
								   IMovable* headMount):
m_leftCam(leftCam),m_rightCam(rightCam),m_offset(0.1),m_headMount(headMount),m_fullVP(0)
{
	m_leftVP=rw->CreateViewport(mT("LeftEyeVP"),leftCam,0,math::rectf(0,0,0.5,1),0);
	m_rightVP=rw->CreateViewport(mT("RightEyeVP"),rightCam,0,math::rectf(0.5,0,1,1),0);
	m_fullVP=rw->CreateViewport(mT("FullVP"),0,0,math::rectf(0,0,1,1),0);

	m_leftVP->setCamera(m_leftCam);
	m_rightVP->setCamera(m_rightCam);
	_init();

}
StereoRenderer::StereoRenderer(ViewPort* left,ViewPort*right,IMovable* headMount):
m_leftCam(0),m_rightCam(0),m_offset(0.1),m_headMount(headMount),m_fullVP(0)
{
	m_leftVP=left;
	m_rightVP=right;
	if(m_leftVP)
		m_leftCam=m_leftVP->getCamera();
	if(m_rightVP)
		m_rightCam=m_rightVP->getCamera();
	_init();
}
StereoRenderer::~StereoRenderer()
{
}
void StereoRenderer::_init()
{
	if(m_headMount)
	{
		m_headMount->addChild(m_leftCam);
		m_headMount->addChild(m_rightCam);
	}

	SetOffset(0.06);

	m_stereoPP=new video::GPUShader();
	m_stereoPP->SetVertexShader(gShaderResourceManager.loadShaderFromFile(mT("stereoscopicRB.cg"),video::EShader_VertexProgram,"main_vp",video::ShaderPredefList(), mT("cg")));
	m_interlanceProg = gShaderResourceManager.loadShaderFromFile(mT("stereoscopicRB.cg"), video::EShader_FragmentProgram, "main_Interlace", video::ShaderPredefList(), mT("cg"));
	m_anglyphProg = gShaderResourceManager.loadShaderFromFile(mT("stereoscopicRB.cg"), video::EShader_FragmentProgram, "main_Anglyph", video::ShaderPredefList(), mT("cg"));

	video::IVideoDevice* device= Engine::getInstance().getDevice();

	SetMode(EStereo_None);
}

void StereoRenderer::SetMode(EStereoMode mode)
{
	m_mode=mode;
	switch(mode)
	{
	case EStereo_None:
		m_leftVP->setActive(true);
		m_leftVP->setViewPort(math::rectf(0,0,1,1));
		m_leftVP->setOnlyToRenderTarget(false);
		m_rightVP->setActive(false);

		if(m_leftCam)
			m_leftCam->setPosition(0);
	break;
	case EStereo_SideBySide:
		m_leftVP->setOnlyToRenderTarget(false);
		m_rightVP->setOnlyToRenderTarget(false);
		SetOffset(m_offset);

		{
			m_leftVP->setActive(true);
			m_rightVP->setActive(true);
			if(m_leftVP->GetRenderWindow()==m_rightVP->GetRenderWindow())
			{
				m_leftVP->setViewPort(math::rectf(0,0,0.5,1));
				m_rightVP->setViewPort(math::rectf(0.5,0,1,1));
			}
		}
		break;
	case EStereo_TopDown:
		m_leftVP->setOnlyToRenderTarget(false);
		m_rightVP->setOnlyToRenderTarget(false);
		SetOffset(m_offset);


		{
			m_leftVP->setActive(true);
			m_rightVP->setActive(true);
			if(m_leftVP->GetRenderWindow()==m_rightVP->GetRenderWindow())
			{
				m_leftVP->setViewPort(math::rectf(0,0,1,0.5));
				m_rightVP->setViewPort(math::rectf(0,0.5,1,1));
			}
		}
		break;
	case EStereo_Interlance:
		m_leftVP->setActive(true);
		m_rightVP->setActive(true);
		m_leftVP->setOnlyToRenderTarget(true);
		m_rightVP->setOnlyToRenderTarget(true);

		m_leftVP->setViewPort(math::rectf(0,0,1,1));
		m_rightVP->setViewPort(math::rectf(0,0,1,1));
		SetOffset(m_offset);

		m_stereoPP->SetFragmentShader(m_interlanceProg);
		break;
	case EStereo_Anglyph:
		m_leftVP->setActive(true);
		m_rightVP->setActive(true);
		m_leftVP->setOnlyToRenderTarget(true);
		m_rightVP->setOnlyToRenderTarget(true);

		m_leftVP->setViewPort(math::rectf(0,0,1,1));
		m_rightVP->setViewPort(math::rectf(0,0,1,1));
		SetOffset(m_offset);
		m_stereoPP->SetFragmentShader(m_anglyphProg);
		break;
	}
}
EStereoMode StereoRenderer::GetMode()
{
	return m_mode;
}

void StereoRenderer::SetLeftCamera(CameraNode* c)
{
	m_leftCam=c;
	if(c)
	{
		m_headMount->addChild(c);
		SetOffset(m_offset);
	}
}
void StereoRenderer::SetRightCamera(CameraNode* c)
{
	m_rightCam=c;
	if(c)
	{
		m_headMount->addChild(c);
		SetOffset(m_offset);
	}
}
void StereoRenderer::SetOffset(float offset)
{
	m_offset=offset;

	if(m_mode!=EStereo_None && m_leftCam && m_rightCam)
	{
		math::vector3d pos(m_offset*0.5,0,0);
		m_leftCam->setPosition(pos);
		m_rightCam->setPosition(-pos);
	}
}
float StereoRenderer::GetOffset()const
{
	return m_offset;
}

void StereoRenderer::Render()
{
	video::IVideoDevice* device= Engine::getInstance().getDevice();
	switch(m_mode)
	{
	case EStereo_None:
	case EStereo_SideBySide:
	case EStereo_TopDown:
		break;
	case EStereo_Interlance:
	case EStereo_Anglyph:
		{
			if(m_leftVP->GetRenderWindow()!=m_rightVP->GetRenderWindow())
				break;
			video::TextureUnit texA,texB;
			video::IShaderConstantsCallback cb;
			device->useShader(m_stereoPP);
			cb.setConstants(m_stereoPP);
			texA.SetTexture(m_leftVP->getRenderTarget()->GetColorTexture());
			texB.SetTexture(m_rightVP->getRenderTarget()->GetColorTexture());
			m_stereoPP->GetFragmentShader()->setTexture(mT("texA"),&texA);
			m_stereoPP->GetFragmentShader()->setTexture(mT("texB"),&texB);

			device->draw2DRectangle(math::rectf(0,m_leftVP->GetRenderWindow()->GetSize()),1);
			device->useShader(0);

		}
		break;
	}

	if(m_fullVP)
		device->setViewport(m_fullVP);
}
}
}
