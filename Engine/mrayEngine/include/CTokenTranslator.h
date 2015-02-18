

#ifndef ___CToketTranslator___
#define ___CToketTranslator___

#include "ITokenTranslator.h"

namespace mray{
namespace script{

enum ETokenTypes
{
	Token_Unkown,

	Token_Int,
	Token_Float,
	Token_String,

	Token_Quote,	// "
	Token_Ident,	// identifier
	Token_Colon,	// :
	Token_Open_Bracket,	//	[
	Token_Closed_Bracket,	// ]
	Token_Open_Brace,	// {
	Token_Closed_Brace,	// }
	Token_Open_Arc,		// (
	Token_Closed_Arc,	// )
	Token_Comma,		// ,
	Token_NewLine,
	Token_SemiColon,			// ;
	Token_Minu,
	Token_Plus,
	Token_Div,
	Token_Mul,
	Token_Equal,
	Token_Less,
	Token_Greater,
	Token_QMark,
	Token_Hash,

	
	
	Token_Type_Count


};

class MRAY_DLL CTokenTranslator:public ITokenTranslator
{
public:
	virtual int translateToken(const core::string&str,ILexical*lexical,core::string &resToken);
};

}
}



#endif
