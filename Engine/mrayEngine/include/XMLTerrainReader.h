

/********************************************************************
	created:	2010/06/18
	created:	18:6:2010   20:10
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\XMLTerrainReader.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	XMLTerrainReader
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef XMLTerrainReader_h__
#define XMLTerrainReader_h__

#include "CompileConfig.h"

namespace mray
{
	class IResLoadListener;
	namespace xml
	{
		class XMLElement;
	}
	namespace OS
	{
		class IStream;
	}
namespace scene
{
	class SPatchTerrain;

class MRAY_DLL XMLTerrainReader
{
private:
protected:
public:
	XMLTerrainReader();
	virtual ~XMLTerrainReader();
	
	scene::SPatchTerrain* loadXML(OS::IStream* stream,IResLoadListener*listener=0);
	scene::SPatchTerrain* loadXML(xml::XMLElement* elem,IResLoadListener*listener=0);

};

}
}

#endif // XMLTerrainReader_h__
