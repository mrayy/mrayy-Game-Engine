
/********************************************************************
	created:	2012/07/14
	created:	14:7:2012   23:08
	filename: 	d:\Development\mrayEngine\Projects\VirtualTelesar\ICommunicatorComponent.h
	file path:	d:\Development\mrayEngine\Projects\VirtualTelesar
	file base:	ICommunicatorComponent
	file ext:	h
	author:		MHD YAMEN SARAIJI
	
	purpose:	
*********************************************************************/
#ifndef ___ICommunicatorComponent___
#define ___ICommunicatorComponent___

#include "IVTComponent.h"
#include "CommunicationData.h"

namespace mray
{
namespace VT
{
	class CommunicatorScheme;
	class CommunicationData;
class ICommunicatorComponent:public IVTComponent
{
	DECLARE_RTTI;
protected:

public:
	ICommunicatorComponent(){}
	virtual~ICommunicatorComponent(){}
	

	virtual CommunicatorScheme* GetScheme()=0;

	virtual void ReceiveData(const core::string& target,const CommValueList& values){}
};

}
}

#endif
