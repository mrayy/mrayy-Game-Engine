

/********************************************************************
	created:	2011/11/29
	created:	29:11:2011   16:40
	filename: 	d:\Development\mrayEngine\Engine\mrayCanvas\TextShape.h
	file path:	d:\Development\mrayEngine\Engine\mrayCanvas
	file base:	TextShape
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __TextShape__
#define __TextShape__

#include "IShape.h"
#include "UTFString.h"

namespace mray
{
namespace canvas
{

class TextShape:public IShape
{
private:
protected:
	core::UTFString m_string;
	math::rectf m_rect;
	video::SColor m_color;
public:
	TextShape();
	virtual~TextShape();

	void SetString(const core::UTFString&str){m_string=str;}
	const core::UTFString& GetString(){return m_string;}

	void SetRect(const math::rectf& r){ m_rect=r;}
	const math::rectf& GetRect(){ return m_rect;}

	virtual void Draw(EMouseStatus status);

	virtual bool IsPointIn(const math::vector2d& pt);
	virtual const math::rectf& GetBoundingRect(){return m_rect;}

};

}
}

#endif
