#include "stdafx.h"


#include "TrainingDataSet.h"
#include "AttributesScheme.h"
#include "IDecisionAttribute.h"

#include "XMLElement.h"


namespace mray
{
namespace AI
{

TrainingDataSet::TrainingDataSet(AttributesScheme* scheme):m_scheme(scheme)
{
}

TrainingDataSet::~TrainingDataSet()
{
}


void TrainingDataSet::SetExamplesCount(int sz)
{
	int attrCnt=m_scheme->GetAttributesCount();
	m_examples.resize(sz);
	for (int i=0;i<m_examples.size();++i)
	{
		m_examples[i].resize(attrCnt);
	}
}

void TrainingDataSet::SetValue(int example,int attr,int bucket)
{
	if(example>=m_examples.size())
		return;
	m_examples[example][attr]=bucket;
}
void TrainingDataSet::SetValue(int example,int attr,void* value)
{
	if(example>=m_examples.size())
		return;
	IDecisionAttribute*decAttr= m_scheme->GetAttribute(attr);
	if(!decAttr)
		return;
	m_examples[example][attr]=decAttr->GetBucket(value);
}

void TrainingDataSet::SetValue(int example,const core::string& attr,void* value)
{
	if(example>=m_examples.size())
		return;
	int index=m_scheme->GetAttributeIndex(attr);
	if(index==-1)
		return;

	IDecisionAttribute*decAttr= m_scheme->GetAttribute(index);
	m_examples[example][index]=decAttr->GetBucket(value);
}

int TrainingDataSet::GetExamplesCount()
{
	return m_examples.size();
}
const TrainingExample& TrainingDataSet::GetExample(int i)
{
	return m_examples[i];
}


void TrainingDataSet::loadExamplesFromXML(xml::XMLElement*elem)
{
	if(!m_scheme)
		return;
	xml::xmlSubElementsMapIT it=elem->getElementsBegin();
	xml::xmlSubElementsMapIT end=elem->getElementsEnd();
	SetExamplesCount(elem->getSubElementsCount());
	int sample=0;
	for (;it!=end;++it,++sample)
	{
		if((*it)->GetType()!=xml::ENT_Element)continue;
		xml::XMLElement* e=dynamic_cast<xml::XMLElement*>(*it);
		if(e->getName().equals_ignore_case(mT("sample")))
		{
			xml::xmlAttributesMapIT ait= e->getAttributesBegin();
			xml::xmlAttributesMapIT aend= e->getAttributesEnd();
			for(;ait!=aend;++ait)
			{
				xml::XMLAttribute*attr=&ait->second;
				 int i=m_scheme->GetAttributeIndex(attr->name);
				SetValue(sample,i,m_scheme->GetAttribute(i)->GetBucketFromString(attr->value));
			}
		}
	}
}


}
}
