#include "stdafx.h"

#include "MaterialResourceManager.h"
#include "MaterialScript.h"
#include "XMLWriter.h"
#include "ILogManager.h"
#include "XMLTree.h"
#include "StreamWriter.h"


namespace mray{

MaterialResourceManager::MaterialResourceManager()
	:IResourceManager(mT("Material Resource Manager"))
{
	new script::MaterialScript();
}
MaterialResourceManager::~MaterialResourceManager(){
	delete script::MaterialScript::getInstancePtr();
}

IResourcePtr MaterialResourceManager::createInternal(const core::string& name){
	GCPtr<video::RenderMaterial> m=new video::RenderMaterial;
	m->setResourceName(name);
	return m;
}

IResourcePtr MaterialResourceManager::loadResourceFromFile(OS::IStream* file){
	return 0;
}

core::string MaterialResourceManager::getDefaultGroup(){
	return mT("Materials");
}

video::RenderMaterialPtr MaterialResourceManager::getMaterial(const core::string&name){
	return getResource(name);
}
void MaterialResourceManager::parseMaterialScript(OS::IStream* file){
	script::MaterialScript::getInstance().loadMaterialScript(file);
}

void MaterialResourceManager::parseMaterialXML(OS::IStream* file)
{
	if (!file)
		return;
	xml::XMLTree t;
	if(!t.load(file))
	{
		gLogManager.log(mT("Failed to load Material xml file: ")+core::string(file->getStreamName()),ELL_WARNING);
		return;
	}
	xml::XMLElement*e=t.getSubElement(mT("Materials"));
	if(!e)
		return;
	parseMaterialXML(e);
	
}
void MaterialResourceManager::exportMaterialXML(const OS::IStreamPtr& file,bool onlyUsed)
{
	ResourceMap::iterator it=m_resourceMap.begin();

	xml::XMLWriter writer;
	xml::XMLElement e(mT("Materials"));
	for(;it!=m_resourceMap.end();++it)
	{
		GCPtr<video::RenderMaterial> m=it->second;
		if(onlyUsed && m.getRefCount()==1)
			continue;
		m->exportXMLSettings(&e);
	}
	writer.addElement(&e);

	OS::StreamWriter w(file);

	w.writeString(writer.flush());
}


void MaterialResourceManager::parseMaterialXML(xml::XMLElement*e)
{
	xml::xmlSubElementsMapIT it= e->getElementsBegin();
	xml::xmlSubElementsMapIT end= e->getElementsEnd();
	for(;it!=end;++it)
	{
		if((*it)->GetType()!=xml::ENT_Element)continue;
		xml::XMLElement*elem=dynamic_cast<xml::XMLElement*>(*it);
		if(elem->getName().equals_ignore_case(mT("Material")))
		{
			GCPtr<video::RenderMaterial> m;
			xml::XMLAttribute*attr=elem->getAttribute(mT("parent"));
			if(attr)
			{
				GCPtr<video::RenderMaterial> p=getResource(attr->value);
				if(p)
					m=p->Duplicate();
			}
			if(!m)
				m=new video::RenderMaterial();
			m->loadXMLSettings(elem);
			if(getResource(m->getResourceName()))
				continue;
			addResource(m,m->getResourceName());
		}
	}
}
xml::XMLElement* MaterialResourceManager::exportMaterialXML(xml::XMLElement*elem,bool onlyUsed)
{
	xml::XMLElement *e=new xml::XMLElement(mT("Materials"));
	ResourceMap::iterator it=m_resourceMap.begin();
	for(;it!=m_resourceMap.end();++it)
	{
		GCPtr<video::RenderMaterial> m=it->second;
		if(onlyUsed && m.getRefCount()==1)
			continue;
		m->exportXMLSettings(e);
	}
	elem->addSubElement(e);
	return e;
}

void MaterialResourceManager::writeResourceToDist(const core::string&resName,const core::string&fileName){
}
void MaterialResourceManager::writeResourceToDist(const IResourcePtr& resource,const core::string&fileName){
}
void MaterialResourceManager::writeResourceToDist(ResourceHandle resource,const core::string&fileName){
}


}