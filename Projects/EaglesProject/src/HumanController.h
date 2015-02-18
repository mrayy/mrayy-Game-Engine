
/********************************************************************
	created:	2009/04/06
	created:	6:4:2009   0:02
	filename: 	i:\Programing\GameEngine\mrayEngine\EaglesProject\src\HumanController.h
	file path:	i:\Programing\GameEngine\mrayEngine\EaglesProject\src
	file base:	HumanController
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___HumanController___
#define ___HumanController___

#include "IPlaneController.h"
#include "ControlMap.h"

namespace mray{
namespace gameMod{

class HumanController:public IPlaneController
{
private:
protected:
	GCPtr<ControlMap> m_controllMap;
public:
	HumanController();
	virtual~HumanController();

	virtual void control();	// update the plane
	virtual bool isNPC(){
		return false;
	}
};

}
}


#endif //___HumanController___
