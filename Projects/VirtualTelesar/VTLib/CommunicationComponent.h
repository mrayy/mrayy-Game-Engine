
/********************************************************************
	created:	2012/07/27
	created:	27:7:2012   1:07
	filename: 	d:\Development\mrayEngine\Projects\VirtualTelesar\VTLib\CommunicationComponent.h
	file path:	d:\Development\mrayEngine\Projects\VirtualTelesar\VTLib
	file base:	CommunicationComponent
	file ext:	h
	author:		MHD YAMEN SARAIJI
	
	purpose:	
*********************************************************************/
#ifndef ___CommunicationComponent___
#define ___CommunicationComponent___

#include "ICommunicatorComponent.h"

namespace mray
{
namespace VT
{

class CommunicationComponent:public ICommunicatorComponent
{
	DECLARE_RTTI;
protected:
	std::vector<ICommunicatorComponent*> m_commComps;
	CommunicatorScheme* m_scheme;
public:
	CommunicationComponent(game::GameEntityManager*mngr);
	virtual~CommunicationComponent();

	virtual bool InitComponent();

	virtual CommunicatorScheme* GetScheme();

	virtual void ReceiveData(const core::string& target,const CommValueList& values);
	
};
DECLARE_GAMECOMPONENT_FACTORY(CommunicationComponent);

}
}

#endif

