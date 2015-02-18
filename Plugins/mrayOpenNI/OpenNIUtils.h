

/********************************************************************
	created:	2013/08/16
	created:	16:8:2013   18:19
	filename: 	C:\Development\mrayEngine\Plugins\mrayOpenNI\OpenNIUtils.h
	file path:	C:\Development\mrayEngine\Plugins\mrayOpenNI
	file base:	OpenNIUtils
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __OpenNIUtils__
#define __OpenNIUtils__

#include <NiTE.h>
#include <LockedPixelBox.h>

namespace mray
{

class OpenNIUtils
{
protected:
public:
	static bool ConvertToTexture(openni::VideoFrameRef& stream,video::LockedPixelBox& buff, std::vector<float>* histo=0);
	static bool CalculateHistogram(openni::VideoFrameRef& stream,std::vector<float>& histo);
};

}


#endif
