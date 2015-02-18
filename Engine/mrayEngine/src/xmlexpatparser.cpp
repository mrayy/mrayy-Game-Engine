#include "stdafx.h"


#include "XMLExpatParser.h"
#include "ILogManager.h"
#include "StringConverter.h"
#include "IStream.h"
#include "XMLTextNode.h"
#include "XMLComment.h"
#include "mraySystem.h"
#include "StringUtil.h"

#include "XMLTree.h"
#ifndef XML_STATIC
#define XML_STATIC
#endif
#include <expat/expat.h>


namespace mray{
namespace xml{

	struct ExpatUserData
	{
		XMLElement* node;
		core::string text;
	};

void startHandler(void* data, const XML_Char* element, const XML_Char**attr){
	//XMLTree*tree=static_cast<XMLTree*>(data);
	XMLElement*node=((ExpatUserData*)data)->node;
	FATAL_ERROR(!node,mT("XML Node is null.."));
	XMLElement* elem=new XMLElement(core::StringConverter::toString(element));
	for(int i=0;attr[i];i+=2){
		core::string n=core::StringConverter::toString(attr[i]);
		core::string v=core::StringConverter::toString(attr[i+1]);
		elem->addAttribute(n,v);
	}
	node->addSubElement(elem);
	((ExpatUserData*)data)->node=elem;
	//listner->startElementHandler(&elem);
}
void endHandler(void* data, const XML_Char* element){
	//IXMLListner*listner=static_cast<IXMLListner*>(data);
	//listner->endElementHandler(core::StringConverter::toString(element));
	XMLElement*node=((ExpatUserData*)data)->node;
	if(!node)return;

	if(((ExpatUserData*)data)->text!="")
	{
		XMLTextNode* elem=new XMLTextNode(((ExpatUserData*)data)->text);
		node->addSubElement(elem);
		((ExpatUserData*)data)->text = "";
	}
	((ExpatUserData*)data)->node=node->getParent();
}
void charDataHandler(void *data, const XML_Char *text, int len)
{
	if(len==1)return;
	core::string str;
	str.resize(len);
	mraySystem::memCopy(&str[0],text,len);
	str=core::StringUtil::Trim(str);
	if(str=="")
		return;

	((ExpatUserData*)data)->text+=str+"\n";
	/*
	IXMLListner*listner=static_cast<IXMLListner*>(data);
	core::string str;
	str.resize(len);
	mraySystem::memCopy(&str[0],data,len);
	str[len-1]=0;
	listner->texDataHandler(str);*/
}
void charCommentHandler(void *userData,
					 const XML_Char *data)
{
	XMLElement*node=((ExpatUserData*)userData)->node;
	if(!node)return;
	XMLComment* elem=new XMLComment(core::StringConverter::toString(data));
	node->addSubElement(elem);
}

XMLExpatParser::XMLExpatParser(){
}
XMLExpatParser::~XMLExpatParser(){
}

bool XMLExpatParser::parserXML(OS::IStream*stream,XMLTree*tree){
	if(!stream)return false;
	XML_Parser parser=XML_ParserCreate(0);
	ExpatUserData ud;
	ud.node=tree;
	XML_SetUserData(parser,&ud);
	XML_SetElementHandler(parser,startHandler,endHandler);
	XML_SetCharacterDataHandler(parser,charDataHandler);
	XML_SetCommentHandler(parser,charCommentHandler);

	
	std::vector<char> data;
	data.resize(stream->length()+1);

	stream->read(&data[0],data.size());
	data[data.size()-1]=0;
	//listner->onStart();
	if(!XML_Parse(parser,&data[0],data.size()-1,true)){
		data.clear();
		//listner->onDone(false);
		int line=XML_GetCurrentLineNumber(parser);
		XML_ParserFree(parser);
		gLogManager.log(core::string(mT("XMLExpatParser::parserXML()-couldn't parse \""))+stream->getStreamName()+mT("\" at line :")+core::StringConverter::toString(line),ELL_WARNING);
		return false;
	}
	//listner->onDone(true);
	data.clear();

	XML_ParserFree(parser);
	return true;
}

core::string XMLExpatParser::getParserName(){
	return mT("Expat");
}


}
}
