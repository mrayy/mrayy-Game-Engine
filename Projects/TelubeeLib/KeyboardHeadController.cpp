

#include "stdafx.h"
#include "KeyboardHeadController.h"
#include "AppData.h"
#include "InputManager.h"
#include "RenderWindow.h"


namespace mray
{
namespace TBee
{

bool KeyboardHeadController::GetHeadOrientation(math::quaternion& v, bool abs)
{
	controllers::IMouseController* mouse= AppData::Instance()->inputMngr->getMouse();
	math::vector2d sz= AppData::Instance()->inputMngr->GetRenderWindow()->GetSize();

	if (mouse->isPressed(controllers::EMB_Right))
	{
		float pan = mouse->getX() - sz.x / 2;
		float tilt = mouse->getY() - sz.y / 2;
		pan /= sz.x;
		tilt /= sz.y;

		pan *= 70 * 2;
		tilt *= 50 * 2;
		v = math::quaternion(0, tilt, -pan);
		return true;
	}
	else
		return false;

}
bool KeyboardHeadController::GetHeadPosition(math::vector3d& v, bool abs)
{
	return false;
}

}
}

