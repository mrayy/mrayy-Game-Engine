

#include "stdafx.h"
#include "IListItem.h"
#include "FontAttributes.h"
#include "IGUIElement.h"
#include "IFOnt.h"



namespace mray
{
namespace GUI
{

	void StringListItem::Draw(const math::rectf& rc,IGUIElement* lstBox, IFont* font, IGUIRenderer* renderer)
	{
		font->print(rc,lstBox->GetFontAttributes(), &rc, toString(), renderer);

	}
	/*
	float StringListItem::GetHeight(IGUIElement* lstBox)
	{
		if (!lstBox->GetFontAttributes())
			return 20;
		return lstBox->GetFontAttributes()->fontSize;
	}*/

}
}