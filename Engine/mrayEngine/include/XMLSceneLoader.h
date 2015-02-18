

/********************************************************************
	created:	2009/02/18
	created:	18:2:2009   17:45
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\XMLSceneLoader.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	XMLSceneLoader
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___XMLSceneLoader___
#define ___XMLSceneLoader___

#include "mstring.h"
#include "XMLElement.h"
#include "IStream.h"

namespace mray{
	class IResLoadListener;
namespace scene{

class ISceneManager;
class ISceneNode;

class MRAY_DLL XMLSceneLoader
{

private:
	static const core::string xmlSceneStr;
	static const core::string xmlNodeStr;
	static const core::string xmlTypeStr;
	static const core::string xmlModelStr;
	static const core::string xmlNameStr;
	static const core::string xmlPositionStr;
	static const core::string xmlRotationStr;
	static const core::string xmlScaleStr;
	static const core::string xmlMaterialStr;
	static const core::string xmlViewDistStr;
	static const core::string xmlParametersStr;
	static const core::string xmlChildrenStr;
protected:

	ISceneManager*m_manager;

	ISceneNode* readNodeElem(xml::XMLElement*attrs,ISceneNode*parent);

	int m_currState;

	IResLoadListener* m_listener;

public:

	XMLSceneLoader(ISceneManager*manager);


	bool loadXML(xml::XMLElement*elem,ISceneNode*parent=0,IResLoadListener*listener=0);
	bool loadXML(OS::IStream* stream,ISceneNode*parent=0,IResLoadListener*listener=0);

};

}
}


#endif //___XMLSceneLoader___



