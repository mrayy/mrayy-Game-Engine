
/********************************************************************
	created:	2009/04/05
	created:	5:4:2009   22:55
	filename: 	i:\Programing\GameEngine\mrayEngine\EaglesProject\src\XMLLevelLoader.h
	file path:	i:\Programing\GameEngine\mrayEngine\EaglesProject\src
	file base:	XMLLevelLoader
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___XMLLevelLoader___
#define ___XMLLevelLoader___

#include <IStream.h>
#include <XMLElement.h>
#include "GameLevel.h"
#include "SGameInfo.h"
#include "IResLoadListener.h"

namespace mray{
namespace gameMod{
	class SGameInfo;

class XMLLevelLoader
{
private:
protected:
	static const core::string xmlLevel;
	static const core::string xmlName;
	static const core::string xmlTerrain;
	static const core::string xmlEnvironment;
	static const core::string xmlPlayer;
	static const core::string xmlAntiPlane;
	static const core::string xmlObjects;
	static const core::string xmlObject;
	static const core::string xmlWater;

	static const core::string xmlAmbient;
	static const core::string xmlDayLength;
	static const core::string xmlFog;
	static const core::string xmlViewDist;
	static const core::string xmlSun;
	static const core::string xmlSky;

	static const core::string xmlStart;
	static const core::string xmlEnd;
	static const core::string xmlColor;

	static const core::string xmlPos;
	static const core::string xmlRot;
	static const core::string xmlScale;
	static const core::string xmlClass;
	static const core::string xmlType;
	static const core::string xmlBuilding;
	static const core::string xmlTree;

	static const core::string xmlHeightMap;
	static const core::string xmlBaseMap;
	static const core::string xmlDetailMap;
	static const core::string xmlDetailNormal;
	static const core::string xmlSize;
	static const core::string xmlNumOfLOD;
	static const core::string xmlLODDist;

	GCPtr<SGameInfo> m_info;
	GCPtr<GameLevel> m_level;
	IResLoadListener*m_listner;

	xml::XMLTree m_xtree;

	void readObjectElement(xml::XMLElement*attrs,bool info);

	void loadFog(xml::XMLElement*attrs,bool info);
	void loadSun(xml::XMLElement*attrs,bool info);
	void loadSky(xml::XMLElement*attrs,bool info);
	void loadWater(xml::XMLElement*attrs,bool info);

	void loadObject(xml::XMLElement*attrs,bool info);

	void loadEnvironment(xml::XMLElement*attrs,bool info);
	void loadPlayer(xml::XMLElement*attrs,bool info);
	void loadAntiPlane(xml::XMLElement*attrs,bool info);
	void loadObjects(xml::XMLElement*attrs,bool info);
	void loadTerrain(xml::XMLElement*attrs,bool info);
public:
	XMLLevelLoader();
	virtual~XMLLevelLoader();

	GCPtr<SGameInfo> getInfo();

	bool loadInfo(GCPtr<OS::IStream> file);

	bool parseXML(GCPtr<OS::IStream> file);

	bool loadLevel(GCPtr<GameLevel>level,IResLoadListener*listner,bool info=false);
};

}
}


#endif //___XMLLevelLoader___
