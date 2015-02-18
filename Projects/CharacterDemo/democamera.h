
#ifndef ___DemoCamera___
#define ___DemoCamera___

#include <SCameraNode.h>
#include <CKeyboardController.h>
#include <CMouseController.h>
#include <SceneManager.h>

namespace mray{
	class DemoCamera:public scene::SCameraNode
	{
		float m_speed;
	public:
		DemoCamera(scene::SceneManager*smngr,float speed):SCameraNode(smngr)
		{
			m_speed=speed;
			this->useTarget=false;
			this->ZFar=4000;
		}
		virtual void update(float dt){
			float dx=m_speed*(gKeyboardController.getKeyState(KEY_A)-
				gKeyboardController.getKeyState(KEY_D))*dt;
			float dz=m_speed*(gKeyboardController.getKeyState(KEY_W)-
				gKeyboardController.getKeyState(KEY_S))*dt;
		
// 			dx*=(1+2*gKeyboardController.isLShiftPress());
// 			dz*=(1+2*gKeyboardController.isLShiftPress());

			if(gMouseController.isPressed(controllers::EMB_Right)){
				this->rotate(-80*gMouseController.getDX()*dt,math::vector3d(0,1,0),scene::TS_World);
				this->rotate(80*gMouseController.getDY()*dt,math::vector3d(1,0,0),scene::TS_Local);
			}
			this->translate(math::vector3d(dx,0,dz),scene::TS_Local);
			SCameraNode::update(dt);
		}
	};
}

#endif

