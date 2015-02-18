


/********************************************************************
	created:	2012/02/16
	created:	16:2:2012   14:17
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\IPhysicalTriggerReporter.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	IPhysicalTriggerReporter
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __IPhysicalTriggerReporter__
#define __IPhysicalTriggerReporter__

#include "IPhysical3DDef.h"

namespace mray
{
namespace physics
{


class IPhysicalTriggerReporter
{
private:
protected:
public:
	IPhysicalTriggerReporter(){}
	virtual~IPhysicalTriggerReporter(){}

	virtual void OnTrigger(IPhysicalShape* triggerShape,IPhysicalShape* shape,ETriggerStatus status)=0;

};


}
}

#endif
