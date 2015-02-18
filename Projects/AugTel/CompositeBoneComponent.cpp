

#include "stdafx.h"
#include "CompositeBoneComponent.h"
#include "BoneComponent.h"


namespace mray
{
namespace game
{


CompositeBoneComponent::CompositeBoneComponent(game::GameEntityManager*m)
{
}

CompositeBoneComponent::~CompositeBoneComponent()
{
}


bool CompositeBoneComponent::InitComponent()
{
	if (!IGameComponent::InitComponent())
		return false;

	for (int i = 0; i < m_subNodes.size(); ++i)
	{
		m_subNodes[i].node = RetriveComponent<IMovableComponent>(m_ownerComponent, m_subNodes[i].name);
	}

	return true;
}


void CompositeBoneComponent::SetPosition(const math::vector3d& pos, bool local)
{
	for (int i = 0; i < m_subNodes.size(); ++i)
	{
		scene::IMovable* m= m_subNodes[i].node->GetMovableObject();
		if (local && m->getParent())
		{
			m->setPosition(m->getParent()->getAbsoluteTransformation().inverseTransformVector(pos));
		}else
			m->setPosition(pos);
	}
}
void CompositeBoneComponent::SetOrientation(const math::quaternion& ori, bool local)
{
	for (int i = 0; i < m_subNodes.size(); ++i)
	{
		scene::IMovable* m = m_subNodes[i].node->GetMovableObject();
		if (local && m->getParent())
		{
			m->setOrintation(m->getParent()->getAbsoluteOrintation().inverse()*ori);
		}
		else
			m->setOrintation(ori);
	}

}

xml::XMLElement* CompositeBoneComponent::loadXMLSettings(xml::XMLElement* elem)
{
	IGameComponent::loadXMLSettings(elem);
	xml::XMLElement* e= elem->getSubElement("Bone");

	while (e)
	{
		SubNodes n;
		n.name = e->getValueString("Name");
		m_subNodes.push_back(n);
		e = e->nextSiblingElement("Bone");
	}
	return elem;
}

IMPLEMENT_RTTI(CompositeBoneComponent, IGameComponent)
IMPLEMENT_GAMECOMPONENT_FACTORY(CompositeBoneComponent);



}
}

