

/********************************************************************
	created:	2013/02/27
	created:	27:2:2013   15:42
	filename: 	D:\Development\mrayEngine\Projects\VirtualTelesar\VTLib\IForceFeedbackDriver.h
	file path:	D:\Development\mrayEngine\Projects\VirtualTelesar\VTLib
	file base:	IForceFeedbackDriver
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/
#ifndef _IForceFeedbackDriver_
#define _IForceFeedbackDriver_



namespace mray
{
namespace VT
{
	class IFeedbackResult;
class IForceFeedbackDriver
{
protected:
public:
	IForceFeedbackDriver(){}
	virtual~IForceFeedbackDriver(){}

	virtual void Init(const core::string& ip,int port)=0;
	virtual void Close()=0;
	virtual void SendData(IFeedbackResult* res)=0;
};

}
}

#endif