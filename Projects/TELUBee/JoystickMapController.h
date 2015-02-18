

/********************************************************************
	created:	2013/01/31
	created:	31:1:2013   18:58
	filename: 	C:\Development\mrayEngine\Projects\TELUBee\JoystickMapController.h
	file path:	C:\Development\mrayEngine\Projects\TELUBee
	file base:	JoystickMapController
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __JoystickMapController__
#define __JoystickMapController__

#include "IMapController.h"


namespace mray
{
namespace TBee
{

class JoystickMapController:public IMapController
{
protected:
	float m_distAccel;
	InputManager* m_input;
	MapObject* m_mapObject;;
public:
	JoystickMapController(InputManager* inputMngr);
	virtual~JoystickMapController(){}

	virtual void SetMapObject(MapObject* map){m_mapObject=map;}
	virtual MapObject* GetMapObject(){return m_mapObject;}

	virtual bool OnEvent(Event* e);
	void Update(float dt);
};

}
}


#endif
