#include "stdafx.h"

#include "ITextDecorateNode.h"
#include "StringUtil.h"


namespace mray
{
namespace GUI
{

void TextContextAttributes::SetAttribute(const core::string&attr,const core::string&value)
{
	core::string str=core::StringUtil::ToLower(attr);

	m_attrs[str]=value;
}
core::string TextContextAttributes::GetAttribute(const core::string&attr)
{
	core::string str=core::StringUtil::ToLower(attr);
	AttributesMap::iterator it=m_attrs.find(str);
	if(it==m_attrs.end())
		return mT("");

	return it->second;
}
//////////////////////////////////////////////////////////////////////////

ITextDecorateNode::ITextDecorateNode()
{
}
ITextDecorateNode::~ITextDecorateNode()
{
	NodeList::iterator it=m_childs.begin();
	for (;it!=m_childs.end();++it)
	{
		delete *it;
	}
	m_childs.clear();
}

void ITextDecorateNode::OnEnter(TextContextAttributes*context)
{
}
void ITextDecorateNode::Draw(TextContextAttributes*context,IGUIRenderer*renderer,const math::rectf*clip)
{
	NodeList::iterator it=m_childs.begin();
	for (;it!=m_childs.end();++it)
	{
		(*it)->OnEnter(context);
		(*it)->Draw(context,renderer,clip);
		(*it)->OnExit(context);
	}
}

void ITextDecorateNode::CalculateSize(TextContextAttributes*context)
{
	OnEnter(context);
	bool first=true;
	NodeList::iterator it=m_childs.begin();
	for (;it!=m_childs.end();++it)
	{
		(*it)->CalculateSize(context);
		if(first){
			m_size=(*it)->GetSize();
			first=false;
		}
		else{
			m_size.x=math::Max(m_size.x,(*it)->GetSize().x);
			m_size.y=math::Max(m_size.y,(*it)->GetSize().y);
		}
	}
	OnExit(context);
}

void ITextDecorateNode::OnExit(TextContextAttributes*context)
{
}

bool ITextDecorateNode::OnEvent(MouseEvent*e)
{
	NodeList::iterator it=m_childs.begin();
	for (;it!=m_childs.end();++it)
	{
		if((*it)->OnEvent(e))
			return true;
	}
	return false;
}


}
}
