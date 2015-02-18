
#ifndef ___GUIFadeEffector___
#define ___GUIFadeEffector___

#include "GUIEffector.h"
#include "SColor.h"


namespace mray{
namespace GUI{

class MRAY_DLL GUIFadeEffector:public GUIEffector
{
	int m_stage;
	float m_clrA;

	bool m_fadeIn,m_fadeOut;
	float m_speed;
	float m_idleTime;
	float m_startTime;
	int m_minAlpha;
	int m_maxAlpha;

protected:

	virtual void fillProperties();
public:

	DECLARE_RTTI;

	rwProperty<GUIFadeEffector,float> Speed;
	rwProperty<GUIFadeEffector,float> IdleTime;
	rwProperty<GUIFadeEffector,bool> FadeIn;
	rwProperty<GUIFadeEffector,bool> FadeOut;
	rwProperty<GUIFadeEffector,int> MinAlpha;
	rwProperty<GUIFadeEffector,int> MaxAlpha;

	GUIFadeEffector(bool fadeOut,bool fadeIn,float speed,float idleTime,int minAlpha=0,int maxAlpha=255);

	bool isDone();
	void reset();

	void setParameters(bool fadeOut,bool fadeIn,float speed,float idleTime,int minAlpha=0,int maxAlpha=255);
	void setFadeIn(const bool& e){m_fadeIn=e;}
	const bool& getFadeIn(){return m_fadeIn;}

	void setFadeOut(const bool& e){m_fadeOut=e;}
	const bool& getFadeOut(){return m_fadeOut;}

	void setSpeed(const float& speed){m_speed=speed;}
	const float&getSpeed(){return m_speed;}

	void setFadeTime(const float& time);

	void setIdleTime(const float& time){m_idleTime=time;}
	const float& getIdleTime(){return m_idleTime;}

	void setMinAlpha(const int& v){m_minAlpha=v;}
	const int& getMinAlpha(){return m_minAlpha;}

	void setMaxAlpha(const int& v){m_maxAlpha=v;}
	const int& getMaxAlpha(){return m_maxAlpha;}


	void setMinMaxAlpha(int minAlpha,int maxAlpha);

	//! 0:fadeOut
	//! 1:idle
	//! 2:fadeIn
	void setStage(int s);

	virtual void effect(GUIElement* elem,float dt);
};

}
}

#endif