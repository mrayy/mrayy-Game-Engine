


/********************************************************************
	created:	2011/10/13
	created:	13:10:2011   15:24
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\IThreadEvent.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	IThreadEvent
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __IThreadEvent__
#define __IThreadEvent__

#include "mtypes.h"

namespace mray
{
namespace OS
{

enum EThreadEventRetType
{
	ETEvent_Object0=0,

	ETEvent_Timeout=100,
	ETEvent_Failed
};

class IThreadEvent
{
private:
protected:
public:

	IThreadEvent(){}
	virtual~IThreadEvent(){}

	virtual void SetEvent()=0;
	virtual void ResetEvent()=0;
	virtual void Close()=0;

	virtual EThreadEventRetType Wait(ulong ms)=0;

};


}
}

#endif