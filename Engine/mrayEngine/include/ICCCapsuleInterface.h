
/********************************************************************
	created:	2009/07/14
	created:	14:7:2009   1:03
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\ICCCapsuleInterface.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	ICCCapsuleInterface
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___ICCCapsuleInterface___
#define ___ICCCapsuleInterface___

#include "ICCTypeInterface.h"

namespace mray{
namespace physics{

class ICCCapsuleInterface:public ICCTypeInterface
{
private:
protected:
public:

	ECCType getType(){return ECCT_Capsule;}

	virtual void setRadius(float r )=0;
	virtual float getRadius()=0;

	virtual void setHeight(float h )=0;
	virtual float getHeight()=0;

	virtual void setClimbing(ECCCapsuleClimbingMode c )=0;
	virtual ECCCapsuleClimbingMode getClimbing()=0;

};

}
}


#endif //___ICCCapsuleInterface___