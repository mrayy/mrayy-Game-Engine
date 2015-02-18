#include "stdafx.h"

#include "TextDecorateNodeFactory.h"

#include "ImageDecorateNode.h"
#include "SizeDecorateNode.h"
#include "FontDecorateFont.h"
#include "ColorDecorateNode.h"

#include "StringUtil.h"

#include "GUID.h"

namespace mray
{
namespace GUI
{

TextDecorateNodeFactory::TextDecorateNodeFactory()
{
	RegisterCreator(new ImageDecorateNodeCreator());
	RegisterCreator(new SizeDecorateNodeCreator());
	RegisterCreator(new FontDecorateFontCreator());
	RegisterCreator(new ColorDecorateNodeCreator());
}
TextDecorateNodeFactory::~TextDecorateNodeFactory()
{
	Clear();
}

void TextDecorateNodeFactory::RegisterCreator(ITextDecorateCreator*c)
{
	core::string str=core::StringUtil::ToLower(c->GetType());
	m_creators[GUID(str).ID()]=c;
}
void TextDecorateNodeFactory::UnRegisterCreator(const core::string&type)
{
	core::string str=core::StringUtil::ToLower(type);
	m_creators.erase(GUID(str).ID());
	
}
void TextDecorateNodeFactory::Clear()
{
	CreatorMap::iterator it= m_creators.begin();
	for(;it!=m_creators.end();++it)
	{
		delete it->second;
	}
	m_creators.clear();
}

ITextDecorateNode* TextDecorateNodeFactory::CreateNode(const core::string&type)
{
	core::string str=core::StringUtil::ToLower(type);
	CreatorMap::iterator it = m_creators.find(GUID(str).ID());
	if(it==m_creators.end())
		return new TextDecorateDummyNode(type);
	return it->second->CreateNode();

}


}
}

