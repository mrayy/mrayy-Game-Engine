

/********************************************************************
	created:	2014/03/24
	created:	24:3:2014   2:09
	filename: 	C:\Development\mrayEngine\Engine\mrayEngine\BitmapFontData.h
	file path:	C:\Development\mrayEngine\Engine\mrayEngine
	file base:	BitmapFontData
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __BitmapFontData__
#define __BitmapFontData__


#include "compileconfig.h"
#include "mTypes.h"
#include "XMLElement.h"

namespace mray
{
namespace GUI
{

struct FontCharAttr
{
public:
	FontCharAttr() :texID(0), c(0)
	{}

	uchar texID;//texture ID
	uint c;//character code
	math::rectf texcoords;
	math::vector2d rectSize;
	bool operator < (const FontCharAttr &o)const{ return c < o.c; }
};

class MRAY_DLL FontCharacterRange
{
	ushort m_min;
	ushort m_max;
	std::vector<FontCharAttr> m_characters;
public:

	FontCharacterRange()
	{
		m_min = 0;
		m_max = 0;
	}

	ushort GetMin()const{ return m_min; }
	ushort GetMax()const{ return m_max; }
	void SetMinMax(int min, int max)
	{
		m_min = min;
		m_max = max;
		if (m_min > m_max)
		{
			m_min = m_max = 0;
			return;
		}
		m_characters.resize(m_max - m_min + 1);
	}

	inline bool IsInRange(uint c){ return (c >= m_min && c <= m_max); }
	inline FontCharAttr* GetCharacterInfo(uint c)
	{
		if (c<m_min || c>m_max)
			return 0;
		return &m_characters[c - m_min];
	}

	void LoadFromXml(xml::XMLElement* e);

};
}
}


#endif
