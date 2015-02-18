
#ifndef ___DemoCamera___
#define ___DemoCamera___

#include <CameraNode.h>
#include <InputManager.h>

namespace mray{
	class DemoCamera:public scene::CameraNode
	{
		DECLARE_RTTI;
	protected:
		float m_speed;
		InputManager* m_input;
	public:
		bool disableInput;
		DECLARE_PROPERTY_TYPE(Speed,float,);

		DemoCamera(scene::ISceneManager*smngr,float speed,InputManager* kb):CameraNode(mT(""),0,smngr)
		{
			disableInput=false;
			m_input=kb;
			m_speed=speed;
			this->setUseTarget(false);
			this->setZFar(4000);


			CPropertieDictionary* dic=0;
			if(CreateDictionary(&dic))
			{
				dic->addPropertie(&PropertyTypeSpeed::instance,mT("Behavior"));
			}
		}

		bool SetSpeed(const float& s)
		{
			m_speed=s;
			return true;
		}
		const float& GetSpeed()const
		{
			return m_speed;
		}
		virtual void update(float dt){
			if(!this->isVisible())
				return;
			if(!disableInput)
			{

				float dx=m_speed*(m_input->getKeyboard()->getKeyState(KEY_A)-
					m_input->getKeyboard()->getKeyState(KEY_D))*dt;
				float dz=m_speed*(m_input->getKeyboard()->getKeyState(KEY_W)-
					m_input->getKeyboard()->getKeyState(KEY_S))*dt;
		
	// 			dx*=(1+2*gKeyboardController.isLShiftPress());
	// 			dz*=(1+2*gKeyboardController.isLShiftPress());

				if(m_input->getMouse()->isPressed(controllers::EMB_Right)){
					this->rotate(-80*m_input->getMouse()->getDX()*dt,math::vector3d(0,1,0),scene::TS_World);
					this->rotate(80*m_input->getMouse()->getDY()*dt,math::vector3d(1,0,0),scene::TS_Local);
				}
				this->translate(math::vector3d(dx,0,dz),scene::TS_Local);
			}
			CameraNode::update(dt);
		}
	};

	IMPLEMENT_RTTI(DemoCamera,scene::CameraNode);

	IMPLEMENT_PROPERTY_TYPE_HEADER(Speed,DemoCamera,float,mT("Speed"),EPBT_Basic,mT(""),100);
	IMPLEMENT_PROPERTY_TYPE_GENERIC(Speed,DemoCamera,float,DemoCamera::SetSpeed,DemoCamera::GetSpeed,core::StringConverter::toString,core::StringConverter::toFloat,false);
}

#endif

