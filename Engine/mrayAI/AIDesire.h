

/********************************************************************
	created:	2010/04/30
	created:	30:4:2010   22:24
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\AIDesire.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	AIDesire
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	inspired from perceptrons (NN),the desire defines the agent needs and target
				
*********************************************************************/

#ifndef AIDesire_h__
#define AIDesire_h__

#include "CompileConfig.h"
#include "mString.h"
#include <map>
#include <vector>

namespace mray
{
	namespace xml
	{
		class XMLElement;
	}
namespace AI
{


typedef std::vector<float> AIDesireInputList;

struct AIDesireTrainingExample
{
	AIDesireInputList input;
	float output;
};

typedef std::vector<AIDesireTrainingExample> AIDesireTrainingSet;

class MRAY_AI_DLL AIDesire
{
private:
protected:

	core::string m_name;

	std::vector<core::string> m_inputsNames;

	AIDesireInputList m_inputs;
	float m_threshold;

public:
	AIDesire();
	virtual ~AIDesire();

	void SetName(const core::string&name);
	const core::string& GetName();

	const core::string& GetInputName(int i);
	int GetInputsCount();

	void Create(const AIDesireInputList& inputs);
	void Create(const AIDesireInputList& inputs,const std::vector<core::string>&names);
	
	void Train(const AIDesireTrainingExample &example,float learnRate=0.1f,float epsilon=0.01f);
	void Batch(const AIDesireTrainingSet &example,float learnRate=0.1f,float epsilon=0.01f);

	bool Check(const AIDesireInputList& input);
	float Calc(const AIDesireInputList& input);


	virtual void loadXMLSettings(xml::XMLElement* elem);

};

}
}

#endif // AIDesire_h__
