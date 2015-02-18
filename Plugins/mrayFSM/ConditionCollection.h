
#ifndef __ConditionCollection__
#define __ConditionCollection__

#include "ICondition.h"
#include <vector>

namespace mray{

	class ConditionList:public ICondition
	{
	public:
		typedef std::vector<ICondition*> ConditionVector;
		ConditionVector m_conditions;

		ConditionList(const core::string&name):
		ICondition(name)
		{

		}
		virtual~ConditionList(){

		}
		void addCondition(ICondition*cond){
			m_conditions.push_back(cond);
		}
		int getConditionCount(){
			return m_conditions.size();
		}

		void clearConditions(){
			m_conditions.clear();
		}
	};

	class AndCondition:public ConditionList
	{
	protected:

	public:
		AndCondition(const core::string&name):
		  ConditionList(name)
		  {

		  }
		  virtual~AndCondition(){

		  }

		virtual bool checkCondition(){
			for (int i=0;i<m_conditions.size();++i)
			{
				if(!m_conditions[i]->checkCondition()){
					return false;
				}
			}
			return true;
		}


	};
	class OrCondition:public ConditionList
	{
	protected:

	public:
		OrCondition(const core::string&name):
		  ConditionList(name)
		  {

		  }
		  virtual~OrCondition(){

		  }

		virtual bool checkCondition(){
			for (int i=0;i<m_conditions.size();++i)
			{
				if(m_conditions[i]->checkCondition()){
					return true;
				}
			}
			return false;
		}
	};

	class NotCondition:public ICondition
	{
	public:
		ICondition*m_condition;
	public:
		NotCondition(const core::string&name,ICondition*condition):
		  ICondition(name),m_condition(condition)
		  {

		  }
		  virtual~NotCondition(){

		  }

		virtual bool checkCondition(){
			return !m_condition->checkCondition();
		}
	};
}


#endif
