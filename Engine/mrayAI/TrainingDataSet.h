

/********************************************************************
	created:	2010/05/26
	created:	26:5:2010   10:24
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\TrainingDataSet.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	TrainingDataSet
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef TrainingDataSet_h__
#define TrainingDataSet_h__

#include "CompileConfig.h"
#include <marray.h>
#include <mstring.h>

namespace mray
{
	namespace xml
	{
		class XMLElement;
	}
namespace AI
{
	class AttributesScheme;

	typedef std::vector<ushort> TrainingExample;

class MRAY_AI_DLL TrainingDataSet
{
private:
protected:
	AttributesScheme* m_scheme;
	std::vector<TrainingExample> m_examples;
public:
	TrainingDataSet(AttributesScheme* scheme);
	virtual ~TrainingDataSet();

	void SetExamplesCount(int sz);
	template<class T>
	void SetTypedValue(int example,int attr,const T& value)
	{
		SetValue(example,attr,(void*)&value);
	}
	void SetValue(int example,int attr,int bucket);
	void SetValue(int example,int attr,void* value);
	void SetValue(int example,const core::string& attr,void* value);

	int GetExamplesCount();
	const TrainingExample& GetExample(int i);

	void loadExamplesFromXML(xml::XMLElement*elem);

};

}
}

#endif // TrainingDataSet_h__
