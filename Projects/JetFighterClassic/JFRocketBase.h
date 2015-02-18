


/********************************************************************
	created:	2012/05/15
	created:	15:5:2012   15:56
	filename: 	d:\Development\mrayEngine\Projects\JetFighterClassic\JFRocketBase.h
	file path:	d:\Development\mrayEngine\Projects\JetFighterClassic
	file base:	JFRocketBase
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __JFRocketBase__
#define __JFRocketBase__

#include "JFIBullet.h"

namespace mray
{
namespace game
{

class JFRocketBase:public JFIBullet
{
private:
protected:
	math::vector3d m_position;
	math::vector3d m_speed;

public:
	JFRocketBase();
	virtual~JFRocketBase();

	virtual void OnCreate(const math::vector3d& pos,const math::vector3d& dir);

	virtual void Update(float dt);
};


}

#endif
