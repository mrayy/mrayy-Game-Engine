
/********************************************************************
	created:	2013/09/08
	created:	8:9:2013   1:02
	filename: 	C:\Development\mrayEngine\Projects\TouchShadow\InteractionRuleImpl.h
	file path:	C:\Development\mrayEngine\Projects\TouchShadow
	file base:	InteractionRuleImpl
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __InteractionRuleImpl__
#define __InteractionRuleImpl__

#include "InteractionRule.h"


namespace mray
{
	class CollectionRulePremise:public IRulePremise
	{
	protected:
		std::vector<IRulePremise*> m_parts;
	public:
		CollectionRulePremise(){}
		virtual~CollectionRulePremise();
		virtual void LoadFromXML(xml::XMLElement* elem);
	};
	class RuleAndPremise:public CollectionRulePremise
	{
	protected:
	public:
		virtual bool Evalute();

	};

	class RuleOrPremise:public CollectionRulePremise
	{
	protected:
	public:
		virtual bool Evalute();

	};
	class RuleAreaTouchingPremise:public IRulePremise
	{
	protected:
		core::string m_areaName;
		float m_threshold;
	public:
		RuleAreaTouchingPremise():m_threshold(0)
		{
		}
		virtual bool Evalute();
		virtual void LoadFromXML(xml::XMLElement* elem);

	};


	class RuleOnKeyPremise:public IRulePremise
	{
	protected:
		core::string m_key;
	public:
		RuleOnKeyPremise()
		{
		}
		virtual bool Evalute();
		virtual void LoadFromXML(xml::XMLElement* elem);

	};

	//////////////////////////////////////////////////////////////////////////
	class RuleRunChannelResult:public IRuleResult
	{
	protected:
		class CInput
		{
		public:
			CInput():weight(1){}
			core::string area;
			float weight;

			void LoadFromXML(xml::XMLElement*e);
		};
		std::vector<CInput> m_inputs;
		core::string m_channelName;
		core::string m_audioName;
		bool m_stopped;
	public:
		RuleRunChannelResult():m_stopped(true)
		{
		}
		virtual bool Execute();
		void Stop();
		virtual void LoadFromXML(xml::XMLElement* elem);
	};

	class RulePrintResult:public IRuleResult
	{
	protected:
		core::string m_label;
		bool m_stopped;
	public:
		RulePrintResult():m_stopped(true)
		{}
		virtual bool Execute();
		void Stop(){m_stopped=true;}
		virtual void LoadFromXML(xml::XMLElement* elem);
	};

}


#endif
