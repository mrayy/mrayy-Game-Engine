#include "stdafx.h"

#include "XMLTree.h"
#include "IXMLParser.h"
#include "IStream.h"
#include "IFileSystem.h"



namespace mray{
namespace xml{


XMLTree::XMLTree():XMLElement(mT("Root"))
{
}
XMLTree::~XMLTree(){
}

bool XMLTree::load(OS::IStream* stream){
	Clear();
	return IXMLParser::getInstance().parserXML(stream,this);
}
bool XMLTree::load(const core::string& path)
{
	OS::IStreamPtr s= gFileSystem.openFile(path);
	if(!s)
		return false;
	bool r=load(s);
	s->close();
	return r;
}


}
}