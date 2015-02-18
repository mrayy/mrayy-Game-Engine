

#include "stdafx.h"
#include "WebURL.h"
#include "StringConverter.h"

namespace mray
{
namespace network
{

core::string WebURL::toString(bool withPath)
{
	core::string url=mT("");

	url=protocol+mT("://");
	if(userName!=mT(""))
	{
		url+=userName;
		if(password!=mT(""))
		{

			url+=mT(":")+password;
		}
		url+=mT("@");
	}else if(password!=mT(""))
		return mT("");//password without user name!

	url+=hostName+mT(":")+core::StringConverter::toString(port);

	if(withPath)
	{
		if(url[url.length()-1]!='/')
		{
			if(urlPath!=mT("") && urlPath[0]!='/')
				url+=mT("/");
		}
		url+=urlPath;
	}else
	{
		if(url[url.length()-1]=='/')
		{
			url[url.length()-1]='\0';
			url.resize(url.length()-1);
		}
	}

	return url;
}


}
}