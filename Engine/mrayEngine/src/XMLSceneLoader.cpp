#include "stdafx.h"

#include "XMLSceneLoader.h"
#include "ISceneManager.h"
#include "XMLTree.h"
#include "ILogManager.h"
#include "IStream.h"
#include "TraceManager.h"
#include "MaterialResourceManager.h"
#include "IResLoadListener.h"
#include "IFileSystem.h"


namespace mray{
namespace scene{

const core::string XMLSceneLoader::xmlSceneStr(mT("scene"));
const core::string XMLSceneLoader::xmlNodeStr(mT("ISceneNode"));
const core::string XMLSceneLoader::xmlTypeStr(mT("type"));
const core::string XMLSceneLoader::xmlModelStr(mT("model"));
const core::string XMLSceneLoader::xmlNameStr(mT("name"));
const core::string XMLSceneLoader::xmlPositionStr(mT("position"));
const core::string XMLSceneLoader::xmlRotationStr(mT("rotation"));
const core::string XMLSceneLoader::xmlScaleStr(mT("scale"));
const core::string XMLSceneLoader::xmlMaterialStr(mT("material"));
const core::string XMLSceneLoader::xmlViewDistStr(mT("viewdist"));
const core::string XMLSceneLoader::xmlParametersStr(mT("parameters"));
const core::string XMLSceneLoader::xmlChildrenStr(mT("children"));



XMLSceneLoader::XMLSceneLoader(ISceneManager*manager){
	m_manager=manager;
	m_listener=0;
}


bool XMLSceneLoader::loadXML(OS::IStream* stream,ISceneNode*parent,IResLoadListener*listener){
	xml::XMLTree xmlTree;
	if(!xmlTree.load(stream)){
		gLogManager.log(mT("Couldn't parse XML Scene File :\"")+core::string(stream->getStreamName())+mT("\""),ELL_WARNING);
		return false;
	}
	m_listener=listener;

	xml::XMLElement*e= xmlTree.getSubElement(xmlSceneStr);
	if(!e)
		return false;

	return loadXML(e,parent,m_listener);
}
bool XMLSceneLoader::loadXML(xml::XMLElement*e,ISceneNode*parent,IResLoadListener*listener)
{
	m_listener=listener;

	if(m_listener)
		m_listener->beginLoadResources(mT("Scene"),e->getSubElementsCount());

	xml::xmlSubElementsMapIT it=e->getElementsBegin();
	xml::xmlSubElementsMapIT end= e->getElementsEnd();
	for(;it!=end;++it){
		if((*it)->GetType()!=xml::ENT_Element)continue;
		xml::XMLElement* elem=dynamic_cast<xml::XMLElement*>(*it);
		if(elem->getName().equals_ignore_case(xmlNodeStr))
			readNodeElem(elem,parent);
	}

	return true;
}

ISceneNode* XMLSceneLoader::readNodeElem(xml::XMLElement*attrs,ISceneNode*parent){

	GCPtr<ISceneNode>node;

	core::string name;
	core::string model;
	core::string material;

	math::vector3d pos;
	math::vector3d rot;
	math::vector3d scale;

	float viewDist=1000;
	xml::XMLAttribute*typeAttr;
	bool isCustomLoad=false;
	{
		typeAttr=attrs->getAttribute(xmlTypeStr);
		if(!typeAttr)
			return 0;

		if(m_listener)
			m_listener->onResourceLoadStart(typeAttr->value);

//		node=m_manager->getFactory()->createNode(typeAttr->value.c_str());
		if(!node)
			return 0;
		if(parent)
		{
			parent->addChild(node);
		}
/*
		if(typeAttr->value.equals_ignore_case(mT("AnimatedMesh")))
		{
			nodeReader.loadModel((scene::SLODAnimatedMesh*)node.pointer(),attrs);
			isCustomLoad=true;
		}*/
		if(!isCustomLoad)
		{
			node->loadXMLSettings(attrs);
			/*
			xml::XMLAttribute*attr=attrs->getAttribute(mT("Model"));
			if(attr){
				xml::XMLTree t;
				OS::IStreamPtr stream=gFileSystem.openFile(attr->value);
				if(stream){
					if(t.load(stream)){
						xml::XMLElement*e=t.getSubElement(mT("ISceneNode"));
						if(e)
							node->loadXMLSettings(e);
					}
				}
			}*/
			/*
			xml::xmlAttributesMapIT it=attrs->getAttributesBegin();
			xml::xmlAttributesMapIT end= attrs->getAttributesEnd();
			for(;it!=end;++it){
				xml::XMLAttribute&attr=it->second;
				if(attr.name.equals_ignore_case(xmlNameStr)){
					name=attr.value;
				}else if(attr.name.equals_ignore_case(xmlModelStr)){
					model=attr.value;
				}
				else if(attr.name.equals_ignore_case(xmlPositionStr)){
					core::StringConverter::parse(attr.value,pos);
				}else if(attr.name.equals_ignore_case(xmlRotationStr)){
					core::StringConverter::parse(attr.value,rot);
				}else if(attr.name.equals_ignore_case(xmlScaleStr)){
					core::StringConverter::parse(attr.value,scale);
				}else if(attr.name.equals_ignore_case(xmlViewDistStr)){
					core::StringConverter::parse(attr.value,viewDist);
				}else if(attr.name.equals_ignore_case(xmlMaterialStr)){
					material=attr.value;
				}
			}*/
		}
		m_manager->addSceneNode(node);
	}
/*
	if(!isCustomLoad)
	{
		xml::xmlSubElementsMapIT it=attrs->getElementsBegin();
		xml::xmlSubElementsMapIT end= attrs->getElementsEnd();
		for(;it!=end;++it){
			xml::XMLElement*elem=*it;
			if(elem->getName().equals_ignore_case(xmlParametersStr)){
				params=elem;
			}else if(elem->getName().equals_ignore_case(xmlNodeStr)){
				ISceneNode*n=readNodeElem(elem,node);
				if(n)
					node->addChild(n);
			} 
		}
		node->Name=name;
		node->setPosition(pos);
		math::quaternion q(rot.x,math::vector3d::XAxis);
		q*=math::quaternion(rot.y,math::vector3d::YAxis);
		q*=math::quaternion(rot.z,math::vector3d::ZAxis);
		node->setOrintation(q);
		node->setScale(scale);

		if(gMaterialResourceManager.isResourceExists(material)){
			node->setMaterial(gMaterialResourceManager.getMaterial(material));
		}

	}*/
	if(m_listener)
		m_listener->onResourceLoadDone(typeAttr->value);
	return node;
}


}
}
