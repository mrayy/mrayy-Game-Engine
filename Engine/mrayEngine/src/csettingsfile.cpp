#include "stdafx.h"


#include "CSettingsFile.h"
#include "ILexical.h"
#include "ILogManager.h"
#include "StringConverter.h"
#include "CTokenTranslator.h"
#include "TraceManager.h"
#include "common.h"
#include "StreamWriter.h"
#include "IStream.h"

namespace mray{
namespace script{


TokenDefine settingTokenDefineTable[]=
{
	TokenDefine(mT("\""),Token_Quote),
	TokenDefine(mT(":"),Token_Colon),
	TokenDefine(mT("{"),Token_Open_Brace),
	TokenDefine(mT("}"),Token_Closed_Brace),

	TokenDefine::EndDefineToken

};
	
CSettingsFile::CSettingsFile(OS::IStream* stream)
{
	loadSettings(stream);
}

CSettingsFile::CSettingsFile(){
	
}

CSettingsFile::~CSettingsFile(){
	clear();
}
bool CSettingsFile::loadSettings(OS::IStream* stream){
	if(!stream)return 0;

	traceFunction(eScene);

	GCPtr<script::ILexical>parser=new script::ILexical();
	GCPtr<script::CTokenList> tokens=new script::CTokenList();
	parser->loadFromStream(stream);
	parser->setTokenDefinitionTable(settingTokenDefineTable,false,4);
	
	parser->setDelimChars(mT("\t\n\r {}\""));

	script::CTokenTranslator translater;

	tokens->fromLexical(parser,&translater,true,true,mT("#"));
	static  mchar buffer[512];
	static char msg[256];

	core::string attrName;
	core::string attrValue;
	SConfigTag *tag;
	enum EStatus{
		ReadTag,
		OpenBrace,
		CloseBrace,
		Attribute
	};
	EStatus state=ReadTag;
	bool done=false;
	ScriptToken* token;
	if(!tokens->isDone()){
		do{
			if(tokens->isDone()){
			//	done=true;
				break;
			}
			
			token=tokens->getNextToken();
			if(token==0 || token->token==ERT_EOF)
				break;
			switch(state){
				case ReadTag:
				{
					tag=addTag(token->lex);
					state=OpenBrace;
				}break;
				case OpenBrace:
				{
					if(token->token!=Token_Open_Brace){
						gLogManager.log(mT("CSettingsFile::loadSettings()")
							,mT("Expected \"{\" after the tag[")+tag->tagName+mT("]!"),ELL_ERROR);
					}else
						state=Attribute;
				}break;
				case Attribute:
				{
					if(tag==0){
						gLogManager.log(mT("CSettingsFile::loadSettings()")
							,mT("No tag while reading attributes!"),ELL_ERROR);
					}
					if(token->token==Token_Closed_Brace){
						tag=0;
						state=ReadTag;
					}
					else
					{
						attrName = token->lex;
						if(tokens->isDone())
						{
							done=1;
							break;
						}
						attrValue=tokens->getNextToken()->lex;
						tag->addAttr(attrName,attrValue);
					}
				}break;
			}
		}while(!done);

		if(state!=ReadTag)
			gLogManager.log(mT("CSettingsFile::loadSettings()")
				,mT("Setting file has wrong syntax!"),ELL_ERROR);
	}
	
	parser=0;
	return true;

}
/*
bool CSettingsFile::loadSettings(OS::IStreamPtrstream){
	if(!stream)return 0;

	traceFunction();

	GCPtr<script::CStreamLexical>parser=new script::CStreamLexical(stream);
	GCPtr<script::CTokenList> tokens=new script::CTokenList();
	
	script::CTokenTranslator translater;

	tokens->fromLexical(parser,&translater);
	static  mchar buffer[512];
	static char msg[256];

	core::string attrName;
	SConfigTag *tag;
	while(!tokens->isDone()){
		tag=new SConfigTag();
		const core::string&tmp=tokens->getNextTokenStr();
		if(!tmp || strcmp(tmp,"")==0){
			
			return 1;
		}
		tag->tagName=tmp;
		tmp=tokens->getNextTokenStr();
		if(strcmp(tmp,"{")!=0){
			delete tag;
			sprintf(msg,"Expected \"{\" after the tag[%s]!",tag->tagName.c_str());
			gLogManager.log("CSettingsFile",msg,ELL_ERROR);
			
			return false;
		}
		while(!tokens->isDone()){
			tmp=tokens->getNextTokenStr();
			if(!tmp || strcmp(tmp,"")==0){
				sprintf(msg,"Expected \"}\" for tag[%s]!",tag->tagName.c_str());
				gLogManager.log("CSettingsFile",msg,ELL_ERROR);
				
				return false;
			}
			if(strcmp(tmp,"}")==0)break;

			attrName = tmp;
			core::string value=tokens->getNextTokenStr();
			tag->addAttr(attrName.c_str(),core::StringConverter::toString(value.c_str()));
		}
		m_Tags.push_back(tag);

	}

	
	parser=0;
	return true;

}
*/

