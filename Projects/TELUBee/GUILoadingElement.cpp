
#include "stdafx.h"
#include "GUILoadingElement.h"
#include "TextureResourceManager.h"
#include "IGUIManager.h"
#include "GUIElementRegion.h"


namespace mray
{
namespace GUI
{

	const GUID GUILoadingElement::ElementType("LoadingElement");

GUILoadingElement::GUILoadingElement(IGUIManager* mngr)
	:IGUIElement(ElementType,mngr)
{
	m_time=0;

	m_layer1.SetTexture(gTextureResourceManager.loadTexture2D("TBee\\Loading1.png"));
	m_layer2.SetTexture(gTextureResourceManager.loadTexture2D("TBee\\Loading2.png"));
	m_layer3.SetTexture(gTextureResourceManager.loadTexture2D("TBee\\Loading3.png"));
}
GUILoadingElement::~GUILoadingElement()
{
}

void GUILoadingElement::Draw(const math::rectf*vp)
{
	if(!IsVisible())return;
	IGUIManager* creator=GetCreator();
	const math::rectf& rect=GetDefaultRegion()->GetRect();
	const math::rectf& clip=GetDefaultRegion()->GetClippedRect();
	video::IVideoDevice*device=creator->GetDevice();
	device->useTexture(0,&m_layer1);
	device->draw2DImageRot(rect,rect.getSize()/2,video::SColor(1),20*m_time);
	device->useTexture(0,&m_layer2);
	device->draw2DImageRot(rect,rect.getSize()/2,video::SColor(1),0);
	device->useTexture(0,&m_layer3);
	device->draw2DImageRot(rect,rect.getSize()/2,video::SColor(1),-10*m_time);

	IGUIElement::Draw( vp);
}
void GUILoadingElement::Update(float dt)
{
	IGUIElement::Update(dt);
	m_time+=dt;
}

IMPLEMENT_ELEMENT_FACTORY(GUILoadingElement)

}
}

