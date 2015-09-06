

/********************************************************************
	created:	2013/10/07
	created:	7:10:2013   19:38
	filename: 	C:\Development\mrayEngine\Plugins\mrayOculusVR\OculusCameraComponent.h
	file path:	C:\Development\mrayEngine\Plugins\mrayOculusVR
	file base:	OculusCameraComponent
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __OculusCameraComponent__
#define __OculusCameraComponent__


#include "IGameComponent.h"

namespace mray
{
	namespace video
	{
		class OculusDevice;
		class ParsedShaderPP;
	}
	namespace scene
	{
		class ViewPort;
	}
namespace game
{

class OculusCameraComponent:public IGameComponent
{
	DECLARE_RTTI;
public:
	enum ETargetCamera
	{
		LeftCamera,
		RightCamera
	};
protected:
	ETargetCamera m_target;
	video::OculusDevice* m_device;
	scene::ViewPort* m_vp;
	video::ParsedShaderPP* m_shader;
	bool m_isDirty;
	float m_perspOffset;
	std::vector<core::string> m_infoString;
public:
	OculusCameraComponent(game::GameEntityManager*mngr);
	virtual~OculusCameraComponent();

	void InitCamera(video::OculusDevice* device,scene::ViewPort* vp,ETargetCamera target,video::ParsedShaderPP* shader);

	float GetPerspectiveOffset(){return m_perspOffset;}

	virtual bool InitComponent();
	virtual void Update(float dt);

	const std::vector<core::string>& GetInfoString(){return m_infoString;}
};

DECLARE_GAMECOMPONENT_FACTORY(OculusCameraComponent);


}
}


#endif
