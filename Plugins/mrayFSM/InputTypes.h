
/********************************************************************
	created:	2009/03/05
	created:	5:3:2009   22:37
	filename: 	i:\Programing\GameEngine\mrayEngine\FSM\FSM\InputTypes.h
	file path:	i:\Programing\GameEngine\mrayEngine\FSM\FSM
	file base:	InputTypes
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___InputTypes___
#define ___InputTypes___

#include "CompareCondition.h"

namespace mray{
template <class T>
class ITemplateInputType:public IInput
{
protected:
	virtual void*innerGetValue()=0;
public:
	virtual~ITemplateInputType(){}
	const T&getValue(){
		return *(T*)innerGetValue();
	}
};
//////////////////////////////////////////////////////////////////////////
template <class T>
class TemplateInputType:public ITemplateInputType<T>
{
	T*m_value;
	void*innerGetValue(){
		return m_value;
	}
public:
	TemplateInputType(T*v){
		m_value=v;
	}
};
//////////////////////////////////////////////////////////////////////////
template <class T>
class FunctionInputType:public ITemplateInputType<T>
{
protected:
	typedef void*(FunctionType)();
	FunctionType* m_function;
	void*innerGetValue(){
		return m_function();
	}
public:
	FunctionInputType(FunctionType* func){
		m_function=func;
	}
};
//////////////////////////////////////////////////////////////////////////
template <typename Container,class T>
class classFunctionInputType:public ITemplateInputType<T>
{
protected:
	typedef void*(Container::*FunctionType)();
	Container*m_container;
	FunctionType m_function;
	void*innerGetValue(){
		return (m_container->*m_function)();
	}
public:
	classFunctionInputType(Container*container,FunctionType func){
		m_container=container;
		m_function=func;
	}
};


//////////////////////////////////////////////////////////////////////////
template <class T,class B>
class TemplateCompareCondition:public CompareCondition
{
	ITemplateInputType<T>*input1;
	ITemplateInputType<B>*input2;
public:
	TemplateCompareCondition(const core::string&name,ITemplateInputType<T>*i1,ECompareType compare,ITemplateInputType<B>*i2)
		:CompareCondition(name,compare),input1(i1),input2(i2)
	{

	}
	virtual bool checkCondition(){
		switch (getType())
		{
		case ECT_Equal:
			return input1->getValue()==input2->getValue();
		case ECT_Greater:
			return !(input1->getValue()<input2->getValue()) && !(input1->getValue()==input2->getValue());
		case ECT_GreaterEq:
			return !(input1->getValue()<input2->getValue());
		case ECT_Less:
			return input1->getValue()<input2->getValue();
		case ECT_LessEq:
			return input1->getValue()<input2->getValue() && input1->getValue()==input2->getValue();
		}
		return false;
	}
};

}


#endif //___InputTypes___
