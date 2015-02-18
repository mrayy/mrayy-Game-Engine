


#include "stdafx.h"
#include "BitmapFontData.h"



namespace mray
{
namespace GUI
{


	void FontCharacterRange::LoadFromXml(xml::XMLElement* elem)
	{
		xml::XMLAttribute* attr;
		attr = elem->getAttribute(mT("Min"));
		if (attr)
			m_min = core::StringConverter::toInt(attr->value);
		attr = elem->getAttribute(mT("Max"));
		if (attr)
			m_max = core::StringConverter::toInt(attr->value);
		SetMinMax(m_min, m_max);
		xml::xmlSubElementsMapIT it = elem->getElementsBegin();
		xml::xmlSubElementsMapIT end = elem->getElementsEnd();
		float maxH = 0;
		for (; it != end; ++it)
		{
			if ((*it)->GetType() != xml::ENT_Element)continue;
			xml::XMLElement*e = dynamic_cast<xml::XMLElement*>(*it);
			if (e->getName().equals_ignore_case(mT("Char")))
			{
				attr = e->getAttribute(mT("C"));
				if (!attr)continue;
				ushort cCode = core::StringConverter::toUInt(attr->value);
				FontCharAttr* c = GetCharacterInfo(cCode);
				if (!c)continue;
				c->c = cCode;
				attr = e->getAttribute(mT("U"));
				if (attr)
					c->texcoords.ULPoint.x = core::StringConverter::toFloat(attr->value);
				attr = e->getAttribute(mT("V"));
				if (attr)
					c->texcoords.ULPoint.y = core::StringConverter::toFloat(attr->value);
				attr = e->getAttribute(mT("dU"));
				if (attr)
					c->texcoords.BRPoint.x = c->texcoords.ULPoint.x + core::StringConverter::toFloat(attr->value);
				float h = 0;
				attr = e->getAttribute(mT("dV"));
				if (attr)
					h = core::StringConverter::toFloat(attr->value);


				attr = e->getAttribute(mT("Tex"));
				if (attr)
					c->texID = (uchar)core::StringConverter::toInt(attr->value);
				else
					c->texID = 0;

				c->texcoords.BRPoint.y = c->texcoords.ULPoint.y + h;
				maxH = math::Max(maxH, h);
			}
		}
		if (maxH > 0){
			float imax = 1.0f / maxH;
			for (int i = 0; i < m_characters.size(); ++i)
			{
				FontCharAttr* c = &m_characters[i];
				c->rectSize = c->texcoords.getSize()*imax;
			}
		}
	}

}
}
