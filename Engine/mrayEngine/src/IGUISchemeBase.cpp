


#include "stdafx.h"
#include "IGUISchemeBase.h"

#include "IGUIElement.h"



namespace mray
{
namespace GUI
{
	IGUISchemeBase::IGUISchemeBase()
	{

	}

	IGUISchemeBase::~IGUISchemeBase()
	{

	}

void IGUISchemeBase::_ElementDeclared(IGUIElement*e)
{
	if(!e)return;
	std::map<core::string,GUI::IGUIElement**>::iterator it=m_elementsMap.find(e->GetName());
	if(it==m_elementsMap.end())return;
	*((*it).second)=e;
}

}
}