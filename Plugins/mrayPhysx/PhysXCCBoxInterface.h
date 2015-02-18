
/********************************************************************
	created:	2009/07/14
	created:	14:7:2009   1:06
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayPhysx\mrayPhysx\PhysXCCBoxInterface.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayPhysx\mrayPhysx
	file base:	PhysXCCBoxInterface
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___PhysXCCBoxInterface___
#define ___PhysXCCBoxInterface___

#include <NxBoxController.h>

#include "ICCBoxInterface.h"

namespace mray{
namespace physics{

class PhysXCCBoxInterface:public ICCBoxInterface
{
private:
protected:

	NxBoxController* m_c;
public:
	PhysXCCBoxInterface(NxBoxController*c);
	virtual~PhysXCCBoxInterface();

	virtual void setExtents(const math::vector3d& e);
	virtual math::vector3d getExtents();
};

}
}


#endif //___PhysXCCBoxInterface___