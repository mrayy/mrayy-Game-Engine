

#include "GUIElementContainer.h"
#include "Event.h"
#include "GUIElement.h"



namespace mray{
namespace GUI{


GUIElementContainer::GUIElementContainer(){
	m_activeElement=0;
}
GUIElementContainer::~GUIElementContainer(){
}


void GUIElementContainer::removeChild(GUIElement* elem){
	std::list<GCPtr<GUIElement> >::iterator it=m_Children.begin();
	for(;it!=m_Children.end();++it)
	{
		if((*it).pointer()==elem)
		{
			m_Children.remove(it);
			return;
		}
	}
}
void GUIElementContainer::addChild(GUIElement* elem){
	if(elem)
	{
		m_Children.push_back(elem);
	}
}

GUIElement* GUIElementContainer::getElementFromPoint(math::Point2df pos){
	GUIElement* elem=0;
	std::list<GCPtr<GUIElement> >::iterator it=m_Children.last();
	for(;it!=m_Children.end();)
	{
		elem=(*it)->getElementFromPoint(pos);
		if(elem)return elem;

		it--;
	}
	return 0;
}

GUIElement* GUIElementContainer::getElementById(int id,bool searchChildren){
	GUIElement *et;
	std::list<GCPtr<GUIElement> >::iterator it=m_Children.begin();
	for(;it!=m_Children.end();++it)
	{
		if((*it)->Id()==id)
			return (*it);
		if(searchChildren)
			et=(*it)->getElementById(id,searchChildren);
		if(et)return et;
	}
	return 0;
}

bool GUIElementContainer::isElementChild(GUIElement*  elem){
	std::list<GCPtr<GUIElement> >::iterator it=m_Children.begin();
	for(;it!=m_Children.end();++it)
	{
		if((*it).pointer()==elem)
			return true;
	}
	return false;
}

GUIElement* GUIElementContainer::getNextChild(GUIElement* elem){

	if(elem->getChildren()->getSize()!=0){
		return *elem->getChildren()->begin();
	}

	bool found=0;
	GCPtr<GUIElement> et=0;
	std::list<GCPtr<GUIElement> >::iterator it=m_Children.begin();
	std::list<GCPtr<GUIElement> >::iterator tmp;
	for(;it!=m_Children.end();++it)
	{
		if((!(*it)->isElementChild(elem) && (*it).pointer()!=elem )&& !found)continue;
		if(!found){
			if(elem->Visible && elem->Selectable){
				et=(*it)->getNextChild(elem);
				if(et)
					return et;
			}
		}else{
			return *it;
			tmp=(*it)->getChildren()->begin();
			if(tmp!=elem->getChildren()->end())
				return *tmp;
		}
		found=1;
	}
	return 0;
}

void GUIElementContainer::removeAll(){
	m_Children.clear();
}

bool GUIElementContainer::bringToFront(GUIElement* elem){
	std::list<GCPtr<GUIElement> >::iterator it=m_Children.begin();
	for(;it!=m_Children.end();++it)
	{
		if((*it).pointer()==elem)
		{
			GCPtr<GUIElement> tmp=elem;
			m_Children.remove(it);
			m_Children.push_back(elem);
			return 1;
		}
	}
	return 0;
}
bool GUIElementContainer::dropToBack(GUIElement* elem){
	std::list<GCPtr<GUIElement> >::iterator it=m_Children.begin();
	for(;it!=m_Children.end();++it)
	{
		if((*it).pointer()==elem)
		{
			GCPtr<GUIElement> tmp=elem;
			m_Children.remove(it);
			m_Children.push_front(elem);
			return 1;
		}
	}
	return 0;
}

std::list<GCPtr<GUIElement> > *GUIElementContainer::getChildren(){
	return &m_Children;
}
void GUIElementContainer::setActiveElement(GUIElement* elem){
	m_activeElement=elem;
}

void GUIElementContainer::onEvent(const EventPtr&  event){
	if(m_activeElement){
		if(m_activeElement->Visible==false){
			m_activeElement=0;
		}else{
			m_activeElement->onEvent(event);
			return;
		}
	}
	std::list<GCPtr<GUIElement> >::iterator it=m_Children.last();
	for(;it!=m_Children.end()&& (!event->isRecived());it--)
		(*it)->onEvent(event);
}


}
}