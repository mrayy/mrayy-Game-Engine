#include "stdafx.h"

#include "GUIMultiQuadRenderOperation.h"


namespace mray
{
namespace GUI
{

GUIMultiQuadRenderOperation::GUIMultiQuadRenderOperation()
{
	lastIndex=0;
	ClipRect=0;
	angle=0;
}
GUIMultiQuadRenderOperation::~GUIMultiQuadRenderOperation()
{
}

void GUIMultiQuadRenderOperation::Reset()
{
	Texture=0;
	lastIndex=0;
	ClipRect=0;
	TexCoords.clear();
	Pos.clear();
}

void GUIMultiQuadRenderOperation::PrepareToRender()
{
	lastIndex=0;
}

bool GUIMultiQuadRenderOperation::AddToBuffer(math::vector2d* pos,math::vector2d* tc,video::SColor*clr,ushort limit,ushort&used)
{


	float cs=1;
	float sn=0;

	if(angle!=0)
	{
		cs=math::cosd(angle);
		sn=math::sind(angle);
	}
	ushort maxQuads=limit/4;
	ushort idx=0;
	ushort cnt=Pos.size();
	ushort quadsToRender=math::Min<ushort>(cnt,maxQuads);
	used=quadsToRender*4;
	ushort i=lastIndex;
	math::rectf *posPtr=&Pos[0];
	math::rectf *tcPtr=&TexCoords[0];
	for(;i<cnt && idx<used;++i,idx+=4)
	{
		//baseIdx=idx*4;
		pos[idx+0].x=(*posPtr).ULPoint.x;
		pos[idx+0].y=(*posPtr).ULPoint.y;
		tc[idx+0].x=(*tcPtr).ULPoint.x;
		tc[idx+0].y=(*tcPtr).ULPoint.y;
		clr[idx+0]=Color;

		pos[idx+1].x=(*posPtr).BRPoint.x;
		pos[idx+1].y=(*posPtr).ULPoint.y;
		tc[idx+1].x=(*tcPtr).BRPoint.x;
		tc[idx+1].y=(*tcPtr).ULPoint.y;
		clr[idx+1]=Color;

		pos[idx+2].x=(*posPtr).BRPoint.x;
		pos[idx+2].y=(*posPtr).BRPoint.y;
		tc[idx+2].x=(*tcPtr).BRPoint.x;
		tc[idx+2].y=(*tcPtr).BRPoint.y;
		clr[idx+2]=Color;

		pos[idx+3].x=(*posPtr).ULPoint.x;
		pos[idx+3].y=(*posPtr).BRPoint.y;
		tc[idx+3].x=(*tcPtr).ULPoint.x;
		tc[idx+3].y=(*tcPtr).BRPoint.y;
		clr[idx+3]=Color;

		++posPtr;
		++tcPtr;
	}
	if(angle!=0)
	{
		float x=0;
		int start=lastIndex*4;
		int end=cnt*4;
		for(int j=start;j<end;++j)
		{
			x=pos[j].x;
			pos[j].x=cs*x-sn*pos[j].y;
			pos[j].y=sn*x+cs*pos[j].y;
		}
	}
	lastIndex=i;
	if(lastIndex==Pos.size())
		return true;
	return false;
}

}
}


