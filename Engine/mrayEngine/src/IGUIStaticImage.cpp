

#include "stdafx.h"
#include "IGUIStaticImage.h"


namespace mray
{
namespace GUI
{


const GUID IGUIStaticImage::ElementType("GUIStaticImage");

IGUIStaticImage::IGUIStaticImage(IGUIManager* creator):IGUIElement(ElementType,creator),
	OnClick(mT("OnClick")),OnMouseMove(mT("OnMouseMove"))
{
	AddEvent(&OnClick);
	AddEvent(&OnMouseMove);
}
IGUIStaticImage::~IGUIStaticImage()
{
}


}
}