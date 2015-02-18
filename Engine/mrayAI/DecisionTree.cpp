#include "stdafx.h"

#include "DecisionTree.h"
#include "DecisionTreeNode.h"
#include "AttributesScheme.h"
#include "TrainingDataSet.h"

#include "XMLWriter.h"
#include "IFileSystem.h"
#include "StreamWriter.h"
#include <math.h>

namespace mray
{
namespace AI
{

const float DecisionTree::m_invLog2=1.0f/logf(2.0f);

DecisionTree::DecisionTree(): m_scheme(0),m_root(0)
{
}
DecisionTree::~DecisionTree()
{
	delete m_scheme;
	delete m_root;
}

void DecisionTree::SetScheme(AttributesScheme* scheme)
{
	m_scheme=scheme;
}
AttributesScheme* DecisionTree::GetScheme()
{
	return m_scheme;
}

int DecisionTree::CalcSamplesCount(const std::vector<int>& ds)
{
	int count=0;
	for (int i=0;i<ds.size();++i)
	{
		count+= ds[i];//m_scheme->GetAttribute(*it)->GetBucketsCount();
	}
	return count;
}
float DecisionTree::CalcEntropy(const std::vector<int>& ds,int c)
{
	float cInv=1.0f/(float)c;
	float e=0;
	for (int i=0;i<ds.size();++i)
	{
		float p=ds[i]*cInv;
		if(p!=0)
			e+=-p*logf(p)*m_invLog2;
	}
	return e;
}
float DecisionTree::CalcGain(DecisionTreeNode* node)
{
	int S=CalcSamplesCount(node->targetVals);
	if(S==0)
		return 0;
	float eS=CalcEntropy(node->targetVals,S);
	float gain=eS;

	float invS=1.0f/(float)S;

	for(int i=0;i<node->values.size();++i)
	{
		int Si=CalcSamplesCount(node->values[i].targetVals);
		if(Si!=0){
			float e=CalcEntropy(node->values[i].targetVals,Si);
			gain-=(float)Si*invS*e;
		}
	}
	return gain;
}
DecisionTreeNode* DecisionTree::SelectBestNode(const TrainingSubSetList& ds)
{
	std::vector<DecisionTreeNode*> nodes;
	nodes.resize(m_scheme->GetAttributesCount()-1);
	int attrID=0;
	int best=0;
	float bestGain=0;
	for(int i=0;i<nodes.size();++i)
	{
		if(attrID==m_targetAttr)
			++attrID;
		if(m_chosedAttrs[i]==true){
			nodes[i]=0;
			++attrID;
			continue;
		}
		nodes[i]=new DecisionTreeNode(attrID);
		nodes[i]->BuildValues(m_scheme,m_trainingSet,ds,m_targetAttr);
 
		float g=CalcGain(nodes[i]);
		if(g>bestGain)
		{
			best=i;
			bestGain=g;
		}
		++attrID;
	}
	for(int i=0;i<nodes.size();++i)
	{
		if(i==best)
			continue;
		delete nodes[i];
	}
	return nodes[best];
}

void DecisionTree::Train(TrainingDataSet*trainingSet,const core::string&targetAttribute)
{
	if(!m_scheme)
		return;
	TrainingSubSetList subset;
	m_trainingSet=trainingSet;
	m_targetAttr=m_scheme->GetAttributeIndex(targetAttribute);
	m_chosedAttrs.resize(m_scheme->GetAttributesCount());
	for (int i=0;i<m_chosedAttrs.size();++i)
	{
		m_chosedAttrs[i]=false;
	}
	m_chosedAttrs[m_targetAttr]=true;

	int cnt=m_trainingSet->GetExamplesCount();
	for(int i=0;i<cnt;++i)
	{
		subset.push_back(i);
	}
	m_root=Build(subset);

	if(m_root)
		m_root->CleanUp();

}
DecisionTreeNode* DecisionTree::Build(const TrainingSubSetList& ds)
{
	if(ds.size()==0)
		return 0;
	DecisionTreeNode*res=SelectBestNode(ds);
	if(!res)
		return 0;

	m_chosedAttrs[res->attributeID]=true;

	for(int i=0;i<res->values.size();++i)
	{
		if(!res->values[i].classificationVal)
			res->values[i].subNode=Build(res->values[i].subExamplesIDs);
	}
	m_chosedAttrs[res->attributeID]=false;//roll back the change
	return res;

}

void* DecisionTree::_Predict(const std::vector<ushort>& input)
{
	DecisionTreeNode*currentNode=m_root;
	int id=currentNode->attributeID;
	while (currentNode->values[input[id]].classificationVal==0) 
	{
		currentNode=currentNode->values[input[id]].subNode;
		id=currentNode->attributeID;
	} 
	IDecisionAttribute*attr= m_scheme->GetAttribute(currentNode->attributeID);
	return attr->GetValue(currentNode->values[input[id]].assignedBucket);
}


xml::XMLElement* DecisionTree::loadXMLSettings(xml::XMLElement* elem)
{
	xml::XMLElement*e;
	e=elem->getSubElement(mT("scheme"));
	if(e)
	{
		if(m_scheme)
			delete m_scheme;
		m_scheme=new AttributesScheme();
		m_scheme->loadXMLSettings(e);
	}
	e=elem->getSubElement(mT("training"));
	if(e)
	{
		xml::XMLAttribute*attr=e->getAttribute(mT("Target"));
		if(!attr)
			return elem;
		TrainingDataSet ds(m_scheme);
		ds.loadExamplesFromXML(e);
		Train(&ds,attr->value);

		//test
		
		xml::XMLElement e(mT("DT"));
		exportXMLSettings(&e);
		GCPtr<OS::IStream> stream=gFileSystem.openFile(mT("IDT.xml"),OS::TXT_WRITE);
		xml::XMLWriter w;
		w.addElement(&e);
		OS::StreamWriter ww(stream);
		ww.writeString(w.flush());
		stream->close();

	}
	return elem;
}

void DecisionTree::ExportNode(DecisionTreeNode*node,xml::XMLElement* elem)
{
	xml::XMLElement*e=new xml::XMLElement(mT("Node"));
	elem->addSubElement(e);
	IDecisionAttribute*attr= m_scheme->GetAttribute(node->attributeID);
	IDecisionAttribute*tAttr= m_scheme->GetAttribute(m_targetAttr);
	e->addAttribute(mT("Name"),attr->GetName());
	for (int i=0;i<node->values.size();++i)
	{
		xml::XMLElement*e1=new xml::XMLElement(mT("Value"));
		e->addSubElement(e1);
		core::string v=attr->GetValueString(i);
		e1->addAttribute(mT("value"),v);
		if(node->values[i].classificationVal)
		{
			core::string v=tAttr->GetValueString(node->values[i].assignedBucket);
			e1->addAttribute(tAttr->GetName(),v);
		}else
		{
			ExportNode(node->values[i].subNode,e1);
		}
	}
}

xml::XMLElement* DecisionTree::exportXMLSettings(xml::XMLElement* elem)
{
	xml::XMLElement*e=new xml::XMLElement(mT("DecisionTree"));
	elem->addSubElement(e);
	if(!m_root)
		return e;

	ExportNode(m_root,e);

	return e;
}

}
}