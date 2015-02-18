
/********************************************************************
	created:	2009/07/29
	created:	29:7:2009   19:08
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\BaseFont.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	BaseFont
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___BaseFont___
#define ___BaseFont___


#include "IFont.h"
#include "IStream.h"
#include "TextureUnit.h"
#include "IVideoDevice.h"
#include "BitmapFontData.h"


namespace mray{
namespace GUI{

	class LanguageFontSpecification;
	class TextDecorator;
	//class GUIMultiQuadRenderOperation;

class MRAY_DLL BaseFont:public IFont
{
private:
protected:

	TextDecorator* m_decorator;

	std::vector<video::TextureUnitPtr>  m_textures;
	core::string attrFileName;

	//typedef std::map<ushort,SCharAttr> CharAttrMap; 
	typedef std::vector<FontCharacterRange*> CharacterRangesList;
	CharacterRangesList m_charsAttr;
/*
	std::vector<math::rectf> m_texRects;
	std::vector<math::vector2d> m_rectsSize;*/
	std::vector<uint> m_tmpIndices;
	std::vector<uint> m_printBuffer;

	math::Point2df CharSZ;

	virtual uint calcSizeInternal();

	FontCharAttr* GetCharacterInfo(uint c);

	//GUIMultiQuadRenderOperation*m_renderOperation;


	void print(const math::rectf& pos,FontAttributes*attributs,const  uint* txt,IGUIRenderer*renderer);
public:
	BaseFont(const core::string&name);
	virtual~BaseFont();

	const video::TextureUnitPtr&  getTexture(uint i);

	virtual void print(const math::rectf& pos,FontAttributes*attributs,const math::rectf*clip,const core::UTFString& txt,IGUIRenderer*renderer);

	virtual int getCharFromPos(const math::vector2d& p,FontAttributes*attributs,const math::rectf &rc,const core::UTFString& txt);
	virtual math::Point2df getCharPos(int len,FontAttributes*attributs,const math::rectf &rc,const core::UTFString& txt);
	virtual math::Point2df getTextDimension(const  core::UTFString& txt,FontAttributes*attributs,const math::rectf *rc);
	virtual math::Point2df getCharDimension( uint c,int size);

};

}
}


#endif //___BaseFont___
