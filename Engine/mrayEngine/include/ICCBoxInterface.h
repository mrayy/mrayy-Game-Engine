
/********************************************************************
	created:	2009/07/14
	created:	14:7:2009   1:00
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\ICCBoxInterface.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	ICCBoxInterface
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___ICCBoxInterface___
#define ___ICCBoxInterface___

#include "ICCTypeInterface.h"


namespace mray{
namespace physics{

class ICCBoxInterface:public ICCTypeInterface
{
private:
protected:
public:

	ECCType getType(){return ECCT_Box;}

	virtual void setExtents(const math::vector3d& e)=0;
	virtual math::vector3d getExtents()=0;
};

}
}


#endif //___ICCBoxInterface___
