


/********************************************************************
	created:	2010/11/19
	created:	19:11:2010   21:04
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\IGUIRenderer.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	IGUIRenderer
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef IGUIRenderer_h__
#define IGUIRenderer_h__



#include "SColor.h"

namespace mray
{
	namespace video
	{
		class IVideoDevice;
		class TextureUnit;
	}
namespace GUI
{

class MRAY_DLL IGUIRenderer
{
private:
protected:
public:
	IGUIRenderer(){}
	virtual ~IGUIRenderer(){}

	virtual video::IVideoDevice* GetDevice()=0;

	virtual void Prepare()=0;

	//4 colors , one for each corner
	virtual void AddQuad(video::TextureUnit*tex, const math::rectf&pos,const math::rectf&tc,const video::SColor*clr,const math::rectf* clip=0,float angle=0)=0;
	//single color for the quad
	virtual void AddQuad(video::TextureUnit*tex, const math::rectf&pos,const math::rectf&tc,const video::SColor&clr,const math::rectf* clip=0,float angle=0)=0;

	virtual void Flush()=0;
};

}
}


#endif // IGUIRenderer_h__
