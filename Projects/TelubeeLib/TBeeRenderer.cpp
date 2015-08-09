

#include "stdafx.h"
#include "TBeeRenderer.h"

#include "AppData.h"
#include "OculusCameraComponent.h"
#include "OculusDevice.h"
#include "OculusManager.h"
#include "RenderWindow.h"
#include "Viewport.h"
#include "ParsedShaderPP.h"
#include "ShaderResourceManager.h"

namespace mray
{
namespace TBee
{
	class TBeeRendererData :public scene::IViewportListener
{
public:

	TBeeRenderer* owner;

	GCPtr<video::OculusManager> oculusManager;
	GCPtr<video::ParsedShaderPP> oculusRenderer[2];
	game::OculusCameraComponent* oculusComponents[2];
	video::ITexturePtr rtTexture;
	video::IRenderTargetPtr renderTarget;;

	scene::ViewPort* m_viewPort[2];

	video::IRenderTargetPtr m_preFinalRT[2];
	video::IRenderTargetPtr m_finalRT[2];

	GCPtr<video::GPUShader> m_stereoPP;

	int m_vpCount;

	bool m_directRender;

	int m_stereoTVRenderingSwitch;

	TBeeRendererData(TBeeRenderer* o)
	{
		owner = o;
		m_vpCount = 0;
		m_stereoTVRenderingSwitch = 0;
		m_directRender = false;
	}
	~TBeeRendererData()
	{
		oculusRenderer[0] = 0;
		oculusRenderer[1] = 0;
		AppData::Instance()->oculusDevice = 0;
		oculusManager = 0;
	}

	void SetDirectRender(bool dr)
	{
		m_directRender = dr;
	}

	void Init(video::RenderWindow* rw)
	{
		video::IVideoDevice* device = Engine::getInstance().getDevice();

		m_stereoPP = new video::GPUShader();
		m_stereoPP->SetVertexShader(gShaderResourceManager.loadShaderFromFile(mT("stereoscopicRB.cg"), video::EShader_VertexProgram, "main_vp",video::ShaderPredefList(),  mT("cg")));
		m_stereoPP->SetFragmentShader(gShaderResourceManager.loadShaderFromFile(mT("stereoscopicRB.cg"), video::EShader_FragmentProgram, "main_Interlace", video::ShaderPredefList(), mT("cg")));
		
		
		{
			if (AppData::Instance()->stereoMode == ERenderStereoMode::None)
			{
				m_viewPort[0] = rw->CreateViewport("SingleVP", 0, 0, math::rectf(0, 0, 1, 1), 0);
				m_viewPort[0]->AddListener(this);
			}
			else
			{
				m_viewPort[0] = rw->CreateViewport("MainVPL", 0, 0, math::rectf(0, 0, 0.5, 1), 0);
				m_viewPort[0]->AddListener(this);
				m_viewPort[1] = rw->CreateViewport("MainVPR", 0, 0, math::rectf(0.5, 0, 1, 1), 0);
				m_viewPort[1]->AddListener(this);
			}
			switch (AppData::Instance()->stereoMode)
			{
			case ERenderStereoMode::SideBySide:
				m_viewPort[0]->setViewPort(math::rectf(0, 0, 1, 1));
				m_viewPort[1]->setViewPort(math::rectf(0, 0, 1, 1));
				break;
			case ERenderStereoMode::Oculus:
				m_viewPort[0]->setViewPort(math::rectf(0, 0, 0.5, 1));
				m_viewPort[1]->setViewPort(math::rectf(0.5, 0, 1, 1));
				break;
			case ERenderStereoMode::TopDown:
				m_viewPort[0]->setViewPort(math::rectf(0, 0, 1, 0.5));
				m_viewPort[1]->setViewPort(math::rectf(0, 0.5, 1, 1));
				break;
			case ERenderStereoMode::StereoTV:
				m_viewPort[0]->setViewPort(math::rectf(0, 0, 1, 1));
				m_viewPort[1]->setViewPort(math::rectf(0, 0, 1, 1));
				break;
			case ERenderStereoMode::NVidia:
				m_viewPort[0]->setViewPort(math::rectf(0, 0, 1, 1));
				m_viewPort[1]->setViewPort(math::rectf(0, 0, 1, 1));
				break;
			default:
				break;
			}

		}

		m_vpCount = 1;
		if (AppData::Instance()->stereoMode != ERenderStereoMode::None)
			m_vpCount = 2;
		for (int i = 0; i < 2; ++i)
		{
			video::ITexturePtr tex = device->createEmptyTexture2D(true);
			tex->setMipmapsFilter(false);

			m_preFinalRT[i] = device->createRenderTarget("", tex, 0, 0, 0);

			tex = device->createEmptyTexture2D(true);
			tex->setMipmapsFilter(false);
			m_finalRT[i] = device->createRenderTarget("", tex, 0, 0, 0);
		}

		// check for Oculus
		if (AppData::Instance()->headController == EHeadControllerType::Oculus || AppData::Instance()->stereoMode == ERenderStereoMode::Oculus)
		{
			gLogManager.log("Initing Oculus", ELL_INFO);
			//AppData::Instance()->headController = EHeadControllerType::Oculus;
			//AppData::Instance()->stereoMode = ERenderStereoMode::Oculus;
			//Create Oculus
			oculusManager = new video::OculusManager();
			AppData::Instance()->oculusDevice = oculusManager->CreateDevice(0);

			if (!AppData::Instance()->oculusDevice)
			{
			}
			else
			{
#ifdef OCULUS_DK1
				int w = AppData::Instance()->oculusDevice->GetDeviceInfo().deviceInfo.HResolution;
				int h = AppData::Instance()->oculusDevice->GetDeviceInfo().deviceInfo.VResolution;
#else

				int w = AppData::Instance()->oculusDevice->GetDeviceInfo().texSize.x;
				int h = AppData::Instance()->oculusDevice->GetDeviceInfo().texSize.y;
#endif
				game::OculusCameraComponent::ETargetCamera cams[2] =
				{
					game::OculusCameraComponent::LeftCamera,
					game::OculusCameraComponent::RightCamera
				};
				//m_viewPort[1]->setActive(false);
				for (int i = 0; i < 2; ++i)
				{
					oculusRenderer[i] = new video::ParsedShaderPP(device);
					oculusRenderer[i]->LoadXML(gFileSystem.openFile("OculusLens.peff"));
					oculusRenderer[i]->Setup(math::rectf(0, 0, w , h));
					oculusComponents[i] = new game::OculusCameraComponent(0);
					oculusComponents[i]->InitCamera(AppData::Instance()->oculusDevice, m_viewPort[i], cams[i], oculusRenderer[i]);

				}

				{
					rtTexture = device->createEmptyTexture2D(true);
					rtTexture->setMipmapsFilter(false);
					rtTexture->createTexture(math::vector3d(w / 2, h, 1), video::EPixel_R8G8B8);

					renderTarget = device->createRenderTarget("", rtTexture, 0, 0, 0);
				}
			}
		}
	}
	

