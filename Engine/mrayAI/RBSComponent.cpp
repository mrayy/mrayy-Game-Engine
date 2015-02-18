#include "stdafx.h"

#include "RBSComponent.h"
#include "AIComponentTypes.h"
#include "RBSParseXML.h"


namespace mray
{
namespace AI
{



RBSComponent::RBSComponent()
{
	m_RBS=new RuleBasedSystem(mT(""));
}
RBSComponent::~RBSComponent()
{
	delete m_RBS;
}


RuleBasedSystem* RBSComponent::GetRBS()
{
	return m_RBS;
}

void RBSComponent::Tick()
{
	m_RBS->Process();
}
xml::XMLElement* RBSComponent::loadXMLSettings(xml::XMLElement* elem)
{
	IAIComponent::loadFromXML(elem);
	RBSParseXML parser;
	parser.ParseRBS(elem,m_RBS);

	return elem;
}

}
}