


/********************************************************************
	created:	2011/09/20
	created:	20:9:2011   11:56
	filename: 	d:\Development\mrayEngine\Engine\mraySerialization\HeaderParser.h
	file path:	d:\Development\mrayEngine\Engine\mraySerialization
	file base:	HeaderParser
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __HeaderParser__
#define __HeaderParser__

#include "ITokenTranslator.h"

namespace mray
{
	namespace script
	{
		class ILexical;
		class CTokenList;
		class ScriptParserUtilities;
		struct TokenDefine;
	}
namespace serialize
{

	enum{
		LEX_Class_ID		=script::ERT_Count+1,
		LEX_NameSpace_ID,
		LEX_OpenBrace_ID,
		LEX_ClosedBrace_ID,
		LEX_OpenArc_ID,
		LEX_ClosedArc_ID,
		LEX_Colon_ID,
		LEX_SemiColon_ID,
		LEX_Dot_ID,

	};
class HeaderParser
{
private:
protected:
	script::ILexical* m_lex;
	script::CTokenList* m_tokenList;
	script::ScriptParserUtilities* m_utilities;

	void ParseToken(script::ScriptToken* t);
	bool ParseNameSpace();
	bool ParseClass();
	bool ParseFunction();
	bool ParseVariable();
public:
	HeaderParser();
	virtual~HeaderParser();


	int loadCHeader(OS::IStream* stream);
	void addDefinitions(script::TokenDefine*definitions);
};

}
}

#endif
