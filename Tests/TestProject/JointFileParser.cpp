
#include "stdafx.h"
#include "JointFileParser.h"



namespace mray{


#define nextTokenFromList(t)		t=nextToken();if(!t){gLogManager.log(mT("End of script!"),ELL_WARNING);return false;}
#define checkTokenTypeID(t,id,err)	if(t->token!=id){gLogManager.log(err,ELL_WARNING);return false;}
#define checkTokenTypeLex(t,id,err) if(!token->lex.equals_ignore_case(id)){gLogManager.log(err+core::string(mT(",line:"))+core::string(token->line),ELL_WARNING);return false;}
#define nextTokenAndChkID(t,id,err) nextTokenFromList(t);checkTokenTypeID(t,id,err)
#define nextTokenAndChkLex(t,id,err) nextTokenFromList(t);checkTokenTypeLex(t,id,err)


script::TokenDefine JointFileParser::tokenDefintion[]={
	script::TokenDefine(mT("joint")		,EJointLex::LEX_Joint_ID),
	script::TokenDefine(mT("pos")		,EJointLex::LEX_Pos_ID),
	script::TokenDefine(mT("rot")		,EJointLex::LEX_Rotate_ID),
	script::TokenDefine(mT("constrain")	,EJointLex::LEX_Constrain_ID),
	script::TokenDefine(mT("{")			,EJointLex::LEX_OpenBrace_ID),
	script::TokenDefine(mT("}")			,EJointLex::LEX_ClosedBrace_ID),
	script::TokenDefine(mT("[")			,EJointLex::LEX_OpenBraket_ID),
	script::TokenDefine(mT("]")			,EJointLex::LEX_ClosedBraket_ID)
};

JointFileParser::JointFileParser(){
	m_lex=new script::ILexical();
	m_lex->setTokenDefinitionTable(tokenDefintion,false);
	m_lex->setDelimChars(mT(" \t\r\n[]{}"));
}
JointFileParser::~JointFileParser(){
	m_tokenList=0;
	m_lex=0;
}

script::ScriptToken* JointFileParser::nextToken(){

	if(m_tokenList->isDone()){
		return 0;
	}
	script::ScriptToken* token=m_tokenList->getNextToken();
	if(!token || token->token==script::ERT_EOF)
		return 0;
	return token;
}


script::ScriptToken* JointFileParser::peekToken(){

	if(m_tokenList->isDone()){
		return 0;
	}
	script::ScriptToken* token=m_tokenList->getToken(1);
	if(!token || token->token==script::ERT_EOF)
		return 0;
	return token;
}
bool JointFileParser::lookAhead(const mchar*string){
	script::ScriptToken* token=peekToken();
	if(token && token->lex.equals_ignore_case(string))
		return true;
	return false;
}

void JointFileParser::loadJointsScript(GCPtr<OS::IStream> stream){
	m_lex->loadFromStream(stream);
	script::CTokenTranslator trans;

	script::ScriptToken* t;

	m_tokenList=new script::CTokenList();
	m_tokenList->fromLexical(m_lex,&trans,true);

	if(!m_tokenList->isDone()){
		bool res=0;
		do{
			t=nextToken();
			if(!t)break;

			if(t->token==LEX_Joint_ID){
			}
		}while(res);
	}

}

void JointFileParser::addDefinitions(script::TokenDefine*definitions){
	m_lex->setTokenDefinitionTable(tokenDefintion,false);
}

}
