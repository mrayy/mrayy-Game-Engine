

#include "stdafx.h"
#include "DynamicFontGenerator.h"
#include "Engine.h"
#include "IVideoDevice.h"

#include <windows.h>
#include <windowsx.h>
#include <algorithm>

#include "BinPacker.h"

namespace mray
{
namespace GUI
{

	class DynamicFontGeneratorImpl
	{
	public:
		HDC dc;
		HFONT font;
		bool fontDirty;
		DynamicFontGeneratorImpl()
		{

			fontDirty = false;
			dc = GetDC(0);
			font = 0;
		}
		~DynamicFontGeneratorImpl()
		{
			if (font)
				DeleteFont(font);
		}

		void Recreate(int res, bool bold, bool italic, bool underline, const core::stringw& fontName)
		{
			if (font)
				DeleteFont(font);
			font = CreateFontW(
				res,
				0, 0, 0,
				bold ? FW_BOLD : 0,
				italic,
				underline,
				0,
				ANSI_CHARSET | ARABIC_CHARSET,
				OUT_DEFAULT_PRECIS,
				CLIP_DEFAULT_PRECIS,
				DEFAULT_QUALITY,
				DEFAULT_PITCH | FF_SWISS,
				fontName.c_str());
			fontDirty = false;
		}
	};

DynamicFontGenerator::DynamicFontGenerator(const core::string& name)
:BaseFont(name)
{
	m_data = new DynamicFontGeneratorImpl;
	m_dirty = false;
	m_underline = false;
	m_bold = false;
	m_italic = false;
	m_textureSize = 4096;
	AddCharacterRange('A', 'Z');
	AddCharacterRange('a', 'z');
	AddCharacterRange('0', '9');
}

DynamicFontGenerator::~DynamicFontGenerator()
{
	delete m_data;
}

void DynamicFontGenerator::Init()
{
	m_texture = Engine::getInstance().getDevice()->createEmptyTexture2D(true);
	m_texture->setMipmapsFilter(false);
	video::TextureUnit*t = new video::TextureUnit();
	t->SetTexture(m_texture);
	m_textures.push_back(t);
}

void DynamicFontGenerator::SetTextureSize(const math::vector2di& size)
{
	m_textureSize = size;
	m_dirty = true;
}
const math::vector2di& DynamicFontGenerator::GetTextureSize()
{
	return m_textureSize;
}

void DynamicFontGenerator::SetFontName(const core::stringw& name)
{
	
	m_fontName = name;
	//m_name = core::wchar_to_string(name.c_str());
	m_dirty = true;
	m_data->fontDirty = true;
}
const core::stringw& DynamicFontGenerator::GetFontName()
{
	return m_fontName;
}

void DynamicFontGenerator::SetFontResolution(int res)
{
	m_fontResolution = res;
	m_dirty = true;
	m_data->fontDirty = true;
}
int  DynamicFontGenerator::GetFontResolution()
{
	return m_fontResolution;
}
bool DynamicFontGenerator::AddCharacterRange(uint minCode, uint maxCode)
{
	bool newChar = false;
	for (wchar_t i = minCode; i < maxCode; ++i)
	{
		if (m_chars.find(i) == m_chars.end())
		{
			newChar = true;
			break;;
		}
	}
	if (!newChar)
		return false;
	m_dirty = true;
	for (wchar_t i = minCode; i < maxCode; ++i)
	{
		m_chars.insert(i);
	}
	return true;
	
}
bool DynamicFontGenerator::AddString(const core::UTFString& text)
{
	const utf32* str = text.c_str();
	bool newChar = false;
	for (int i = 0; i < text.Length(); ++i)
	{
		if (m_chars.find(str[i]) == m_chars.end())
		{
			newChar = true;
			break;;
		}
	}
	if (!newChar)
		return false;
	m_dirty = true;
	for (int i = 0; i < text.Length();++i)
	{
		m_chars.insert(str[i]);
	}
	return true;
}
void DynamicFontGenerator::ClearAll()
{
	m_chars.clear();
	m_dirty = true;
}
void DynamicFontGenerator::GenerateFont()
{
	if (!m_dirty)
		return;

	m_dirty = false;
	m_texture->createTexture(math::vector3di(m_textureSize.x,m_textureSize.y, 1), video::EPixel_Alpha8);

	bool m_mask = true;

	if (m_data->fontDirty)
	{
		m_data->Recreate(m_fontResolution, m_bold, m_italic,m_underline, m_fontName);
	}


	HBITMAP bmp = CreateCompatibleBitmap(m_data->dc, m_textureSize.x, m_textureSize.y);
	HDC bmpdc = CreateCompatibleDC(m_data->dc);

	HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));
	HPEN pen = CreatePen(PS_NULL, 0, RGB(0, 0, 0));

