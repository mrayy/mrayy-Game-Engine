 

#include "SALSound.h"
#include "SALSoundManager.h"
#include <openal\al.h>
#include <LogManager.h>
#include <TraceManager.h>
 
namespace mray{
namespace sound{

	static int BUFFER_SIZE=4096 * 4;

SALSound::SALSound(SALSoundManager*sndMngr,const core::string&name,math::vector3d pos,math::vector3d vel,math::vector3d dir):
ISound(name),
soundSystem(sndMngr),m_pitch(1),m_volume(1),m_looping(0),m_bAttachToListener(0),dieAfterEnd(0),dataFormat(0)
,m_SourceID(0),m_format(0),m_bufferCount(0)
{

#ifdef ___DEBUG___
	setDebugName(mT("SALSound"));
#endif 

	m_buffersID[0]=0;
	alGenSources(1,&m_SourceID);
	

    alSource3f(m_SourceID, AL_POSITION,        0.0, 0.0, 0.0);
    alSource3f(m_SourceID, AL_VELOCITY,        0.0, 0.0, 0.0);
    alSource3f(m_SourceID, AL_DIRECTION,       0.0, 0.0, 0.0);
    alSourcef (m_SourceID, AL_ROLLOFF_FACTOR,  0.0          );
    alSourcei (m_SourceID, AL_SOURCE_RELATIVE, AL_TRUE      );
	/*
	alSourcef(m_SourceID,AL_REFERENCE_DISTANCE,0);
	setPosition(pos);
	setVelocity(vel);
	setDirection(dir);
	setVolume(m_volume);
	setMinDistance(1);
	setMaxDistance(10);*/
}
SALSound::~SALSound(){
	traceFunction(eSound);
	alSourceStop(m_SourceID);

	if(m_buffersID[0] && m_bufferCount!=0){
		try{
			alDeleteBuffers(m_bufferCount,&m_buffersID[0]);
			m_buffersID[0]=0;
		}catch(...){}
	}
	if(m_SourceID){
		try{
			alDeleteSources(1,&m_SourceID);
			m_SourceID=0;
		}catch(...){}
	}
	
	
}



void SALSound::checkForError(const mchar*info)
{
	core::string Err;
	int error = alGetError();

	if(error != AL_NO_ERROR){
		core::char_to_string(alGetString(error),Err);
		gLogManager.log(info+Err,ELL_ERROR);
	}
/*
	int error = alutGetError();
	if(error != ALUT_ERROR_NO_ERROR)
		gLogManager.log(info,alutGetErrorString(error),ELL_ERROR);*/
}

void SALSound::setPosition(math::vector3d pos){
	position=pos;

	alSource3f(m_SourceID,AL_POSITION,pos.x,pos.y,pos.z);
	checkForError(mT("SALSound::setPosition()-alSource3f(AL_POSITION)"));

}


void SALSound::setVelocity(math::vector3d vel){
	velocity=vel;

	alSource3f(m_SourceID,AL_VELOCITY,vel.x,vel.y,vel.z);
	checkForError(mT("SALSound::setVelocity()-alSource3f(AL_VELOCITY)"));

}

void SALSound::setDirection(math::vector3d dir){
	direction=dir;

	alSource3f(m_SourceID,AL_DIRECTION,dir.x,dir.y,dir.z);
	checkForError(mT("SALSound::setDirection()-alSource3f(AL_DIRECTION)"));

}

void SALSound::setPitch(float pitch){
	if(pitch<0)return;//should be positive
	m_pitch=pitch;

	alSourcef(m_SourceID,AL_PITCH,pitch);
	checkForError(mT("SALSound::setPitch()-alSource3(AL_PITCH)"));

}

void SALSound::setVolume(float vol){
	if(vol<0)return;//should be positive
	m_volume=vol;

	alSourcef(m_SourceID,AL_GAIN,vol);
	checkForError(mT("SALSound::setVolume()-alSourcef(AL_GAIN)"));

}
 
void SALSound::setLooping(bool loop){
	m_looping=loop;

	if(dataFormat && !dataFormat->isStreaming()){
		alSourcei(m_SourceID,AL_LOOPING,loop);
		checkForError(mT("SALSound::setLooping()-alSourcei(AL_LOOPING)"));
	}
}

void SALSound::setMaxDistance(float max_dist){
	if(max_dist<0)return;
	max_distance=max_dist;
	if(!dataFormat)return;

	
	alSourcef(m_SourceID,AL_MAX_DISTANCE,max_dist);
	checkForError(mT("SALSound::setMaxDistance()-alSourcef(AL_MAX_DISTANCE)"));

}
void SALSound:: setMinDistance(float min_dist){
	if(min_dist<0)return;
	min_distance=min_dist;
}

void SALSound::setData(ISoundStreamFrame*data){
	dataFormat=data;

	if(!data)return;
	
	traceFunction(eSound);

	switch(data->bitsPerSample){
		case ESF_8:
			switch(data->channels){
				case 1:m_format=AL_FORMAT_MONO8;
					break;
				case 2:m_format=AL_FORMAT_STEREO8;
					break;
			   }
			   break;
		case ESF_16:
			switch(data->channels){
				case 1:m_format=AL_FORMAT_MONO16;
					break;
				case 2:m_format=AL_FORMAT_STEREO16;
					break;
			   }
	}

	if(m_bufferCount>0){
		alDeleteBuffers(m_bufferCount,&m_buffersID[0]);
		checkForError(mT("SALSound::setData()-alDeleteBuffers"));
		m_bufferCount=0;
	}
	if(data->isStreaming()){
		m_bufferCount=2;
		alGenBuffers(2,&m_buffersID[0]);
		checkForError(mT("SALSound::setData()-alGenBuffers"));
	}else{
		m_bufferCount=1;

		//non straming audio
		alGenBuffers(1,&m_buffersID[0]);
		checkForError(mT("SALSound::setData()-alGenBuffers"));
		alBufferData(m_buffersID[0],
					m_format,
					dataFormat->getData(),
					dataFormat->getDataSize(),
					dataFormat->samplesPerSec);
		checkForError(mT("SALSound::setData()-alBufferData"));
		alSourcei(m_SourceID,AL_BUFFER,m_buffersID[0]);
		checkForError(mT("SALSound::setData()-alSourcei"));
	}
	checkForError(mT("SALSound::setData()"));
	
	

}


int SALSound::streamBuffer(uint bufferID,int size){

   int ActualSize = dataFormat->streamData( size );
	alBufferData(bufferID,
				m_format,
				dataFormat->getData(),
				dataFormat->getDataSize(),
				dataFormat->samplesPerSec);

	checkForError(mT("SALSound::streamBuffer()"));
	return ActualSize;
}

void SALSound::unqueueAll(){

	int queued;
	alGetSourcei(m_SourceID,AL_BUFFERS_QUEUED,&queued);
	checkForError(mT("SALSound::unqueueAll()-alGetSourcei"));
	if(queued>0)
		alSourceUnqueueBuffers(m_SourceID,queued,&m_buffersID[0]);
	checkForError(mT("SALSound::unqueueAll()"));
}

void SALSound::setMode(ESoundType mode){
}

bool SALSound::isPlaying(){
	
	int state;
	alGetSourcei(m_SourceID,AL_SOURCE_STATE,&state);
	checkForError(mT("SALSound::isPlaying()-alGetSourcei"));
	return state==AL_PLAYING;
}


void SALSound::play(){
	if(isPlaying() || !soundSystem)return;
	
	int state;
	
	alGetSourcei(m_SourceID,AL_SOURCE_STATE,&state);
	checkForError(mT("SALSound::play()-alGetSourcei"));

	if(state!=AL_PAUSED && dataFormat->isStreaming()){
		unqueueAll();
		streamBuffer(m_buffersID[0],BUFFER_SIZE);
		streamBuffer(m_buffersID[1],BUFFER_SIZE);
		
		alSourceQueueBuffers(m_SourceID,2,&m_buffersID[0]);
		checkForError(mT("SALSound::play()-alSourceQueueBuffers"));
	}
	

	alSourcePlay(m_SourceID);
	checkForError(mT("SALSound::play()-alSourcePlay"));
}

void SALSound::pause(){

	alSourcePause(m_SourceID);
	unqueueAll();
}


void SALSound::stop(){

	alSourceStop(m_SourceID);
}


void SALSound::update(float dt){
	if(m_looping && !isPlaying())
		play();
	if(lastPos!=position){
		velocity=(lastPos-position)*dt;
		setVelocity(velocity);
	}
	lastPos=position;
	
	if ( dataFormat->isStreaming() )
	{
		int Processed;

		alGetSourcei( m_SourceID, AL_BUFFERS_PROCESSED, &Processed );
		checkForError(mT("SALSound::update()-alGetSourcei"));

		while ( Processed-- )
		{
			uint BufID;

			alSourceUnqueueBuffers( m_SourceID, 1, &BufID );
			checkForError(mT("SALSound::update()-alSourceUnqueueBuffers"));

			if ( streamBuffer(BufID, BUFFER_SIZE) == 0 )
			{
				if(m_looping){
					dataFormat->seek( 0 );

					streamBuffer( BufID, BUFFER_SIZE );
				}else
				{
					stop();
					return;
				}
			}

			alSourceQueueBuffers( m_SourceID, 1, &BufID );
			checkForError(mT("SALSound::update()-alSourceQueueBuffers"));
		}
	}
}


}
}

