


/********************************************************************
	created:	2011/10/20
	created:	20:10:2011   14:50
	filename: 	d:\Development\mrayEngine\Engine\mrayNet\Win32WebRequest.h
	file path:	d:\Development\mrayEngine\Engine\mrayNet
	file base:	Win32WebRequest
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __Win32WebRequest__
#define __Win32WebRequest__


#include "CompileConfig.h"
#include "IWebRequest.h"

#include <windows.h>
#include <wininet.h>

namespace mray
{
namespace network
{
	class Win32WebStream;

class MRAY_NET_DLL Win32WebRequest:public IWebRequest
{
private:
protected:
	HINTERNET m_hSession;
	HINTERNET m_hConnect;
	HINTERNET m_hRequest;
	WebURL m_url;
	int m_timeOut;
	bool m_eof;
	DWORD m_dataValiable;

	core::string m_postData;
	Win32WebStream* m_stream;

	core::string m_method;
	DWORD m_length;
public:
	Win32WebRequest();
	~Win32WebRequest();

	bool SetURL(const core::string& url);
	void SetURL(const WebURL& url);
	WebURL& GetURL();
	const WebURL& GetURL()const;

	void SetMethod(const core::string&method);
	const core::string& GetMethod()const;

	void SetPostData(const core::string&data);
	const core::string& GetPostData()const;

	void SetTimeout(int time);
	int  GetTimeout()const;

	bool IsConnected();

	bool Connect(const core::string& url);
	bool Connect();
	void Disconnect();

	OS::IStream* GetStream();

	int Read(void*data,int len);
	int Write(const void*data,int len);
	bool EoF();
	int Length(){ return m_length; }
};

}
}

#endif
