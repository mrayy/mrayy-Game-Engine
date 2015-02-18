


/********************************************************************
	created:	2011/10/20
	created:	20:10:2011   15:11
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\WebURL.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	WebURL
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __WebURL__
#define __WebURL__

#include "CompileConfig.h"
#include "mstring.h"

namespace mray
{
namespace network
{

class MRAY_DLL WebURL
{
private:
public:

	core::string protocol;//http,ftp,...

	core::string hostName;
	int port;

	core::string urlPath;

	core::string userName;
	core::string password;

public:
	WebURL(const core::string&proto,const core::string& host,const core::string &path,int _port=80,
		const core::string& user=mT(""),const core::string& pass=mT("")):protocol(proto),hostName(host),port(_port),
		userName(user),password(pass),urlPath(path)
	{
	}
	WebURL():port(80){}
	~WebURL(){}

	core::string toString(bool withPath);

};


}
}

#endif
