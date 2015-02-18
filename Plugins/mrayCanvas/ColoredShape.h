


/********************************************************************
	created:	2011/11/29
	created:	29:11:2011   14:42
	filename: 	d:\Development\mrayEngine\Engine\mrayCanvas\ColoredShape.h
	file path:	d:\Development\mrayEngine\Engine\mrayCanvas
	file base:	ColoredShape
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __ColoredShape__
#define __ColoredShape__

#include "ICShape.h"


namespace mray
{
namespace canvas
{

class ColoredShape:public ICShape
{
private:
protected:
	std::vector<video::SColor> m_pointColors;
	video::SColor m_cornersColor[4];
public:
	ColoredShape();
	virtual~ColoredShape();

	void SetColors(const video::SColor* c);

	virtual void Draw(EMouseStatus status);

	virtual void SetShapePoints(const PointList &points);

	const std::vector<video::SColor>& GetPointColors();
};

}
}

#endif
