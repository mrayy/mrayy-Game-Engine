

#ifndef __AWESOMIUMHELPERS__
#define __AWESOMIUMHELPERS__

#include <Awesomium/WebString.h>
#include <mString.h>

namespace mray
{
namespace web
{

	
class AwesomiumHelpers
{
protected:

public:
	AwesomiumHelpers(){}
	virtual ~AwesomiumHelpers(){}

	static Awesomium::WebString ToString(const core::string& str)
	{
		return Awesomium::WebString::CreateFromUTF8(str.c_str(), str.length());
	}
	static core::string FromString(const Awesomium::WebString & str)
	{
		core::string r;
		r.resize(str.length());
		str.ToUTF8(&r[0], r.length());
		return r;
	}

};

}
}


#endif
