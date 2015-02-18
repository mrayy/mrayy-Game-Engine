#include "stdafx.h"


#include "AIDesire.h"
#include <math.h>
#include <assert.h>
#include <XMLElement.h>
#include <StringConverter.h>

namespace mray
{
namespace AI
{

AIDesire::AIDesire():m_threshold(0.5)
{
}
AIDesire::~AIDesire()
{
}


void AIDesire::SetName(const core::string&name)
{
	m_name=name;
}
const core::string& AIDesire::GetName()
{
	return m_name;
}

void AIDesire::Create(const AIDesireInputList& inputs)
{
	m_inputs.resize(inputs.size());
	m_inputsNames.resize(inputs.size());
	for (int i=0;i<inputs.size();++i)
	{
		m_inputs[i]=inputs[i];
	}
}

void AIDesire::Create(const AIDesireInputList& inputs,const std::vector<core::string>&names)
{
	assert(inputs.size()==names.size());
	Create(inputs);
	for (int i=0;i<inputs.size();++i)
	{
		m_inputsNames[i]=names[i];
	}
}

const core::string& AIDesire::GetInputName(int i)
{
	assert(i<m_inputsNames.size());
	return m_inputsNames[i];
}
int AIDesire::GetInputsCount()
{
	return m_inputs.size();
}

void AIDesire::Train(const AIDesireTrainingExample &example,float learnRate,float epsilon)
{
	if(example.input.size()!=m_inputs.size())
		return;
	float e=0;
	do{
		float v=Calc(example.input);
		e=example.output-v;
		for (int i=0;i<m_inputs.size();++i)
		{
			m_inputs[i]+=learnRate*e*example.input[i];
		}
	}while(e>epsilon);
}
void AIDesire::Batch(const AIDesireTrainingSet &examples,float learnRate,float epsilon)
{
	float totalE;
	float e=0;
	do{
		totalE=0;
		for (int j=0;j<examples.size();++j)
		{
			float v=Calc(examples[j].input);
			e=examples[j].output-v;
			for (int i=0;i<m_inputs.size();++i)
			{
				m_inputs[i]+=learnRate*e*examples[j].input[i];
			}
			totalE+=fabs(e);
		}
		totalE/=examples.size();
	}while(totalE>epsilon);
}

float AIDesire::Calc(const AIDesireInputList& input)
{
	if(input.size()!=m_inputs.size())
		return 0;
	float value=0;
	for (int i=0;i<m_inputs.size();++i)
	{
		value+=m_inputs[i]*input[i];
	}
	return value;
}

bool AIDesire::Check(const AIDesireInputList& input){
	return Calc(input)>m_threshold;
}

void AIDesire::loadXMLSettings(xml::XMLElement* elem)
{
	m_inputsNames.clear();
	m_inputs.clear();

	xml::XMLAttribute*attr;
	attr=elem->getAttribute(mT("name"));
	if(attr)
		m_name=attr->value;
	attr=elem->getAttribute(mT("threshold"));
	if(attr)
		m_threshold=core::StringConverter::toFloat(attr->value);

	xml::xmlSubElementsMapIT it= elem->getElementsBegin();
	xml::xmlSubElementsMapIT end= elem->getElementsEnd();
	for (;it!=end;++it)
	{
		if((*it)->GetType()!=xml::ENT_Element)continue;
		xml::XMLElement* e=dynamic_cast<xml::XMLElement*>(*it);
		if(e->getName().equals_ignore_case(mT("input")))
		{
			attr=elem->getAttribute(mT("name"));
			if(attr)
				m_inputsNames.push_back(attr->value);
			else m_inputsNames.push_back(mT(""));

			attr=elem->getAttribute(mT("weight"));
			if(attr)
				m_inputs.push_back(core::StringConverter::toFloat(attr->value));
			else m_inputs.push_back(0.5);
		}
	}

}

}
}
