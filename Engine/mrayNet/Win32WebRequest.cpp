

#include "stdafx.h"
#include "Win32WebRequest.h"
#include "ILogManager.h"
#include "StringConverter.h"
#include "Win32WebStream.h"


namespace mray
{
namespace network
{

Win32WebRequest::Win32WebRequest():
m_hConnect(NULL),m_hRequest(NULL),m_method(mT("GET")),m_eof(false),m_timeOut(15000)
{
	m_hSession=InternetOpen(mT("MRAYAgent_1.0"),INTERNET_OPEN_TYPE_PRECONFIG,0,0,0);
	m_stream=new Win32WebStream(this);
	m_dataValiable=0;
	m_length = 0;
}
Win32WebRequest::~Win32WebRequest()
{
	Disconnect();
	InternetCloseHandle(m_hSession);
	delete m_stream;
}

void Win32WebRequest::SetMethod(const core::string&method)
{
	m_method=method;
}
const core::string& Win32WebRequest::GetMethod()const
{
	return m_method;
}

bool Win32WebRequest::SetURL(const core::string& url)
{

	URL_COMPONENTS urlComponents;
	::memset( &urlComponents, 0, sizeof(urlComponents) );
	urlComponents.dwStructSize = sizeof(urlComponents);
	urlComponents.dwSchemeLength = 1;
	urlComponents.dwHostNameLength = 1;
	urlComponents.dwUrlPathLength = 1;
	if(!::InternetCrackUrlA(url.c_str(),url.length(),0,&urlComponents))
		return false;
	m_url.hostName.resize(urlComponents.dwHostNameLength);
	memcpy(&m_url.hostName[0],urlComponents.lpszHostName,urlComponents.dwHostNameLength);
	m_url.urlPath.resize(urlComponents.dwUrlPathLength);
	memcpy(&m_url.urlPath[0],urlComponents.lpszUrlPath,urlComponents.dwUrlPathLength);
	m_url.port=urlComponents.nPort;
	switch(urlComponents.nScheme)
	{
	case INTERNET_SCHEME_HTTP:m_url.protocol="http";break;
	case INTERNET_SCHEME_HTTPS:m_url.protocol="https";break;
	case INTERNET_SCHEME_FTP:m_url.protocol="ftp";break;
	default:
		return false;
	}

	if (urlComponents.lpszPassword!=0)
		m_url.password=urlComponents.lpszPassword;
	else m_url.password = "";
	if (urlComponents.lpszUserName!=0)
		m_url.userName=urlComponents.lpszUserName;
	else m_url.userName = "";
	return true;
}
void Win32WebRequest::SetURL(const WebURL& url)
{
	m_url=url;
}
WebURL& Win32WebRequest::GetURL()
{
	return m_url;
}

const WebURL& Win32WebRequest::GetURL()const
{
	return m_url;
}

void Win32WebRequest::SetPostData(const core::string&data)
{
	m_postData=data;
}

const core::string& Win32WebRequest::GetPostData()const
{
	return m_postData;
}

void Win32WebRequest::SetTimeout(int time)
{
	m_timeOut=time;
}
int  Win32WebRequest::GetTimeout()const
{
	return m_timeOut;
}

bool Win32WebRequest::IsConnected()
{
	return m_hConnect!=0;
}

bool Win32WebRequest::Connect(const core::string& url)
{
	if(!SetURL(url))
		return false;
	return Connect();
}

bool Win32WebRequest::Connect()
{
	Disconnect();

	int iflag=INTERNET_SERVICE_HTTP;
	if(m_url.protocol=="ftp")
		iflag=0;
	m_hConnect=InternetConnect(m_hSession,m_url.hostName.c_str(),
		m_url.port,m_url.userName.c_str(),m_url.password.c_str(),iflag,0,0);
	if( m_hConnect==0)
	{
		gLogManager.log(mT("InternetConnect Failed : ErrorCode[")+core::StringConverter::toString(GetLastError())+mT("]"),ELL_WARNING);
		return false;
	}
	bool bRet = 0;

	if(m_url.protocol=="http" || m_url.protocol=="https")
	{
		DWORD flags=INTERNET_FLAG_NO_CACHE_WRITE |
			INTERNET_FLAG_NO_COOKIES |
			INTERNET_FLAG_RELOAD |
			INTERNET_FLAG_DONT_CACHE |
			INTERNET_FLAG_KEEP_CONNECTION |
			INTERNET_FLAG_IGNORE_CERT_CN_INVALID |
			INTERNET_FLAG_IGNORE_CERT_DATE_INVALID |
			INTERNET_FLAG_PRAGMA_NOCACHE;

		flags = INTERNET_FLAG_NO_CACHE_WRITE;

		if(m_url.protocol=="https")
			flags |=INTERNET_FLAG_SECURE;

		LPTSTR accessTypes[]={mT("*/*"),0};

		InternetSetOption(m_hSession,INTERNET_OPTION_CONNECT_TIMEOUT,&m_timeOut,sizeof(m_timeOut));
		m_hRequest=HttpOpenRequest(m_hConnect,m_method.c_str(),m_url.urlPath.c_str(),HTTP_VERSION,0,(LPCTSTR*)accessTypes,flags,0);
		if(!m_hRequest)
		{
			gLogManager.log(mT("HttpOpenRequest Failed : ErrorCode[")+core::StringConverter::toString(GetLastError())+mT("]"),ELL_WARNING);
			InternetCloseHandle(m_hConnect);
			m_hConnect=0;
			return false;
		}


		DWORD dwInternetFlags = SECURITY_FLAG_IGNORE_UNKNOWN_CA
			| SECURITY_FLAG_IGNORE_REVOCATION
			| SECURITY_FLAG_IGNORE_REDIRECT_TO_HTTP
			| SECURITY_FLAG_IGNORE_REDIRECT_TO_HTTPS
			| SECURITY_FLAG_IGNORE_CERT_DATE_INVALID
			| SECURITY_FLAG_IGNORE_CERT_CN_INVALID;
		//bRet = InternetSetOption(m_hRequest, INTERNET_OPTION_SECURITY_FLAGS, &dwInternetFlags, sizeof(dwInternetFlags));

		char szHeader[1024] = { 0, };
		int nPostDataLen =  0;


		sprintf_s(szHeader, _countof(szHeader),
			//"Accept: text/*\r\n"
			"Accept: text/*, image/gif, image/x-xbitmap, image/jpeg, image/pjpeg, application/vnd.ms-excel, application/msword, application/vnd.ms-powerpoint, */*"
			//"Accept-Language: en-us"
			//"Accept-Encoding: gzip, deflate"
			"User-Agent: Mozilla/4.0 (compatible; MSIE 5.0;* Windows NT)\r\n"
			"Content-type: application/x-www-form-urlencoded\r\n");
#define __HTTP_ACCEPT "Accept: */*\r\n"
		bRet = HttpAddRequestHeaders(m_hRequest, __HTTP_ACCEPT, strlen(__HTTP_ACCEPT), HTTP_ADDREQ_FLAG_REPLACE);

		static core::string hdrs =
			_T("Content-Type: application/x-www-form-urlencoded\r\n");

		if (m_postData != "")
		{
			bRet = HttpSendRequest(m_hRequest, hdrs.c_str(), hdrs.length(), (void*)m_postData.c_str(), m_postData.length());
			
		}
		else
		{
			bRet= HttpSendRequest(m_hRequest, 0,0,0,0);
		}
		if (!bRet)
		{
			gLogManager.log(mT("HttpSendRequest Failed : ErrorCode[") + core::StringConverter::toString(GetLastError()) + mT("]"), ELL_WARNING);
			InternetCloseHandle(m_hRequest);
			InternetCloseHandle(m_hConnect);
			m_hRequest = 0;
			m_hConnect = 0;
			return false;
		}

		m_length = 0;
		DWORD len = sizeof(m_length);
		if (!HttpQueryInfo(m_hRequest, HTTP_QUERY_CONTENT_LENGTH | HTTP_QUERY_FLAG_NUMBER, &m_length, &len, 0))
		{
			gLogManager.log(mT("HttpOpenRequest  : Failed to get content length, ErrorCode[") + core::StringConverter::toString(GetLastError()) + mT("]"), ELL_WARNING);
		}

		char contentType[512];
		len = 512;
		if (!HttpQueryInfo(m_hRequest, HTTP_QUERY_CONTENT_TYPE, contentType, &len, 0))
		{

		}

	}else
	{
		m_hRequest=FtpOpenFile(m_hConnect,m_url.urlPath.c_str(),GENERIC_READ,FTP_TRANSFER_TYPE_BINARY,0);
		if(!m_hRequest)
		{
			InternetCloseHandle(m_hConnect);
			m_hConnect=0;
			return false;
		}
	}

	m_stream->SetURL(m_url.protocol+"://"+m_url.hostName+m_url.urlPath);
	m_stream->Refresh();
	::InternetQueryDataAvailable(m_hRequest,&m_dataValiable,0,0);
	m_eof=false;
	return true;
}
void Win32WebRequest::Disconnect()
{
	if(m_hRequest)
	{
		InternetCloseHandle(m_hRequest);
		m_hRequest=0;
	}
	if(m_hConnect)
	{
		InternetCloseHandle(m_hConnect);
		m_hConnect=0;
	}
	m_eof=true;
}

OS::IStream* Win32WebRequest::GetStream()
{
	return m_stream;
}

int Win32WebRequest::Read(void*data,int len)
{
	if(m_hConnect==0)
		return 0;
	DWORD bytesRed;
	DWORD totalData=0;
	BYTE* ptr=(BYTE*)data;
	do 
	{
		BOOL res=InternetReadFile(m_hRequest,ptr,len-totalData,&bytesRed);
		if(res!=TRUE)
		{
			DWORD err;
			mchar buffer[1024];
			DWORD sz=1024;
			InternetGetLastResponseInfo(&err,buffer,&sz);

			m_eof=true;
			return 0;
		}
		if(!bytesRed)
			m_eof=true;
		ptr+=bytesRed;
		totalData+=bytesRed;
	} while (totalData<len && !m_eof);
	return totalData;
}
int Win32WebRequest::Write(const void*data,int len)
{
	if(m_hConnect==0)
		return 0;
	DWORD bytesRed;
	if(!InternetWriteFile(m_hRequest,data,len,&bytesRed))
		return 0;
	return bytesRed;
}
bool Win32WebRequest::EoF()
{
	return m_eof;
}

}
}
