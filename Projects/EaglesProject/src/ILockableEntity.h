

/********************************************************************
	created:	2009/06/03
	created:	3:6:2009   19:15
	filename: 	i:\Programing\GameEngine\mrayEngine\EaglesProject\src\ILockableEntity.h
	file path:	i:\Programing\GameEngine\mrayEngine\EaglesProject\src
	file base:	ILockableEntity
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___ILockableEntity___
#define ___ILockableEntity___

#include "Missile.h"

namespace mray{
namespace gameMod{

class ILockableEntity
{
private:
protected:
public:
	ILockableEntity(){}
	virtual~ILockableEntity(){}

	virtual void addLockedMissile(GCPtr<Missile> m)=0;
	virtual void removeLockedMissile(GCPtr<Missile> m)=0;

	virtual GameEntity *getAsGameEntity()=0;
};

}
}


#endif //___ILockableEntity___
