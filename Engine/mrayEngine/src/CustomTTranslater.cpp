#include "stdafx.h"



#include "CustomTTranslater.h"
#include "ILexical.h"


namespace mray{
namespace script{

CustomTTranslater::CustomTTranslater(){
}
CustomTTranslater::~CustomTTranslater(){
	m_lexemes.clear();
}


void CustomTTranslater::addLexeme(const core::string&lex,uint token,bool hasAction){
	TokenInfo t;
//	t.caseSensitive=caseSensitive;
	t.id=token;
	t.lexeme=lex;
	t.hasAction=hasAction;
/*	if(!caseSensitive)
		t.lexeme.make_lower();*/
	m_lexemes.insert(LexemeMap::value_type(lex,t));
}
int CustomTTranslater::getLexemeCount(){
	return m_lexemes.size();
}


int CustomTTranslater::translateToken(const core::string&str,ILexical*lexical,core::string &resToken)
{
	LexemeMap::iterator it=m_lexemes.find(str.c_str());
	if(it!=m_lexemes.end()){
		TokenInfo&t=it->second;
		if(t.hasAction)
			triggerAction(t.lexeme,t.id);
		return t.id;
	}
	return (uint)-1;
}


}
}

