

#ifndef ___ILexical___
#define ___ILexical___

#include "mTypes.h"
#include "mString.h"
#include "GCPtr.h"
#include "ITokenTranslator.h"


namespace mray{
namespace OS{
	enum EStreamSeek;
	class IStream;
}

namespace script{


struct MRAY_DLL TokenDefine 
{
public:
	TokenDefine(){}
	TokenDefine(ESpecialTokens type){
		this->type=type;
	}
	TokenDefine(const core::string&image,int id)
	{
		this->Image=image;
		this->id=id;
		type=EST_Other;
	}
	TokenDefine(const core::string&image,int id,ESpecialTokens type)
	{
		this->Image=image;
		this->id=id;
		this->type=type;
	}
	core::string Image;
	int id;
	ESpecialTokens type;

	bool operator<(const TokenDefine&o)const
	{
		return Image<o.Image;
	}
	bool operator==(const TokenDefine&o)const
	{
		return	Image==o.Image ;
	}

	static const TokenDefine EndDefineToken;
};



class MRAY_DLL ILexical
{

protected:

	std::vector<TokenDefine> m_tokenDefinition;
	std::vector<mchar> m_script;

	mchar*m_pScript;

	int m_currLine;
	int m_currCol;
	ScriptToken m_currToken;
	core::string m_fileName;

	core::string m_delim;

	bool m_caseSensitive;

	mchar currChar();
	mchar nextChar(int i=1);
	mchar peekNextChar(int i=1);

	void translateToken();
public:
	ILexical();
	virtual~ILexical();

	void setTokenDefinitionTable(TokenDefine*tokens,bool caseSensitive,int count=-1);
	void setScriptString(const core::string&text);
	void loadFromStream(OS::IStream* stream);

	bool isCaseSensitive();

	virtual void skipWhiteSpaces();
	virtual mchar getCharNonComment();

	void setDelimChars(const mchar*delim=mT("\t \n\r"));
	const mchar* getDelimChars();

	const mchar*readToChar(mchar c,bool skipWhitSpace);

	void setFileName(const core::string&name);
	core::string& getFileName();

	bool done();

	void reset();

	virtual ScriptToken&getNextToken();
	virtual ScriptToken&getCurrToken();

	virtual const mchar*getLine();
	virtual const mchar*getString();

//	void nextToken();
//	virtual int getNextToken();
//	virtual int getCurrToken();


	virtual void skipLine();

	virtual bool getInt(int &out);
	virtual bool getFloat(float &out);

	bool match(const core::string&word);

	
	bool isFloat(const core::string&word);
	bool isInt(const core::string&word);
	bool isCharInt(char c);
//	bool isString(const core::string&word);

	
	virtual bool isStringIdent(const core::string&word);
	virtual bool isCharIdent(mchar c);
	virtual bool isCharDelim(mchar c);

};


class MRAY_DLL CTokenList
{
	typedef std::vector<ScriptToken*> TokensList;
	TokensList m_tokens;

	int m_currToken;

	void createTokens(ILexical*l,ITokenTranslator*translator,bool skipWhitSpace,bool skipComments,const mchar * commentChar);
public:
	CTokenList();
	virtual~CTokenList();

	void clear();
	void fromLexical(ILexical*l,ITokenTranslator*translator,bool skipWhitSpace=true,bool skipComments=true,const mchar * commentChar=mT("#"));

	bool isDone();

	void reset(int i);

	ScriptToken* getToken(int i);
	ScriptToken* getNextToken();
	ScriptToken* peekToken();

	virtual void seek(int cnt,OS::EStreamSeek mode);
};


}
}


#endif