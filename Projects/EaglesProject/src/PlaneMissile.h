
/********************************************************************
	created:	2009/05/08
	created:	8:5:2009   10:50
	filename: 	i:\Programing\GameEngine\mrayEngine\EaglesProject\src\PlaneMissile.h
	file path:	i:\Programing\GameEngine\mrayEngine\EaglesProject\src
	file base:	PlaneMissile
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___PlaneMissile___
#define ___PlaneMissile___

#include "Missile.h"

namespace mray{
namespace gameMod{


	class AirPlane;

class PlaneMissile:public Missile
{
private:
protected:

public:
	PlaneMissile(GameLevel*level,GCPtr<scene::ISceneNode> shape,AirPlane*owner);
	virtual~PlaneMissile();

	virtual void update(float dt);

	virtual void LaunchMissile();
};

}
}


#endif //___PlaneMissile___
