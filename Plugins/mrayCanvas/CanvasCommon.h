

/********************************************************************
	created:	2011/11/29
	created:	29:11:2011   9:04
	filename: 	d:\Development\mrayEngine\Engine\mrayCanvas\CanvasCommon.h
	file path:	d:\Development\mrayEngine\Engine\mrayCanvas
	file base:	CanvasCommon
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __CanvasCommon__
#define __CanvasCommon__


namespace mray
{
namespace canvas
{

	typedef std::vector<math::vector2d> PointList;

	enum EMouseStatus
	{
		ENone,
		EHovered,
		EClicked
	};
}
}

#endif