	HGDIOBJ oldbmp = SelectObject(bmpdc, bmp);
	HGDIOBJ oldbmppen = SelectObject(bmpdc, pen);
	HGDIOBJ oldbmpbrush = SelectObject(bmpdc, brush);
	HGDIOBJ oldbmpFont = SelectObject(bmpdc, m_data->font);


	SetBkMode(bmpdc, OPAQUE);
	SetBkColor(bmpdc, RGB(0,0,0));
	SetTextColor(bmpdc, RGB(255, 255, 255));
	Rectangle(bmpdc, 0, 0, m_textureSize.x, m_textureSize.y);

	core::stringw text = L"";

	// prepare characters 
	std::vector<wchar_t> sorted;
	std::vector<math::vector2d> rectSize;
	std::vector< core::BinPackerOutRect > packs;
	{
		for (int i = 0; i < m_charsAttr.size(); ++i)
			delete m_charsAttr[i];
		m_charsAttr.clear();
		FontCharacterRange* range = 0;
		std::set<wchar_t>::iterator it = m_chars.begin();
		sorted.resize(m_chars.size());
		std::vector<int> rangeMap;
		wchar_t lastID = -1;
		wchar_t minID = -1;
		int firstIdx = 0;
		
		for (int i=0; it != m_chars.end(); ++it,++i)
		{
			sorted[i] = *it;
		}

		std::sort(sorted.begin(), sorted.end());
		for (int i = 0; i < sorted.size();++i)
		{
			wchar_t c = sorted[i];
			if (lastID == -1)
			{
				minID = c;
				lastID = c;
			}
			else
			{
				if (c!=lastID+1)
				{
					if (!range)
					{
						range = new FontCharacterRange();
					}
					range->SetMinMax(minID,lastID);
					m_charsAttr.push_back(range);
					range = new FontCharacterRange();

					minID = c;
					firstIdx = i;
				}
				lastID = c;
				rangeMap.push_back(m_charsAttr.size());
			}
			text.append(1,c);
			//calculate character size
			SIZE sz;
			GetTextExtentPoint32W(bmpdc, &c, 1, &sz);

			rectSize.push_back(math::vector2d(sz.cx, sz.cy));
		}
		if (range != 0){
			range->SetMinMax(minID, lastID);
			m_charsAttr.push_back(range);
		}

		core::BinPacker packer;
		packer.Pack(rectSize, packs, math::vector2d(m_textureSize.x, m_textureSize.y), false);
		math::vector2d invSize(1.0f / m_textureSize.x, 1.0f / m_textureSize.y);
		float invRes = 1.0f / (float)m_fontResolution;
		for (int i = 0; i < packs.size();++i)
		{
			int id = packs[i].ID;
			FontCharacterRange* range= m_charsAttr[rangeMap[id]];
			FontCharAttr* attr = range->GetCharacterInfo(sorted[id]);
			attr->rectSize = rectSize[id] * invRes;
			attr->texID = 0;
			attr->texcoords.ULPoint = packs[i].pos*invSize;
			attr->texcoords.BRPoint = attr->texcoords.ULPoint + rectSize[id] * invSize;

		}
	}
	for (int i = 0; i < packs.size(); ++i)
	{
		core::BinPackerOutRect& p = packs[i];
		TextOutW(bmpdc, p.pos.x, p.pos.y, &sorted[p.ID], 1);

	}

	BITMAP bmpInfo;

	//get image info
	GetObject(bmp, sizeof(BITMAP), &bmpInfo);
	uchar *buffer;
	int pixel = m_mask ? 4 : 3;

	int line = (bmpInfo.bmWidth*pixel + pixel) & 0xfffffffc;
	int dataSize = line*(bmpInfo.bmHeight);
	//get image data
	buffer = new uchar[dataSize];

	video::IHardwarePixelBuffer* pixBuff= m_texture->getSurface(0);
	video::LockedPixelBox lbox = pixBuff->lock(math::box3d(0, 0, 0, m_textureSize.x, m_textureSize.y, 1), video::IHardwareBuffer::ELO_Discard);
	uchar* dstPixels = (uchar*)lbox.data;

	GetBitmapBits(bmp, dataSize, buffer);
	for (int i = 0; i < m_textureSize.x*m_textureSize.y; ++i)
	{
		dstPixels[i] = buffer[i * 4];
	}
	pixBuff->unlock();

	delete[] buffer;

	SelectObject(bmpdc, oldbmp);
	SelectObject(bmpdc, oldbmppen);
	SelectObject(bmpdc, oldbmpbrush);


	DeleteDC(bmpdc);
	DeleteObject(brush);
	DeleteObject(pen);
	DeleteObject(bmp);
}

void DynamicFontGenerator::print(const math::rectf& pos, FontAttributes*attributs, const math::rectf*clip, const core::UTFString& txt, IGUIRenderer*renderer)
{
	AddString(txt);
	GenerateFont();
	BaseFont::print(pos, attributs, clip, txt, renderer);
}

}
}

