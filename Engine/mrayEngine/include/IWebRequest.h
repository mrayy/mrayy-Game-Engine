

/********************************************************************
	created:	2011/10/20
	created:	20:10:2011   14:33
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\IWebRequest.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	IWebRequest
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __IWebRequest__
#define __IWebRequest__

#include "mstring.h"
#include "GCPtr.h"
#include "WebURL.h"

namespace mray
{
	namespace OS
	{
		class IStream;
	}
namespace network
{

class IWebRequest
{
private:
protected:
public:
	IWebRequest(){}
	virtual~IWebRequest(){}

	virtual bool SetURL(const core::string& url)=0;
	virtual void SetURL(const WebURL& url)=0;
	virtual WebURL& GetURL()=0;
	virtual const WebURL& GetURL()const=0;


	virtual void SetMethod(const core::string&method)=0;
	virtual const core::string& GetMethod()const=0;

	virtual void SetPostData(const core::string&data)=0;
	virtual const core::string& GetPostData()const=0;

	virtual void SetTimeout(int time)=0;
	virtual int  GetTimeout()const=0;

	virtual bool IsConnected()=0;

	virtual bool Connect(const core::string& url)=0;
	virtual bool Connect()=0;
	virtual void Disconnect()=0;

	virtual OS::IStream* GetStream()=0;

};

MakeSharedPtrType(IWebRequest);

}
}

#endif