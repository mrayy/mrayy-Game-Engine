#include "stdafx.h"

#include "IAIArchitect.h"
#include "IAIComponent.h"

namespace mray{
namespace AI{


IAIArchitect::IAIArchitect():m_isVisible(false)
{
}
IAIArchitect::~IAIArchitect()
{

}

void IAIArchitect::EnableVisualize(bool e)
{
	m_isVisible=e;
}
bool IAIArchitect::IsEnableVisualize()
{
	return m_isVisible;
}
void IAIArchitect::Update(float dt)
{
	ComponentsMap::iterator it= m_components.begin();
	for (;it!=m_components.end();++it)
	{
		std::list<IObjectComponent*>& lst=it->second;
		std::list<IObjectComponent*>::iterator oit= lst.begin();
		for (;oit!=lst.end();++oit)
		{
			IAIComponent* comp=dynamic_cast<IAIComponent*>(*oit);
			if(comp)
				comp->UpdateComponent(dt);
		}
	}
}

void IAIArchitect::Visualize(IRenderDevice*dev)
{
	ComponentsMap::iterator it= m_components.begin();
	for (;it!=m_components.end();++it)
	{
		std::list<IObjectComponent*>& lst=it->second;
		std::list<IObjectComponent*>::iterator oit= lst.begin();
		for (;oit!=lst.end();++oit)
		{
			IAIComponent* comp=dynamic_cast<IAIComponent*>(*oit);
			if(comp)
				comp->VisualizeComponent(dev);
		}
	}
}



}
}
