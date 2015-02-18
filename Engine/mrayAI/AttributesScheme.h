

/********************************************************************
	created:	2010/05/26
	created:	26:5:2010   9:45
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\AttributesScheme.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	AttributesScheme
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef AttributesScheme_h__
#define AttributesScheme_h__

#include "CompileConfig.h"
#include <map>
#include <vector>
#include "mString.h"

namespace mray
{
	namespace xml
	{
		class XMLElement;
	}
namespace AI
{
	class IDecisionAttribute;

class MRAY_AI_DLL AttributesScheme
{
private:
protected:
	typedef std::map<core::string,int> AttrNameIDMap;
	typedef std::vector<IDecisionAttribute*> AttributeList;

	AttrNameIDMap m_nameID;
	AttributeList m_attributes;
public:
	AttributesScheme();
	virtual ~AttributesScheme();
	
	void AddAttribute(IDecisionAttribute*attr);

	int GetAttributeIndex(const core::string&name);
	IDecisionAttribute* GetAttribute(const core::string&name);
	IDecisionAttribute* GetAttribute(int i);

	int GetAttributesCount();

	void ParseAttribute(xml::XMLElement* elem);
	virtual void loadXMLSettings(xml::XMLElement* elem);
};

}
}

#endif // AttributesScheme_h__
