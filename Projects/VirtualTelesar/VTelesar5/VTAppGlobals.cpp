

#include "StdAfx.h"
#include "VTAppGlobals.h"
#include "Application.h"
#include "XMLTree.h"
#include "XMLWriter.h"




namespace mray
{
namespace VT
{



void VTAppGlobals::SetValue(const core::string&catagory,const core::string&name,const core::string& v)
{
	s_values.setPropertie(catagory,name,v);
}

core::string VTAppGlobals::GetValue(const core::string&catagory,const core::string&name)
{
	return s_values.getPropertie(catagory,name);
}
void VTAppGlobals::Load(const core::string& path)
{
	OS::IStreamPtr stream=gFileSystem.openFile(path,OS::BIN_READ);
	if(!stream)
		return;
	s_values.loadSettings(stream);
	stream->close();
	/*
	xml::XMLTree t;
	if(!t.load(path))
		return;
	xml::XMLElement* root= t.getSubElement("Settings");
	if(!root)
		return;

	xml::XMLElement* tag=root->getSubElement("Catagory");
	while(tag)
	{
		xml::XMLAttribute*attr=tag->getAttribute("Name");
		xml::xmlAttributesMapIT it= root->getAttributesBegin();
		xml::xmlAttributesMapIT end= root->getAttributesEnd();
		for (;it!=end;++it)
		{
			SetValue(attr->value,it->second.name,it->second.value);
		}
		tag=tag->nextSiblingElement("Catagory");
	}*/

}
void VTAppGlobals::Save(const core::string& path)
{
	OS::IStreamPtr stream=gFileSystem.openFile(path,OS::BIN_WRITE);
	if(!stream)
		return;
	s_values.writeSettings(stream);
	stream->close();
	/*
	xml::XMLElement root("Settings");
	std::map<core::string,core::string>::iterator it= s_values.begin();
	for(;it!=s_values.end();++it)
	{
		root.addAttribute(it->first,it->second);
	}

	xml::XMLWriter wrtr;
	wrtr.addElement(&root);

	core::string xmlTxt= wrtr.flush();

	OS::IStreamPtr stream=gFileSystem.openFile(path,OS::BIN_WRITE);
	if(!stream)
		return;
	OS::StreamWriter sw(stream);
	sw.writeString(xmlTxt);
	stream->close();*/


}

}
}

