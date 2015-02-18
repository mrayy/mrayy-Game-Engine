#include "stdafx.h"

#include "AnimationController.h"
//#include "AnimationTrackGroup.h"



namespace mray
{
namespace animation
{

AnimationController::AnimationController()
{
	m_weight=1;
	m_targetWeight=1;
	m_targetDelay=0;
	m_time=0;
	m_animationMode=EAM_Cycle;
	m_autoPlay=0;
	m_playing=0;
	m_animationDirection=1;
	m_interType=EIT_Linear;
	m_speed=1;

	m_start=0;
	m_end=1;
}
AnimationController::~AnimationController()
{
}
void AnimationController::SetStartEnd(float start,float end)
{
	m_start=start;
	m_end=end;
}
void AnimationController::setAnimationDirection(float dir)
{
	
	if(dir>=0)m_animationDirection=1;
	else m_animationDirection=-1;
}
void AnimationController::setInterpolationType(EInterpolationType type)
{
	m_interType=type;
}
EInterpolationType AnimationController::getInterpolationType()
{
	return m_interType;
}
void AnimationController::play(float speed,EAnimationMode mode,bool forward,float weight)
{
	m_animationDirection=forward?+1:-1;
	m_playing=true;
	m_speed=speed;
	m_animationMode=mode;
	m_weight=weight;
}

void AnimationController::stop()
{
	m_playing=false;
}

bool AnimationController::isPlaying()
{
	return m_playing;
}

float AnimationController::getTime()
{
	return m_time;
}
void AnimationController::setTime(float t)
{
	m_time=t;
	if(t<m_start || t>m_end){
		switch(m_animationMode){
		case EAM_Cycle:
			m_time=m_start;
			break;
		case EAM_PingPong:
			m_animationDirection*=-1;
			m_time=math::clamp(t,m_start,m_end);
			break;
		case EAM_Clamp:
			if(t<m_start)
				m_time=m_start;
			else{
				m_time=m_end;
				stop();
			}
			break;
		}
	}
}

void AnimationController::update(float dt){
	if(m_playing){
		float t=m_time+m_animationDirection*m_speed*dt;
		setTime(t);

		if(m_targetDelay<=dt){
			m_targetDelay=0;
			m_weight=m_targetWeight;
		}else{
			float factor=math::Max<float>(0,dt/m_targetDelay);
			m_weight=math::lerp(m_weight,m_targetWeight,factor);
			m_targetDelay-=dt;
		}

		if(!m_weight && !m_targetWeight)
			stop();
	}
}

}
}