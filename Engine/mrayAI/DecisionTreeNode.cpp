#include "stdafx.h"

#include "DecisionTreeNode.h"

#include "IDecisionAttribute.h"
#include "AttributesScheme.h"
#include "TrainingDataSet.h"

namespace mray
{
namespace AI
{

DecisionTreeNodeValue::~DecisionTreeNodeValue()
{
	delete subNode;
}

void DecisionTreeNode::BuildValues(AttributesScheme*scheme,TrainingDataSet*ds,const std::list<int>& samples,int targetAttr)
{
	IDecisionAttribute*attr= scheme->GetAttribute(attributeID);
	IDecisionAttribute*tAttr= scheme->GetAttribute(targetAttr);
	if(!attr || !tAttr)
		return;
	values.resize(attr->GetBucketsCount());
	targetVals.resize(tAttr->GetBucketsCount());
	memset(&targetVals[0],0,targetVals.size()*sizeof(int));
	for(int i=0;i<values.size();++i)
	{
		values[i].targetVals.resize(tAttr->GetBucketsCount());
		memset(&values[i].targetVals[0],0,values[i].targetVals.size()*sizeof(int));
	}
	std::list<int>::const_iterator it=samples.begin();
	for(;it!=samples.end();++it)
	{
		const TrainingExample& e=ds->GetExample(*it);
		DecisionTreeNodeValue&nv=values[e[attributeID]];
		nv.subExamplesIDs.push_back(*it);
		nv.targetVals[e[targetAttr]]++;
		targetVals[e[targetAttr]]++;
	}
	int c=0;
	for(int i=0;i<values.size();++i)
	{
		int val=-1;
		for(int j=0;j<values[i].targetVals.size();++j)
		{
			if(values[i].targetVals[j]!=0){
				if(val!=-1)
				{
					val=-1;
					break;
				}
				val=j;
			}
		}
		if(val!=-1)
		{
			values[i].classificationVal=true;
			values[i].assignedBucket=val;
		}
	}
}

void DecisionTreeNode::CleanUp()
{

	for(int i=0;i<values.size();++i)
	{
		values[i].subExamplesIDs.clear();
		if(values[i].subNode)
			values[i].subNode->CleanUp();
	}
}


}
}
