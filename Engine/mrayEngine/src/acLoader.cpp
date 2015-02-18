#include "stdafx.h"

#include "acLoader.h"
#include "IFileSystem.h"
#include "ILogManager.h"
#include "CTokenTranslator.h"

#include "ILexical.h"
#include "StringUtil.h"


namespace mray{
namespace loaders{

acLoader::acLoader(){
}
acLoader::~acLoader(){
}

scene::SMesh* acLoader::load(const  mchar* filename){
	if(!canLoad(filename))return 0;
	OS::IStreamPtr m_FilePointer=gFileSystem.openFile(filename);
	if(!m_FilePointer->isOpen())return 0;
	return load(m_FilePointer);
}
scene::SMesh* acLoader::load(OS::IStream* stream){

	stream->seek(0,OS::ESeek_Set);
	script::ILexical lex;
	lex.loadFromStream(stream);

	script::CTokenTranslator trans;

	script::CTokenList tokenList;
	tokenList.fromLexical(&lex,&trans,true);
	script::ScriptToken* token;
	token=tokenList.getNextToken();
	if(!token->lex.equals_ignore_case(mT("AC3Db"))){
		gLogManager.log(mT("This is not a 'ac' valid file : ")+core::string(stream->getStreamName()),ELL_WARNING);
		return 0;
	}
	if(!tokenList.isDone()){
		bool res=0;
		do{
		}while(res);
	}


	return 0;
}
bool acLoader::canLoad(const  mchar*name){
	core::string str=core::StringUtil::ToLower(name);
	return strSearch(str.c_str(),mT(".ac"))!=0;
}
const core::string g_sACExt=mT("ac");
const core::string&acLoader::getExtension(){
	return g_sACExt;
}


}
}