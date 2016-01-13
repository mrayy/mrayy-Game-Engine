

#include "stdafx.h"
#include "GUISessionSidePanel.h"
#include "GUIElementRegion.h"
#include "SessionContainer.h"
#include "SessionDetails.h"
#include "CSubProject.h"

#include "TextureResourceManager.h"
#include "ShaderResourceManager.h"
#include "ImageSetResourceManager.h"

#include "GUIOverlayManager.h"
#include "GUIOverlay.h"

namespace mray
{
namespace GUI
{


IMPLEMENT_RTTI(GUISessionSidePanel, IGUIPanelElement);

const GUID GUISessionSidePanel::ElementType = "GUISessionSidePanel";

float GUISessionSidePanel_ShrinkSpeed = 150;

const core::string GUISessionSidePanel::ProfileRedneringShader =
"half4 main_fp(float2 texCoord : TEXCOORD0, "
"uniform sampler2D texA : register(s0),uniform float alpha,uniform float gray) : COLOR "
	"{"
	"half4 clr=tex2D(texA,texCoord);"
	"half2 v=(texCoord-0.5);"
	"half dst=dot(v,v);"
	"dst=dst>0.25?0:1;"
	"half3 gclr=dot(clr.rgb,half3(0.5,0.3,0.2));"
	"clr.rgb=gclr*gray+clr.rgb*(1-gray);"
	"return half4(clr.rgb,dst*alpha);"
	"}";
GUISessionSidePanel::GUISessionSidePanel(IGUIManager* m):
IGUIPanelElement(ElementType, m)
{
	m_minWidth = 100;
	m_maxWidth = 120;
	m_state = Expand;
	m_sessions = 0;
	m_szT = 0;
	m_currentSpeaker = 0;
	m_speakerTransition = 0;

	video::IGPUShaderProgramPtr shader = gShaderResourceManager.getResource("ProfileRendering");
	if (!shader)
	{
		shader = gShaderResourceManager.loadShaderFromProgram("ProfileRendering",ProfileRedneringShader, video::EShader_FragmentProgram, "main_fp", video::ShaderPredefList(), mT("cg"));
		shader->setResourceName("ProfileRendering");
		gShaderResourceManager.addResource(shader, "ProfileRendering");
	}

	GUI::GUIOverlay* o = GUI::GUIOverlayManager::getInstance().LoadOverlay("GUISessionSidePanelLayout.GUI");
	if (o)
	{
		o->CreateElements(m, this, this, this);
	}

	gAppData.subProjectChange.AddListener(this);
}
GUISessionSidePanel::~GUISessionSidePanel()
{
	gAppData.subProjectChange.RemoveListener(this);
}
void GUISessionSidePanel::LoadXMLSettings(xml::XMLElement*e)
{
	IGUIPanelElement::LoadXMLSettings(e);

	math::vector2d sz;
	sz = this->GetSize();
	sz.x = core::StringConverter::toInt(gAppData.GetValue("UI", "SessionPanelWidth", core::StringConverter::toString(sz.x)));
	this->SetSize(sz);

	m_maxWidth = sz.x;
	m_minWidth = sz.x * 70 / 100;
}

void GUISessionSidePanel::SetSessionContainer(kmd::SessionContainer* s)
{
	m_sessions = s;
	const std::vector<kmd::SessionDetails*>& sessions= m_sessions->GetSessions();
	for (int i = 0; i < sessions.size(); ++i)
	{
		ProjectImageIfo ifo;
		ifo.project = sessions[i];
		ifo.texture = gTextureResourceManager.loadTexture2D(ifo.project->GetPicture());
		m_projects.push_back(ifo);
	}
	SetCurrentProject(0);
}
bool GUISessionSidePanel::OnEvent(Event* e)
{
// 	if (e->getType() == ET_Mouse)
// 	{
// 		MouseEvent* evt = dynamic_cast<MouseEvent*>(e);
// 		const GUIElementRegion* r = GetDefaultRegion();
// 		if (r->GetClippedRect().IsPointInside(evt->pos))
// 		{
// 			m_state = Expand;
// 		}
// 		else 
// 			m_state=Shrink;
// 	}
// 	else 
	/*
	if (e->getType() == ET_Keyboard)
	{
		KeyboardEvent* evt = dynamic_cast<KeyboardEvent*>(e);
		if (evt->press)
		{
			if (evt->key == KEY_DOWN)
			{
				SetCurrentSpeaker(m_currentSpeaker + 1);
			}
			if (evt->key == KEY_UP)
			{
				SetCurrentSpeaker(m_currentSpeaker - 1);
			}
		}
	}*/
	return IGUIPanelElement::OnEvent(e);
}
void GUISessionSidePanel::_OnSubProjectChange(kmd::CSubProject* s)
{
	if (m_currentSpeaker == -1 || s->GetSession() == m_projects[m_currentSpeaker].project)
		return;
	for (int i = 0; i < m_projects.size();++i)
		if (s->GetSession() == m_projects[i].project)
		{
			SetCurrentProject(i);
			break;
		}
}
void GUISessionSidePanel::SetCurrentProject(int s)
{
	if (s < 0)
	{
		s = 0;
	}
	if (s >= m_projects.size())
		s = m_projects.size() - 1;
	m_currentSpeaker = s;

	if (m_currentSpeaker <0)
		return;

	//OnProjectChange(this, m_projects[m_currentSpeaker].project);

}
void GUISessionSidePanel::Update(float dt)
{
	IGUIPanelElement::Update(dt);


#define DECREASE(x,l,v) if(x>(l)){ x-=(v);} if(x<(l)){x=(l);}
#define INCREASE(x,l,v) if(x<(l)){ x+=(v);} if(x>(l)){x=(l);}

	math::vector2d sz = this->GetSize();
	float a= GetAlpha();
	if (m_state == Shrink)
	{
		DECREASE(sz.x, m_minWidth, GUISessionSidePanel_ShrinkSpeed*dt);
		//DECREASE(a, 0.2, dt);
	}

	if (m_state == Expand)
	{
		//INCREASE(a, 1, dt);
		INCREASE(sz.x, m_maxWidth, GUISessionSidePanel_ShrinkSpeed*dt);
	}
	m_szT = ((sz.x - m_minWidth) / (m_maxWidth - m_minWidth));
	a = 0.2 + 0.8*m_szT;
	SetSize(sz);
	SetAlpha(a);
	
	float dst = 2*fabs(m_speakerTransition - m_currentSpeaker);

	if (m_speakerTransition > m_currentSpeaker)
	{
		DECREASE(m_speakerTransition, m_currentSpeaker, (dst*dst)* dt);
	}

	if (m_speakerTransition < m_currentSpeaker)
	{
		INCREASE(m_speakerTransition, m_currentSpeaker, (dst*dst) * dt);
	}
}
void GUISessionSidePanel::Draw(const math::rectf*vp)
{
	video::IVideoDevice* dev= Engine::getInstance().getDevice();

	const GUIElementRegion* r = GetDefaultRegion();
	math::rectf rc;

	dev->draw2DRectangle(r->GetRect(), GetColor());

	video::TextureUnit tex;
	if (m_szT != 1)
	{
		video::ImageSet* tedxTokyoSet = gImageSetResourceManager.getImageSet("TEDxTokyo").pointer();
		dev->useTexture(0, tedxTokyoSet->GetTexture());
		video::ImageRegion* upA= tedxTokyoSet->GetRegion("UpArrow");
		video::ImageRegion* downA = tedxTokyoSet->GetRegion("DownArrow");
		
		float sz = r->GetClippedRect().getWidth()*0.8f;
		float szH = r->GetClippedRect().getHeight();
		float y = r->GetClippedRect().ULPoint.y+szH*0.5f;
		float x = r->GetClippedRect().ULPoint.x+ sz*0.5f;
		rc.ULPoint.x = x - sz*0.5f;
		rc.BRPoint.x = x + sz*0.5f;

		float r = upA->GetSrcRect().getHeight() / upA->GetSrcRect().getWidth();

		float offY = 50+ szH*0.5*m_szT*0.5;
		rc.ULPoint.y = y - r*sz - offY;
		rc.BRPoint.y = y - offY;

		dev->draw2DImage(rc, video::SColor(1, 1, 1, 1-m_szT),0,&upA->GetTextureRect());

		rc.ULPoint.y = y + offY;
		rc.BRPoint.y = y + r*sz + offY;

		dev->draw2DImage(rc, video::SColor(1, 1, 1, 1 - m_szT), 0, &downA->GetTextureRect());
	}


	if (m_szT > 0)
	{
		video::IGPUShaderProgram *shader = (video::IGPUShaderProgram*)gShaderResourceManager.getResource("ProfileRendering").pointer();
		video::GPUUniform* alphaU = shader->getUniform("alpha");
		video::GPUUniform* grauU = shader->getUniform("gray");
		if (shader)
			dev->setFragmentShader(shader);
		float sz = r->GetClippedRect().getWidth()*0.6f;

		float x = r->GetClippedRect().ULPoint.x + r->GetClippedRect().getWidth()*0.5;
		float step = r->GetClippedRect().getHeight() / ((float)m_projects.size() + 1);
		float y = step;
		const int selectedSpacing = 20;
		for (int i = 0; i < m_projects.size(); ++i)
		{
			float dst = (float)abs(m_speakerTransition - i) / (float)m_projects.size();
			tex.SetTexture(m_projects[i].texture);
			dev->useTexture(0, &tex);

			if (i == m_currentSpeaker)
				y += selectedSpacing;
			float t = (powf(1 - dst,2.0f)*0.6+ 0.4);;
			rc.ULPoint.y = y - sz*0.5f*t;
			rc.BRPoint.y = y + sz*0.5f*t;

			rc.ULPoint.x = x - sz*0.5*t;
			rc.BRPoint.x = x + sz*0.5*t;
			y += rc.getHeight();
			float a = m_szT* t;
			float g =  1-(i==m_currentSpeaker);//(dst != 0); //1-powf(1 - dst,10);//
			shader->setConstant(alphaU, &a, 1);
			shader->setConstant(grauU, &g, 1);
			dev->draw2DImage(rc, video::SColor(1, 1, 1, a));
			if (i == m_currentSpeaker)
				y += selectedSpacing;
		}
		if (shader)
			dev->setFragmentShader(0);
	}

	IGUIPanelElement::Draw(vp);

}

IMPLEMENT_ELEMENT_FACTORY(GUISessionSidePanel);

}
}
