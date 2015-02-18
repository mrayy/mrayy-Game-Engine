#include "stdafx.h"

#include "EffectScript.h"
#include <CTokenTranslator.h>
#include <LogManager.h>
#include <StringConverter.h>

namespace mray{
namespace script{


script::TokenDefine __eff_tokenDefintion[]={
	script::TokenDefine(mT("effect")		,LEX_Effect_ID),

	script::TokenDefine(mT("{")				,LEX_OpenBrace_ID),
	script::TokenDefine(mT("}")				,LEX_ClosedBrace_ID),
	script::TokenDefine(mT("[")				,LEX_OpenBraket_ID),
	script::TokenDefine(mT("]")				,LEX_ClosedBraket_ID),


	script::TokenDefine::EndDefineToken
};




EffectScript::EffectScript(){
	m_lex=new script::ILexical();
	m_lex->setTokenDefinitionTable(__eff_tokenDefintion,false);
	m_lex->setDelimChars(mT(" \t\r\n"));
}

EffectScript::~EffectScript(){
}
int EffectScript::loadEffectScript(OS::IStreamPtr stream){

	m_lex->loadFromStream(stream);
	script::CTokenTranslator trans;

	m_tokenList=new script::CTokenList();
	m_tokenList->fromLexical(m_lex,&trans,true);
	if(!m_tokenList->isDone()){
		bool res=0;
		do{
		}while(res);
	}

	return 0;
}
void EffectScript::addDefinitions(script::TokenDefine*definitions){
	m_lex->setTokenDefinitionTable(definitions,false);
}


}
}


