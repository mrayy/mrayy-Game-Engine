

/********************************************************************
	created:	2009/07/17
	created:	17:7:2009   19:56
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\IRenderDevice.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	IRenderDevice
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___IRenderDevice___
#define ___IRenderDevice___

#include "line3d.h"
#include "SColor.h"
#include "Point2d.h"
#include "mString.h"

namespace mray{
namespace AI{

class IRenderDevice
{
private:
protected:
public:
	IRenderDevice(){}
	virtual~IRenderDevice(){}

	virtual void drawLine(const math::vector3d&s,const math::vector3d&e,const video::SColor&c)=0;

	virtual void drawText(const math::vector3d&p,const core::string&text,const video::SColor&c)=0;

	virtual void clearTextLines()=0;

	virtual void addTextLine(const core::string&text)=0;

	virtual void renderTriangles(const math::vector3d*points,int ptCnt,const ushort*idx,int idxCnt,const video::SColor&c)=0;

};

}
}


#endif //___IRenderDevice___