	void _RenderVP(int i)
	{

		video::IVideoDevice* device = Engine::getInstance().getDevice();
		math::rectf rc = math::rectf(0, m_viewPort[i]->getAbsRenderingViewPort().getSize());
		video::ITexture* tex = m_preFinalRT[i]->GetColorTexture();
		if (tex->getSize().x != rc.getWidth() ||
			tex->getSize().y != rc.getHeight())
		{
			tex->createTexture(math::vector3d(rc.getWidth(), rc.getHeight(), 1), video::EPixel_R8G8B8A8);
			m_finalRT[i]->GetColorTexture()->createTexture(math::vector3d(rc.getWidth(), rc.getHeight(), 1), video::EPixel_R8G8B8A8);
		}

		owner->__FIRE_OnRendererDraw(owner, rc, m_preFinalRT[i], i == 0 ? Eye_Left : Eye_Right);
		device->setRenderTarget(0, false, true, video::SColor(1, 1, 1, 0));

		video::TextureUnit texU;
		if (rtTexture && (rtTexture->getSize().x != rc.getSize().x || rtTexture->getSize().x != rc.getSize().y))
		{
			rtTexture->createTexture(math::vector3di(rc.getSize().x, rc.getSize().y, 1), rtTexture->getImageFormat());
		}
		if (AppData::Instance()->stereoMode == ERenderStereoMode::Oculus && oculusRenderer[i])
		{
			device->set2DMode();
			{
				device->setRenderTarget(renderTarget);
				math::matrix4x4 m, pm;
				device->getTransformationState(video::TS_PROJECTION, m);
				pm.f14 = oculusComponents[i]->GetPerspectiveOffset();
				pm = pm*m;
				device->setTransformationState(video::TS_PROJECTION, pm);

				texU.SetTexture(m_preFinalRT[i]->GetColorTexture());
				device->useTexture(0, &texU);
				math::rectf tc = math::rectf(0, 0, 1, 1);
				device->draw2DImage(math::rectf(0, renderTarget->GetSize()), 1, 0, &tc);
				device->setRenderTarget(0);
				device->setTransformationState(video::TS_PROJECTION, m);
			}
			oculusRenderer[i]->Setup(math::rectf(0, renderTarget->GetSize()));
			oculusRenderer[i]->render(renderTarget);
			texU.SetTexture(oculusRenderer[i]->getOutput()->GetColorTexture());
			device->setRenderTarget(m_finalRT[i], false, true, video::SColor(1, 1, 1, 0));
			device->useTexture(0, &texU);
			device->draw2DImage(rc, 1);
			device->setRenderTarget(0, false, true, video::SColor(1, 1, 1, 0));
		}
		else
		{
			texU.SetTexture(m_preFinalRT[i]->GetColorTexture());
			device->setRenderTarget(m_finalRT[i], false, true, video::SColor(1, 1, 1, 0));
			device->useTexture(0, &texU);
			math::rectf tc = math::rectf(0, 0, 1, 1);
			device->draw2DImage(rc, 1,0,&tc);
			device->setRenderTarget(0, false, true, video::SColor(1, 1, 1, 0));
		}
	}

