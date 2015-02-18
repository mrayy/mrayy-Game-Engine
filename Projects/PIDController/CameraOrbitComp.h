

/********************************************************************
	created:	2013/08/20
	created:	20:8:2013   21:36
	filename: 	C:\Development\mrayEngine\Projects\PIDController\CameraOrbitComp.h
	file path:	C:\Development\mrayEngine\Projects\PIDController
	file base:	CameraOrbitComp
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __CameraOrbitComp__
#define __CameraOrbitComp__



#include "IGameComponent.h"
#include "CameraNode.h"
#include "InputManager.h"

namespace mray
{
namespace game
{


class CameraOrbitComp:public IGameComponent
{
protected:
	scene::CameraNode *m_camera;
	InputManager* m_inputManager;

	math::vector3d m_speed;
	math::vector3d m_angles;
	float m_distance;
	float m_followSpeed;
	float m_minLimit,m_maxLimit;
public:
	CameraOrbitComp();
	virtual~CameraOrbitComp();

	void SetCamera(scene::CameraNode* cam,InputManager* m);

	virtual void Update(float dt);
};

}
}


#endif
