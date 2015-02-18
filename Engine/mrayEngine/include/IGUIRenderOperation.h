

/********************************************************************
	created:	2010/11/05
	created:	5:11:2010   21:29
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\IGUIRenderOperation.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	IGUIRenderOperation
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef IGUIRenderOperation_h__
#define IGUIRenderOperation_h__


#include "Point4d.h"
#include "GCPtr.h"

namespace mray
{
namespace video
{
	class TextureUnit;
}
namespace GUI
{

class IGUIRenderOperation
{
private:
protected:
public:
	IGUIRenderOperation(){}
	virtual ~IGUIRenderOperation(){}
	
	virtual void PrepareToRender()=0;

	virtual video::TextureUnit* GetTexture()=0;
	virtual bool AddToBuffer(math::vector2d* pos,math::vector2d* tc,video::SColor*clr,ushort limit,ushort&used)=0;
};

MakeSharedPtrType(IGUIRenderOperation);

}
}


#endif // IGUIRenderOperation_h__

