


/********************************************************************
	created:	2010/07/17
	created:	17:7:2010   16:11
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\AnimationController.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	AnimationController
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef AnimationController_h__
#define AnimationController_h__

#include "CompileConfig.h"
#include "AnimationTypes.h"

namespace mray
{
namespace animation
{

//	class AnimationTrackGroup;

class MRAY_DLL AnimationController
{
private:
protected:
	float m_weight;
	float m_speed;

	float m_targetWeight;
	float m_targetDelay;

	float m_time;
	float m_animationDirection;

	bool m_autoPlay;
	bool m_playing;

	EInterpolationType m_interType;

	EAnimationMode m_animationMode;

	float m_start,m_end;

public:
	AnimationController();
	virtual ~AnimationController();

	void SetStartEnd(float start,float end);
	float GetStart()const{return m_start;}
	float GetEnd()const{return m_end;}


	//AnimationTrackGroup* getGroup(){return m_group;}

	virtual void setInterpolationType(EInterpolationType type);
	virtual EInterpolationType getInterpolationType();

	void setAnimationMode(EAnimationMode mode){m_animationMode=mode;}
	EAnimationMode getAnimationMode(){return m_animationMode;}

	void setAnimationDirection(float dir);
	float getAnimationDirection(){return m_animationDirection;}

	void setTime(float w);
	float getTime();

	void setWeight(float w){m_weight=w;}
	float getWeight(){return m_weight;}

	void setSpeed(float s){m_speed=s;}
	float getSpeed(){return m_speed;}

	void setTargetWeight(float s){m_targetWeight=s;}
	float getTargetWeight(){return m_targetWeight;}

	void setTargetDelay(float s){m_targetDelay=s;}
	float getTargetDelay(){return m_targetDelay;}


	virtual void play(float speed,EAnimationMode mode,bool forward=true,float weight=1);
	virtual void stop();
	virtual bool isPlaying();

	virtual void update(float dt);
};

}
}


#endif // AnimationController_h__
