

/********************************************************************
	created:	2009/07/14
	created:	14:7:2009   1:10
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayPhysx\mrayPhysx\PhysXCCCapsuleInterface.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayPhysx\mrayPhysx
	file base:	PhysXCCCapsuleInterface
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___PhysXCCCapsuleInterface___
#define ___PhysXCCCapsuleInterface___

#include <NxCapsuleController.h>

#include "ICCCapsuleInterface.h"

namespace mray{
namespace physics{

class PhysXCCCapsuleInterface:public ICCCapsuleInterface
{
private:
protected:
	NxCapsuleController *m_c;
public:
	PhysXCCCapsuleInterface(NxCapsuleController*c);
	virtual~PhysXCCCapsuleInterface();

	virtual void setRadius(float r );
	virtual float getRadius();

	virtual void setHeight(float h );
	virtual float getHeight();

	virtual void setClimbing(ECCCapsuleClimbingMode c );
	virtual ECCCapsuleClimbingMode getClimbing();

};

}
}


#endif //___PhysXCCCapsuleInterface___