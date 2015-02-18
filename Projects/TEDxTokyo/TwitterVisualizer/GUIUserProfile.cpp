

#include "stdafx.h"
#include "GUIUserProfile.h"
#include "TwitterUserProfile.h"
#include "TextureResourceManager.h"
#include "Engine.h"
#include "IVideoDevice.h"
#include "IGUIManager.h"
#include "TextureUnit.h"
#include "IGUIRenderer.h"
#include "FontResourceManager.h"
#include "GUIElementRegion.h"
#include "ShaderResourceManager.h"

namespace mray
{
namespace GUI
{

	IMPLEMENT_RTTI(GUIUserProfile, IGUIElement);
	IMPLEMENT_ELEMENT_FACTORY(GUIUserProfile);

	const core::string Maskingshader =
		"float4 main_fp(float2 texCoord : TEXCOORD0, \
				uniform sampler2D texA : register(s0),uniform sampler2D texB : register(s1)) : COLOR \
		{\
			vec4 clr=tex2D(texB,texCoord);\
			return float4(tex2D(texA,texCoord).rgb+clr.rgb,clr.a);\
		}";

	const GUID GUIUserProfile::ElementType("UserProfile");

GUIUserProfile::GUIUserProfile(GUI::IGUIManager* creator):
	IGUIElement(ElementType,creator)
{
	m_user = 0;
	m_profilePic = 0;

	m_maskingShader = gShaderResourceManager.getResource("ProfileMasking");
	if (!m_maskingShader)
	{
		m_maskingShader = gShaderResourceManager.loadShaderFromProgram("ProfileMasking",Maskingshader, video::EShader_FragmentProgram, "main_fp", video::ShaderPredefList(), mT("cg"));
		m_maskingShader->setResourceName("ProfileMasking");
		gShaderResourceManager.addResource(m_maskingShader, "ProfileMasking");
	}

	m_openAnimation = 0;
}
GUIUserProfile::~GUIUserProfile()
{

}

void GUIUserProfile::SetUser(ted::TwitterUserProfile* u)
{
	m_user = u;
	if (m_user == 0)
	{
		m_profilePic = 0;
	}
	else
	{
		m_profilePic = gTextureResourceManager.loadTexture2D("url=" + u->imageUrlHi);
		m_profilePic->setMipmapsFilter(false);
	}
}
ted::TwitterUserProfile* GUIUserProfile::GetUser()
{
	return m_user;
}


void GUIUserProfile::Update(float dt)
{
	IGUIElement::Update(dt);

	if (!m_user)
	{
		if (m_openAnimation > 0)
		{
			m_openAnimation -= dt;
			if (m_openAnimation < 0)m_openAnimation = 0;
		}
	}else
	{
		if (m_openAnimation < 1)
		{
			m_openAnimation += dt;
			if (m_openAnimation >1)m_openAnimation = 1;
		}
	}
}
void GUIUserProfile::Draw(const math::rectf*vp)
{
	video::IVideoDevice *dev= Engine::getInstance().getDevice();

	GUI::IGUIManager* creator = GetCreator();

	const math::rectf& rc = GetDefaultRegion()->GetRect();
	const math::rectf& clip = GetDefaultRegion()->GetClippedRect();

	math::rectf rect;
	rect.ULPoint = rc.ULPoint;
	rect.BRPoint.x = rect.ULPoint.x + rc.getWidth()*m_openAnimation;
	rect.BRPoint.y =  rc.BRPoint.y;
	math::rectf bgR (rect);
	math::rectf picR(rect);
	picR.BRPoint = picR.ULPoint + picR.getHeight();
	bgR.ULPoint.x += bgR.getHeight() / 2;

	video::TextureUnit tex1,tex2;

	IFont* font = gFontResourceManager.getFontByName("Default");
	if (!font)
		font = gFontResourceManager.getDefaultFont();

	math::rectf oldScissor = GetCreator()->GetDevice()->getScissorRect();
	dev->setScissorRect(rc);
	tex1.SetTexture(gTextureResourceManager.loadTexture2D("profileBG.png"));
	dev->useTexture(0, &tex1);
	dev->draw2DImage(bgR, 1);

	bgR.ULPoint.x = picR.BRPoint.x;
	
	if (m_openAnimation == 1)
	{
		if (font && m_user)
		{
			m_fontAttrs.fontAligment = GUI::EFA_MiddleCenter;
			m_fontAttrs.fontColor.Set(1, 1, 1, 1);
			m_fontAttrs.hasShadow = false;
			m_fontAttrs.fontSize = 24;
			math::rectf textRc = bgR;	//Name
			math::rectf tmpRc;
			textRc.BRPoint = textRc.ULPoint + math::vector2d(bgR.getWidth()*0.75, bgR.getHeight() / 2);
			font->print(textRc, &m_fontAttrs, &clip, m_user->displayName, creator->GetRenderQueue());

			//draw Tweets
			tex1.SetTexture(gTextureResourceManager.loadTexture2D("twitter.png"));
			tmpRc.ULPoint.x = textRc.BRPoint.x;
			tmpRc.ULPoint.y = textRc.ULPoint.y + 10;
			tmpRc.BRPoint = tmpRc.ULPoint + 20;
			dev->useTexture(0, &tex1);
			dev->draw2DImage(tmpRc, 1);

			//Tweets count
			m_fontAttrs.fontSize = 14;
			m_fontAttrs.fontAligment = GUI::EFA_MiddleLeft;
			textRc.ULPoint.x = tmpRc.BRPoint.x;
			textRc.ULPoint.y = tmpRc.ULPoint.y;
			textRc.BRPoint.x = rect.BRPoint.x;
			textRc.BRPoint.y = tmpRc.BRPoint.y;
			textRc.BRPoint = textRc.ULPoint + math::vector2d(bgR.getWidth(), bgR.getHeight() / 2);
			font->print(textRc, &m_fontAttrs, &clip, core::StringConverter::toString(m_user->Tweets.size()), creator->GetRenderQueue());

			m_fontAttrs.fontSize = 18;
			textRc = bgR;		//URL
			textRc.ULPoint.y += textRc.getHeight() / 2;
			font->print(textRc, &m_fontAttrs, &clip, m_user->URL, creator->GetRenderQueue());
		}
	}
	GetCreator()->GetRenderQueue()->Flush();



	tex1.SetTexture(m_profilePic);
	tex2.SetTexture(gTextureResourceManager.loadTexture2D("profileMask.png"));

	dev->setFragmentShader(m_maskingShader);
	m_maskingShader->setTexture("texA", &tex1);
	m_maskingShader->setTexture("texB", &tex2);
	dev->useTexture(0, &tex1);
	dev->useTexture(1, &tex2);
	dev->draw2DImage(picR, 1);
	dev->setFragmentShader(0);

	dev->useTexture(1,0);

	dev->setScissorRect(oldScissor);


}

}
}

