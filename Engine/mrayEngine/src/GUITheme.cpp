#include "stdafx.h"



#include "GUITheme.h"
#include "IVideoDevice.h"
#include "GUIMultiQuadRenderOperation.h"

#include "GUIBatchRenderer.h"
#include "ImageSet.h"
#include "GUIProfile.h"

namespace mray{
namespace GUI{


GUITheme::GUITheme(const core::string&name):IGUITheme(name)
{
//	m_renderOperation=new GUIMultiQuadRenderOperation();
}
GUITheme::~GUITheme()
{
}


void GUITheme::drawSizableBox(IGUIRenderer*renderer, const math::rectf &r,int state,const core::string &profile,const video::SColor& globalColor,math::rectf*clip)
{
	if(!r.isValid())
		return;
	math::rectf rc=r;
	if(clip)
		rc.clipRect(*clip);

	GUIProfile* p=getProfile(profile);
	if(!p)
		return;
 	video::ImageSet*iset= p->GetImageSet();
 	if(!iset)
 		return;



	int numCells=9;
	int startIndex=(state)*numCells;
	if(p->getRegionCount()<=startIndex+numCells-1)
		return;

	GUIProfile::GUIRegion* regions[9];
	//video::ImageRegion* imgRegions[9];
	for(int i=0;i<9;++i)
	{
		regions[i]=p->getRegion(startIndex+i);
		if(!regions[i])return;
// 		imgRegions[i]=iset->GetRegion(regions[i]->targetImage);
// 		if(!imgRegions[i])return;
	}
/*
	GUIProfile::GUIRegion* cTopLeft		=p->getRegion(startIndex);
	GUIProfile::GUIRegion* cTop			=p->getRegion(startIndex+1);
	GUIProfile::GUIRegion* cTopRight	=p->getRegion(startIndex+2);
	GUIProfile::GUIRegion* cLeft		=p->getRegion(startIndex+3);
	GUIProfile::GUIRegion* cCenter		=p->getRegion(startIndex+4);
	GUIProfile::GUIRegion* cRight		=p->getRegion(startIndex+5);
	GUIProfile::GUIRegion* cBottomLeft	=p->getRegion(startIndex+6);
	GUIProfile::GUIRegion* cBottom		=p->getRegion(startIndex+7);
	GUIProfile::GUIRegion* cBottomRight	=p->getRegion(startIndex+8);
	if(	!cTopLeft || !cTop || !cTopRight ||
		!cLeft || !cCenter || !cRight ||
		!cBottomLeft || !cBottom || !cBottom )
		return;

*/

	float midW=rc.getWidth()-(regions[0]->GetSrcRect().getWidth()+regions[2]->GetSrcRect().getWidth());
	float midH=rc.getHeight()-(regions[0]->GetSrcRect().getHeight()+regions[2]->GetSrcRect().getHeight());

	math::vector2d fix=0;
	if(midW<0){
		//fix.x=midW*0.5f;
		fix.x=-rc.getWidth()/(regions[0]->GetSrcRect().getWidth()+regions[2]->GetSrcRect().getWidth());
		midW=0;
	}
	if(midH<0){
		//fix.y=midH*0.5f;
		fix.y=-rc.getHeight()/(regions[0]->GetSrcRect().getHeight()+regions[2]->GetSrcRect().getHeight());
		midH=0;
	}
	/*
	m_renderOperation->Reset();
	m_renderOperation->Texture=iset->GetTexture();
	m_renderOperation->Color=globalColor;
	m_renderOperation->ClipRect=clip;
*/
	video::TextureUnit*tex=iset->GetTexture();

	math::rectf tr;
	tr.ULPoint=rc.ULPoint;
	tr.BRPoint=tr.ULPoint+regions[0]->GetSrcRect().getSize()+fix;
// 	m_renderOperation->Pos.push_back(tr);
// 	m_renderOperation->TexCoords.push_back(regions[0]->GetTextureRect());
	//m_device->draw2DImage(tr,m_texture,globalColor,clip,&regions[0]->GetTextureRect());
	renderer->AddQuad(tex,tr,regions[0]->GetTextureRect(),globalColor,clip,0);
	
	tr.ULPoint.x=tr.BRPoint.x;
	tr.BRPoint.x=tr.ULPoint.x+midW;
// 	m_renderOperation->Pos.push_back(tr);
// 	m_renderOperation->TexCoords.push_back(regions[1]->GetTextureRect());
	//m_device->draw2DImage(tr,m_texture,globalColor,0,&regions[1]->GetTextureRect());
	renderer->AddQuad(tex,tr,regions[1]->GetTextureRect(),globalColor,clip,0);
	
	tr.ULPoint.x=tr.BRPoint.x;
	tr.BRPoint.x=tr.ULPoint.x+regions[2]->GetSrcRect().getWidth()+fix.x;
// 	m_renderOperation->Pos.push_back(tr);
// 	m_renderOperation->TexCoords.push_back(regions[2]->GetTextureRect());
	//m_device->draw2DImage(tr,m_texture,globalColor,clip,&regions[2]->GetTextureRect());
	//renderer->AddRenderOperation(tr,true);
	renderer->AddQuad(tex,tr,regions[2]->GetTextureRect(),globalColor,clip,0);

	////////////////////////////////////////////////
	tr.ULPoint=rc.ULPoint;
	tr.ULPoint.y+=regions[0]->GetSrcRect().getHeight()+fix.y;
	tr.BRPoint.x=tr.ULPoint.x+regions[3]->GetSrcRect().getWidth()+fix.x;
	tr.BRPoint.y=tr.ULPoint.y+midH;
// 	m_renderOperation->Pos.push_back(tr);
// 	m_renderOperation->TexCoords.push_back(regions[3]->GetTextureRect());
	//m_device->draw2DImage(tr,m_texture,globalColor,clip,&regions[3]->GetTextureRect());
	renderer->AddQuad(tex,tr,regions[3]->GetTextureRect(),globalColor,clip,0);
	
	tr.ULPoint.x=tr.BRPoint.x;
	tr.BRPoint.x=tr.ULPoint.x+midW;
	tr.BRPoint.y=tr.ULPoint.y+midH;
// 	m_renderOperation->Pos.push_back(tr);
// 	m_renderOperation->TexCoords.push_back(regions[4]->GetTextureRect());
	//m_device->draw2DImage(tr,m_texture,globalColor,clip,&regions[4]->GetTextureRect());
	renderer->AddQuad(tex,tr,regions[4]->GetTextureRect(),globalColor,clip,0);
	
	tr.ULPoint.x=tr.BRPoint.x;
	tr.BRPoint.x=tr.ULPoint.x+regions[5]->GetSrcRect().getWidth()+fix.x;
	tr.BRPoint.y=tr.ULPoint.y+midH;
// 	m_renderOperation->Pos.push_back(tr);
// 	m_renderOperation->TexCoords.push_back(regions[5]->GetTextureRect());
	//m_device->draw2DImage(tr,m_texture,globalColor,clip,&regions[5]->GetTextureRect());
	renderer->AddQuad(tex,tr,regions[5]->GetTextureRect(),globalColor,clip,0);

	///////////////////////////////////////////////
	tr.ULPoint=rc.ULPoint;
	tr.ULPoint.y+=regions[0]->GetSrcRect().getHeight()+midH;
	tr.BRPoint=tr.ULPoint+regions[6]->GetSrcRect().getSize()+fix;
// 	m_renderOperation->Pos.push_back(tr);
// 	m_renderOperation->TexCoords.push_back(regions[6]->GetTextureRect());
	//m_device->draw2DImage(tr,m_texture,globalColor,clip,&regions[6]->GetTextureRect());
	renderer->AddQuad(tex,tr,regions[6]->GetTextureRect(),globalColor,clip,0);
	
	tr.ULPoint.x=tr.BRPoint.x;
	tr.BRPoint.x=tr.ULPoint.x+midW;
// 	m_renderOperation->Pos.push_back(tr);
// 	m_renderOperation->TexCoords.push_back(regions[7]->GetTextureRect());
	//m_device->draw2DImage(tr,m_texture,globalColor,clip,&regions[7]->GetTextureRect());
	renderer->AddQuad(tex,tr,regions[7]->GetTextureRect(),globalColor,clip,0);
	
	tr.ULPoint.x=tr.BRPoint.x;
	tr.BRPoint.x=tr.ULPoint.x+regions[8]->GetSrcRect().getWidth()+fix.x;
// 	m_renderOperation->Pos.push_back(tr);
// 	m_renderOperation->TexCoords.push_back(regions[8]->GetTextureRect());
	//m_device->draw2DImage(tr,m_texture,globalColor,clip,&regions[8]->GetTextureRect());
	renderer->AddQuad(tex,tr,regions[8]->GetTextureRect(),globalColor,clip,0);

	//renderer->AddRenderOperation(m_renderOperation,true);

}
void GUITheme::drawSizableBox3PH(IGUIRenderer*renderer, const math::rectf &r,int state,const core::string &profile,const video::SColor& globalColor,math::rectf*clip){
	if(!r.isValid())
		return;
	math::rectf rc=r;
	if(clip)
		rc.clipRect(*clip);
	GUIProfile* p=getProfile(profile);
	if(!p)return;
	video::ImageSet*iset= p->GetImageSet();
	if(!iset)
		return;

	int numCells=3;
	int startIndex=(int)(state)*numCells;
	if(p->getRegionCount()<=startIndex+numCells-1)
		return;
	GUIProfile::GUIRegion* regions[3];
	video::ImageRegion* imgRegions[3];
	for(int i=0;i<3;++i)
	{
		regions[i]=p->getRegion(startIndex+i);
		if(!regions[i])return;
		imgRegions[i]=iset->GetRegion(regions[i]->targetImage);
		if(!imgRegions[i])return;
	}
	/*
	GUIProfile::GUIRegion* cLeft		=p->getRegion(startIndex);
	GUIProfile::GUIRegion* cMiddle		=p->getRegion(startIndex+1);
	GUIProfile::GUIRegion* cRight		=p->getRegion(startIndex+2);
	if(	!cLeft || !cMiddle || !cRight )
	return;*/
/*
	m_renderOperation->Reset();
	m_renderOperation->Texture=iset->GetTexture();
	m_renderOperation->Color=globalColor;
	m_renderOperation->ClipRect=clip;*/

	float midW=rc.getWidth()-(imgRegions[0]->GetSrcRect().getWidth()+imgRegions[2]->GetSrcRect().getWidth());

	float fix=0;
	if(midW<0){
		//fix=-midW*0.5f;
		//fix=midW*0.5f;
		fix=-rc.getWidth()/(imgRegions[0]->GetSrcRect().getWidth()+imgRegions[2]->GetSrcRect().getWidth());
		midW=0;
	}

	video::TextureUnit*tex=iset->GetTexture();

	math::rectf tr;
	tr.ULPoint=rc.ULPoint;
	tr.BRPoint.x=tr.ULPoint.x+imgRegions[0]->GetSrcRect().getWidth()+fix;
	tr.BRPoint.y=rc.BRPoint.y;
// 	m_renderOperation->Pos.push_back(tr);
// 	m_renderOperation->TexCoords.push_back(imgRegions[0]->GetTextureRect());
	//	m_device->draw2DImage(tr,m_texture,globalColor,clip,&imgRegions[0]->GetTextureRect());
	renderer->AddQuad(tex,tr,imgRegions[0]->GetTextureRect(),globalColor,clip,0);
	
	tr.ULPoint.x=tr.BRPoint.x;
	tr.BRPoint.x=tr.ULPoint.x+midW;
// 	m_renderOperation->Pos.push_back(tr);
// 	m_renderOperation->TexCoords.push_back(imgRegions[1]->GetTextureRect());
	//	m_device->draw2DImage(tr,m_texture,globalColor,0,&imgRegions[1]->GetTextureRect());
	renderer->AddQuad(tex,tr,imgRegions[1]->GetTextureRect(),globalColor,clip,0);
	
	tr.ULPoint.x=tr.BRPoint.x;
	tr.BRPoint.x=tr.ULPoint.x+imgRegions[2]->GetSrcRect().getWidth()+fix;
// 	m_renderOperation->Pos.push_back(tr);
// 	m_renderOperation->TexCoords.push_back(imgRegions[2]->GetTextureRect());
	//m_device->draw2DImage(tr,m_texture,globalColor,clip,&imgRegions[2]->GetTextureRect());
	//renderer->AddRenderOperation(m_renderOperation,true);
	renderer->AddQuad(tex,tr,imgRegions[2]->GetTextureRect(),globalColor,clip,0);

}
void GUITheme::drawSizableBox3PV(IGUIRenderer*renderer, const math::rectf &r,int state,const core::string &profile,const video::SColor& globalColor,math::rectf*clip)
{
	if(!r.isValid())
		return;
	math::rectf rc=r;
	if(clip)
		rc.clipRect(*clip);
	GUIProfile* p=getProfile(profile);
	if(!p)return;
	video::ImageSet*iset= p->GetImageSet();
	if(!iset)
		return;

	int numCells=3;
	int startIndex=(int)(state)*numCells;
	if(p->getRegionCount()<=startIndex+numCells-1)
		return;
	GUIProfile::GUIRegion* regions[3];
	video::ImageRegion* imgRegions[3];
	for(int i=0;i<3;++i)
	{
		regions[i]=p->getRegion(startIndex+i);
		if(!regions[i])return;
		imgRegions[i]=iset->GetRegion(regions[i]->targetImage);
		if(!imgRegions[i])return;
	}
	/*
	GUIProfile::GUIRegion* cTop		=p->getRegion(startIndex);
	GUIProfile::GUIRegion* cMiddle		=p->getRegion(startIndex+1);
	GUIProfile::GUIRegion* cBottom		=p->getRegion(startIndex+2);
	if(	!cTop || !cMiddle || !cBottom )
		return;*/

	float midH=rc.getHeight()-imgRegions[0]->GetSrcRect().getHeight()-imgRegions[2]->GetSrcRect().getHeight();

	float fix=1;
	if(midH<0){
		//fix=-midH*0.5f;
		fix=-rc.getHeight()/(imgRegions[0]->GetSrcRect().getHeight()+imgRegions[2]->GetSrcRect().getHeight());
		midH=0;
	}
	video::TextureUnit*tex=iset->GetTexture();

/*	m_renderOperation->Reset();
	m_renderOperation->Texture=iset->GetTexture();
	m_renderOperation->Color=globalColor;
	m_renderOperation->ClipRect=clip;*/

	math::rectf tr;
	tr.ULPoint=rc.ULPoint;
	tr.BRPoint.x=rc.BRPoint.x;
	tr.BRPoint.y=tr.ULPoint.y+imgRegions[0]->GetSrcRect().getHeight()*fix;
// 	m_renderOperation->Pos.push_back(tr);
// 	m_renderOperation->TexCoords.push_back(imgRegions[0]->GetTextureRect());
	//m_device->draw2DImage(tr,m_texture,globalColor,clip,&imgRegions[0]->GetTextureRect());
	renderer->AddQuad(tex,tr,imgRegions[0]->GetTextureRect(),globalColor,clip,0);
	
	tr.ULPoint.y=tr.BRPoint.y;
	tr.BRPoint.y=tr.ULPoint.y+midH;
// 	m_renderOperation->Pos.push_back(tr);
// 	m_renderOperation->TexCoords.push_back(imgRegions[1]->GetTextureRect());
	//m_device->draw2DImage(tr,m_texture,globalColor,0,&imgRegions[1]->GetTextureRect());
	renderer->AddQuad(tex,tr,imgRegions[1]->GetTextureRect(),globalColor,clip,0);
	
	tr.ULPoint.y=tr.BRPoint.y;
	tr.BRPoint.y=tr.ULPoint.y+imgRegions[2]->GetSrcRect().getHeight()*fix;
// 	m_renderOperation->Pos.push_back(tr);
// 	m_renderOperation->TexCoords.push_back(imgRegions[2]->GetTextureRect());
	//m_device->draw2DImage(tr,m_texture,globalColor,clip,&imgRegions[2]->GetTextureRect());
	renderer->AddQuad(tex,tr,imgRegions[2]->GetTextureRect(),globalColor,clip,0);
//	renderer->AddRenderOperation(m_renderOperation,true);

}
void GUITheme::drawBox(IGUIRenderer*renderer, const math::rectf &r,const core::string &profile,int index,const video::SColor& globalColor,math::rectf*clip,bool fill){
	GUIProfile* p=getProfile(profile);
	const math::rectf *tcoord=0;
	video::TextureUnit*tex=0;
	if(p){
		video::ImageSet*iset= p->GetImageSet();
		if(iset)
		{
			tex=iset->GetTexture();
			if(p->getRegionCount()<=index)
				return;
			GUIProfile::GUIRegion* region		=p->getRegion(index);
// 			video::ImageRegion* ir=0;
// 			if(region)
// 				ir=iset->GetRegion(region->targetImage);
// 			if(ir){
// 				tcoord=&ir->GetTextureRect();
// 			}
			if(region)
				tcoord=&region->texCoords;
		}
		//tex=m_texture->texture;
	}
	math::rectf rc=r;
	if(clip)
		rc.clipRect(*clip);
	if(tcoord)
		renderer->AddQuad(tex,rc,*tcoord,globalColor,clip,0);
	else
		renderer->AddQuad(tex,rc,math::rectf(0,0,1,1),globalColor,clip,0);
	/*
	if(p)
		renderer->GetDevice()->draw2DImage(rc,tex,globalColor,clip,tcoord);
	else
		renderer->GetDevice()->draw2DImage(rc,0,globalColor,clip,tcoord);*/
}


math::rectf GUITheme::getSizableRect(int rect,const math::rectf &r,int state,const core::string &profile){
	
	math::rectf tr;

	GUIProfile* p=getProfile(profile);
	if(!p)return tr;
	video::ImageSet*iset= p->GetImageSet();
	if(!iset)
		return tr;

	int numCells=9;
	int startIndex=(state)*numCells;
	if(p->getRegionCount()<=startIndex+numCells-1)
		return tr;
	GUIProfile::GUIRegion* regions[9];
	video::ImageRegion* imgRegions[9];
	for(int i=0;i<9;++i)
	{
		regions[i]=p->getRegion(startIndex+i);
		if(!regions[i])return tr;
		imgRegions[i]=iset->GetRegion(regions[i]->targetImage);
		if(!imgRegions[i])return tr;
	}
	/*
	GUIProfile::GUIRegion* cTopLeft		=p->getRegion(startIndex);
	GUIProfile::GUIRegion* cTop			=p->getRegion(startIndex+1);
	GUIProfile::GUIRegion* cTopRight	=p->getRegion(startIndex+2);
	GUIProfile::GUIRegion* cLeft		=p->getRegion(startIndex+3);
	GUIProfile::GUIRegion* cCenter		=p->getRegion(startIndex+4);
	GUIProfile::GUIRegion* cRight		=p->getRegion(startIndex+5);
	GUIProfile::GUIRegion* cBottomLeft	=p->getRegion(startIndex+6);
	GUIProfile::GUIRegion* cBottom		=p->getRegion(startIndex+7);
	GUIProfile::GUIRegion* cBottomRight	=p->getRegion(startIndex+8);
	if(	!cTopLeft || !cTop || !cTopRight ||
		!cLeft || !cCenter || !cRight ||
		!cBottomLeft || !cBottom || !cBottom )
		return tr;*/

	float midW=r.getWidth()-imgRegions[0]->GetSrcRect().getWidth()-imgRegions[2]->GetSrcRect().getWidth();
	float midH=r.getHeight()-imgRegions[0]->GetSrcRect().getHeight()-imgRegions[2]->GetSrcRect().getHeight();

	math::vector2d fix=0;
	if(midW<0){
		fix.x=midW*0.5f;
		midW=0;
	}
	if(midH<0){
		fix.y=midH*0.5f;
		midH=0;
	}

	switch(rect){
	case 0:
	{
		tr.ULPoint=r.ULPoint;
		tr.BRPoint=tr.ULPoint+imgRegions[0]->GetSrcRect().getSize()+fix;
	}break;
	case 1:
	{
		tr.ULPoint=r.ULPoint;
		tr.ULPoint.x+=imgRegions[0]->GetSrcRect().getSize().x+fix.x;
		tr.BRPoint=tr.ULPoint;
		tr.BRPoint.x+=midW;
		tr.BRPoint.y+=imgRegions[0]->GetSrcRect().getSize().y;
	}break;
	case 2:
	{
		tr.ULPoint=r.ULPoint;
		tr.ULPoint.x+=imgRegions[0]->GetSrcRect().getSize().x+fix.x +midW;
		tr.BRPoint=tr.ULPoint;
		tr.BRPoint.x+=imgRegions[2]->GetSrcRect().getWidth()+fix.x;
		tr.BRPoint.y+=imgRegions[0]->GetSrcRect().getSize().y;
	}break;
	case 3:
	{
		tr.ULPoint=r.ULPoint;
		tr.ULPoint.y+=imgRegions[0]->GetSrcRect().getHeight()+fix.y;
		tr.BRPoint=tr.ULPoint;
		tr.BRPoint.x+=imgRegions[3]->GetSrcRect().getWidth()+fix.x;
		tr.BRPoint.y+=midH;
	}break;
	case 4:
	{
		tr.ULPoint=r.ULPoint;
		tr.ULPoint.x+=imgRegions[3]->GetSrcRect().getWidth()+fix.x;
		tr.ULPoint.y+=imgRegions[0]->GetSrcRect().getHeight()+fix.y;
		tr.BRPoint=tr.ULPoint;
		tr.BRPoint.x+=midW;
		tr.BRPoint.y+=midH;
	}break;
	case 5:
	{
		tr.ULPoint=r.ULPoint;
		tr.ULPoint.x+=imgRegions[3]->GetSrcRect().getWidth()+fix.x+midW;
		tr.ULPoint.y+=imgRegions[0]->GetSrcRect().getHeight()+fix.y;
		tr.BRPoint=tr.ULPoint;
		tr.BRPoint.x+=imgRegions[5]->GetSrcRect().getWidth();
		tr.BRPoint.y+=midH;
	}break;
	case 6:
	{
		tr.ULPoint=r.ULPoint;
		tr.ULPoint.y+=imgRegions[0]->GetSrcRect().getHeight()+fix.y+midH;
		tr.BRPoint=tr.ULPoint+imgRegions[6]->GetSrcRect().getSize()+fix;
	}break;
	case 7:
	{
		tr.ULPoint=r.ULPoint;
		tr.ULPoint.x+=imgRegions[6]->GetSrcRect().getWidth()+fix.x;
		tr.ULPoint.y+=imgRegions[0]->GetSrcRect().getHeight()+fix.y+midH;
		tr.BRPoint=tr.ULPoint;
		tr.BRPoint.x+=midW;
		tr.BRPoint.y+=imgRegions[7]->GetSrcRect().getHeight()+fix.y;
	}break;
	case 8:
	{
		tr.ULPoint=r.ULPoint;
		tr.ULPoint.x+=imgRegions[6]->GetSrcRect().getWidth()+fix.x+midW;
		tr.ULPoint.y+=imgRegions[0]->GetSrcRect().getHeight()+fix.y+midH;
		tr.BRPoint=tr.ULPoint+imgRegions[8]->GetSrcRect().getSize()+fix;
	}break;
	
	}
	return tr;
}
math::rectf GUITheme::getRect3PH(int rect,const math::rectf &rc,int state,const core::string &profile){
	
	math::rectf tr;

	GUIProfile* p=getProfile(profile);
	if(!p)return tr;
	video::ImageSet*iset= p->GetImageSet();
	if(!iset)
		return tr;

	int numCells=3;
	int startIndex=(int)(state)*numCells;
	if(p->getRegionCount()<=startIndex+numCells-1)
		return tr;

	GUIProfile::GUIRegion* regions[3];
	video::ImageRegion* imgRegions[3];
	for(int i=0;i<3;++i)
	{
		regions[i]=p->getRegion(startIndex+i);
		if(!regions[i])return tr;
		imgRegions[i]=iset->GetRegion(regions[i]->targetImage);
		if(!imgRegions[i])return tr;
	}
	/*
	GUIProfile::GUIRegion* cLeft		=p->getRegion(startIndex);
	GUIProfile::GUIRegion* cMiddle		=p->getRegion(startIndex+1);
	GUIProfile::GUIRegion* cRight		=p->getRegion(startIndex+2);
	if(	!cLeft || !cMiddle || !cRight )
		return tr;
*/
	float midW=rc.getWidth()-imgRegions[0]->GetSrcRect().getWidth()-imgRegions[2]->GetSrcRect().getWidth();

	float fix=0;
	if(midW<0){
		fix=-midW*0.5f;
		midW=0;
	}

	switch(rect){
	case 0:
	{
		tr.ULPoint=rc.ULPoint;
		tr.BRPoint.x=tr.ULPoint.x+imgRegions[0]->GetSrcRect().getWidth()+fix;
		tr.BRPoint.y=rc.BRPoint.y;
	}break;
	case 1:
	{
		tr.ULPoint=rc.ULPoint;
		tr.ULPoint.x+=imgRegions[0]->GetSrcRect().getWidth()+fix;
		tr.BRPoint.x=tr.ULPoint.x+midW;
		tr.BRPoint.y=rc.BRPoint.y;
	}break;
	case 2:
	{
		tr.ULPoint=rc.ULPoint;
		tr.ULPoint.x+=imgRegions[0]->GetSrcRect().getWidth()+fix+midW;
		tr.BRPoint.x=tr.ULPoint.x+imgRegions[2]->GetSrcRect().getWidth()+fix;
		tr.BRPoint.y=rc.BRPoint.y;
	}break;
	}
	return tr;
}
math::rectf GUITheme::getRect3PV(int rect,const math::rectf &rc,int state,const core::string &profile){
	math::rectf tr;

	GUIProfile* p=getProfile(profile);
	if(!p)return tr;
	video::ImageSet*iset= p->GetImageSet();
	if(!iset)
		return tr;

	int numCells=3;
	int startIndex=(int)(state)*numCells;
	if(p->getRegionCount()<=startIndex+numCells-1)
		return tr;
	/*
	GUIProfile::GUIRegion* cTop		=p->getRegion(startIndex);
	GUIProfile::GUIRegion* cMiddle		=p->getRegion(startIndex+1);
	GUIProfile::GUIRegion* cBottom		=p->getRegion(startIndex+2);
	if(	!cTop || !cMiddle || !cBottom )
		return tr;*/

	GUIProfile::GUIRegion* regions[3];
	video::ImageRegion* imgRegions[3];
	for(int i=0;i<3;++i)
	{
		regions[i]=p->getRegion(startIndex+i);
		if(!regions[i])return tr;
		imgRegions[i]=iset->GetRegion(regions[i]->targetImage);
		if(!imgRegions[i])return tr;
	}

	float midH=rc.getWidth()-imgRegions[0]->GetSrcRect().getWidth()-imgRegions[2]->GetSrcRect().getWidth();

	float fix=0;
	if(midH<0){
		fix=-midH*0.5f;
		midH=0;
	}

	switch(rect){
	case 0:
	{
		tr.ULPoint=rc.ULPoint;
		tr.BRPoint.x=rc.BRPoint.y;
		tr.BRPoint.y=tr.ULPoint.x+imgRegions[0]->GetSrcRect().getHeight()+fix;
	}break;
	case 1:
	{
		tr.ULPoint=rc.ULPoint;
		tr.ULPoint.y+=imgRegions[0]->GetSrcRect().getHeight()+fix;
		tr.BRPoint.x=rc.BRPoint.x;
		tr.BRPoint.y=tr.ULPoint.y+midH;
	}break;
	case 2:
	{
		tr.ULPoint=rc.ULPoint;
		tr.ULPoint.y+=imgRegions[0]->GetSrcRect().getHeight()+fix+midH;
		tr.BRPoint.x=rc.BRPoint.x;
		tr.BRPoint.y=tr.ULPoint.y+imgRegions[2]->GetSrcRect().getHeight()+fix;
	}break;
	}
	return tr;
}
math::rectf GUITheme::getRect(int rect,const math::rectf &r,const core::string &profile)
{
	math::rectf tr;

	GUIProfile* p=getProfile(profile);
	if(!p)return tr;
	if(p->getRegionCount()<=rect)
		return tr;
	video::ImageSet*iset= p->GetImageSet();
	if(!iset)
		return tr;
	
	GUIProfile::GUIRegion* cRect		=p->getRegion(rect);
	if(	!cRect )
		return tr;
	video::ImageRegion*ir= iset->GetRegion(cRect->targetImage);
	if(!ir)return tr;

	return ir->GetSrcRect();

}

}
}