			/*
			core::string token=parser->getNextWord();
			
			if(token!="\""){
				sprintf(msg,"Expected \'\"\' for Propertie Value [%s]!",attrName.c_str());
				gLogManager.log("CSettingsFile",msg,ELL_WARNING);
				return false;
			}
			core::string value="";
			token="";
			token=core::StringConverter::toString(parser->getChar());
			while(!parser->eof() && token!="\""){
				value+=token;
				token=core::StringConverter::toString(parser->getChar());
			}

			tag->m_attrSet.insert(std::make_pair(std::string(attrName.c_str()),
				CAttribute<core::string>(attrName.c_str(),"string",mT(""),core::StringConverter::toString(value.c_str()))));
				*/

void CSettingsFile::writeSettings(OS::IStream* file){

	OS::StreamWriter writer(file);

	TagMap::iterator itag=m_Tags.begin();
	for(;itag!=m_Tags.end();++itag)
	{
		writer.writeString(itag->second->tagName.c_str());
		writer.writeString(mT("{\n"));
		SConfigTag::AttributesIT it=itag->second->m_attrSet.begin();
		for(;it!=itag->second->m_attrSet.end();++it){
// 			SConfigTag::MultiValuesIT itv=it->second.begin();
 			core::string s;
//			for(;itv!=it->second.end();itv++){
				s=mT("\t")+it->first+mT("\t\"")+it->second+mT("\"\n");
				writer.writeString(s);
//			}
		}
		writer.writeString(mT("}\n"));
	}


	file->close();

}

const core::string& CSettingsFile::getPropertie(const core::string&tag,const core::string&name){
	
	SConfigTag *t=getTag(tag);
	if(!t)
		return core::string::Empty;
	return t->getAttr(name);
}

bool CSettingsFile::hasPropertie(const core::string&tag,const core::string&name){

	SConfigTag *t=getTag(tag);
	if(!t)
		return 0;
	return t->hasAttr(name);
}


SConfigTag*CSettingsFile::getTag(const core::string&tag){
	TagMap::iterator it=m_Tags.find(tag);
	if(it==m_Tags.end())
		return 0;
	return it->second;
}


void CSettingsFile::clear()
{
	TagMap::iterator it=m_Tags.begin();
	for(;it!=m_Tags.end();++it)
		delete it->second;
	m_Tags.clear();
}

SConfigTag* CSettingsFile::addTag(const core::string&tag){
	SConfigTag*t=new SConfigTag();
	t->tagName=tag;
	m_Tags[tag]=t;
	return t;
}

void CSettingsFile::setPropertie(const core::string&tag,const core::string&name,const  core::string&value){
	SConfigTag*t=getTag(tag);
	if(!t)
	{
		t=addTag(tag);
		//return;
	}
	t->addAttr(name,value);
}

}
}




