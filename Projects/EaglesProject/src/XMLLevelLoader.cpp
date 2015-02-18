
#include "XMLLevelLoader.h"
#include "EnvironmentManager.h"
#include <LogManager.h>
#include <XMLTree.h>
#include <SPatchTerrain.h>
#include <HeightMapRaw16.h>
#include <IFileSystem.h>
#include <TextureResourceManager.h>
#include "SGameInfo.h"

namespace mray{
namespace gameMod{

const core::string XMLLevelLoader::xmlLevel=mT("level");
const core::string XMLLevelLoader::xmlBuilding=mT("building");
const core::string XMLLevelLoader::xmlTerrain=mT("terrain");
const core::string XMLLevelLoader::xmlEnvironment=mT("environment");
const core::string XMLLevelLoader::xmlPlayer=mT("player");
const core::string XMLLevelLoader::xmlObjects=mT("objects");
const core::string XMLLevelLoader::xmlObject=mT("object");
const core::string XMLLevelLoader::xmlAntiPlane=mT("AntiPlane");
const core::string XMLLevelLoader::xmlWater=mT("water");

const core::string XMLLevelLoader::xmlAmbient=mT("ambient");
const core::string XMLLevelLoader::xmlDayLength=mT("daylength");
const core::string XMLLevelLoader::xmlFog=mT("fog");
const core::string XMLLevelLoader::xmlViewDist=mT("viewdist");
const core::string XMLLevelLoader::xmlSun=mT("sun");
const core::string XMLLevelLoader::xmlSky=mT("sky");

const core::string XMLLevelLoader::xmlStart=mT("start");
const core::string XMLLevelLoader::xmlEnd=mT("end");
const core::string XMLLevelLoader::xmlColor=mT("color");

const core::string XMLLevelLoader::xmlClass=mT("class");
const core::string XMLLevelLoader::xmlPos=mT("pos");
const core::string XMLLevelLoader::xmlRot=mT("rot");
const core::string XMLLevelLoader::xmlScale=mT("scale");
const core::string XMLLevelLoader::xmlType=mT("type");
const core::string XMLLevelLoader::xmlTree=mT("tree");
const core::string XMLLevelLoader::xmlName=mT("name");

const core::string XMLLevelLoader::xmlHeightMap=mT("heightmap");
const core::string XMLLevelLoader::xmlBaseMap=mT("basemap");
const core::string XMLLevelLoader::xmlDetailMap=mT("detailmap");
const core::string XMLLevelLoader::xmlDetailNormal=mT("detailnorm");
const core::string XMLLevelLoader::xmlSize	=mT("size");
const core::string XMLLevelLoader::xmlNumOfLOD=mT("numoflod");
const core::string XMLLevelLoader::xmlLODDist=mT("loddist");


XMLLevelLoader::XMLLevelLoader(){
	m_info=new SGameInfo();
}
XMLLevelLoader::~XMLLevelLoader(){
}


void XMLLevelLoader::loadFog(xml::XMLElement*attrs,bool info){
	if(info)
		return;

	xml::xmlAttributesMapIT it=attrs->getAttributesBegin();
	xml::xmlAttributesMapIT end= attrs->getAttributesEnd();
	FogAttributes fog;


	for(;it!=end;++it){
		xml::XMLAttribute&elem=it->second;
		if(elem.name.equals_ignore_case(xmlStart)){
			fog.start=core::StringConverter::toFloat(elem.value);
		}else if(elem.name.equals_ignore_case(xmlEnd)){
			fog.end=core::StringConverter::toFloat(elem.value);
		}else if(elem.name.equals_ignore_case(xmlColor)){
			fog.clr=core::StringConverter::toColor(elem.value);
		}
	}
	EnvironmentManager::getInstance().setFogAttributes(fog.start,fog.end,fog.clr);
}

void XMLLevelLoader::loadSun(xml::XMLElement*attrs,bool info){
	if(info)
		return;

	xml::xmlAttributesMapIT it=attrs->getAttributesBegin();
	xml::xmlAttributesMapIT end= attrs->getAttributesEnd();

	for(;it!=end;++it){
		xml::XMLAttribute&elem=it->second;
	}
}
void XMLLevelLoader::loadWater(xml::XMLElement*attrs,bool info){
	if(info)
		return;

	xml::xmlAttributesMapIT it=attrs->getAttributesBegin();
	xml::xmlAttributesMapIT end= attrs->getAttributesEnd();

	float height=0;
	int detail=1;
	math::vector2d size=100;

	m_listner ->onResourceLoadStart(mT("water"));
	for(;it!=end;++it){
		xml::XMLAttribute&elem=it->second;
		if(elem.name.equals_ignore_case(mT("height")))
		{
			height=core::StringConverter::toFloat(elem.value);
		}else if(elem.name.equals_ignore_case(mT("detail")))
		{
			detail=core::StringConverter::toInt(elem.value);
		}else if(elem.name.equals_ignore_case(mT("size")))
		{
			size=core::StringConverter::toVector2d(elem.value);
		}
	}
	m_level->createWaterSurface(height,detail,size);
	m_listner ->onResourceLoadDone(mT("water"));

}
void XMLLevelLoader::loadSky(xml::XMLElement*attrs,bool info){
	if(info)
		return;

	if(!info && m_listner){
		m_listner ->onResourceLoadStart(mT("SkyBox"));
	}

	xml::xmlAttributesMapIT it=attrs->getAttributesBegin();
	xml::xmlAttributesMapIT end= attrs->getAttributesEnd();

	core::string name;
	core::string path;

	for(;it!=end;++it){
		xml::XMLAttribute&elem=it->second;
		if (elem.name==mT("name"))
		{
			name=elem.value;
		}else if (elem.name==mT("path"))
		{
			path=elem.value;
		}
	}
	EnvironmentManager::getInstance().loadSkyBox(path,name);
	if(m_listner && !info){
		m_level->addSceneNode(EnvironmentManager::getInstance().getSky());
		m_listner ->onResourceLoadDone(mT("SkyBox"));
	}
}

void XMLLevelLoader::loadEnvironment(xml::XMLElement*attrs,bool info){
	if(!info && m_listner){
		m_listner ->onResourceLoadStart(mT("Environment"));
	}
	{

		xml::xmlAttributesMapIT it=attrs->getAttributesBegin();
		xml::xmlAttributesMapIT end= attrs->getAttributesEnd();
		for(;it!=end;++it){
			xml::XMLAttribute&elem=it->second;
			if(elem.name.equals_ignore_case(xmlAmbient)){
				EnvironmentManager::getInstance().setAmbientColor(core::StringConverter::toColor(elem.value));
			}else if(elem.name.equals_ignore_case(xmlDayLength)){
				EnvironmentManager::getInstance().setDayLong(core::StringConverter::toFloat(elem.value));
			}else if(elem.name.equals_ignore_case(xmlViewDist)){
				EnvironmentManager::getInstance().setViewDistance(core::StringConverter::toFloat(elem.value));
			}
		}
	}
	{

		xml::xmlSubElementsMapIT it=attrs->getElementsBegin();
		xml::xmlSubElementsMapIT end= attrs->getElementsEnd();
		for(;it!=end;++it){
			xml::XMLElement*elem=*it;
			if(elem->getName().equals_ignore_case(xmlFog)){
				loadFog(elem,info);
			}else if(elem->getName().equals_ignore_case(xmlSun)){
				loadSun(elem,info);
			} else if(elem->getName().equals_ignore_case(xmlSky)){
				loadSky(elem,info);
			} 
		}
	}

	if(m_listner && !info){
		m_listner ->onResourceLoadDone(mT("Environment"));
	}
}
void XMLLevelLoader::loadPlayer(xml::XMLElement*attrs,bool info){
	if(!info && m_listner){
		m_listner ->onResourceLoadStart(mT("Player"));
	}
	core::string type;
	core::string missile;
	math::vector3d pos;
	math::vector3d rot;
	math::vector3d scale;
	{
		xml::xmlAttributesMapIT it=attrs->getAttributesBegin();
		xml::xmlAttributesMapIT end= attrs->getAttributesEnd();
		for(;it!=end;++it){
			xml::XMLAttribute&elem=it->second;
			if(elem.name.equals_ignore_case(xmlType)){
				type=elem.value;
			}else if(elem.name.equals_ignore_case(mT("missile"))){
				missile=elem.value;
			}else  if (elem.name.equals_ignore_case(xmlPos)){
				pos=core::StringConverter::toVector3d(elem.value);
			}else if (elem.name.equals_ignore_case(xmlRot)){
				rot=core::StringConverter::toVector3d(elem.value);
			}else if (elem.name.equals_ignore_case(xmlScale)){
				scale=core::StringConverter::toVector3d(elem.value);
			}
		}
	}
	if(m_listner && !info){
		m_level->setPlayer(type,missile,pos,rot,scale);
		m_listner ->onResourceLoadDone(mT("Player"));
	}
}
void XMLLevelLoader::loadAntiPlane(xml::XMLElement*attrs,bool info){
	if(!info && m_listner){
		m_listner ->onResourceLoadStart(mT("AntiPlane"));
	}
	core::string type;
	core::string missile;
	math::vector3d pos;
	math::vector3d rot;
	math::vector3d scale;
	{
		xml::xmlAttributesMapIT it=attrs->getAttributesBegin();
		xml::xmlAttributesMapIT end= attrs->getAttributesEnd();
		for(;it!=end;++it){
			xml::XMLAttribute&elem=it->second;
			if(elem.name.equals_ignore_case(xmlType)){
				type=elem.value;
			}else if(elem.name.equals_ignore_case(mT("missile"))){
				missile=elem.value;
			}else  if (elem.name.equals_ignore_case(xmlPos)){
				pos=core::StringConverter::toVector3d(elem.value);
			}else if (elem.name.equals_ignore_case(xmlRot)){
				rot=core::StringConverter::toVector3d(elem.value);
			}else if (elem.name.equals_ignore_case(xmlScale)){
				scale=core::StringConverter::toVector3d(elem.value);
			}
		}
	}
	if(m_listner && !info){
		m_level->addAntiPlane(type,missile,pos,rot,scale);
		m_listner ->onResourceLoadDone(mT("AntiPlane"));
	}
}
void XMLLevelLoader::loadObject(xml::XMLElement*attrs,bool info){
	{
		core::string cl;
		int type=0;
		math::vector3d pos;
		math::vector3d rot;
		math::vector3d scale;
		xml::xmlAttributesMapIT it=attrs->getAttributesBegin();
		xml::xmlAttributesMapIT end= attrs->getAttributesEnd();



		for(;it!=end;++it){
			xml::XMLAttribute&elem=it->second;
			if(elem.name.equals_ignore_case(xmlClass)){
				cl=elem.value;
			}else if (elem.name.equals_ignore_case(xmlType)){
				type=core::StringConverter::toInt(elem.value);
			}else if (elem.name.equals_ignore_case(xmlPos)){
				pos=core::StringConverter::toVector3d(elem.value);
			}else if (elem.name.equals_ignore_case(xmlRot)){
				rot=core::StringConverter::toVector3d(elem.value);
			}else if (elem.name.equals_ignore_case(xmlScale)){
				scale=core::StringConverter::toVector3d(elem.value);
			}
		}
		if(!info){
			core::string resName=mT("Object:")+cl+type;
			if(m_listner){
				m_listner ->onResourceLoadStart(resName);
			}
			m_level->addEntity(cl,type,pos,rot,scale);
			 if(m_listner){
				 m_listner ->onResourceLoadDone(resName);
			}
		}
	}
}
void XMLLevelLoader::loadObjects(xml::XMLElement*attrs,bool info){
	{

		xml::xmlSubElementsMapIT it=attrs->getElementsBegin();
		xml::xmlSubElementsMapIT end= attrs->getElementsEnd();
		for(;it!=end;++it){
			xml::XMLElement*elem=*it;
			if(elem->getName().equals_ignore_case(xmlObject)){
				if(!info){
					loadObject(elem,info);
				}else{
					m_info->objectsCount++;
				}
			}
		}
	}
}
void XMLLevelLoader::loadTerrain(xml::XMLElement*attrs,bool info){
	{
		core::string heightMap;
		core::string baseMap;
		core::string detailMap;
		core::string detailNorm;
		math::vector3d size=1;
		float numOfLod=0;
		float lodDist=0;

		if(!info && m_listner){
			m_listner ->onResourceLoadStart(mT("Terrain"));
		}

		xml::xmlAttributesMapIT it=attrs->getAttributesBegin();
		xml::xmlAttributesMapIT end= attrs->getAttributesEnd();
		for(;it!=end;++it){
			xml::XMLAttribute&elem=it->second;
			if(elem.name.equals_ignore_case(xmlHeightMap)){
				heightMap=elem.value;
				if(info){
					m_info->terrainHM=heightMap;
				}
			}else if(elem.name.equals_ignore_case(xmlBaseMap)){
				baseMap=elem.value;
			}else if(elem.name.equals_ignore_case(xmlDetailMap)){
				detailMap=elem.value;
			}else  if(elem.name.equals_ignore_case(xmlDetailNormal)){
				detailNorm=elem.value;
			}else if(elem.name.equals_ignore_case(xmlSize)){
				size=core::StringConverter::toVector3d(elem.value);
			}else if(elem.name.equals_ignore_case(xmlNumOfLOD)){
				numOfLod=core::StringConverter::toInt(elem.value);
			}else if(elem.name.equals_ignore_case(xmlLODDist)){
				lodDist=core::StringConverter::toFloat(elem.value);
			} 
		}
		if(!info){
			GCPtr<scene::SPatchTerrain> terrain;
			GCPtr<loaders::HeightMapRaw16>l=new loaders::HeightMapRaw16();
			l->loadHeightMap(gFileSystem.openFile(heightMap));
			terrain=new scene::SPatchTerrain(0,0,math::quaternion::Identity,size);
			terrain->loadTerrain(l,numOfLod,lodDist);
			terrain->setBaseTexture(gTextureResourceManager.loadTexture2D(baseMap));
			terrain->setDetailTexture(gTextureResourceManager.loadTexture2D(detailMap));
			terrain->setDetailNormal(gTextureResourceManager.loadTexture2D(detailNorm));
			m_level->setTerrain(terrain);
			if(m_listner){
				m_listner ->onResourceLoadDone(mT("Terrain"));
			}
		}
	}
}

bool XMLLevelLoader::loadInfo(GCPtr<OS::IStream> file){
	if(!parseXML(file)){
		return 0;
	}
	bool res=loadLevel(0,0,true);
	return res;
}

bool XMLLevelLoader::parseXML(GCPtr<OS::IStream> file){
	if(!file)
		return 0;
	if(!m_xtree.load(file)){
		gLogManager.log(mT("Couldn't parse XML Level File :\"")+core::string(file->getStreamName())+mT("\""),ELL_WARNING);
		return false;
	}
	return true;
}

bool XMLLevelLoader::loadLevel(GCPtr<GameLevel>level,IResLoadListener*listner,bool info){

	xml::XMLElement *MainElem= m_xtree.getSubElement(xmlLevel);

	if(!MainElem){
		gLogManager.log(mT("Couldn't Find Level Tree !"),ELL_WARNING);
		return 0;
	}

	m_listner=listner;
	if(m_listner && !info){
		m_listner->beginLoadResources(m_info->objectsCount+2);	//+2 for terrain and environment
	}

	m_level=level;

	{

		xml::xmlAttributesMapIT it=MainElem->getAttributesBegin();
		xml::xmlAttributesMapIT end= MainElem->getAttributesEnd();
		for(;it!=end;++it){
			xml::XMLAttribute&elem=it->second;
			if(elem.name.equals_ignore_case(xmlName)){
				if(!info)
					level->setName(elem.value);
				else m_info->name=elem.value;
			}else if(elem.name.equals_ignore_case(mT("BoundingBox"))){
				math::box3d bb=core::StringConverter::toBox3d(elem.value);
				if(!info)
					level->setLevelBoundingBox(bb);
			}
		}
	}
	{

		xml::xmlSubElementsMapIT it=MainElem->getElementsBegin();
		xml::xmlSubElementsMapIT end= MainElem->getElementsEnd();
		for(;it!=end;++it){
			xml::XMLElement*elem=*it;
			if(elem->getName().equals_ignore_case(xmlEnvironment)){
				loadEnvironment(elem,info);
			}else if(elem->getName().equals_ignore_case(xmlPlayer)){
				loadPlayer(elem,info);
			}else if(elem->getName().equals_ignore_case(xmlTerrain)){
				loadTerrain(elem,info);
			}else if(elem->getName().equals_ignore_case(xmlObjects)){
				loadObjects(elem,info);
			}else if(elem->getName().equals_ignore_case(xmlWater)){
				loadWater(elem,info);
			}else if(elem->getName().equals_ignore_case(xmlAntiPlane)){
				loadAntiPlane(elem,info);
			}
		}
	}
	if(m_listner && !info){
		m_listner ->loadDone();
	}

	m_level=0;

	return true;
}

GCPtr<SGameInfo> XMLLevelLoader::getInfo(){
	return m_info;
}

}
}

