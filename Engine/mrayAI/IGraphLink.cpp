#include "stdafx.h"

#include "IGraphLink.h"
#include "IGraphNode.h"

namespace mray{
namespace AI{

IGraphLink::IGraphLink(){
	ID=0;
	//Name="";
	m_begin=0;
	m_end=0;
}
IGraphLink::IGraphLink(IGraphNode* begin,IGraphNode* end,int id/*,const core::string &name,*/){
	this->ID=id;
	//this->Name=name;
	this->m_begin=begin;
	this->m_end=end;
}
IGraphNode* IGraphLink::getNode(int id)
{
	if(m_begin->getID()==id)
		return m_begin;
	if(m_end->getID()==id)
		return m_end;
	return 0;
}
bool IGraphLink::isBegin(IGraphNode* node)
{
	if(m_begin==node)return true;
	return false;
}
IGraphNode*IGraphLink::getOtherNode(int id)
{
	return getOtherNode(getNode(id));
}
IGraphNode*IGraphLink::getOtherNode(IGraphNode* a)
{
	if(m_begin==a)return m_end;
	return m_begin;
}


}
}



