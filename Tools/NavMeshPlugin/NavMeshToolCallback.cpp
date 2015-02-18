
#include "stdafx.h"
#include "NavMeshToolCallback.h"


namespace mray{

NavMeshToolCallback::NavMeshToolCallback()
{
	m_NavMeshBtnId=-1;
}

NavMeshToolCallback::~NavMeshToolCallback()
{
}


void NavMeshToolCallback::onEvent(GCPtr<Event> event)
{
	if(event->getType()==ET_GuiEvent){
		GCPtr<GUIEvent> e=event;
		if(e->Event==GET_Mouse_UP){
		}
	}
}

}
