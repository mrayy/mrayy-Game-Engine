

/********************************************************************
	created:	2012/01/17
	created:	17:1:2012   12:55
	filename: 	d:\Development\mrayEngine\Engine\mrayGLDevice\GLSupport.h
	file path:	d:\Development\mrayEngine\Engine\mrayGLDevice
	file base:	GLSupport
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __GLSupport__
#define __GLSupport__

#include "compileConfig.h"
#include <mstring.h>
#include <set>

namespace mray
{
	class OptionContainer;
namespace video
{
	class RenderWindow;

class MRAY_GL_DLL GLSupport
{
private:
protected:
	core::string m_version;
	core::string m_vendor;
	std::set<core::string> m_extensionList;

	static GLSupport* s_instance;
public:
	GLSupport();
	virtual~GLSupport();

	static GLSupport* GetInstance(){return s_instance;}

	virtual void getConfigParams(OptionContainer& params);

	bool CheckExtension(const core::string& ext);

	virtual void InitExtensions();

	const core::string& GetVersion()const{return m_version;}
	const core::string& GetVendor()const{return m_vendor;}

	virtual RenderWindow* CreateRenderWindow(const core::string& name,const math::vector2di& size,
		bool fullScreen,const OptionContainer&params,ulong hWND)=0;
};

}
}

#endif
