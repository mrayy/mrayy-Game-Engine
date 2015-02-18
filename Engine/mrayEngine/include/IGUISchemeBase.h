



/********************************************************************
	created:	2012/07/08
	created:	8:7:2012   23:44
	filename: 	c:\Development\mrayEngine\Engine\mrayEngine\include\IGUISchemeBase.h
	file path:	c:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	IGUISchemeBase
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __IGUISchemeBase__
#define __IGUISchemeBase__

#include "CompileConfig.h"
#include "mstring.h"

namespace mray
{
namespace GUI
{
	class IGUIElement;

class MRAY_DLL IGUISchemeBase
{
protected:
	std::map<core::string,GUI::IGUIElement**> m_elementsMap;
public:
	IGUISchemeBase();
	virtual~IGUISchemeBase();

	void _ElementDeclared(IGUIElement*e);

	virtual void DoneLoadingElements(){}

};

}
}

#endif