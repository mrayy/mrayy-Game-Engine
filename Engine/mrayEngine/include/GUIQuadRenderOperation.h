

/********************************************************************
	created:	2010/11/05
	created:	5:11:2010   20:51
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\GUIQuadRenderOperation.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	GUIQuadRenderOperation
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef GUIQuadRenderOperation_h__
#define GUIQuadRenderOperation_h__


#include "SColor.h"

#include "IGUIRenderOperation.h"

namespace mray
{
namespace GUI
{


class MRAY_DLL GUIQuadRenderOperation:public IGUIRenderOperation
{
private:
protected:
public:
	GUIQuadRenderOperation();
	virtual ~GUIQuadRenderOperation();

	virtual void PrepareToRender();

	virtual video::TextureUnit* GetTexture(){return Texture;}
	virtual bool AddToBuffer(math::vector2d* pos,math::vector2d* tc,video::SColor*clr,ushort limit,ushort&used);
	video::TextureUnit* Texture;
	float angle;
	math::rectf Pos;
	math::rectf TexCoords;
	video::SColor Color[4];
};

}
}


#endif // GUIQuadRenderOperation_h__


