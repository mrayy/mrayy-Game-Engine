#include "stdafx.h"

#include "FTFont.h"
#include "macros.h"
#include "GUILoggerSystem.h"
#include "TraceManager.h"
#include "CMemoryStream.h"
#include "IFileSystem.h"
#include "TextureResourceManager.h"




namespace mray{
namespace GUI{



	static FT_Library g_ftLib;
	static int g_ftUsageCount=0;
#define FT_POS_COEF  (1.0/64.0)


uint firstPO2From(uint n)
{
	--n;            
	n |= n >> 16;
	n |= n >> 8;
	n |= n >> 4;
	n |= n >> 2;
	n |= n >> 1;
	++n;
	return n;
}

FTFont::FTFont(const core::string&name,uint ttSize,uint ttResolution):BaseFont(name)
{
	m_ttSize=ttSize;
	m_ttResolution=ttResolution;
	m_ttfMaxBearingY=0;
	m_AntialiasColor=0;


	if(!g_ftUsageCount++){
		if(FT_Init_FreeType(&g_ftLib)!=0){
			gGUILoggerSystem.log(mT("Couldn't init FreeType Library!"),ELL_ERROR);
		}
	}
}
FTFont::~FTFont()
{
	if(!--g_ftUsageCount){
		FT_Done_FreeType(g_ftLib);
	}
}



uint FTFont::calcSizeInternal(){
	return sizeof(FTFont);
}

void FTFont::loadFont(OS::IStream* font ){

	traceFunction(eGUI);
	uint charSpacing=5;



	GCPtr<OS::CMemoryStream> memFile=gFileSystem.createMemoryBufferStream(font);

	if(FT_New_Memory_Face(g_ftLib,memFile->getData(),memFile->length(),0,&m_fontFace)!=0){
		gGUILoggerSystem.log(mT("Couldn't open font face for :") + font->getStreamName(),ELL_WARNING);
		return;
	}

	if(!m_fontFace->charmap){
		FT_Done_Face(m_fontFace);
		m_fontFace=0;
		gGUILoggerSystem.log(mT("The font:")+font->getResourceName()+mT(" Dosn't have unicode charmap"),ELL_WARNING);
		return;
	}

	video::ImageInfo info;

	FT_F26Dot6 ftSz=m_ttSize*(1<<6) ;//* m_ttResolution;
	if(FT_Set_Char_Size(m_fontFace,ftSz, 0,m_ttResolution,m_ttResolution)!=0){
		gGUILoggerSystem.log(mT("Couldn't set char size!"),ELL_WARNING);
		return;
	}/*else
	{
		float ptSize_72 = (m_ttSize * 72.0f) / m_ttResolution;
		float best_delta = 99999;
		float best_size = 0;
		for (int i = 0; i < m_fontFace->num_fixed_sizes; i++)
		{
			float size = m_fontFace->available_sizes [i].size * float(FT_POS_COEF);
			float delta = fabs (size - ptSize_72);
			if (delta < best_delta)
			{
				best_delta = delta;
				best_size = size;
			}
		}

		if ((best_size <= 0) ||
			FT_Set_Char_Size (m_fontFace, 0, FT_F26Dot6 (best_size * 64), 0, 0))
		{
			gGUILoggerSystem.log (mT("The font '") + font->getStreamName() + mT("' cannot be rasterized at a size of " )+ core::StringConverter::toString(best_size) + mT(" points, and cannot be used."),ELL_WARNING);
			return;
		}
	}*/

#if 1
	int maxWidth=0,maxHeight=0;
	int glyphCount=0;

	//calc max width,height and bearing
	for(int j=0;j<m_charsAttr.size();++j)
	{
		for(ulong i=m_charsAttr[j]->GetMin();i<m_charsAttr[j]->GetMax();++i){
			glyphCount++;

			int glyph_index=i;//FT_Get_Char_Index(m_fontFace,i);

			FT_Load_Char(m_fontFace,glyph_index,FT_LOAD_RENDER);
			int tmp;
			tmp=(m_fontFace->glyph->advance.x >> 6) + (m_fontFace->glyph->metrics.horiBearingX >> 6) ;
			if(tmp>maxWidth)
				maxWidth=tmp;

			tmp=(2*(m_fontFace->glyph->bitmap.rows<<6) - m_fontFace->glyph->metrics.horiBearingY);
			if( tmp> maxHeight){
				maxHeight=tmp;
			}
			if(m_fontFace->glyph->metrics.horiBearingY > m_ttfMaxBearingY)
				m_ttfMaxBearingY=m_fontFace->glyph->metrics.horiBearingY ;


		}
	}

	// calc the size of the texture
	math::vector2d cDim(maxWidth + charSpacing,(maxHeight>>6)+charSpacing);
	uint rawSize=cDim.x*cDim.y*glyphCount;
	math::vector2di texSide=ceil(sqrt((float)glyphCount));//*cDim.x*cDim.y;
	texSide.x*=cDim.x;
	texSide.y*=cDim.y;
	//texSide+=math::Max(maxWidth,maxHeight>>6);

	rawSize=texSide.x*texSide.y;

	uint finalWidth,finalHeight;
	finalHeight=firstPO2From(texSide.x);
	finalWidth=firstPO2From(texSide.y);
	while(finalWidth*finalHeight>rawSize)
	{
		int newW=finalWidth/2;
		int newH=finalHeight/2;
		if(newW*finalHeight<finalWidth*newH && newW*finalHeight>rawSize)
		{
			finalWidth=newW;
		}else
		{
			if(finalWidth*newH>rawSize)
				finalHeight=newH;
			else 
				break;
		}
	}

	float texAspect=finalWidth/finalHeight;

	const uint pixel_bytes = 2;
	uint data_width = finalWidth * pixel_bytes;
	uint data_size = data_width * finalHeight;

//	m_baseTextureSize.x=finalWidth;
//	m_baseTextureSize.y=finalHeight;

	gGUILoggerSystem.log(mT("Font: ")+font->getResourceName()+mT(" using texture size:")+
		core::StringConverter::toString(finalWidth)+mT("x")+core::StringConverter::toString(finalHeight),ELL_INFO);

	info.createData(math::vector3di(finalWidth,finalHeight,1),video::EPixel_Alpha8Luminance8);

	uchar*imgData=info.imageData;
	for(uint i=0;i<data_size;i+=pixel_bytes){
		imgData[i]=0xFF;
		imgData[i+1]=0x00;
	}

	math::vector2d vTexSize(finalWidth,finalHeight);
	float invWidth=1.0f/ (float)finalWidth;
	float invHeight=1.0f/ (float)finalHeight;

	float charH=maxHeight >> 6;

/*	m_charsAttr.resize(FT_END_CHARACTER);
	m_texRects.resize(FT_END_CHARACTER);
	m_rectsSize.resize(FT_END_CHARACTER);*/

	//float maxY=0;

	uint l=0,m=0;
	float maxH=0;
	for(int j=0;j<m_charsAttr.size();++j)
	{
		for(ulong i=m_charsAttr[j]->GetMin();i<m_charsAttr[j]->GetMax();++i){
			int idx=i;//-FT_START_CHARACTER;
			FT_Error err;
			int glyph_index=i;//FT_Get_Char_Index(m_fontFace,i);
			err=FT_Load_Char(m_fontFace,glyph_index,FT_LOAD_RENDER);
			if(err){
				gGUILoggerSystem.log(mT("font:")+font->getResourceName()+mT(" Cann't load character: ")+(mchar)i,ELL_WARNING);
				continue;
			}
			FT_Int advance= (m_fontFace->glyph->advance.x >> 6 ) + ( m_fontFace->glyph->metrics.horiBearingX >> 6 );

			unsigned char* buffer = m_fontFace->glyph->bitmap.buffer;

			float charW=( m_fontFace->glyph->advance.x >> 6 );

			FontCharAttr *attr=m_charsAttr[j]->GetCharacterInfo(i);
			if(!attr)continue;

			attr->c=i;
			attr->texID=0;
			attr->texcoords.ULPoint.x=(float)l*invWidth ;
			attr->texcoords.ULPoint.y=(float)m*invHeight ;
			attr->texcoords.BRPoint.x=(float)(l+charW)*invWidth ;
			attr->texcoords.BRPoint.y=(float)(m+charH)*invHeight ;
		//	attr->rectSize=attr->texcoords.getSize()*aspect;
			if(maxH<attr->texcoords.getHeight())
			{
				maxH=attr->texcoords.getHeight();
			}

			


			//maxY=math::Max(maxY,attr.rectSize.y);

			if (!buffer)
			{
				l += (advance + charSpacing);

				// If at end of row
				if( finalWidth - 1 < l + ( advance ) )
				{
					m += ( maxHeight >> 6 ) + charSpacing;
					l = 0;
				}
				continue;
			}
			int y_bearnig = ( m_ttfMaxBearingY >> 6 ) - ( m_fontFace->glyph->metrics.horiBearingY >> 6 );

			for(int j = 0; j < m_fontFace->glyph->bitmap.rows; j++ )
			{
				size_t row = /*finalHeight-*/(j + m + y_bearnig);
				uchar* pDest = &imgData[(row * data_width) + l * pixel_bytes];
				for(int k = 0; k < m_fontFace->glyph->bitmap.width; k++ )
				{
					if (m_AntialiasColor)
					{
						*pDest++= *buffer;
					}
					else
					{
						*pDest++= 0xFF;
					}
					*pDest++= *buffer++; 
				}
			}
			l += (advance + charSpacing);

			// If at end of row
			if( finalWidth - 1 < l + ( advance ) )
			{
				m += ( maxHeight >> 6 ) + charSpacing;
				l = 0;
			}
		}
	}
	
	if(maxH>0)
	{
		float imax=1.0f/maxH;
		for(int j=0;j<m_charsAttr.size();++j)
		{
			for(ulong i=m_charsAttr[j]->GetMin();i<m_charsAttr[j]->GetMax();++i)
			{
				FontCharAttr*a= m_charsAttr[j]->GetCharacterInfo(i);
				a->rectSize=a->texcoords.getSize()*imax;
				a->rectSize.x*=texAspect;
			}
		}
	}
	/*
	float imaxY=(float)1.0f/maxY;
	for(int i=FT_START_CHARACTER;i<FT_END_CHARACTER;++i){
		//		m_charsAttr[i].texcoords.BRPoint.y=m_charsAttr[i].texcoords.ULPoint.y+CharSZ.y;
		m_rectsSize[i]=m_rectsSize[i]*imaxY;
	}*/

	FT_Done_Face(m_fontFace);
	//FT_Done_FreeType(ftLib);

	core::string texName=font->getResourceName()+mT("_Font");
	
	video::TextureUnit*t=new video::TextureUnit();
	t->SetTexture(gTextureResourceManager.getInstance().createEmptyTexture2D(texName,true));
	m_textures.push_back(t);
	t->GetTexture()->setMipmapsFilter(false);
	const video::ImageInfo *lst[1];
	lst[0]=&info;
	//t->GetTexture()->setMipmapsFilter(false);
	t->GetTexture()->loadSurfaces(lst,1);
#endif
}

/*
void FTFont::createGlyph(FT_Face face,int c){
	if(FT_Load_Glyph(face,FT_Get_Char_Index(face,c),FT_LOAD_DEFAULT)){
		return;
	}
	FT_Glyph glyph;
	if(FT_Get_Glyph(face->glyph,&glyph))
		return;
}*/


void FTFont::loadFont(xml::XMLElement* attrs )
{
	xml::XMLAttribute*source=attrs->getAttribute(mT("source"));
	if(!source){
		gLogManager.log(mT("Error While Parsing Font XML File, No source for the Font:")+getName(),ELL_ERROR);
	}

	xml::XMLAttribute*attr=attrs->getAttribute(mT("Antialiased"));
	if(attr){
		m_AntialiasColor=core::StringConverter::toBool(attr->value);
	}

	xml::XMLElement* e=attrs->getSubElement(mT("CharacterRange"));
	while(e)
	{
		FontCharacterRange* range = new FontCharacterRange();
		range->LoadFromXml(e);
		m_charsAttr.push_back(range);
		e=e->nextSiblingElement(mT("CharacterRange"));
	}
	OS::IStreamPtr s=gFileSystem.openFile(source->value);
	loadFont(s);
	s->close();
}

}
}



