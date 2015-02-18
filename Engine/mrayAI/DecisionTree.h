


/********************************************************************
	created:	2010/04/30
	created:	30:4:2010   20:48
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\DecisionTree.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	DecisionTree
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	giving set of attribute,form a decision tree
				basic algorithm is : ID3
*********************************************************************/

#ifndef DecisionTree_h__
#define DecisionTree_h__

#include "CompileConfig.h"
#include "IAIComponent.h"
#include "IDecisionAttribute.h"

namespace mray
{
namespace AI
{

	class AttributesScheme;
	class TrainingDataSet;
	class DecisionTreeNode;

class MRAY_AI_DLL DecisionTree:public IAIComponent
{
	DECLARE_RTTI
private:
	static const float m_invLog2;
protected:
	typedef std::list<int> TrainingSubSetList;
	AttributesScheme* m_scheme;
	TrainingDataSet* m_trainingSet;
	int m_targetAttr;
	std::vector<bool> m_chosedAttrs;

	DecisionTreeNode* m_root;

	int CalcSamplesCount(const std::vector<int>& ds);
	float CalcEntropy(const std::vector<int>& ds,int c);
	float CalcGain(DecisionTreeNode* node);
	DecisionTreeNode* SelectBestNode(const TrainingSubSetList& ds);


	DecisionTreeNode* Build(const TrainingSubSetList& ds);

	void ExportNode(DecisionTreeNode*node,xml::XMLElement* elem);

	void* _Predict(const std::vector<ushort>& input);
public:
	DecisionTree();
	virtual ~DecisionTree();

	void SetScheme(AttributesScheme* scheme);
	AttributesScheme* GetScheme();

	void Train(TrainingDataSet*trainingSet,const core::string&targetAttribute);

	template<class T>
	T Predict(const std::vector<ushort>& input)
	{
		return *(T*)_Predict(input);
	}

	virtual xml::XMLElement* loadXMLSettings(xml::XMLElement* elem);
	virtual xml::XMLElement* exportXMLSettings(xml::XMLElement* elem);
};

}
}

#endif // DecisionTree_h__
