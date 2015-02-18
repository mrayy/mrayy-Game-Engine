
#include "stdafx.h"
#include "GUIManagerRT.h"

namespace mray
{
namespace scene
{

GUIManagerRT::GUIManagerRT()
{
}

GUIManagerRT::~GUIManagerRT()
{
}

void GUIManagerRT::AddElemenet(IGUIElementRT* e)
{
	if(!e)
		return;
	e->SetCreator(this);
	m_elements.push_back(e);
}

GCPtr<GUIElementRT> GUIManagerRT::CreateFromElement(GUI::IGUIElement* elem)
{
	GUIElementRT* e=new GUIElementRT();
	e->SetElement(elem);
	AddElemenet(e);
	return e;
}

GCPtr<GUIElementRT_Manager> GUIManagerRT::CreateFromManager(GUI::IGUIManager* mngr)
{
	GUIElementRT_Manager* e=new GUIElementRT_Manager();
	e->SetMananger(mngr);
	AddElemenet(e);
	return e;
}


void GUIManagerRT::Clear()
{
	m_elements.clear();
}

void GUIManagerRT::RemoveElement(IGUIElementRT* e)
{
	std::list<IGUIElementRTPtr>::iterator it= m_elements.begin();
	for (;it!=m_elements.end();++it)
	{
		if((*it).pointer()==e)
		{
			m_elements.erase(it);
			break;
		}
	}
}



bool GUIManagerRT::OnEvent(Event* e)
{
	std::list<IGUIElementRTPtr>::iterator it= m_elements.begin();
	for (;it!=m_elements.end();++it)
	{
		if((*it)->OnEvent(e))
			return true;
	}
	return false;
}

void GUIManagerRT::DrawRT()
{
	std::list<IGUIElementRTPtr>::iterator it= m_elements.begin();
	for (;it!=m_elements.end();++it)
	{
		if(!(*it)->GetManualRender())
			(*it)->DrawElement();
	}
}

}
}

