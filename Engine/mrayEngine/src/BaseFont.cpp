#include "stdafx.h"

#include "BaseFont.h"
#include <tchar.h>
#include <strsafe.h>
#include "IFileSystem.h"

#include "FontAttributes.h"
#include "LanguageFontSpecification.h"

#include "GUIBatchRenderer.h"
#include "GUIMultiQuadRenderOperation.h"
#include "TextDecorator.h"


namespace mray{
namespace GUI{



//////////////////////////////////////////////////////////////////////////

BaseFont::BaseFont(const core::string&name):IFont(name)
{
//	m_renderOperation=new GUIMultiQuadRenderOperation();

	m_decorator = new TextDecorator();
}
BaseFont::~BaseFont()
{
	m_tmpIndices.clear();
	//delete m_renderOperation;
	for (int i=0;i<m_charsAttr.size();++i)
	{
		delete m_charsAttr[i];
	}
	m_charsAttr.clear();

	delete m_decorator;
}

uint BaseFont::calcSizeInternal(){
	return m_charsAttr.size()*sizeof(FontCharAttr);
}
FontCharAttr* BaseFont::GetCharacterInfo(uint c)
{
	for (int i=0;i<m_charsAttr.size();++i)
	{
		FontCharAttr* ret=m_charsAttr[i]->GetCharacterInfo(c);
		if(ret)return ret;
	}
	return 0;
}

const video::TextureUnitPtr& BaseFont::getTexture(uint i)
{
	if(i>=m_textures.size())return video::TextureUnitPtr::Null;
	return m_textures[i];
}




math::vector2d BaseFont::getTextDimension(const  core::UTFString& txt,FontAttributes*attributs,const math::rectf *rc)
{
	float sz=(float)attributs->fontSize;
	math::vector2d dim;
	//core::string processed=m_specifications->ProcessText(txt);
	for(const  uint *p=txt.c_str();*p;++p)
	{
		FontCharAttr* attr=GetCharacterInfo(*p);
		if(!attr)continue;
		dim.x+=attr->rectSize.x;
		dim.y=math::Max(dim.y,attr->rectSize.y * sz);
	}
	dim.x*=sz;
	dim.x+=attributs->spacing*txt.Length();
	return dim;

	/*

	wchar_t tmpChr;
	mchar a0=0,
		a1=0,
		a2=0;
	int k=0;
	int n=0;
	for(const  mchar *p=txt.c_str();*p;p++,++k){
		if(k>0)
			a0=getUnicodeChar(*(p-1));
		else a0=0;
		a1=getUnicodeChar(*p);
		if(*(p+1)!=0)
			a2=getUnicodeChar(*(p+1));
		else a2=0;
		tmpChr=processChar(a0,a1,a2);
		if(tmpChr>m_rectsSize.size())
			continue;
		w+=m_rectsSize[tmpChr].x;
		n++;
		h=math::Max(h,m_rectsSize[fa.c].y);
	}

	dim.x=w*sz;
	dim.y=h*sz;
	dim.x+=n*spacing;
	return dim;*/
}

// math::vector2d BaseFont::getDimension(const char* txt,int size)
// {
// 	float sz=(float)size;
// 	float w=0;
// 	float iW=0;
// 	float h=0;
// 	int n=0;
// 	math::vector2d dim;
// 	FontCharAttr fa;
// 	for(const char *p=txt;*p;p++){
// 		fa.c=getUnicodeChar(*p);;
// 		int i=m_charsAttr.binary_search(fa);
// 		if(i==-1)continue;//unknown char
// 		w+=m_rectsSize[i].x;
// 		h=max_(h,m_rectsSize[i].y);
// 		n++;
// 	}
// 	dim.x=w*sz;
// 	dim.y=h*sz;
// 	dim.x+=n*m_spacing;
// 	return dim;
// }

math::vector2d BaseFont::getCharDimension( uint c,int size){

	FontCharAttr* attr= GetCharacterInfo(c);
	if(!attr)
		return math::vector2d::Zero;
	return attr->rectSize*(float)size;
	/*
	mchar cc=getUnicodeChar(c);;
	if(cc>m_rectsSize.size())
		return 0;;
	math::vector2d res=math::vector2d(m_rectsSize[cc]) * size;
	//	res.x+=m_spacing;
	return res;*/
}

// math::vector2d BaseFont::getDimension(char c,int size){
// 	return getDimension(getUnicodeChar(c),size);
// }



int BaseFont::getCharFromPos(const math::vector2d&cp,FontAttributes*attributs,const math::rectf &rc,const core::UTFString& txt)
{	
	math::vector2d pos(rc.ULPoint.x,rc.ULPoint.y);

	math::vector2d p(cp);

	p-=pos;
	int col=0,row=0;
	//	if(wrap){
	const uint*ptr=txt.c_str();
	int j=0;
	float w=0;
	int i=0;
	while(*ptr)
	{			
		w+=getCharDimension(*ptr,attributs->fontSize).x+attributs->spacing;
		if(p.x<w)
			return i;
		++ptr;
		++i;
	}
	return txt.Length();

	/*}
	if(cPos<0)return 0;
	return cPos;*/
}

math::vector2d BaseFont::getCharPos(int len,FontAttributes*attributs,const math::rectf &rc,const core::UTFString& txt)
{	
	math::vector2d pos(rc.ULPoint.x,rc.ULPoint.y);

	//	if(wrap){
	int tLen=txt.Length();
	math::vector2d sz;
	len=math::Min(len,tLen);
//	txt[len]=0;

	
	float fsz=(float)attributs->fontSize;
	//core::string processed=m_specifications->ProcessText(txt);
	for(uint i=0;i<len;++i)
	{
		FontCharAttr* attr=GetCharacterInfo(txt[i]);
		if(!attr)continue;
		sz.x+=attr->rectSize.x;
	//	sz.y=math::Max<float>(sz.y,attr->rectSize.y * fsz);
	}
	sz.x*=fsz;
	sz.x+=attributs->spacing*txt.Length();

	//sz.x=getTextDimension(txt,attributs,&rc).x;
	/*
	for(int i=0;i<tLen && i<len;++i)
	{
	math::vector2d dim=getDimension(txt[i],fontsize);
	sz.x+=dim.x;
	//	sz.y=max_(sz.y,dim.y);

	}*/
	return pos+sz;

	/*}
	if(cPos<0)return 0;
	return cPos;*/
}

void BaseFont::print(const math::rectf&  rc,FontAttributes*attributes,const math::rectf*clip,const core::UTFString& processed,IGUIRenderer*renderer)
{
	if(!attributes)return;
	if(attributes->fontSize<=0)
		attributes->fontSize=1 ;

	math::rectf pos=rc;

	if(clip)
		pos.clipRect(*clip);


	//core::string processed=m_specifications->ProcessText(txt);
	math::vector2d dim=getTextDimension(processed,attributes,&rc);

	if(!clip ){
		if(pos.BRPoint.x<pos.ULPoint.x+dim.x)
			pos.BRPoint.x=pos.ULPoint.x+dim.x;
		if(pos.BRPoint.y<pos.ULPoint.y+dim.y)
			pos.BRPoint.y=pos.ULPoint.y+dim.y;
	}
	math::vector2d rSize=pos.getSize();

	//TODO, fix using CharSZ and use fontWidth
	//calculate the rect
	switch (attributes->fontAligment)
	{
	case EFA_TopLeft:
	//	pos.ULPoint.x=pos.ULPoint.x;
	//	pos.ULPoint.y=pos.ULPoint.y;
		break;
	case EFA_MiddleLeft:
	//	pos.ULPoint.x=pos.ULPoint.x;
		pos.ULPoint.y+=(rSize.y-dim.y)*0.5;
		break;
	case EFA_BottomLeft:
	//	pos.ULPoint.x=pos.ULPoint.x;
		pos.ULPoint.y+=(rSize.y-dim.y);
		break;

	case EFA_TopCenter:
		pos.ULPoint.x+=(rSize.x-dim.x)*0.5;
	//	pos.ULPoint.y=pos.ULPoint.y;
		break;
	case EFA_MiddleCenter:
		pos.ULPoint.x+=(rSize.x-dim.x)*0.5;
		pos.ULPoint.y+=(rSize.y-dim.y)*0.5;
		break;
	case EFA_BottomCenter:
		pos.ULPoint.x+=(rSize.x-dim.x)*0.5;
		pos.ULPoint.y+=(rSize.y-dim.y);
		break;

	case EFA_TopRight:
		pos.ULPoint.x+=(rSize.x-dim.x);
	//	pos.ULPoint.y=pos.ULPoint.y;
		break;
	case EFA_MiddleRight:
		pos.ULPoint.x+=(rSize.x-dim.x);
		pos.ULPoint.y+=(rSize.y-dim.y)*0.5;
		break;
	case EFA_BottomRight:
		pos.ULPoint.x+=(rSize.x-dim.x);
		pos.ULPoint.y+=(rSize.y-dim.y);
		break;
	}
	//if(attributes->fontAligment!=EFA_MiddleCenter)
	//pos.BRPoint=pos.ULPoint+dim;
	//pos.BRPoint.y=pos.ULPoint.y+(CharSZ.y+1)*size;

	if(clip)
		pos.clipRect(*clip);
	if(!pos.isValid())return;

	rSize=pos.getSize();

	uint a=0;
	int len=processed.Length();
	dim=0;
	const uint* Ptr=processed.c_str();
	const uint* startPtr=Ptr;
	const uint* endPtr=Ptr;

	m_printBuffer.resize(0);
	for(;*Ptr;++Ptr)
	{
		a=*Ptr;
		math::vector2d d=getCharDimension(a,attributes->fontSize);
		dim.x+=d.x+attributes->spacing;
		dim.y=math::Max(d.x,dim.y);


		if(a=='\n')
			endPtr=Ptr;

		if(dim.x>rSize.x+1 || a=='\n')
		{
			if(startPtr==endPtr)endPtr=Ptr;
			const uint*tmpPtr=startPtr;
			for (;tmpPtr!=endPtr;++tmpPtr)
			{
				m_printBuffer.push_back(*tmpPtr);
			}
			m_printBuffer.push_back(0);
			print(pos,attributes,&m_printBuffer[0],renderer);
			startPtr=endPtr;
			m_printBuffer.resize(0);
			if(!attributes->wrap)
				break;
			pos.ULPoint.y += dim.y + attributes->lineSpacing;
			if(pos.getHeight()<=0)
				break;
			dim=0;
		}
		if(a==' ' || a=='\n' || a=='\t')
		{
			endPtr=Ptr;
		}
//		toPrint+=a;
	}
	endPtr=Ptr;
	if(startPtr!=endPtr)
	{
		const uint*tmpPtr=startPtr;
		for (;tmpPtr!=endPtr;++tmpPtr)
		{
			m_printBuffer.push_back(*tmpPtr);
		}
		m_printBuffer.push_back(0);
		print(pos,attributes,&m_printBuffer[0],renderer);
		m_printBuffer.resize(0);
	}
}



void BaseFont::print(const math::rectf& pos,FontAttributes*attributes,const  uint* text,IGUIRenderer*renderer)
{
	math::rectf rc;
	math::vector2d dim;
	math::vector2d offset=0;
	math::rectf shadowRect;


	rc.ULPoint=pos.ULPoint+offset;
	for(int i=0;*text;++text,++i)
	{
		FontCharAttr* attr= GetCharacterInfo(*text);
		if(!attr)continue;
		video::TextureUnit*tex=getTexture(attr->texID);
		dim=attr->rectSize*attributes->fontSize;
		rc.BRPoint=rc.ULPoint+dim;
// 		m_renderOperation->Pos[i]=rc;
// 		m_renderOperation->TexCoords[i]=attr->texcoords;

		if(attributes->hasShadow)
		{
			shadowRect=rc;
			shadowRect.ULPoint+=attributes->shadowOffset;
			shadowRect.BRPoint+=attributes->shadowOffset;
			renderer->AddQuad(tex,shadowRect,attr->texcoords,attributes->shadowColor);
		}
		renderer->AddQuad(tex,rc,attr->texcoords,attributes->fontColor);
		rc.ULPoint.x=rc.BRPoint.x+attributes->spacing;
	}

}



}
}

