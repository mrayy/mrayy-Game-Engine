#include "stdafx.h"


#include "TheoraVideo.h"
#include "TheoraManager.h"

#include <IFileSystem.h>
#include <ILogManager.h>
#include <mraySystem.h>
#include <StringConverter.h>
#include <TextureResourceManager.h>
#include <IThreadManager.h>
#include <ITimer.h>
#include <Engine.h>
#include <IVideoDevice.h>
#include <ISoundManager.h>
#include <ISoundStream.h>

#define _NO_AUDIO_

namespace mray{
namespace video{

	int nextPow2(int x)
	{
		int y;
		for (y=1;y<x;y*=2);
		return y;

	}

//////////////////////////////////////////////////////////////////////////

	class TheoraVideoData
	{
	public:
		TheoraVideoData()
		{
		}

	};

TheoraVideo::TheoraVideo(int precach):
		m_NumPrecachedFrames(precach),
		m_theoraStreams(0),
		m_vorbisStreams(0),
		m_duration(-1),
		m_AudioGain(1),
		m_endOfFile(0),
		m_autoRestart(true),
		m_backColourChanged(0),
		m_seekPos(-1),
		m_outputMode(ETO_RGB),
		m_assignedWorker(0),
		m_speed(1),
		m_AudioInterface(0),
		m_priority(1),
		m_time(0),
		m_AudioTempBuffer(0),
		m_AudioBufferSize(0),
		m_maxAudioBufferSize(0),
		m_framesCount(0),
		m_fps(0),
		m_audioSkipSeekFlag(false)
{

	m_AudioMutex=OS::IThreadManager::getInstance().createMutex();

	mraySystem::memSet(&m_oggSyncState, 0, sizeof(ogg_sync_state));
	mraySystem::memSet(&m_oggPage, 0, sizeof(ogg_page));
	mraySystem::memSet(&m_vorbisStreamState, 0, sizeof(ogg_stream_state));
	mraySystem::memSet(&m_theoraStreamState, 0, sizeof(ogg_stream_state));
	mraySystem::memSet(&m_theoraInfo, 0, sizeof(th_info));
	mraySystem::memSet(&m_theoraComment, 0, sizeof(th_comment));
	mraySystem::memSet(&m_vorbisInfo, 0, sizeof(vorbis_info));
	mraySystem::memSet(&m_vorbisDSPState, 0, sizeof(vorbis_dsp_state));
	mraySystem::memSet(&m_vorbisBlock, 0, sizeof(vorbis_block));
	mraySystem::memSet(&m_vorbisComment, 0, sizeof(vorbis_comment));
	m_theoraSetup=0;
	m_theoraDecoder=0;
	m_hasNewFrame=false;


}
TheoraVideo::~TheoraVideo(){
	while(m_assignedWorker){
		OS::IThreadManager::getInstance().sleep(1);
	}

	if(m_stream){
		m_stream->close();
	}

	th_decode_free(m_theoraDecoder);

	th_setup_free(m_theoraSetup);
	m_AudioMutex->lock();
	m_AudioInterface=0;
	m_AudioMutex->unlock();
	m_AudioMutex=0;

	delete [] m_AudioTempBuffer;

}

ETheoraOutput TheoraVideo::getOutputType(){
	return m_outputMode;
}

int TheoraVideo::getNumPrecachedFrames(){
	return m_NumPrecachedFrames;
}


uint TheoraVideo::calcSizeInternal()
{
	return sizeof(TheoraVideo);
}

int TheoraVideo::getPriority(){
	return m_priority;
}

void TheoraVideo::decodeNextFrame(){

	if (m_endOfFile || m_status!=EVS_Playing && getNumPrecachedFrames() > 0) return;
	TheoraFrame* frame=m_frameQueue->requestAnyFreeFrame();
	if (!frame) return; // max number of precached frames reached
	long seek_granule=-1;
	ogg_packet opTheora;
	ogg_int64_t granulePos;
	th_ycbcr_buffer buff;

	for(;;)
	{
		int ret=ogg_stream_packetout(&m_theoraStreamState,&opTheora);

		if (ret > 0)
		{
			if (th_decode_packetin(m_theoraDecoder, &opTheora,&granulePos ) != 0) continue; // 0 means success
			float time=th_granule_time(m_theoraDecoder,granulePos);
			unsigned long frame_number = (unsigned long) th_granule_frame(m_theoraDecoder, granulePos);
			if (m_seekPos == -2)
			{
				if (!th_packet_iskeyframe(&opTheora)) continue; // get keyframe after seek
				else
				{
					m_seekPos=-3; // -3 means we need to ensure the frame is displayed (otherwise it won't be if the video is paused)

					//if we use audio we must maintain perfect sync
					if (seek_granule != -1)
					{
						m_time=vorbis_granule_time(&m_vorbisDSPState,seek_granule);
					}
				}
			}

			if (time < m_time) continue; // drop frame
			frame->timeToDisplay=time;
			frame->inUse=true;
			th_decode_ycbcr_out(m_theoraDecoder,buff);
			frame->decodeFrame(&buff);
			break;
		}
		else
		{
			if(!_readData())
			{
				break;
			}
		}
	}
	if(m_AudioInterface)
	{
		m_AudioMutex->lock();
		_decodedAudioCheck();
		m_AudioMutex->unlock();
	}
}

bool TheoraVideo::_readData()
{
	int audio_eos = 0, serno;
	float audio_time = 0;
	float time=m_time;

	for(;;)
	{
		char *buffer = ogg_sync_buffer( &m_oggSyncState, 4096);
		int bytesRead = m_stream->read( buffer, 4096 );
		ogg_sync_wrote( &m_oggSyncState, bytesRead );
		if (bytesRead < 4096)
		{
			if (bytesRead == 0)
			{
				if(!m_autoRestart)
					m_endOfFile=true;
				return false;
			}
		}
		while ( ogg_sync_pageout( &m_oggSyncState, &m_oggPage ) > 0 )
		{
			serno=ogg_page_serialno(&m_oggPage);
			if(serno==m_theoraStreamState.serialno)
				ogg_stream_pagein(&m_theoraStreamState,&m_oggPage);
			if (m_AudioInterface && serno==m_vorbisStreamState.serialno)// && m_seekPos != -2)
			{
				ogg_int64_t g = ogg_page_granulepos(&m_oggPage);
				audio_time = (float) vorbis_granule_time(&m_vorbisDSPState, g);
				audio_eos = ogg_page_eos(&m_oggPage);	
				ogg_stream_pagein(&m_vorbisStreamState, &m_oggPage);
			}
		}
		if (!m_AudioInterface || audio_eos || audio_time >= time + 1.0f)
			break;
	}

	return true;
}

bool TheoraVideo::blitFrame(float dt){

	if (m_status!=EVS_Playing && m_seekPos != -3) return false;
	if (m_seekPos == -3) m_seekPos=-1; // -3 ensures the first frame after seek gets displayed even when the movie is paused
	TheoraFrame* frame;
	/*if (m_autoRestart && m_time >= getDuration())
	{
		m_time=m_time-getDuration();
	}*/

	while (true)
	{
		frame=m_frameQueue->requestFirstAvaliableFrame();
		if (!frame) 
		{
			if(m_endOfFile && m_autoRestart)
			{
				long granule=0;
				th_decode_ctl(m_theoraDecoder,TH_DECCTL_SET_GRANPOS,&granule,sizeof(granule));
				th_decode_free(m_theoraDecoder);
				m_theoraDecoder=th_decode_alloc(&m_theoraInfo,m_theoraSetup);
				ogg_stream_reset(&m_theoraStreamState);
				ogg_sync_reset(&m_oggSyncState);
				m_stream->seek(0,OS::ESeek_Set);
				m_time=0;
				m_endOfFile=false;
				m_frameQueue->clear();
			}
			return false;
		}

/*		if (frame->timeToDisplay-m_time > 1)
		{
			m_frameQueue->pop(); // this happens on Auto restart if there are some leftover frames
			continue;
		}
*/		if (frame->timeToDisplay > m_time) return false;
		if (frame->timeToDisplay < m_time-0.1)
		{
			m_frameQueue->pop();
		}
		else break;
	}
	if(m_seekPos==-3)
		m_seekPos=-1;

	// use blitFromMemory or smtg faster
	mraySystem::memCopy(m_imageData.imageData,frame->getBuffer(),m_texSize.x*m_size.y*4);
	//m_imageData.imageData=frame->getBuffer();

	if (m_backColourChanged)
	{
		mraySystem::memSet(m_imageData.imageData+m_texSize.x*m_size.y*4,
			m_frameQueue->getFillColor(),m_texSize.x*(m_texSize.y-m_size.y)*4);
		m_backColourChanged=false;
	}


	m_frameQueue->pop(); // after transfering frame data to the texture, free the frame
	// so it can be used again
	return true;
}

float TheoraVideo::_decodedAudioCheck()
{
	if (!m_AudioInterface || m_status!=EVS_Playing) return -1;


	ogg_packet opVorbis;
	float **pcm;
	int len=0;
	int total=0;
	float timeStamp=-1;
	bool read_past_timestamp = 0;

	while (1)
	{
		len = vorbis_synthesis_pcmout(&m_vorbisDSPState,&pcm);
		if (!len)
		{
			if (ogg_stream_packetout(&m_vorbisStreamState,&opVorbis) > 0)
			{
				if (vorbis_synthesis(&m_vorbisBlock,&opVorbis) == 0)
				{
					if(timeStamp<0 && opVorbis.granulepos>=0)
					{
						timeStamp=(float) vorbis_granule_time(&m_vorbisDSPState, opVorbis.granulepos);
					}else if(timeStamp>=0)
					{
						read_past_timestamp=1;
					}
					vorbis_synthesis_blockin(&m_vorbisDSPState,&m_vorbisBlock);
				}
				continue;
			}
			else break;
		}/*
		if (m_AudioGain < 1)
		{
			// gain applied, let's attenuate the samples
			for (int i=0;i<len;i++)
			{
				for (int j=0;j<m_vorbisInfo.channels;j++)
				{
					pcm[j][i]*=m_AudioGain;
				}
			}
		}*/
		total+=len;

		m_AudioMutex->lock();
		processAudioData(pcm,len);
		m_AudioMutex->unlock();
		if(read_past_timestamp)
		{
			timeStamp+=(float)len/m_vorbisInfo.rate;
		}
		vorbis_synthesis_read(&m_vorbisDSPState,len);
	}

	return timeStamp;
}

void TheoraVideo::decodedAudioCheck()
{
	if(!m_AudioInterface)
		return;
	m_AudioMutex->lock();
	std::list<sound::ISoundStream*> tmp(m_audioBuffers);
	m_audioBuffers.clear();
	m_AudioMutex->unlock();

	std::list<sound::ISoundStream*>::iterator it= tmp.begin();
	for (;it!=tmp.end();++it)
	{
		m_AudioInterface->QueueSoundBuffer(*it);
	}
	if(!m_AudioInterface->isPlaying())
		m_AudioInterface->play();
}
void TheoraVideo::processAudioData(float** data,int len)
{
	if(!m_AudioInterface)
		return;
	for(int i=0;i<len;++i)
	{
		if(m_AudioBufferSize<m_maxAudioBufferSize)
		{
			m_AudioTempBuffer[m_AudioBufferSize++]=(short)(math::clamp<float>(data[0][i],-1,1)*32767);
			if(m_vorbisInfo.channels==2)
				m_AudioTempBuffer[m_AudioBufferSize++]=(short)(math::clamp<float>(data[1][i],-1,1)*32767);
		}
		if(m_AudioBufferSize==m_vorbisInfo.rate*m_vorbisInfo.channels/4)
		{
			sound::ISoundStream* stream=TheoraManager::getInstance().getSoundManager()->CreateManualStream();
			stream->SetStreamData(m_AudioTempBuffer,m_AudioBufferSize,m_vorbisInfo.rate,sound::ESF_16,m_vorbisInfo.channels);
			
			m_audioBuffers.push_back(stream);
			m_AudioBufferSize=0;
		}
	}
}

bool TheoraVideo::loadVideo(const  core::string&fileName){
	m_stream=gFileSystem.openFile(fileName);
	if(!m_stream){
		gLogManager.log( mT("Theora File not found: ")+fileName,ELL_WARNING);
		return false;
	}

	if(!readTheoraVorbisHeaders())
		return false;

	m_theoraDecoder=th_decode_alloc(&m_theoraInfo,m_theoraSetup);

	m_size.x=m_theoraInfo.frame_width;
	m_size.y=m_theoraInfo.frame_height;
	m_fps=m_theoraInfo.fps_numerator/m_theoraInfo.fps_denominator;

	//m_texSize.x=nextPow2(m_size.x);
	//m_texSize.y=nextPow2(m_size.y);
	m_texSize=m_size;

	m_frameQueue=MRAY_new TheoraFrameQueue(this,m_NumPrecachedFrames);

	setOutputType(m_outputMode);

	//long stream_pos=m_stream->getPos();

	for (int i=1;i<=50;i++)
	{
		ogg_sync_reset(&m_oggSyncState);
		m_stream->seek(m_stream->length()-4096*i,OS::ESeek_Set);

		char *buffer = ogg_sync_buffer( &m_oggSyncState, 4096*i);
		int bytesRead = m_stream->read( buffer, 4096*i);
		ogg_sync_wrote( &m_oggSyncState, bytesRead );
		long offset=ogg_sync_pageseek(&m_oggSyncState,&m_oggPage);

		while (1)
		{
			int ret=ogg_sync_pageout( &m_oggSyncState, &m_oggPage );
			if (ret == 0)
				break;
			int serno=ogg_page_serialno(&m_oggPage);
			// if page is not a theora page, skip it
			if (serno != m_theoraStreamState.serialno) continue;

			ogg_int64_t granule=ogg_page_granulepos(&m_oggPage);
			if (granule >= 0)
				m_framesCount=th_granule_frame(m_theoraDecoder,granule)+1;
			else if(m_duration>0)
				++m_framesCount;
		}
		if (m_framesCount > 0) break;

	}
	if (m_framesCount < 0)
	{
		gLogManager.log(mT("TheoraVideo: unable to determine file duration!"),ELL_WARNING);
	}
	m_duration=m_framesCount/m_fps;
	// restore to beginning of stream.
	// the following solution is temporary and hacky, will be replaced soon

	ogg_sync_reset(&m_oggSyncState);
	m_stream->seek(0,OS::ESeek_Set);
	/*memset(&m_oggSyncState, 0, sizeof(ogg_sync_state));
	memset(&m_oggPage, 0, sizeof(ogg_page));
	memset(&m_vorbisStreamState, 0, sizeof(ogg_stream_state));
	memset(&m_theoraStreamState, 0, sizeof(ogg_stream_state));
	memset(&m_theoraInfo, 0, sizeof(th_info));
	memset(&m_theoraComment, 0, sizeof(th_comment));
	//memset(&m_theoraState, 0, sizeof(th_state));
	memset(&m_vorbisInfo, 0, sizeof(vorbis_info));
	memset(&m_vorbisDSPState, 0, sizeof(vorbis_dsp_state));
	memset(&m_vorbisBlock, 0, sizeof(vorbis_block));
	memset(&m_vorbisComment, 0, sizeof(vorbis_comment));
	m_theoraStreams=m_vorbisStreams=0;
	readTheoraVorbisHeaders();*/

	// END HACKY CODE

	if (m_vorbisStreams) // if there is no audio interface factory defined, even though the video
		// clip might have audio, it will be ignored
	{
		vorbis_synthesis_init(&m_vorbisDSPState,&m_vorbisInfo);
		vorbis_block_init(&m_vorbisDSPState,&m_vorbisBlock);
		// create an audio interface instance if available
		sound::ISoundManager* smngr=TheoraManager::getInstance().getSoundManager();
		if (smngr) 
		{
			m_AudioInterface=smngr->CreateManualSound(fileName+"_audio");
			m_maxAudioBufferSize=m_vorbisInfo.rate*m_vorbisInfo.channels*2;
			m_AudioTempBuffer=new short[m_maxAudioBufferSize];
			m_AudioBufferSize=0;
		}
	}

	createVideoBuffers();

	return true;
}
void TheoraVideo::createVideoBuffers()
{
	m_imageData.createData(math::vector3d(m_texSize.x,m_texSize.y,1),EPixel_X8R8G8B8);
	if(m_outputMode==ETO_YUV){
		mraySystem::memSet(m_imageData.imageData,0xFF008080,m_texSize.x*m_texSize.y*4);
	}else
		mraySystem::memSet(m_imageData.imageData,0,m_texSize.x*m_texSize.y*4);

// 	m_imageData.autoDel=false;
// 	m_imageData.Size.set(m_texSize.x,m_texSize.y,1);
// 	m_imageData.format=EPixel_X8R8G8B8;
// 	m_imageData.imageDataSize=m_texSize.x*m_texSize.y*4;
}


bool TheoraVideo::readTheoraVorbisHeaders()
{
	ogg_packet tempOggPacket;
	bool decode_audio=TheoraManager::getInstance().getSoundManager()!=0;
	//init Vorbis/Theora Layer
	//start up Ogg stream synchronization layer
	ogg_sync_init(&m_oggSyncState);
	//init supporting Theora structures needed in header parsing
	th_comment_init(&m_theoraComment);
	th_info_init(&m_theoraInfo);
	//init supporting Vorbis structures needed in header parsing
	vorbis_info_init(&m_vorbisInfo);
	vorbis_comment_init(&m_vorbisComment);

	bool done = false;
	while (!done)
	{
		char *buffer = ogg_sync_buffer( &m_oggSyncState, 4096);
		int bytesRead = m_stream->read( buffer, 4096 );
		ogg_sync_wrote( &m_oggSyncState, bytesRead );

		if( bytesRead == 0 )
			break;

		while( ogg_sync_pageout( &m_oggSyncState, &m_oggPage ) > 0 )
		{
			ogg_stream_state OggStateTest;

			//is this an initial header? If not, stop
			if( !ogg_page_bos( &m_oggPage ) )
			{
				//This is done blindly, because stream only accept them selfs
				if (m_theoraStreams) 
					ogg_stream_pagein( &m_theoraStreamState, &m_oggPage );
				if (m_vorbisStreams) 
					ogg_stream_pagein( &m_vorbisStreamState, &m_oggPage );

				done=true;
				break;
			}

			ogg_stream_init( &OggStateTest, ogg_page_serialno( &m_oggPage ) );
			ogg_stream_pagein( &OggStateTest, &m_oggPage );
			ogg_stream_packetout( &OggStateTest, &tempOggPacket );

			//identify the codec
			int ret;
			if( !m_theoraStreams)
			{
				ret=th_decode_headerin( &m_theoraInfo, &m_theoraComment, &m_theoraSetup, &tempOggPacket);

				if (ret > 0)
				{
					//This is the Theora Header
					mraySystem::memCopy( &m_theoraStreamState, &OggStateTest, sizeof(OggStateTest));
					m_theoraStreams = 1;
					continue;
				}
			}
			if (decode_audio && !m_vorbisStreams &&
				vorbis_synthesis_headerin(&m_vorbisInfo, &m_vorbisComment, &tempOggPacket) >=0 )
			{
				//This is vorbis header
				mraySystem::memCopy( &m_vorbisStreamState, &OggStateTest, sizeof(OggStateTest));
				m_vorbisStreams = 1;
				continue;
			}

			//Hmm. I guess it's not a header we support, so erase it
			ogg_stream_clear(&OggStateTest);
		}
	}

	while ((m_theoraStreams && (m_theoraStreams < 3)) ||
		(m_vorbisStreams && (m_vorbisStreams < 3)) )
	{
		//Check 2nd'dary headers... Theora First
		int iSuccess;
		while( m_theoraStreams && 
			( m_theoraStreams < 3) && 
			( iSuccess = ogg_stream_packetout( &m_theoraStreamState, &tempOggPacket)) ) 
		{
			if( iSuccess < 0 ) {
				gLogManager.log( mT("Error parsing Theora stream headers."),ELL_WARNING);
				return false;
			}

			if( !th_decode_headerin(&m_theoraInfo, &m_theoraComment, &m_theoraSetup, &tempOggPacket) ){
				gLogManager.log( mT("invalid Theora stream."),ELL_WARNING);
				return false;
			}

			m_theoraStreams++;			
		} //end while looking for more theora headers

		//look 2nd vorbis header packets
		while(// m_vorbisStreams && 
			( m_vorbisStreams < 3 ) && 
			( iSuccess=ogg_stream_packetout( &m_vorbisStreamState, &tempOggPacket))) 
		{
			if(iSuccess < 0) {
				gLogManager.log( mT("Error parsing vorbis stream headers"),ELL_WARNING);
				return false;
			}

			if(vorbis_synthesis_headerin( &m_vorbisInfo, &m_vorbisComment,&tempOggPacket)) {
				gLogManager.log( mT("invalid Theora stream"),ELL_WARNING);
				return false;
			}

			m_vorbisStreams++;
		} //end while looking for more vorbis headers

		//Not finished with Headers, get some more file data
		if( ogg_sync_pageout( &m_oggSyncState, &m_oggPage ) > 0 )
		{
			if(m_theoraStreams) 
				ogg_stream_pagein( &m_theoraStreamState, &m_oggPage );
			if(m_vorbisStreams) 
				ogg_stream_pagein( &m_vorbisStreamState, &m_oggPage );
		}
		else
		{
			char *buffer = ogg_sync_buffer( &m_oggSyncState, 4096);
			int bytesRead = m_stream->read( buffer, 4096 );
			ogg_sync_wrote( &m_oggSyncState, bytesRead );

			if( bytesRead == 0 ){
				gLogManager.log( mT("End of Theora file found prematurely"),ELL_WARNING);
				return false;
			}

		}
	} //end while looking for all headers

	core::string temp1 = core::StringConverter::toString( m_vorbisStreams );
	core::string temp2 = core::StringConverter::toString( m_theoraStreams );
	gLogManager.log( mT("Vorbis Headers: ") + temp1 + mT(" Theora Headers : ") + temp2,ELL_SUCCESS);
	return true;
}



void TheoraVideo::setOutputType(ETheoraOutput o){
	m_outputMode=o;
	if(m_outputMode==ETO_YUV)m_frameQueue->setFillColor(0xFF008080);
	else m_frameQueue->setFillColor(0xFF000000);

	m_backColourChanged=true;
}

void TheoraVideo::play(){
	m_status=EVS_Playing;
}
void TheoraVideo::stop(){
	m_status=EVS_Stopped;
	m_time=0;
}
void TheoraVideo::pause(){
	if(m_status==EVS_Playing)
		m_status=EVS_Paused;
}
EVideoStatus TheoraVideo::getStatus(){
	return m_status;
}


void TheoraVideo::doSeek()
{
	int i,seek_min=0, seek_max=m_stream->length();
	float time;
	ogg_int64_t granule,th_granule;

	m_frameQueue->clear();
	ogg_stream_reset(&m_theoraStreamState);
	th_decode_free(m_theoraDecoder);
	if (m_AudioInterface)
	{
		vorbis_synthesis_restart(&m_vorbisDSPState);
		ogg_stream_reset(&m_vorbisStreamState);
		m_AudioMutex->lock();
	}
	m_theoraDecoder=th_decode_alloc(&m_theoraInfo,m_theoraSetup);

	for (i=0;i<10;i++) // max 10 seek operations
	{
		ogg_sync_reset( &m_oggSyncState );
		m_stream->seek((seek_min+seek_max)/2,OS::ESeek_Set);

		memset(&m_oggPage, 0, sizeof(ogg_page));
		ogg_sync_pageseek(&m_oggSyncState,&m_oggPage);
		while (1)
		{
			int ret=ogg_sync_pageout( &m_oggSyncState, &m_oggPage );
			if (ret == 1)
			{
				//ogg_stream_pagein( &m_theoraStreamState, &m_oggPage );

				int serno=ogg_page_serialno(&m_oggPage);
				if (serno == m_theoraStreamState.serialno)
				{
					int g=ogg_page_granulepos(&m_oggPage);
					if (g > -1) th_granule=g;
				}
				// if audio is available, use vorbis for time positioning
				// othervise use theora
				if ( m_AudioInterface && serno == m_vorbisStreamState.serialno ||
					!m_AudioInterface && serno == m_theoraStreamState.serialno)
				{
					granule=ogg_page_granulepos(&m_oggPage);
					if (granule >= 0 && th_granule >= 0) break;
				}
				else continue; // unknown page (could be flac or whatever)
				int eos=ogg_page_eos(&m_oggPage);
				if (eos > 0) break;
			}
			else
			{
				char *buffer = ogg_sync_buffer( &m_oggSyncState, 4096);
				int bytesRead = m_stream->read( buffer, 4096);
				if (bytesRead < 4096) break;
				ogg_sync_wrote( &m_oggSyncState, bytesRead );
			}
		}
		if (m_AudioInterface)
			time=vorbis_granule_time(&m_vorbisDSPState,granule);
		else
			time=th_granule_time(m_theoraDecoder,granule);
		if (time <= m_seekPos && m_seekPos-time < 0.5) break; // ok, we're close enough

		if (time < m_seekPos) seek_min=(seek_min+seek_max)/2;
		else				 seek_max=(seek_min+seek_max)/2;
	}
	ogg_sync_reset( &m_oggSyncState );
	th_decode_ctl(m_theoraDecoder,TH_DECCTL_SET_GRANPOS,&th_granule,sizeof(th_granule));
	m_time=time; // this will be changed in decodeNextFrame when seeking to the next keyframe
	m_stream->seek((seek_min+seek_max)/2,OS::ESeek_Set);
	m_seekPos=-2;
	m_audioSkipSeekFlag=false;

	if (m_AudioInterface) m_AudioMutex->unlock();
}


void TheoraVideo::SetImageFormat(video::EPixelFormat fmt)
{
}
video::EPixelFormat TheoraVideo::GetImageFormat()
{
	return EPixel_X8R8G8B8;
}


void TheoraVideo::SetFrameSize(int w,int h)
{
	if(m_texSize.x==w && m_texSize.y==h)
		return;
	m_texSize.set(w,h);
	createVideoBuffers();
}
const math::Point2di& TheoraVideo::GetFrameSize()
{
	return m_texSize;
}
const math::Point2di& TheoraVideo::GetTheoraFrameSize()
{
	return m_size;
}

float TheoraVideo::getCurrFrame(){
	return m_time;
}
float TheoraVideo::getPercentage(){
	return m_time/m_duration;
}
void TheoraVideo::setLoop(bool loop){
	m_autoRestart=loop;
}
bool TheoraVideo::isLoop(){
	return m_autoRestart;
}
bool TheoraVideo::isDone(){
	return m_time==m_duration;
}
float TheoraVideo::getDuration(){
	return m_duration;
}

void TheoraVideo::setSpeedFactor(float speed){
	m_speed=speed;
}
float TheoraVideo::getSpeedFactor(){
	return m_speed;
}
bool TheoraVideo::setFrame(float frame){
	m_time=frame;
	return true;
}
bool TheoraVideo::GrabFrame(float dt){
	if(m_status==EVS_Playing)
	{
		m_time+=m_speed*dt;

		if(!m_frameQueue->requestFirstAvaliableFrame().isNull())
		{
			//setFrame(m_time+m_speed*dt);
			m_hasNewFrame=blitFrame(dt);
			//decodedAudioCheck();
			return m_hasNewFrame;
		}
	}
	return false;
}
bool TheoraVideo::HasNewFrame()
{
	return m_hasNewFrame;
}


}
}