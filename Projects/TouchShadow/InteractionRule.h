
/********************************************************************
	created:	2013/09/07
	created:	7:9:2013   22:44
	filename: 	C:\Development\mrayEngine\Projects\TouchShadow\InteractionRule.h
	file path:	C:\Development\mrayEngine\Projects\TouchShadow
	file base:	InteractionRule
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __InteractionRule__
#define __InteractionRule__




namespace mray
{

	class InteractionRules;
	class IRulePremise
	{
	protected:
	public:

		IRulePremise()
		{}

		virtual bool Evalute()=0;
		virtual void LoadFromXML(xml::XMLElement* elem)=0;

		static IRulePremise* Create(const core::string& type);
	};
	class IRuleResult
	{
	public:
		virtual bool Execute()=0;
		virtual void Stop(){}
		virtual void LoadFromXML(xml::XMLElement* elem)=0;
		static IRuleResult* Create(const core::string& type);
	};

class SingleRule
{
protected:
	core::string m_name;
	IRulePremise* m_premise;
	std::vector<IRuleResult*> m_results;
	bool m_lastSuccess;
public:
	SingleRule():m_premise(0),m_lastSuccess(false)
	{
	}
	virtual~SingleRule()
	{
		delete m_premise;
		for (int i=0;i<m_results.size();++i)
		{
			delete m_results[i];
		}
	}

	bool Run();
	void LoadFromXML(xml::XMLElement* elem);
};


class InteractionRules
{
protected:
	std::vector<SingleRule*> m_rules;
public:
	InteractionRules()
	{
	}
	~InteractionRules()
	{
		Clear();
	}

	void RunRules();

	void Clear();
	void LoadFromXML(xml::XMLElement* elem);

};

}


#endif
