
#ifndef ___GUITranslateEffector___
#define ___GUITranslateEffector___

#include "Point2d.h"
#include "GUIEffector.h"

namespace mray{
namespace GUI{

class MRAY_DLL GUITranslateEffector:public GUIEffector
{
	math::vector2d m_startPos;
	math::vector2d m_endPos;
	float m_t;
	float m_speed;
	int m_dirSgn;
	bool m_autoReverse;

	virtual void fillProperties();
public:
	DECLARE_RTTI;

	rwProperty<GUITranslateEffector,math::vector2d> StartPos;
	rwProperty<GUITranslateEffector,math::vector2d> EndPos;
	rwProperty<GUITranslateEffector,float> Speed;
	rwProperty<GUITranslateEffector,bool> AutoReverse;

	GUITranslateEffector(math::vector2d startP,math::vector2d endP,float speed);
	virtual void effect(GUIElement* elem,float dt);

	void setStartPos(const math::vector2d& v){m_startPos=v;}
	const math::vector2d& getStartPos(){return m_startPos;}

	void setEndPos(const math::vector2d& v){m_endPos=v;}
	const math::vector2d& getEndPos(){return m_endPos;}

	void setAutoReverse(const bool& v){m_autoReverse=v;}
	const bool& getAutoReverse(){return m_autoReverse;}

	void setSpeed(const float& v){m_speed=v;}
	const float& getSpeed(){return m_speed;}


	void setDir(const int& v){m_dirSgn=v;}
	const int& getDir(){return m_dirSgn;}

	void setT(float t);
	bool isDone();

};

}
}


#endif