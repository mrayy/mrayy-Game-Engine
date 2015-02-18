#include "stdafx.h"

#include "TheoraWorker.h"
#include "TheoraManager.h"
#include "TheoraVideo.h"



namespace mray{
namespace video{

TheoraWorker::TheoraWorker()
:m_video(0)
{
}
TheoraWorker::~TheoraWorker(){
}

bool TheoraWorker::ExecuteJob()
{
	if(m_video)
	{
		if(m_video->m_seekPos>=0)
			m_video->doSeek();
		m_video->decodeNextFrame();
		
		m_video->m_assignedWorker=0;
		m_video=0;
	}

	return true;
}


}
}

