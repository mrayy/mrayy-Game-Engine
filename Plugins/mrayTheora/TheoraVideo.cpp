#include "stdafx.h"


#include "TheoraVideo.h"
#include "TheoraManager.h"

#include "IFileSystem.h"
#include "PixelUtil.h"


namespace mray{
namespace video{


TheoraVideo::TheoraVideo(TheoraVideoClip*clip):m_clip(clip)
{

	m_hasNewFrame=false;
	m_bufferID = 0;

}
TheoraVideo::~TheoraVideo()
{

}

uint TheoraVideo::calcSizeInternal()
{
	return sizeof(TheoraVideo);
}


bool TheoraVideo::loadVideo(const  core::string&fileName){

	core::string path;
	gFileSystem.getCorrectFilePath(fileName,path);
	if(path=="")
		return false;
	try
	{
		m_clip->loadClip(path.c_str());
	}
	catch(std::exception e)
	{
	}
	createVideoBuffers();

	return true;
}
void TheoraVideo::createVideoBuffers()
{
	TheoraOutputMode o= m_clip->getOutputMode();
	EPixelFormat fmt=EPixel_R8G8B8;
	int comps=3;
	switch(o)
	{
	case TH_RGB:
	case TH_YUV:
	case TH_GREY3:
		fmt=EPixel_R8G8B8;
		comps=3;
		break;
	case TH_RGBA:
	case TH_GREY3A:
		fmt=EPixel_R8G8B8A8;
		comps=4;
		break;
	case TH_RGBX:
	case TH_GREY3X:
	case TH_YUVX:
		fmt=EPixel_X8R8G8B8;
		comps=4;
		break;
	case TH_BGR:
		fmt=EPixel_B8G8R8;
		comps=3;
		break;
	case TH_BGRA:
		fmt=EPixel_B8G8R8A8;
		comps=4;
		break;
	case TH_BGRX:
		fmt=EPixel_X8B8G8R8;
		comps=4;
		break;
	case TH_GREY:
		fmt=EPixel_LUMINANCE8;
		comps=1;
		break;
	}
	m_size.set(m_clip->getWidth(),m_clip->getHeight());
	m_imageData.createData(math::vector3d(m_clip->getWidth(),m_clip->getHeight(),1),fmt);
	mraySystem::memSet(m_imageData.imageData,0x00,m_clip->getHeight()*m_clip->getWidth()*comps);

}


void TheoraVideo::play(){
	m_clip->play();
}
void TheoraVideo::stop()
{
	m_clip->stop();
}
void TheoraVideo::pause(){
	m_clip->pause();
}
EVideoStatus TheoraVideo::getStatus(){
	EVideoStatus s;
	if(m_clip->isPaused())
	{
		if(m_clip->getTimePosition()==0)
			s=EVS_Stopped;
		else
			s=EVS_Paused;
	}else
		s=EVS_Playing;
	return s;
}


void TheoraVideo::SetImageFormat(video::EPixelFormat fmt)
{
}
video::EPixelFormat TheoraVideo::GetImageFormat()
{
	return m_imageData.format;
}


void TheoraVideo::SetFrameSize(int w,int h)
{
}
const math::Point2di& TheoraVideo::GetFrameSize()
{
	return m_size;
}

float TheoraVideo::getTime(){
	return m_clip->getTimePosition();
}
float TheoraVideo::getPercentage(){
	return m_clip->getTimePosition()/m_clip->getDuration();
}
void TheoraVideo::setLoop(bool loop)
{
	m_clip->setAutoRestart(loop);
}
bool TheoraVideo::isLoop(){
	return m_clip->getAutoRestart();
}
bool TheoraVideo::isDone(){
	return m_clip->isDone();
}
float TheoraVideo::getDuration(){
	return m_clip->getDuration();
}

void TheoraVideo::setSpeedFactor(float speed){
	m_clip->setPlaybackSpeed(speed);
}
float TheoraVideo::getSpeedFactor(){
	return m_clip->getPlaybackSpeed();
}
bool TheoraVideo::seek(float time)
{
	m_clip->seek(time);
	return true;
}
bool TheoraVideo::GrabFrame(){
	m_hasNewFrame=false;
	TheoraVideoFrame* f=m_clip->getNextFrame();
	if(!f)
		return false;
	m_bufferID++;
	m_hasNewFrame=true;
	int pixelsCount=video::PixelUtil::getPixelDescription(m_imageData.format).elemSizeB;
	mraySystem::memCopy(m_imageData.imageData,f->getBuffer(),f->getHeight()*f->getWidth()*pixelsCount);
	m_clip->popFrame();
	return true;
}
bool TheoraVideo::HasNewFrame()
{
	return m_hasNewFrame;
}


}
}