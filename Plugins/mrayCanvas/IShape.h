


/********************************************************************
	created:	2011/11/29
	created:	29:11:2011   16:36
	filename: 	d:\Development\mrayEngine\Engine\mrayCanvas\IShape.h
	file path:	d:\Development\mrayEngine\Engine\mrayCanvas
	file base:	IShape
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __IShape__
#define __IShape__

#include "CanvasCommon.h"

namespace mray
{
namespace canvas
{

	class ICanvasObject;

class IShape
{
private:
protected:
	ICanvasObject* m_owner;
public:
	IShape():m_owner(0){}
	virtual~IShape(){}

	void SetOwner(ICanvasObject*o){m_owner=o;}
	ICanvasObject* GetOwner(){return m_owner;}

	virtual void Draw(EMouseStatus status)=0;

	virtual bool IsPointIn(const math::vector2d& pt)=0;

	virtual const math::rectf& GetBoundingRect()=0;
};

}
}

#endif
