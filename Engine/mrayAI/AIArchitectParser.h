

/********************************************************************
	created:	2010/04/18
	created:	18:4:2010   23:16
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\AIArchitectParser.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	AIArchitectParser
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef AIArchitectParser_h__
#define AIArchitectParser_h__

#include "CompileConfig.h"
#include "ISingleton.h"
#include "IAIComponent.h"
#include "IAIArchitect.h"
//#include "IAIComponentParser.h"

namespace mray
{
namespace OS
{
	class IStream;
}
namespace AI
{

class MRAY_AI_DLL AIArchitectParser
{
private:
protected:
	//typedef std::map<core::string,IAIComponentParser*> ParsersMap;

	//ParsersMap m_parsers;

public:
	AIArchitectParser();
	virtual ~AIArchitectParser();
	/*
	void RegisterParser(IAIComponentParser* parser);

	void UnregisterParser(const core::string& type);

	//is "type" component parser been registered
	bool IsParserExist(const core::string& type);

	IAIComponentParser* GetParser(const core::string& type);
*/
	//parse the element for the an architect
	bool ParseXML(IAIArchitect*arch,xml::XMLElement*elem,const core::string&path);
	bool ParseXML(IAIArchitect*arch,OS::IStream*stream);
};

}
}

#endif // AIArchitectParser_h__
