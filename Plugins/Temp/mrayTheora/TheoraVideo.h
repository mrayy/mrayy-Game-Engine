

/********************************************************************
	created:	2009/05/17
	created:	17:5:2009   18:27
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayTheora\TheoraVideo.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayTheora
	file base:	TheoraVideo
	file ext:	h
	author:		m_ohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___TheoraVideo___
#define ___TheoraVideo___


#include "TheoraFrameQueue.h"

#include <IVideoSource.h>
#include <IThread.h>
#include <IStream.h>
#include <ISound.h>

namespace mray{
namespace video{

	enum ETheoraOutput
	{
		ETO_RGB,
		ETO_YUV,
		ETO_GREY
	};

	class TheoraManager;
	class TheoraWorker;

	class TheoraVideoData;

class TheoraVideo:public IVideoSource
{
private:
protected:

	friend class TheoraManager;
	friend class TheoraWorker;

	// ogg/vorbis/theora variables
	ogg_sync_state   m_oggSyncState;
	ogg_page         m_oggPage;
	ogg_stream_state m_vorbisStreamState;
	ogg_stream_state m_theoraStreamState;
	//Theora State
	th_info        m_theoraInfo;
	th_comment     m_theoraComment;
	th_setup_info* m_theoraSetup;
	th_dec_ctx*    m_theoraDecoder;
	//th_state     m_theoraState;
	//Vorbis State
	vorbis_info      m_vorbisInfo;
	vorbis_dsp_state m_vorbisDSPState;
	vorbis_block     m_vorbisBlock;
	vorbis_comment   m_vorbisComment;

	GCPtr<TheoraFrameQueue> m_frameQueue;

	GCPtr<OS::IStream> m_stream;

	ImageInfo m_imageData;

	TheoraWorker* m_assignedWorker;

	TheoraVideoData* m_data;
	int m_theoraStreams, m_vorbisStreams;	// Keeps track of Theora and Vorbis Streams

	int m_NumPrecachedFrames;

	int m_framesCount;
	float m_fps;
	float m_duration;
	float m_speed;

	bool m_endOfFile;
	bool m_autoRestart;
	bool m_backColourChanged;

	float m_AudioGain; //! multiplier for audio samples. between 0 and 1
	float m_seekPos; //! stores desired seek position. next worker thread will do the seeking and reset this var to -1
	ETheoraOutput m_outputMode;

	GCPtr<OS::IMutex> m_AudioMutex; //! syncs audio decoding and extraction

	EVideoStatus m_status;

	bool m_audioSkipSeekFlag;
	sound::ISoundPtr m_AudioInterface;

	float m_time;

	math::vector2di m_size;
	math::vector2di m_texSize;

	std::list<sound::ISoundStream*> m_audioBuffers;

	int m_priority;
	bool m_hasNewFrame;

	short* m_AudioTempBuffer;
	int m_maxAudioBufferSize;
	int m_AudioBufferSize;


protected:

	bool readTheoraVorbisHeaders();
	void createVideoBuffers();
	void processAudioData(float** data,int len);

	virtual uint calcSizeInternal();
	bool _readData();
public:
	TheoraVideo(int precachedFrames);
	virtual~TheoraVideo();


	int getNumPrecachedFrames();

	void decodeNextFrame();
	bool blitFrame(float dt);
	float _decodedAudioCheck();
	void decodedAudioCheck();

	void setOutputType(ETheoraOutput o);
	ETheoraOutput getOutputType();

	void play();
	void stop();
	void pause();

	EVideoStatus getStatus();

	virtual bool loadVideo(const  core::string&fileName);

	int getPriority();

	void doSeek();

	virtual void SetImageFormat(video::EPixelFormat fmt);
	virtual video::EPixelFormat GetImageFormat();

	virtual void SetFrameSize(int w,int h);
	virtual const math::Point2di& GetFrameSize();
	virtual const math::Point2di& GetTheoraFrameSize();

	virtual float getCurrFrame();
	virtual float getPercentage();
	virtual void setLoop(bool loop);
	virtual bool isLoop();
	virtual bool isDone();
	virtual float getDuration();


	virtual void setSpeedFactor(float speed);
	virtual float getSpeedFactor();
	virtual bool setFrame(float frame);
	virtual bool GrabFrame(float dt);
	virtual bool HasNewFrame();


	virtual const ImageInfo* GetLastFrame(){return &m_imageData;}

};

}
}


#endif //___TheoraVideo___
