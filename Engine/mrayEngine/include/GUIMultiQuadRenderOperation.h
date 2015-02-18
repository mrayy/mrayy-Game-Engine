

/********************************************************************
	created:	2010/11/05
	created:	5:11:2010   23:13
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\GUIMultiQuadRenderOperation.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	GUIMultiQuadRenderOperation
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef GUIMultiQuadRenderOperation_h__
#define GUIMultiQuadRenderOperation_h__



#include "SColor.h"

#include "IGUIRenderOperation.h"

namespace mray
{
namespace GUI
{

class MRAY_DLL GUIMultiQuadRenderOperation:public IGUIRenderOperation
{
private:
protected:
public:
	GUIMultiQuadRenderOperation();
	virtual ~GUIMultiQuadRenderOperation();

	virtual video::TextureUnit* GetTexture(){return Texture;}
	virtual bool AddToBuffer(math::vector2d* pos,math::vector2d* tc,video::SColor*clr,ushort limit,ushort&used);
	virtual void PrepareToRender();

	void Reset();

	ushort lastIndex;
	video::TextureUnit* Texture;
	float angle;
	std::vector<math::rectf> Pos;
	std::vector<math::rectf> TexCoords;
	video::SColor Color;

	math::rectf* ClipRect;
};

}
}

#endif // GUIMultiQuadRenderOperation_h__
