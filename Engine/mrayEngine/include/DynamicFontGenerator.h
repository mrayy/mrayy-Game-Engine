

/********************************************************************
	created:	2014/03/24
	created:	24:3:2014   1:15
	filename: 	C:\Development\mrayEngine\Projects\TEDxTokyo\TwitterVisualizer\DynamicFontGenerator.h
	file path:	C:\Development\mrayEngine\Projects\TEDxTokyo\TwitterVisualizer
	file base:	DynamicFontGenerator
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __DynamicFontGenerator__
#define __DynamicFontGenerator__


#include "BaseFont.h"
#include <set>

namespace mray
{
namespace GUI
{

	class DynamicFontGeneratorImpl;
class MRAY_DLL DynamicFontGenerator :public BaseFont
{
protected:

	DynamicFontGeneratorImpl* m_data;
	math::vector2di m_textureSize; 
	video::ITexturePtr m_texture;

	//std::vector<FontCharacterRange*> m_fontRange;
	std::set<wchar_t> m_chars;
	bool m_dirty;
	core::stringw m_fontName;
	int m_fontResolution;
	bool m_bold;
	bool m_underline;
	bool m_italic;
public:
	DynamicFontGenerator(const core::string& name);
	virtual~DynamicFontGenerator();

	void Init();

	void SetBold(bool b){ m_bold = b; }
	bool GetBold(){ return m_bold; }
	void SetUnderline(bool b){ m_underline = b; }
	bool GetUnderline(){ return m_underline; }
	void SetItalic(bool b){ m_italic = b; }
	bool GetItalic(){ return m_italic; }

	void SetTextureSize(const math::vector2di& size);
	const math::vector2di& GetTextureSize();

	void SetFontName(const core::stringw& name);
	const core::stringw& GetFontName();

	void SetFontResolution(int res);
	int  GetFontResolution();



	bool AddCharacterRange(uint minCode, uint maxCode);
	bool AddString(const core::UTFString& text);
	void ClearAll();


	void GenerateFont();
	virtual void print(const math::rectf& pos, FontAttributes*attributs, const math::rectf*clip, const core::UTFString& txt, IGUIRenderer*renderer);


	video::ITexturePtr GetTexture(){ return m_texture; }
};

}
}


#endif
