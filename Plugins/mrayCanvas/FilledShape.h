



/********************************************************************
	created:	2011/11/29
	created:	29:11:2011   14:40
	filename: 	d:\Development\mrayEngine\Engine\mrayCanvas\FilledShape.h
	file path:	d:\Development\mrayEngine\Engine\mrayCanvas
	file base:	FilledShape
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __FilledShape__
#define __FilledShape__

#include "ICShape.h"

namespace mray
{
namespace canvas
{

class FilledShape:public ICShape
{
private:
protected:
	video::SColor m_color;
public:
	FilledShape();
	virtual~FilledShape();

	void SetColor(const video::SColor& c){ m_color=c;}
	const video::SColor& GetColor(){ return m_color;}

	virtual void Draw(EMouseStatus status);
};

}
}

#endif
