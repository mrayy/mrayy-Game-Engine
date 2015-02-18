
#include "stdafx.h"
#include "InteractionRule.h"
#include "InteractionRuleImpl.h"


namespace mray
{

	IRulePremise* IRulePremise::Create(const core::string& type)
	{
		if(type.equals_ignore_case("and"))
			return new RuleAndPremise();
		if(type.equals_ignore_case("or"))
			return new RuleOrPremise();
		if(type.equals_ignore_case("AreaTouching"))
			return new RuleAreaTouchingPremise();
		if(type.equals_ignore_case("OnKey"))
			return new RuleOnKeyPremise();
		return 0;
	}

	IRuleResult* IRuleResult::Create(const core::string& type)
	{
		if(type.equals_ignore_case("RunChannel"))
			return new RuleRunChannelResult();
		if(type.equals_ignore_case("Print"))
			return new RulePrintResult();
		return 0;
	}




	bool SingleRule::Run()
	{
		if(!m_premise || m_results.size()==0)
			return false;
		if(!m_premise->Evalute())
		{
			if(m_lastSuccess)
			{
				for (int i=0;i<m_results.size();++i)
				{
					m_results[i]->Stop();
				}
				m_lastSuccess=false;
			}
			return false;
		}
		for (int i=0;i<m_results.size();++i)
		{
			m_results[i]->Execute();
		}
		m_lastSuccess=true;
		return true;
	}

	void SingleRule::LoadFromXML(xml::XMLElement* elem)
	{
		m_name=elem->getValueString("name");
		xml::XMLElement* e= elem->getSubElement("Premise");
		if(e)
		{
			xml::xmlSubElementsMapIT it=e->getElementsBegin();
			for(;it!=e->getElementsEnd();++it)
			{
				if((*it)->GetType()==xml::ENT_Element)
				{
					xml::XMLElement* e=(xml::XMLElement*)(*it);
					m_premise=IRulePremise::Create(e->getName());
					if(m_premise)
					{
						m_premise->LoadFromXML(e);
						break;
					}
				}
			}
		}
		e= elem->getSubElement("Results");
		if(e)
		{
			xml::xmlSubElementsMapIT it=e->getElementsBegin();
			for(;it!=e->getElementsEnd();++it)
			{
				if((*it)->GetType()==xml::ENT_Element)
				{
					xml::XMLElement* e=(xml::XMLElement*)(*it);
					IRuleResult* res=IRuleResult::Create(e->getName());
					if(res)
					{
						res->LoadFromXML(e);
						m_results.push_back(res);
					}
				}
			}
		}

	}

	void InteractionRules::Clear()
	{
		for (int i=0;i<m_rules.size();++i)
		{
			delete m_rules[i];
		}
		m_rules.clear();
	}

	void InteractionRules::RunRules()
	{
		for (int i=0;i<m_rules.size();++i)
		{
			m_rules[i]->Run();
		}
	}

	void InteractionRules::LoadFromXML(xml::XMLElement* e)
	{

		Clear();
		xml::XMLElement* elem=e->getSubElement("Rule");
		while (elem)
		{
			SingleRule* rule=new SingleRule();
			
			rule->LoadFromXML(elem);
			m_rules.push_back(rule);
			elem=elem->nextSiblingElement("Rule");
		}
	}
}

