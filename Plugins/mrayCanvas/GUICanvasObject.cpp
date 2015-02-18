

#include "stdafx.h"
#include "GUICanvasObject.h"
#include "CanvasManager.h"
#include "GUIElementRegion.h"

namespace mray
{
namespace GUI
{

	IMPLEMENT_RTTI(GUICanvasObject,IGUIElement);

const GUID GUICanvasObject::ElementType("CanvasObject");

GUICanvasObject::GUICanvasObject(IGUIManager*mngr)
:IGUIElement(ElementType,mngr),m_canvasManager(0)
{
}
GUICanvasObject::~GUICanvasObject()
{
	delete m_canvasManager;
}

void GUICanvasObject::Draw(video::IRenderArea*vp)
{
	if(!IsVisible())return;
	IGUIManager* creator=GetCreator();
	const math::rectf& clipRect=GetDefaultRegion()->GetClippedRect();

	if(!m_canvasManager)
		return;

	m_canvasManager->DrawObjects(clipRect);

	IGUIElement::Draw(vp);
}

IGUIElement* GUICanvasObject::Duplicate()
{
	return new GUICanvasObject(GetCreator());
}

}
}
