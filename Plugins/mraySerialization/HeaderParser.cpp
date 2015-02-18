

#include "stdafx.h"
#include "HeaderParser.h"
#include "ILexical.h"
#include "ScriptParserUtilities.h"
#include "CTokenTranslator.h"



namespace mray
{
namespace serialize
{

	script::TokenDefine tokenDefintion[]={
		script::TokenDefine(mT("class")		,LEX_Class_ID),
		script::TokenDefine(mT("namespace")		,LEX_NameSpace_ID),
		script::TokenDefine(mT("{")		,LEX_OpenBrace_ID),
		script::TokenDefine(mT("}")		,LEX_ClosedBrace_ID),
		script::TokenDefine(mT("(")		,LEX_OpenArc_ID),
		script::TokenDefine(mT(")")		,LEX_ClosedArc_ID),
		script::TokenDefine(mT(":")		,LEX_Colon_ID),
		script::TokenDefine(mT(";")		,LEX_SemiColon_ID),
		script::TokenDefine(mT(".")		,LEX_Dot_ID)
	};
	HeaderParser::HeaderParser():m_tokenList(0)
	{
		m_lex=new script::ILexical();
		m_utilities=new script::ScriptParserUtilities();
		m_lex->setTokenDefinitionTable(tokenDefintion,false);
		m_lex->setDelimChars(mT(" \t\r\n[]{}:;()."));
	}
	HeaderParser::~HeaderParser()
	{
	}

	void HeaderParser::ParseToken(script::ScriptToken* t)
	{
		switch(t->token)
		{
		case LEX_Class_ID:
			break;
		case LEX_NameSpace_ID:
			break;
		}
	}



	int HeaderParser::loadCHeader(OS::IStream* stream)
	{
		script::CTokenTranslator trans;
		script::ScriptToken* t=0;

		m_tokenList=new script::CTokenList();
		m_tokenList->fromLexical(m_lex,&trans,true);

		m_utilities->SetTokens(m_tokenList);

		if(!m_tokenList->isDone())
		{
			bool res=0;
			do{
				t=m_utilities->nextToken();
				if(!t)break;
				ParseToken(t);
			}while(true);
		}
		return 0;
	}
	void HeaderParser::addDefinitions(script::TokenDefine*definitions)
	{
	}

}
}

