#include "stdafx.h"



#include "CTokenTranslator.h"
#include "ILexical.h"


namespace mray{
namespace script{

int CTokenTranslator::translateToken(const core::string&str,ILexical*lexical,core::string &resToken)
{
#ifdef IGNORE_CODE
	if(str[0]=='\"'){
		//seems like string
		mchar tmp;
		resToken="";
		while(!lexical->done()){
			tmp=lexical->nextChar();
			if(tmp=='\"')
				break;
			resToken+=tmp;
		}
		return Token_String;
		//return Token_Quote;
	}
	resToken=str;
	if(lexical->isInt(str)){
		return Token_Int;
	}
	if(lexical->isFloat(str)){
		return Token_Float;
	}/*
	if(lexical->isString(str)){
		return Token_String;
	}*/
	if(lexical->isFloat(str)){
		return Token_Float;
	}
	if(lexical->isStringIdent(str)){
		return Token_Ident;
	}
	if(str[0]==':'){
		return Token_Colon;
	}
	if(str[0]=='['){
		return Token_Open_Bracket;
	}
	if(str[0]==']'){
		return Token_Closed_Bracket;
	}
	if(str[0]=='{'){
		return Token_Open_Brace;
	}
	if(str[0]=='}'){
		return Token_Closed_Brace;
	}
	if(str[0]=='('){
		return Token_Open_Arc;
	}
	if(str[0]==')'){
		return Token_Closed_Arc;
	}
	if(str[0]==','){
		return Token_Comma;
	}
	if(str[0]=='\n'){
		return Token_NewLine;
	}
	if(str[0]==';'){
		return Token_SemiColon;
	}
	if(str[0]=='-'){
		return Token_Minu;
	}
	if(str[0]=='+'){
		return Token_Plus;
	}
	if(str[0]=='/'){
		return Token_Div;
	}
	if(str[0]=='*'){
		return Token_Mul;
	}
	if(str[0]=='='){
		return Token_Equal;
	}
	if(str[0]=='<'){
		return Token_Less;
	}
	if(str[0]=='>'){
		return Token_Greater;
	}
	if(str[0]=='?'){
		return Token_QMark;
	}
	if(str[0]=='#'){
		return Token_Hash;
	}
#endif
	return Token_Unkown;

};

}
}

