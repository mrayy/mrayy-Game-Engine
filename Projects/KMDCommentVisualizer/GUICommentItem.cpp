

#include "stdafx.h"
#include "GUICommentItem.h"
#include "KMDComment.h"

#include "IGUIManager.h"
#include "GUIElementRegion.h"
#include "TextureResourceManager.h"
#include "FontResourceManager.h"
#include "IGUIRenderer.h"
#include "AppData.h"

namespace mray
{
namespace GUI
{

	IMPLEMENT_RTTI(GUICommentItem,IGUIElement);
	IMPLEMENT_ELEMENT_FACTORY(GUICommentItem);

	const GUID GUICommentItem::ElementType("CommentItem");

	GUICommentItem::GUICommentItem(IGUIManager* m) :IGUIElement(ElementType, m)
	{
		m_Comment = 0;
	}
	GUICommentItem::~GUICommentItem()
	{

	}

	void GUICommentItem::SetComment(kmd::KMDComment* t)
	{
		m_Comment = t;
	}


	void GUICommentItem::Update(float dt)
	{
		IGUIElement::Update(dt);
	}
	void GUICommentItem::Draw(const math::rectf*vp)
	{
		if (!IsVisible())
			return;
		video::IVideoDevice *dev = Engine::getInstance().getDevice();

		GUI::IGUIManager* creator = GetCreator();

		const math::rectf& rect = GetDefaultRegion()->GetRect();
		const math::rectf& clip = GetDefaultRegion()->GetClippedRect();

		math::rectf bgR(rect);

		video::TextureUnit tex1;

		IFont* font = gFontResourceManager.getFontByName("Default");
		if (!font)
			font = gFontResourceManager.getDefaultFont();

		math::rectf oldScissor = GetCreator()->GetDevice()->getScissorRect();
		dev->setScissorRect(clip);
		tex1.SetTexture(gTextureResourceManager.loadTexture2D("CommentBG.png"));
		dev->useTexture(0, &tex1);
		dev->draw2DImage(bgR, 1);

		if (font && m_Comment)
		{
			math::rectf tmpRc;

			tex1.SetTexture(gTextureResourceManager.loadTexture2D("twitter.png"));
			tmpRc.ULPoint= bgR.ULPoint+20;
			tmpRc.BRPoint = tmpRc.ULPoint + 30;
			dev->useTexture(0, &tex1);
			dev->draw2DImage(tmpRc, 1);
			tmpRc.ULPoint = tmpRc.BRPoint;
			tmpRc.BRPoint = bgR.BRPoint - 60;

			m_fontAttrs.fontAligment = GUI::EFA_TopLeft;
			m_fontAttrs.fontColor.Set(1, 1, 1, 1);
			m_fontAttrs.hasShadow = false;
			m_fontAttrs.spacing = 0;
			m_fontAttrs.lineSpacing = 10;
			m_fontAttrs.wrap = true;
			m_fontAttrs.fontSize = core::StringConverter::toInt(gAppData.GetValue("UI", "CommentSize", "18"));
			font->print(tmpRc, &m_fontAttrs, &clip, m_Comment->text, creator->GetRenderQueue());

		}
		GetCreator()->GetRenderQueue()->Flush();


		dev->setScissorRect(oldScissor);
		IGUIElement::Draw(vp);
	}

}
}
