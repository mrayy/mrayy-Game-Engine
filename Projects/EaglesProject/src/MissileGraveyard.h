

/********************************************************************
	created:	2009/05/08
	created:	8:5:2009   18:54
	filename: 	i:\Programing\GameEngine\mrayEngine\EaglesProject\src\MissileGraveyard.h
	file path:	i:\Programing\GameEngine\mrayEngine\EaglesProject\src
	file base:	MissileGraveyard
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___MissileGraveyard___
#define ___MissileGraveyard___

#include <ISingleton.h>

namespace mray{
namespace gameMod{

class MissileGraveyard:public ISingleton<MissileGraveyard>
{
private:
protected:
public:
	MissileGraveyard();
	virtual~MissileGraveyard();

	void addMissile(GCPtr<PlaneMissile> missile);

};

}
}


#endif //___MissileGraveyard___
