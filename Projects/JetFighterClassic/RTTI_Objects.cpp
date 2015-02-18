

#include "stdafx.h"
#include "JFObject.h"

#include "JFIWeapon.h"
#include "JFObstacle.h"
#include "JFSpaceShip.h"

namespace mray
{
namespace game
{

	IMPLEMENT_RTTI(JFIWeapon,IObject);
	IMPLEMENT_RTTI(JFObject,IGameComponent);
	IMPLEMENT_RTTI(JFObstacle,JFObject);
	IMPLEMENT_RTTI(JFSpaceShip,JFObject);

}
}
