
#include "stdafx.h"
#include "GeomDepthState.h"
#include "OpenNIHandler.h"
#include "TextureResourceManager.h"

#include "ViewPort.h"
#include "FontResourceManager.h"
#include "GUIBatchRenderer.h"
#include "DepthVisualizer.h"


namespace mray
{
namespace AugTel
{

	GeomDepthState::GeomDepthState(const core::string& name)
		:IRenderingState(name)
{
	m_openNiHandler = new TBee::OpenNIHandler();
	m_visualizer = new TBee::DepthVisualizer();
}

GeomDepthState::~GeomDepthState()
{
	delete m_openNiHandler;
	delete m_visualizer;
}

void GeomDepthState::InitState()
{
	IRenderingState::InitState();
	m_openNiHandler->Init();
	m_openNiHandler->GetNormalCalculator().SetAmplitude(0.5f);
	m_mouseDown = false;
	m_openNiHandler->SetCalculateNormals(true);

	GUI::GUIBatchRenderer*r = new GUI::GUIBatchRenderer();
	r->SetDevice(Engine::getInstance().getDevice());
	m_guiRenderer = r;
	m_visualizer->SetDepthFrame(m_openNiHandler->GetNormalCalculator().GetDepthFrame());
	m_visualizer->Init();
	m_visualizer->SetViewNormals(true);

}

bool GeomDepthState::OnEvent(Event* e, const math::rectf& rc)
{
	bool ok = false;
	if (e->getType() == ET_Keyboard)
	{
		KeyboardEvent* evt = (KeyboardEvent*)e;
		if (evt->press)
		{
			if ( evt->key == KEY_D)
			{
				//m_openNiHandler->SetViewDepth(!m_openNiHandler->IsViewDepth());
				ok = true;
			}
			else if ( evt->key == KEY_F10)
			{
				core::string name = "screenShots/ScreenShot_";
				name += core::StringConverter::toString((int)gEngine.getTimer()->getSeconds());
				gTextureResourceManager.writeResourceToDist(m_visualizer->GetDepthTexture(), name+"_depth.png");
				gTextureResourceManager.writeResourceToDist(m_visualizer->GetNormalsTexture(), name + "_normals.png");

				ok = true;

			}
			else if (evt->key == KEY_ESCAPE)
			{
				m_exitCode = STATE_EXIT_CODE;
				ok = true;
			}
			else if (evt->key == KEY_T)
			{
				TBee::DepthFrame* df= m_openNiHandler->GetNormalCalculator().GetDepthFrame();
				df->EnableThreshold(!df->IsThresholdEnabled());
				ok = true;
			}
			else if (evt->key == KEY_S)
			{
				TBee::DepthFrame* df = m_openNiHandler->GetNormalCalculator().GetDepthFrame();
				df->EnableSmoothing(!df->IsSmoothingEnabled());
				ok = true;
			}
			else if (evt->key == KEY_G)
			{
				TBee::DepthFrame* df = m_openNiHandler->GetNormalCalculator().GetDepthFrame();
				df->EnableGapFill(!df->IsGapFillEnabled());
				ok = true;
			}
			else if (evt->key == KEY_Q)
			{
				TBee::DepthFrame* df = m_openNiHandler->GetNormalCalculator().GetDepthFrame();
				int inner = df->GetInnerBandThreshold();
				int outer = df->GetOuterBandThreshold();
				if (evt->shift)
					outer++;
				else inner++;
				df->SetBandThreshold(inner, outer);
				ok = true;
			}
			else if (evt->key == KEY_E)
			{
				TBee::DepthFrame* df = m_openNiHandler->GetNormalCalculator().GetDepthFrame();
				int inner = df->GetInnerBandThreshold();
				int outer = df->GetOuterBandThreshold();
				if (evt->shift)
					outer = math::Max(0, outer - 1);
				else inner=math::Max(0,inner-1);
				df->SetBandThreshold(inner, outer);
				ok = true;
			}

		}
	}
	else if (e->getType() == ET_Mouse)
	{
		MouseEvent* evt = (MouseEvent*)e;
		if (evt->event == MET_LEFTDOWN)
		{
			m_mouseDown = true;
		}
		else if(evt->event == MET_LEFTUP)
			m_mouseDown = false;
		if (evt->event == MET_MOVED)
		{
			if (m_openNiHandler->IsStarted() && evt->GetViewPort() )
			{
				m_mousePos = evt->pos / evt->GetViewPort()->GetSize();
				m_mousePos.x *= m_openNiHandler->GetNormalCalculator().Width();
				m_mousePos.y *= m_openNiHandler->GetNormalCalculator().Height();
			}
		}
	}
	return ok;
}

void GeomDepthState::OnEnter(IRenderingState*prev)
{
	IRenderingState::OnEnter(prev);
	m_openNiHandler->Start(640,480);
}

void GeomDepthState::OnExit()
{
	IRenderingState::OnExit();
	m_openNiHandler->Close();
}

video::IRenderTarget* GeomDepthState::Render(const math::rectf& rc, TBee::ETargetEye eye)
{
	video::IRenderTarget* rt = IRenderingState::Render(rc, eye);
	video::TextureUnit tex;
	m_visualizer->Update();

	Engine::getInstance().getDevice()->setRenderTarget(rt);

	tex.SetTexture(m_visualizer->GetNormalsTexture());
	Engine::getInstance().getDevice()->useTexture(0, &tex);

	Engine::getInstance().getDevice()->draw2DImage(rc, video::SColor(1, 1, 1, 1));


	GUI::IFont* font = gFontResourceManager.getDefaultFont();
	GUI::FontAttributes attr;
	video::IVideoDevice* dev = Engine::getInstance().getDevice();
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

		if (m_mouseDown)
		{
			float depth = ( m_openNiHandler->GetNormalCalculator().GetDepthAtPoint(m_mousePos.x, m_mousePos.y));
			math::vector3d norm = m_openNiHandler->GetNormalCalculator().GetNormalAtPoint(m_mousePos.x, m_mousePos.y);
			math::vector4d depthNorm;
			depthNorm.set(norm.x, norm.y, norm.z, depth);
			core::string msg = mT("Depth Norm:") + core::StringConverter::toString(depthNorm);
			font->print(r, &attr, 0, msg, m_guiRenderer);
			r.ULPoint.y += attr.fontSize + 5;
		}


		TBee::DepthFrame* df = m_openNiHandler->GetNormalCalculator().GetDepthFrame();
		int inner = df->GetInnerBandThreshold();
		int outer = df->GetOuterBandThreshold();
		bool s = df->IsSmoothingEnabled();
		bool g = df->IsGapFillEnabled();

		core::string msg = mT("Smoothing:") + core::StringConverter::toString(s);
		font->print(r, &attr, 0, msg, m_guiRenderer);
		r.ULPoint.y += attr.fontSize + 5;
		msg = mT("Gap Fill:") + core::StringConverter::toString(g);
		font->print(r, &attr, 0, msg, m_guiRenderer);
		r.ULPoint.y += attr.fontSize + 5;
		msg = mT("Inner:") + core::StringConverter::toString(inner);
		msg += mT("/ Outer:") + core::StringConverter::toString(outer);
		font->print(r, &attr, 0, msg, m_guiRenderer);
		r.ULPoint.y += attr.fontSize + 5;

		m_guiRenderer->Flush();
	}
	return rt;
}

void GeomDepthState::Update(float dt)
{
	IRenderingState::Update(dt);
	m_openNiHandler->Update(dt);
}


void GeomDepthState::LoadFromXML(xml::XMLElement* e)
{
}

}
}