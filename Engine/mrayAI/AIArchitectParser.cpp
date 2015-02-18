#include "stdafx.h"

#include "AIArchitectParser.h"

#include <XMLTree.h>
#include <ILogManager.h>
#include <IStream.h>
#include <IFileSystem.h>


namespace mray
{
namespace AI
{


AIArchitectParser::AIArchitectParser()
{
}

AIArchitectParser::~AIArchitectParser()
{/*
	ParsersMap::iterator it= m_parsers.begin();
	for (;it!=m_parsers.end();++it)
	{
		delete it->second;
	}*/
}
/*

void AIArchitectParser::RegisterParser(IAIComponentParser* parser)
{
	if(m_parsers[parser->GetType()]!=0)
	{
		delete m_parsers[parser->GetType()];
	}
	m_parsers[parser->GetType()]=parser;
}


void AIArchitectParser::UnregisterParser(const core::string& type)
{
	ParsersMap::iterator it= m_parsers.find(type);
	if(it!=m_parsers.end()){
		delete it->second;
		m_parsers.erase(it);
	}
}

bool AIArchitectParser::IsParserExist(const core::string& type)
{
	ParsersMap::iterator it= m_parsers.find(type);
	return it!=m_parsers.end();
}


IAIComponentParser* AIArchitectParser::GetParser(const core::string& type)
{
	ParsersMap::iterator it= m_parsers.find(type);
	if(it!=m_parsers.end())
		return it->second;
	return 0;
}*/


bool AIArchitectParser::ParseXML(IAIArchitect*arch,xml::XMLElement*elem,const core::string&path)
{
	xml::XMLAttribute*attr;
	xml::xmlSubElementsMapIT it= elem->getElementsBegin();
	xml::xmlSubElementsMapIT end= elem->getElementsEnd();
	for (;it!=end;++it)
	{
		if((*it)->GetType()!=xml::ENT_Element)continue;
		xml::XMLElement* e=dynamic_cast<xml::XMLElement*>(*it);
		std::list<IObjectComponent*> comps= arch->GetComponent(e->getName());
		//IAIComponent*c= arch->GetComponent(e->getName());
		if(comps.size()==0)
			continue;

		IAIComponent*c=0;
		std::list<IObjectComponent*>::iterator oit=comps.begin();
		for(;oit!=comps.end();++oit)
		{
			c=dynamic_cast<IAIComponent*>(*oit);
			if(c)
				c->loadXMLSettings(e);
		}

		//Look for additional definition file
		attr=e->getAttribute(mT("DefinitionFile"));
		if(attr)
		{
			//Load the definition file
			GCPtr<OS::IStream> stream=gFileSystem.openFile(path+attr->value,OS::TXT_READ);
			if(stream){

				xml::XMLTree xmlTree;
				if(!xmlTree.load(stream)){
					stream->close();
					gLogManager.log(core::string(mT("Couldn't parse XML Component Definition File :\""))+stream->getStreamName()+mT("\""),ELL_WARNING);
					continue;
				}
				stream->close();

				e=xmlTree.getSubElement(mT("Definition"));
				if(!e){
					gLogManager.log(core::string(mT("Cann't find [Definition] tage in the Definition File :\""))+stream->getStreamName()+mT("\""),ELL_WARNING);
					continue;
				}
				c->loadXMLSettings(e);
			}
		}

	}
	return true;
}

bool AIArchitectParser::ParseXML(IAIArchitect*arch,OS::IStream*stream)
{
	xml::XMLTree xmlTree;
	if(!xmlTree.load(stream)){
		gLogManager.log(core::string(mT("Couldn't parse XML AI Architecture File :\""))+stream->getStreamName()+mT("\""),ELL_WARNING);
		return 0;
	}

	xml::XMLElement*e=xmlTree.getSubElement(mT("AIArchitect"));

	if(!e){
		gLogManager.log(core::string(mT("cann't find AIArchitect tag :\""))+stream->getStreamName()+mT("\""),ELL_WARNING);
		return 0;
	}

	core::string path=stream->getStreamName();
	int i=path.findlast('\\');
	if(i!=-1)
	{
		path=path.substr(0,i);
	}else
		path=mT("");
	

	return ParseXML(arch,e,path);
}


}
}
