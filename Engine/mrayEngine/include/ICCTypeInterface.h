
/********************************************************************
	created:	2009/07/14
	created:	14:7:2009   1:00
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\ICCTypeInterface.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	ICCTypeInterface
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___ICCTypeInterface___
#define ___ICCTypeInterface___

#include "PhysicalCCDesc.h"

namespace mray{
namespace physics{

class ICCTypeInterface
{
private:
protected:
public:
	virtual ECCType getType()=0;
};

}
}


#endif //___ICCTypeInterface___
