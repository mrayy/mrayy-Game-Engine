
/********************************************************************
	created:	2009/06/03
	created:	3:6:2009   17:08
	filename: 	i:\Programing\GameEngine\mrayEngine\EaglesProject\src\AntiPlaneMissile.h
	file path:	i:\Programing\GameEngine\mrayEngine\EaglesProject\src
	file base:	AntiPlaneMissile
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___AntiPlaneMissile___
#define ___AntiPlaneMissile___

#include "Missile.h"
#include "ILockableEntity.h"

namespace mray{
namespace gameMod{

class AntiPlaneMissile:public Missile
{
private:
protected:
	ILockableEntity* m_target;
public:
	AntiPlaneMissile(GameLevel*level,GCPtr<scene::ISceneNode> shape,GameEntity*owner);
	virtual~AntiPlaneMissile();

	virtual void update(float dt);

	virtual void LaunchMissile(ILockableEntity* target);

};

}
}


#endif //___AntiPlaneMissile___
