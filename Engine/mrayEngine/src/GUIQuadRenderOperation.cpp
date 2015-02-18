#include "stdafx.h"

#include "GUIQuadRenderOperation.h"


namespace mray
{
namespace GUI
{

GUIQuadRenderOperation::GUIQuadRenderOperation()
{
	angle=0;
	Texture=0;
}
GUIQuadRenderOperation::~GUIQuadRenderOperation()
{
}
void GUIQuadRenderOperation::PrepareToRender()
{
}

bool GUIQuadRenderOperation::AddToBuffer(math::vector2d* pos,math::vector2d* tc,video::SColor*clr,ushort limit,ushort&used)
{
	if(limit<4)return false;
	pos[0].x=Pos.ULPoint.x;
	pos[0].y=Pos.ULPoint.y;
	tc[0].x=TexCoords.ULPoint.x;
	tc[0].y=TexCoords.ULPoint.y;
	clr[0]=Color[0];

	pos[1].x=Pos.BRPoint.x;
	pos[1].y=Pos.ULPoint.y;
	tc[1].x=TexCoords.BRPoint.x;
	tc[1].y=TexCoords.ULPoint.y;
	clr[1]=Color[1];

	pos[2].x=Pos.BRPoint.x;
	pos[2].y=Pos.BRPoint.y;
	tc[2].x=TexCoords.BRPoint.x;
	tc[2].y=TexCoords.BRPoint.y;
	clr[2]=Color[2];

	pos[3].x=Pos.ULPoint.x;
	pos[3].y=Pos.BRPoint.y;
	tc[3].x=TexCoords.ULPoint.x;
	tc[3].y=TexCoords.BRPoint.y;
	clr[3]=Color[3];

	if(angle!=0)
	{
		float cs=math::cosd(angle);
		float sn=math::sind(angle);
		float x=0;
		for(int i=0;i<4;++i)
		{
			x=pos[i].x;
			pos[i].x=cs*x-sn*pos[i].y;
			pos[i].y=sn*x+cs*pos[i].y;
		}
	}

	used=4;

	return true;
}

}
}
