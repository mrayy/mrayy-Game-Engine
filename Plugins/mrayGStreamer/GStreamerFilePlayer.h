

/********************************************************************
	created:	2014/02/22
	created:	22:2:2014   18:00
	filename: 	C:\Development\mrayEngine\Plugins\mrayGStreamer\GStreamerFilePlayer.h
	file path:	C:\Development\mrayEngine\Plugins\mrayGStreamer
	file base:	GStreamerFilePlayer
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __GStreamerFilePlayer__
#define __GStreamerFilePlayer__


#include "IGStreamerPlayer.h"

namespace mray
{
namespace video
{

class GStreamerFilePlayer:public IGStreamerPlayer
{
protected:
	core::string& m_videoPath;
public:
	GStreamerFilePlayer();
	virtual~GStreamerFilePlayer();

	bool LoadMovie(const core::string& path);
};

}
}


#endif
