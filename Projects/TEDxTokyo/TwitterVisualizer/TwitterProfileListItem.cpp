

#include "stdafx.h"
#include "TwitterProfileListItem.h"
#include "TwitterUserProfile.h"
#include "IGUIListBox.h"
#include "IGUIRenderer.h"
#include "IFOnt.h"
#include "TextureResourceManager.h"
#include "ShaderResourceManager.h"

namespace mray
{
namespace GUI
{


TwitterProfileListItem::TwitterProfileListItem(ted::TwitterUserProfile* user)
{
	m_user = user;
	m_profilePic= gTextureResourceManager.loadTexture2D("url=" + user->imageUrl);
	m_profilePic->setMipmapsFilter(false);
}

TwitterProfileListItem::~TwitterProfileListItem()
{
}


core::UTFString TwitterProfileListItem::toString()const
{
	if (!m_user)
		return core::UTFString::Empty;
	return m_user->displayName;
}


void TwitterProfileListItem::Draw(const math::rectf& rc, IGUIElement* lstBox, IFont* font, IGUIRenderer* renderer)
{
	math::rectf r = rc;
	r.BRPoint.x = r.ULPoint.x+ r.getHeight();

	video::IVideoDevice* dev= renderer->GetDevice();

	video::TextureUnit tex1, tex2;
	tex1.SetTexture(m_profilePic);
	tex2.SetTexture(gTextureResourceManager.loadTexture2D("profileMask32.png"));

	video::IGPUShaderProgramPtr maskingShader = gShaderResourceManager.getResource("ProfileMasking");
	if (maskingShader)
	{
		dev->setFragmentShader(maskingShader);
		maskingShader->setTexture("texA", &tex1);
		maskingShader->setTexture("texB", &tex2);
		dev->useTexture(0, &tex1);
		dev->useTexture(1, &tex2);
		dev->draw2DImage(r, 1);
		dev->setFragmentShader(0);

		dev->useTexture(1, 0);
	}
	else
	{
		dev->useTexture(0, &tex1);
		dev->draw2DImage(r, 1);

	}

	r.ULPoint.x += r.getHeight();
	r.BRPoint.x = rc.BRPoint.x;
	font->print(r, lstBox->GetFontAttributes(), &rc, toString(), renderer);
}


}
}

