
/********************************************************************
	created:	2012/07/15
	created:	15:7:2012   12:44
	filename: 	d:\Development\mrayEngine\Projects\VirtualTelesar\ICommunicationLayer.h
	file path:	d:\Development\mrayEngine\Projects\VirtualTelesar
	file base:	ICommunicationLayer
	file ext:	h
	author:		MHD YAMEN SARAIJI
	
	purpose:	
*********************************************************************/
#ifndef ___ICommunicationLayer___
#define ___ICommunicationLayer___

#include "ListenerContainer.h"

namespace mray
{
namespace VT
{

class CommunicationData ;
class ICommunicationLayer;
class ICommunicationLayerListener
{
public:
	virtual void OnDataArrived(ICommunicationLayer* layer,const CommunicationData*d){}
	virtual void OnClosed(ICommunicationLayer* ){}
	//this call back is fired when the list of joint names is available
	virtual void OnSchemeChanged(ICommunicationLayer* ,const std::vector<core::string>& names ){}
};

class ICommunicationLayer:public ListenerContainer<ICommunicationLayerListener*>
{
public:

	DECLARE_FIRE_METHOD(OnDataArrived,(ICommunicationLayer* layer,const CommunicationData*d),(layer,d));
	DECLARE_FIRE_METHOD(OnClosed,(ICommunicationLayer* layer),(layer));
	DECLARE_FIRE_METHOD(OnSchemeChanged,(ICommunicationLayer* layer,const std::vector<core::string>& names),(layer,names));

public:
	ICommunicationLayer(){}
	virtual~ICommunicationLayer(){}
	
	virtual void Start()=0;
	virtual void Close()=0;
	virtual void Update(float dt)=0;
	
	virtual const std::vector<core::string>& GetScheme()=0;

	virtual core::string InjectCommand(const core::string& cmd, const core::string& args){ return ""; }
};

}
}

#endif

