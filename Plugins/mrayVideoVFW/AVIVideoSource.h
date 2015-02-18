
/********************************************************************
	created:	2009/02/10
	created:	10:2:2009   0:33
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\AVIVideoSource.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	AVIVideoSource
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	Implementation for video texture : Video for windows
*********************************************************************/

#ifndef ___AVIVideoSource___
#define ___AVIVideoSource___




#include "IVideoSource.h"



namespace mray{
namespace video{


	struct AVIVideoSourceData;

class  AVIVideoSource:public IVideoSource
{
protected:

	AVIVideoSourceData* m_data;

	int m_lastFrame;
	math::Point2di m_SrcSize;
	math::Point2di m_DstSize;
	uchar*data;

	ImageInfo m_Texture;

	float m_invLastFrame;
	float m_mpf;
	float m_frame;

	float m_speedFactor;

	EVideoStatus m_status;

	bool m_loop;

	bool m_hasNewFrame;

	video::EPixelFormat m_pixelFormat;

	void destory();

protected:
	virtual uint calcSizeInternal();

public:

	AVIVideoSource();
	virtual~AVIVideoSource();

	bool loadVideo(const  core::string&fileName);


	bool setFrame(float frame);
	bool GrabFrame();
	virtual bool HasNewFrame(){return m_hasNewFrame;}


	void setSpeedFactor(float speed);
	float getSpeedFactor();

	float getDuration();

	float getCurrFrame();
	float getPercentage();
	void setLoop(bool loop);
	bool isLoop();
	bool isDone();

	void play();
	void stop();
	void pause();
	
	EVideoStatus getStatus();

	void SetFrameSize(int w,int h);
	const math::vector2di& GetFrameSize(){return m_DstSize;}

	virtual void SetImageFormat(video::EPixelFormat fmt);
	virtual video::EPixelFormat GetImageFormat();

	const ImageInfo* GetLastFrame();
};

}
}

#endif //___AVIVideoSource___






