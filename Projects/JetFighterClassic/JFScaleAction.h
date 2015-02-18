


/********************************************************************
	created:	2012/05/15
	created:	15:5:2012   16:21
	filename: 	d:\Development\mrayEngine\Projects\JetFighterClassic\JFScaleAction.h
	file path:	d:\Development\mrayEngine\Projects\JetFighterClassic
	file base:	JFScaleAction
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __JFScaleAction__
#define __JFScaleAction__

#include "JFIAction.h"

namespace mray
{
namespace game
{
	class JFObject;

class JFScaleAction:public JFIAction
{
private:
protected:
	JFObject* m_object;

	math::vector3d m_scale;
	math::vector3d m_oldScale;
	float m_speed;
	float m_time;
public:
	JFScaleAction(JFObject* o,float speed,math::vector3d scale);
	virtual~JFScaleAction();


	void OnErrupt();
	bool Execute(float dt);
};


}
}

#endif
