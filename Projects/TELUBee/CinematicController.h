

/********************************************************************
	created:	2013/02/08
	created:	8:2:2013   13:08
	filename: 	C:\Development\mrayEngine\Projects\TELUBee\CinematicController.h
	file path:	C:\Development\mrayEngine\Projects\TELUBee
	file base:	CinematicController
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __CinematicController__
#define __CinematicController__


#include "IMapController.h"

namespace mray
{
namespace TBee
{

class CinematicController:public IMapController
{
protected:
	float m_time;
	InputManager* m_input;
	MapObject* m_mapObject;;
	IMapController* m_nextController;

	enum EStatus
	{
		ECinematic,
		EControll
	}m_status;

	struct KeyFrame
	{
		float time;
		float x,y;
		float tilt;
		float yaw;
		float zoom;
		int zoomLevel;
		float blur;
	};

	std::vector<KeyFrame> m_keyframes;
	float getKeyFrames(float t,KeyFrame*& k1,KeyFrame*& k2);
public:
	CinematicController(InputManager* inputMngr);
	virtual~CinematicController();


	virtual void SetMapObject(MapObject* map)
	{
		m_mapObject=map;
		if(m_nextController)
			m_nextController->SetMapObject(map);
	}
	virtual MapObject* GetMapObject(){return m_mapObject;}

	virtual bool OnEvent(Event* e);
	void Update(float dt);
};

}
}


#endif
