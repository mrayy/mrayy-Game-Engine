#include "stdafx.h"

#include "XMLTerrainReader.h"
#include "IResLoadListener.h"
#include "XMLElement.h"
#include "IStream.h"
#include "ILogManager.h"
#include "IFileSystem.h"
#include "SPatchTerrain.h"

#include "HeightMapRaw16.h"
#include "HeightMapRaw8.h"
#include "HeightMapTexture.h"

#include "TextureResourceManager.h"
#include "XMLTree.h"

namespace mray
{
namespace scene
{

XMLTerrainReader::XMLTerrainReader()
{
}
XMLTerrainReader::~XMLTerrainReader()
{
}

scene::SPatchTerrain* XMLTerrainReader::loadXML(OS::IStream* stream,IResLoadListener*listener)
{
	xml::XMLTree xmlTree;
	if(!xmlTree.load(stream)){
		gLogManager.log(mT("Couldn't parse XML Terrain File :\"")+core::string(stream->getStreamName())+mT("\""),ELL_WARNING);
		return 0;
	}

	xml::XMLElement*e= xmlTree.getSubElement(mT("SPatchTerrain"));
	if(!e)
		return 0;

	return loadXML(e,listener);
}

scene::SPatchTerrain* XMLTerrainReader::loadXML(xml::XMLElement* e,IResLoadListener*listener)
{

	if(listener)
		listener->onResourceLoadStart(mT("Terrain"));

	int lod=5;
	float lodDist=1;
	core::string heightMap;

	xml::XMLAttribute*attr=0;
	attr=e->getAttribute(mT("HeightMap"));
	if(!attr)
	{
		gLogManager.log(mT("Couldn't find \"HeightMap\" tag in terrain XML file"),ELL_WARNING);
		return 0;
	}
	heightMap=attr->value;
	core::string ext;
	int i=heightMap.findlast('.');
	if(i!=-1)
	{
		ext=heightMap.substr(i+1,heightMap.length()-i);
	}
	loaders::IHeightMapLoader* hmLoader=0;

	if(ext==mT("r8"))
	{
		loaders::HeightMapRaw8*hm=new loaders::HeightMapRaw8();
		hm->loadHeightMap(gFileSystem.openFile(heightMap,OS::BIN_READ));
		hmLoader=hm;
	}else if(ext==mT("r16"))
	{
		loaders::HeightMapRaw16*hm=new loaders::HeightMapRaw16();
		hm->loadHeightMap(gFileSystem.openFile(heightMap,OS::BIN_READ));
		hmLoader=hm;
	}else
	{
		video::ITexturePtr  tex=gTextureResourceManager.loadTexture2D(heightMap);
		if(!tex)
		{
			gLogManager.log(mT("Couldn't open height map file"),ELL_WARNING);
			return 0;
		}
		loaders::HeightMapTexture*hm=new loaders::HeightMapTexture();
		hm->setTexture(tex);
		hmLoader=hm;
	}
	attr=e->getAttribute(mT("LODCount"));
	if(attr)
	{
		lod=core::StringConverter::toInt(attr->value);
	}
	attr=e->getAttribute(mT("LODDist"));
	if(attr)
	{
		lodDist=core::StringConverter::toFloat(attr->value);
	}

/*
	xml::xmlSubElementsMapIT it=e->getElementsBegin();
	xml::xmlSubElementsMapIT end= e->getElementsEnd();
	for(;it!=end;++it){
		xml::XMLElement*elem=*it;
	}*/

	scene::SPatchTerrain* terr=new SPatchTerrain(0,1);
	terr->loadXMLSettings(e);
	terr->loadTerrain(hmLoader,lod,lodDist);


	if(listener)
		listener->onResourceLoadDone(mT("Terrain"));

	return terr;
}



}
}