	void onRenderDone(scene::ViewPort*vp)
	{
		if (vp == m_viewPort[0])
		{
			_RenderVP(0);
		}
		if (vp == m_viewPort[1])
		{
			_RenderVP(1);
		}

	}
	bool IsStereo()
	{
		return AppData::Instance()->stereoMode != ERenderStereoMode::None;
	}
	
	void Render(scene::ViewPort* vp)
	{
		video::IVideoDevice* device = Engine::getInstance().getDevice();
		video::TextureUnit tex;
		int vpCount = (AppData::Instance()->stereoMode == ERenderStereoMode::None) ? 1 : 2;
 
		if (AppData::Instance()->stereoMode == ERenderStereoMode::StereoTV && false)
		{
			device->useShader(m_stereoPP);
			video::TextureUnit texA, texB;
			video::IShaderConstantsCallback cb;
			device->useShader(m_stereoPP);
			cb.setConstants(m_stereoPP);
			texA.SetTexture(m_finalRT[0]->GetColorTexture());
			texB.SetTexture(m_finalRT[1]->GetColorTexture());
			m_stereoPP->GetFragmentShader()->setTexture(mT("texA"), &texA);
			m_stereoPP->GetFragmentShader()->setTexture(mT("texB"), &texB);

			device->draw2DRectangle(math::rectf(0, vp->getAbsRenderingViewPort().getSize()), 1);
			device->useShader(0);
		}
		else
		{

			math::rectf targetRect[2];
			math::rectf rc = vp->getAbsRenderingViewPort();
			switch (AppData::Instance()->stereoMode)
			{
			case ERenderStereoMode::SideBySide:
			case ERenderStereoMode::Oculus:
				targetRect[0] = math::rectf(0, 0, rc.getWidth() / 2, rc.getHeight());
				targetRect[1] = math::rectf(rc.getWidth() / 2, 0, rc.getWidth(), rc.getHeight());
				break;
			case ERenderStereoMode::TopDown:
				targetRect[0] = math::rectf(0, 0, rc.getWidth(), rc.getHeight() / 2);
				targetRect[1] = math::rectf(0, rc.getHeight()/2, rc.getWidth(), rc.getHeight());
				break;
			default:
				targetRect[0] = math::rectf(0, 0, rc.getWidth(), rc.getHeight());
				targetRect[1] = math::rectf(0, 0, rc.getWidth(), rc.getHeight());
			}
			for (int i = 0; i < vpCount; ++i)
			{
// 				if (AppData::Instance()->stereoMode == ERenderStereoMode::NVidia)
// 				{
// 					if (i == 0)
// 						glDrawBuffer(GL_BACK_LEFT);
// 					else
// 						glDrawBuffer(GL_BACK_RIGHT);
// 					glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
// 
// 				}
				tex.SetTexture(m_finalRT[i]->GetColorTexture());
				device->useTexture(0, &tex);
				device->draw2DImage(targetRect[i], 1);
				/*
				if (AppData::Instance()->stereoMode == ERenderStereoMode::StereoTV)
				{
				if (m_stereoTVRenderingSwitch == 0)m_stereoTVRenderingSwitch = 1;
				else m_stereoTVRenderingSwitch = 0;
				break;
				}*/
			}
		//	glDrawBuffer(GL_BACK);

		}
	}


	void Update(float dt)
	{

		if ( AppData::Instance()->oculusDevice)
		{
			oculusManager->Update(dt);
			AppData::Instance()->oculusDevice->Update(dt);

			oculusComponents[0]->Update(dt);
			oculusComponents[1]->Update(dt);
		}
	}
};

TBeeRenderer::TBeeRenderer()
{
	m_data = new TBeeRendererData(this);
}
TBeeRenderer::~TBeeRenderer()
{
	delete m_data;
}

int TBeeRenderer::GetViewportCount()
{
	return m_data->m_vpCount;
}
scene::ViewPort* TBeeRenderer::GetViewport(int i)
{
	if (i >= GetViewportCount())
		return 0;
	return m_data->m_viewPort[i];
}
void TBeeRenderer::Init(video::RenderWindow* rw)
{

	m_data->Init(rw);
}
video::IRenderTargetPtr TBeeRenderer::GetEyeImage(int i)
{
	return m_data->m_preFinalRT[i];
}
void TBeeRenderer::SetDirectRender(bool dr)
{
	m_data->SetDirectRender(dr);
}

void TBeeRenderer::Render(scene::ViewPort* vp)
{
	m_data->Render(vp);
}
void TBeeRenderer::Update(float dt)
{
	m_data->Update(dt);


}

bool TBeeRenderer::IsStereo()
{
	return m_data->IsStereo();
}


}
}


