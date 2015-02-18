#include "stdafx.h"


#include "TexFont.h"
#include "TextureResourceManager.h"

#include "LanguageFontSpecification.h"

namespace mray{
namespace GUI{

TexFont::TexFont(const core::string &name):BaseFont(name)
{

}

TexFont::~TexFont()
{
	CharacterRangesList::iterator it = m_charsAttr.begin();
	for(;it!=m_charsAttr.end();++it)
		delete *it;

}

void TexFont::LoadFont(xml::XMLElement* elem)
{
	xml::XMLAttribute* attr;
	/*
	attr=elem->getAttribute("LanguageSpecification");
	if(attr)
	{
		m_specifications->Load(attr->value);
	}*/

	xml::xmlSubElementsMapIT it=elem->getElementsBegin();
	xml::xmlSubElementsMapIT end=elem->getElementsEnd();
	for (;it!=end;++it)
	{
		if((*it)->GetType()!=xml::ENT_Element)continue;
		xml::XMLElement*e=dynamic_cast<xml::XMLElement*>(*it);
		if(e->getName().equals_ignore_case(mT("FontCharacterRange")))
		{
			FontCharacterRange* range=new FontCharacterRange();
			range->LoadFromXml(e);
			m_charsAttr.push_back(range);
		}else if(e->getName().equals_ignore_case(mT("Texture")))
		{
			int id=0;
			video::ITexturePtr  tex;
			attr=e->getAttribute(mT("ID"));
			if(attr)
				id=core::StringConverter::toInt(attr->value);
			attr=e->getAttribute(mT("Name"));
			if(attr)
				tex=gTextureResourceManager.loadTexture2D(attr->value);
			if(!tex || tex->getType()!=video::ETT_2DTex)
				tex=0;
			else{
				video::TextureUnit*t=new video::TextureUnit();
				tex->setMipmapsFilter(false);
				t->SetTexture(tex);
				tex->load(false);
				m_textures.resize(id+1);
				m_textures[id]=t;
			}
		}
	}
}

#if 0

void TexFont::loadFont(OS::IStream* file){

	attrFileName=file->getStreamName();
	core::string texname;

	OS::StreamReader reader(file);
	texname=reader.readString();
	video::ITexturePtr  tex=gTextureResourceManager.loadTexture2D(texname);
	if(!tex || tex->getType()!=video::ETT_2DTex)
		tex=0;
	else
		m_texture->SetTexture(tex);

	
	int cnt;
	fontAttr.baseTextureSize.x=reader.txtReadInt();
	fontAttr.baseTextureSize.y=reader.txtReadInt();
	cnt=reader.txtReadInt();
	math::vector2d p;
	if(m_texture->GetTexture())
		p=math::vector2di(m_texture->GetTexture()->getSize().x,m_texture->GetTexture()->getSize().y);
	else 
		p=fontAttr.baseTextureSize;
	math::vector2d invSize(1.0f/p.x,1.0f/p.y);
	math::vector2d invSize2(1.0f/(float)fontAttr.baseTextureSize.x,1.0f/(float)fontAttr.baseTextureSize.y);
	CharSZ=0;
	float maxY=0;

	int MCHAR_MAX=(1<<(8*sizeof(mchar)))-1;

	for(int i=0;i<cnt;++i){
		uint c=reader.txtReadInt();
		SFontAttr::SCharAttr& attrs=fontAttr.charsAttr[c];
		attrs.c=( mchar)(c&MCHAR_MAX);;
		attrs.sourceRect.ULPoint.x=reader.txtReadInt();
		attrs.sourceRect.ULPoint.y=reader.txtReadInt();
		attrs.sourceRect.BRPoint=attrs.sourceRect.ULPoint;
		attrs.sourceRect.BRPoint.x+=reader.txtReadInt();
		attrs.sourceRect.BRPoint.y+=reader.txtReadInt();
		attrs.sourceRect.ULPoint*=invSize2*p;
		attrs.sourceRect.BRPoint*=invSize2*p;

		attrs.texcoords=attrs.sourceRect;
		attrs.texcoords.ULPoint*=invSize;
		attrs.texcoords.BRPoint*=invSize;
		attrs.rectSize=attrs.sourceRect.getSize();
		/*
		m_texRects[ch]=fontAttr.charsAttr[ch].texcoords;
		m_rectsSize[ch]=fontAttr.charsAttr[ch].sourceRect.getSize();//*fontAttr.charsAttr[i].texcoords.getlength();
*/
		CharSZ.y=math::Max(CharSZ.y,attrs.texcoords.getHeight());
		CharSZ.x=math::Max(CharSZ.x,attrs.texcoords.getWidth());

		maxY=math::Max(maxY,attrs.rectSize.y);

	}
	std::map<uint,SFontAttr::SCharAttr>::iterator it=fontAttr.charsAttr.begin();
	float imaxY=1.0f/maxY;
	for(;it!=fontAttr.charsAttr.end();++it){
//		fontAttr.charsAttr[i].texcoords.BRPoint.y=fontAttr.charsAttr[i].texcoords.ULPoint.y+CharSZ.y;
		it->second.rectSize.x*=imaxY;
		it->second.rectSize.y=1;
	}

	//after we build the font , we sort fontAttr array
	//fontAttr.charsAttr.sort();
}
void TexFont::loadFont(const  core::string&  file)
{
	OS::FILE_MODE mode;
	mode=OS::TXT_READ;

	OS::IStreamPtr fin=gFileSystem.openFile(file,mode);
	if(!fin)return;
	loadFont(fin);
	fin->close();
}

#endif

}
}

