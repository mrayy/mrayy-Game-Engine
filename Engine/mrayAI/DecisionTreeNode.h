

/********************************************************************
	created:	2010/05/26
	created:	26:5:2010   10:43
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\DecisionTreeNode.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	DecisionTreeNode
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef DecisionTreeNode_h__
#define DecisionTreeNode_h__

#include <marray.h>
#include <list>

namespace mray
{
namespace AI
{

class DecisionTreeNode;
class TrainingDataSet;
class AttributesScheme;


class DecisionTreeNodeValue
{
public:
	DecisionTreeNodeValue():subNode(0),assignedBucket(0),classificationVal(0)
	{
	}
	virtual~DecisionTreeNodeValue();

	bool classificationVal;

	std::vector<int> targetVals;
	DecisionTreeNode* subNode;
	ushort assignedBucket;

	std::list<int> subExamplesIDs;
};

class DecisionTreeNode
{
private:
protected:
public:
	DecisionTreeNode():attributeID(-1)
	{}
	DecisionTreeNode(int attrID):attributeID(attrID)
	{}
	virtual ~DecisionTreeNode(){
	}

	void BuildValues(AttributesScheme*scheme,TrainingDataSet*ds,const std::list<int>& samples,int targetAttr);

	void CleanUp();

	int attributeID;
	std::vector<DecisionTreeNodeValue> values;

	std::vector<int> targetVals;//holds number of examples per value
	
};

}
}

#endif // DecisionTreeNode_h__
