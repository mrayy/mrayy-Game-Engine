
#ifndef ___CustomTTranslater___
#define ___CustomTTranslater___


#include "ITokenTranslator.h"


namespace mray{
namespace script{

class MRAY_DLL CustomTTranslater:public ITokenTranslator
{

	struct TokenInfo{
		core::string lexeme;
		uint id;
		bool hasAction;
	//	bool caseSensitive;
	};
	typedef std::map<core::string,TokenInfo> LexemeMap;
	LexemeMap m_lexemes;
	
protected:

	virtual void triggerAction(const core::string&lex,uint token){};

public:

	CustomTTranslater();
	virtual~CustomTTranslater();

	void addLexeme(const core::string&lex,uint token,bool hasAction=false);
	int getLexemeCount();

	virtual int translateToken(const core::string&str,ILexical*lexical,core::string &resToken);
};

}
}




#endif


