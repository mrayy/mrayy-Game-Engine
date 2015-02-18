
#include "stdafx.h"
#include "IGUIButton.h"

namespace mray
{
namespace GUI
{


	const GUID IGUIButton::ElementType("GUIButton");

	IGUIButton::IGUIButton(IGUIManager* creator):IGUIElement(ElementType,creator),
		OnKeyDown(mT("OnKeyDown")),OnClick(mT("OnClick"))
	{
		AddEvent(&OnKeyDown);
		AddEvent(&OnClick);
	}
	IGUIButton::~IGUIButton()
	{
	}


}
}