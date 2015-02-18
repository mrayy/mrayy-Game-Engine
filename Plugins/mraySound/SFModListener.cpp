#include "stdafx.h"
#include "SFModListener.h"


namespace mray{
namespace sound{


SFModListener::SFModListener(FMOD::System*sys){
	m_node=0;
	m_system=sys;
}
SFModListener::~SFModListener(){
}

void SFModListener::setPosition(const math::vector3d& p){
	m_position=p;
}

void SFModListener::setVelocity(const math::vector3d& p){
	m_velocity=p;
}

void SFModListener::setOreintation(const math::quaternion& d){
	m_orient=d;
}

void SFModListener::attachToListener(scene::IMovable*node){
	m_node=node;
	if(m_node)
		m_lastPos=m_node->getAbsolutePosition();
}

scene::IMovable* SFModListener::getListenerNode(){
	return m_node;
}


void SFModListener::setDistanceModel(EListenerDistanceModel m)
{
}
EListenerDistanceModel SFModListener::getDistanceModel()
{
	return LinearDistance;
}
void SFModListener::GetSamples(float* samples, float count, int c)
{
	m_system->getWaveData(samples, count, c);
}
void SFModListener::GetSpectrum(float* samples, float count, int c, EFFT_WindowType window)
{
	FMOD_DSP_FFT_WINDOW windowtype;
	switch (c)
	{
	case FFT_WT_Rect: windowtype = FMOD_DSP_FFT_WINDOW_RECT; break;
	case FFT_WT_Triangle: windowtype = FMOD_DSP_FFT_WINDOW_TRIANGLE; break;
	case FFT_WT_Hamming: windowtype = FMOD_DSP_FFT_WINDOW_HAMMING; break;
	case FFT_WT_Hanning: windowtype = FMOD_DSP_FFT_WINDOW_HANNING; break;
	case FFT_WT_Blackman: windowtype = FMOD_DSP_FFT_WINDOW_BLACKMAN; break;
	case FFT_WT_BlackmanHarris: windowtype = FMOD_DSP_FFT_WINDOW_BLACKMANHARRIS; break;
	default:
		windowtype = FMOD_DSP_FFT_WINDOW_HAMMING;
	}
	m_system->getSpectrum(samples, count, c, windowtype);
}

void SFModListener::update(float dt){
	if(m_node){
		setPosition(m_node->getAbsolutePosition());
		setOreintation(m_node->getOrintation());
		m_velocity=(m_position-m_lastPos)*dt;
		m_lastPos=m_position;
	}


	math::vector3d face=m_orient*math::vector3d::ZAxis;
	math::vector3d up=m_orient*math::vector3d::YAxis;

	FMOD_RESULT result;
	FMOD_VECTOR lup={up.x,up.y,up.z};

	FMOD_VECTOR lpos={m_position.x,m_position.y,m_position.z};
	FMOD_VECTOR lvel={m_velocity.x,m_velocity.y,m_velocity.z};
	FMOD_VECTOR ldir={face.x,face.y,face.z};

	result=m_system->set3DListenerAttributes(0,&lpos,&lvel,&ldir,&lup);
}

}
}
