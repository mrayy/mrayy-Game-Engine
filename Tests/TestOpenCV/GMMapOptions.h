

/********************************************************************
	created:	2013/01/27
	created:	27:1:2013   0:03
	filename: 	C:\Development\mrayEngine\Tests\TestOpenCV\GMMapOptions.h
	file path:	C:\Development\mrayEngine\Tests\TestOpenCV
	file base:	GMMapOptions
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __GMMapOptions__
#define __GMMapOptions__

#include "GMMapTypeControlOptions.h"


namespace mray
{
namespace google
{

class GMStyleInfo;

class GMMapOptions
{
protected:
public:
	GMMapOptions();
	virtual~GMMapOptions();

	video::SColor backcolor;
	math::vector2d center;
	bool disableUI;
	bool disableDoubleClickZoom;
	bool draggable;
	float heading;
	bool keyboardShortcuts;
	bool mapMarker;
	GMMapTypeControlOptions options;
	core::string mapTypeID;
	float maxZoom;
	float minZoom;
	float zoom;
	bool noClear;
	bool streetView;
	std::vector<GMStyleInfo*> styles;

};

}
}


#endif
