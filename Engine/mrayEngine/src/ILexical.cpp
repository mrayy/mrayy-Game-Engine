#include "stdafx.h"

#include "ILexical.h"
#include "IStream.h"

#include "macros.h"
#include "StringConverter.h"
#include "IStream.h"

#include <algorithm>

namespace mray{
namespace script{

#ifdef UNICODE
#	define isSpace isspace
#else
#	define isSpace iswspace
#endif

mchar g_buffer[512];

const TokenDefine TokenDefine::EndDefineToken(mT(""),-1,EST_EOD_T);

ILexical::ILexical()
{
	m_currLine=0;
	m_currCol=0;
	m_delim=mT("\t \n\r");
	m_pScript=0;
	m_caseSensitive=false;
}

ILexical::~ILexical(){
	m_tokenDefinition.clear();
	m_script.clear();
}

void ILexical::setTokenDefinitionTable(TokenDefine*tokens,bool caseSensitive,int count)
{
	m_caseSensitive=caseSensitive;
	for(int i=0;count==-1 || i<count;++i){
		if(tokens[i].type==EST_EOD_T){
			break;
		}
		m_tokenDefinition.push_back(tokens[i]);
		if(!caseSensitive){
			(m_tokenDefinition.end()-1)->Image.make_lower();
		}
	}
	std::sort(m_tokenDefinition.begin(),m_tokenDefinition.end());
}
void ILexical::setScriptString(const core::string&text){
	int len=text.length()+1;
	m_script.resize(len);
	const mchar*pstr=text.c_str();
	mchar*ptxt=&m_script[0];
	for(int i=0;i<len;++i)
		ptxt[i]=pstr[i];
	m_pScript=&m_script[0];
	m_currLine=0;
	m_currCol=0;
}
void ILexical::loadFromStream(OS::IStream* stream){
	if(!stream){
		m_script.resize(1);
		m_script[0]='\0';
		m_pScript=&m_script[0];
		return;
	}
	int len=stream->length();
	stream->seek(0,OS::ESeek_Set);
	char*data=new char[len+1];
	m_script.resize(len+1);
	stream->read(data,len);
	data[len]=0;
	for(int i=0;i<=len;++i)
		m_script[i]=data[i];
	delete [] data;
	m_pScript=&m_script[0];
	m_currLine=0;
	m_currCol=0;
}

void ILexical::setFileName(const core::string&name){

	m_fileName=name;
	m_currToken.file=m_fileName;
}
core::string& ILexical::getFileName(){
	return m_fileName;
}

mchar ILexical::nextChar(int i){
	if(!(*m_pScript))
		return 0;
	for(;*m_pScript && i>0;m_pScript++,i--){
		if(*m_pScript=='\n'){
			m_currCol=0;
			m_currLine++;
		}else
			m_currCol++;
	}
	return *m_pScript;
}

void ILexical::reset()
{
	m_pScript=&m_script[0];
}
mchar ILexical::currChar(){
	return *m_pScript;
}
mchar ILexical::peekNextChar(int i){
	if(!(*m_pScript))
		return 0;
	return *(m_pScript+i);
}

void ILexical::translateToken(){
	TokenDefine t;
	t.Image=m_currToken.lex;
	if(!m_caseSensitive)
		t.Image.make_lower();
	std::vector<TokenDefine>::iterator it= std::find(m_tokenDefinition.begin(),m_tokenDefinition.end(),t);
	if(it==m_tokenDefinition.end()){
		m_currToken.token=ERT_Unkown;
		//m_currToken.type=EST_Other;
		return;
	}else{
		m_currToken.token=(*it).id;
		m_currToken.type=(*it).type;
	}
}

bool ILexical::isStringIdent(const core::string&word){
	int len=word.length();
	const mchar*pw=word.c_str();

	//return if it starts with number
	if(isCharInt(pw[0]))
		return false;

	for(int i=0;i<len;++i){
		if(!isCharIdent(pw[i]))
			return false;
	}
	return true;
}
bool ILexical::isCharIdent(mchar c){
	return (c>='0' && c<='9' ||
			c>='a' && c<='z' ||
			c>='A' && c<='Z' ||
			c=='_');
}
bool ILexical::isCharDelim(mchar c){
	return m_delim.find(c)!=-1;
	/*
	return (c=='-' || c=='+' || c=='/' || c=='*' || c==':'||
			c=='=' || c=='!' || c=='@' || c=='#' ||
			c=='$' || c=='%' || c=='^' || c=='(' ||
			c==')' || c=='[' || c==']' || c=='{' ||
			c=='}' || c=='\"' || c=='\'' || c=='<' ||
			c=='>' || c=='?' || c=='~' || c=='`' ||
			c==',' || c==';' || c=='|' || c=='.' || isspace(c));// || c=='.'
	*/
}

bool ILexical::isFloat(const core::string&word){
	int len=word.length();
	const mchar*pw=word.c_str();

	bool found_dot=0;
	int dotPos=-1;
	int start=0;

	//skip +,-
	if(len>0 && (pw[0]=='-' || pw[0]=='+'))
		start=1;

	//search throw the string and check for '.'

	for(int i=start;i<len;++i){
		if(!isCharInt(pw[i])){
			if(pw[i]=='.' && !found_dot){
				found_dot=true;
			}
			else{
				return false;
			}
		}
	}
	if(found_dot && dotPos==len-1)
		return false;

	//check if the string is only + or -
	if(len==1 && start==1)
		return false;
	return true;
}

bool ILexical::isInt(const core::string&word){
	int len=word.length();
	const mchar*pw=word.c_str();

	int start=0;

	//skip +,-
	if(len>0 && (pw[0]=='-' || pw[0]=='+'))
		start=1;
	for(int i=start;i<len;++i){
		if(!isCharInt(pw[i])){
			return false;
		}
	}

	//check if the string is only + or -
	if(len==1 && start==1)
		return false;
	return true;
}/*
bool ILexical::isString(const core::string&word){
	int len=word.size();
	const mchar*pw=word.c_str();
}*/


void ILexical::setDelimChars(const mchar*delim){
	m_delim=delim;
}
const mchar* ILexical::getDelimChars(){
	return m_delim.c_str();
}


const mchar*ILexical::readToChar(mchar c,bool skipWhitSpace){
	mchar ch;
	int i=0;
	do{
		ch=nextChar();
		if (!ch)
		{
			break;
		}
		g_buffer[++i]=ch;
	}while(ch!=c );
	g_buffer[i]=0;
	return g_buffer;
}

bool ILexical::isCharInt(char c){
	if(c>='0' && c<='9')
		return true;
	return false;
}

bool ILexical::done()
{
	return (*m_pScript)==0;
}

ScriptToken&ILexical::getNextToken(){

	int i=0;
	mchar c;

	skipWhiteSpaces();
	m_currToken.line=m_currLine;
	m_currToken.col=m_currCol;

	c=currChar();
	if(c==0){
		g_buffer[0]=0;
		m_currToken.lex="";
		m_currToken.token=ERT_EOF;
		m_currToken.type=EST_Other;
		return m_currToken;
	}
	if(c=='\"'){
		m_currToken.lex=getString();
		m_currToken.token=ERT_Unkown;
		m_currToken.type=EST_String;
		return m_currToken;
	}

	g_buffer[i]=c;
	i++;
	if(isCharDelim(c)){
		nextChar();
		g_buffer[i]=0;
		m_currToken.lex=g_buffer;
		translateToken();
		return m_currToken;
	}
	if(isCharIdent(c)){
		m_currToken.type=EST_Ident;
	}else if(isCharInt(c)){
		m_currToken.type=EST_Int;
	}else{
		m_currToken.type=EST_Other;
	}

	while(1){
		c=nextChar();
		if(isSpace(c) || c==0 || isCharDelim(c)){
			break;
		}
		if(m_currToken.type==EST_Int && !isCharInt(c)){
			if(c=='.')
				m_currToken.type=EST_Float;
			else
				m_currToken.type=EST_Ident;
		}
		g_buffer[i]=c;
		i++;
	}
	g_buffer[i]=0;
	m_currToken.lex=g_buffer;
	translateToken();
	return m_currToken;

}
ScriptToken&ILexical::getCurrToken(){
	return m_currToken;
}

void ILexical::skipLine(){
	mchar c;
	c=nextChar();
	if(c==0)return;

	while(c!='\n'){
		c=nextChar();
		if(c==0)return;
	}
	nextChar();
}


mchar ILexical::getCharNonComment(){
	mchar c=currChar();
	mchar tmp=0;
	if(c==0)return 0;
	while(c=='/'){
		c=peekNextChar(1);
		if(c=='/'){
			tmp=nextChar(1);
			while(tmp!='\n'){
				tmp=nextChar();
				if(tmp==0)return c;
			}
			skipWhiteSpaces();
		}
	}
	return c;
}
void ILexical::skipWhiteSpaces(){
	mchar c=currChar();
	while(true){
		if(c=='/' ){
			c=peekNextChar();
			if(c=='/'){
				do{
					c=nextChar();
				}while(c!='\n');
			}else if(c=='*')
			{
				nextChar(1);
				do{
					c=nextChar();
					if(c=='*' && peekNextChar()=='/'){
						nextChar();
						break;
					}
				}while(1);
			}			
		}else if(!isSpace(c))
			return;
		if (!(*m_pScript) || !nextChar())
		{
			break;
		}
		c=currChar();
	};
}

const mchar*ILexical::getString(){
	int i=0;
	mchar c;
	bool in=0;
	bool escChar=0;
	mchar startCh=0;
	
	skipWhiteSpaces();

	c=getCharNonComment();
	if(c==0){
		g_buffer[i]=0;
		return g_buffer;
	}
	if(c=='\"'){
		in = 1;
		startCh='\"';
	}
	else if(c=='\''){
		in = 1;
		startCh='\'';
	}else if(c=='\\')
		escChar=1;
	else
		g_buffer[i++]=c;
	if(!in && isCharDelim(c)){
		g_buffer[i++]=0;
		return g_buffer;
	}
	bool done=0;
	while(!done){
		c=nextChar();
		
		if(c==0)
			break;


		if(escChar){
			switch(c){
				case 'n':	c='\n';break;
				case 't':	c='\t';break;
			}
			escChar=0;
		}else{
			switch(c){
				case '\"':
					if(startCh=='\"' ){
						done=1;
						nextChar();
					}
				break;
				case '\'':
					if(startCh=='\'' ){
						done=1;
						nextChar();
					}
				break;
				case  '\\':	escChar=1;
				break;
			}
		}
		if(!escChar && !done)
			g_buffer[i++]=c;

	}

	g_buffer[i]=0;
	return g_buffer;
}


const mchar*ILexical::getLine(){

	int i=0;
	mchar c;

	skipWhiteSpaces();

	c=getCharNonComment();
	while(c!='\n' && c!='\r' && c!=-1){
		nextChar();
		g_buffer[i++]=c;
		c=peekNextChar();
	}
	g_buffer[i]=0;
	return g_buffer;
}

bool ILexical::getInt(int &out){
	ScriptToken t;

	t=getNextToken();
	if(!isInt(t.lex))
		return 0;

	out=core::StringConverter::toInt(t.lex);

	return true;
}
bool ILexical::getFloat(float &out){
	ScriptToken t;

	t=getNextToken();
	if(!isFloat(t.lex))
		return 0;

	out=core::StringConverter::toFloat(t.lex);

	return true;

}

bool ILexical::match(const core::string&word){
	int i=0;
	bool ok=true;
	for (i=0;i<word.length();++i)
	{
		mchar c=peekNextChar(i);
		if(c!=word[i]){
			ok=false;
			break;
		}
	}
	if(ok){
		nextChar(i);
	}
	return ok;
}
/*
void ILexical::setTranslater(ITokenTranslator*translator){
	m_tokenTranslator=translator;
}

int ILexical::getNextToken(){
	if(m_tokenTranslator){
		return m_tokenTranslator->translateToken(getNextWord(),this);
	}
	return -1;
}
int ILexical::getCurrToken(){
	if(m_tokenTranslator){
		return m_tokenTranslator->translateToken(m_currWord,this);
	}
	return -1;
}
void ILexical::nextToken(){
	getNextWord();
}

*/







///////////////////////////////////////////////////////////////////////

CTokenList::CTokenList(){
	m_currToken=-1;
}
CTokenList::~CTokenList(){
	clear();
}


void CTokenList::clear(){
	for(int i=0;i<m_tokens.size();++i)
		delete m_tokens[i];
	m_tokens.clear();
}

bool CTokenList::isDone(){
	return m_tokens.size()==0 || m_currToken>=(int)m_tokens.size()-1 || m_currToken!=-1 && m_tokens[m_currToken]->token==ERT_EOF;
}

void CTokenList::fromLexical(ILexical*l,ITokenTranslator*translator,bool skipWhitSpace,bool skipComments,const mchar * commentChar){
	createTokens(l,translator,skipWhitSpace,skipComments,commentChar);
}

void CTokenList::createTokens(ILexical*l,ITokenTranslator*translator,bool skipWhitSpace,bool skipComments,const mchar *commentChar){
	if(!l)
		return;

	ScriptToken tmp;
	clear();
	l->reset();
	
	core::string res;
	ScriptToken* token;
	while(!l->done()){
		tmp=l->getNextToken();
		token=new ScriptToken();
		*token=tmp;

		/*
		if(skipComments && tmp.lex==commentChar){
			l->skipLine();
			continue;
		}
		tmp.token=translator->translateToken(tmp.lex,l,res);
		tmp.lex=res;*/
		m_tokens.push_back(token);
	}
}

void CTokenList::reset(int i){
	if( i>=m_tokens.size())
		i=m_tokens.size()-1;
	m_currToken=i;
}

ScriptToken* CTokenList::getToken(int i){
	if(m_currToken+i<0 || m_currToken+i>=m_tokens.size())
		FATAL_ERROR(true,mT("CTokenList::getToken() - out of range!"));
	return m_tokens[m_currToken+i];
}
ScriptToken* CTokenList::getNextToken(){
	FATAL_ERROR(isDone(),mT("CTokenList::getNextToken() - list is done!"));
	return m_tokens[++m_currToken];
}
ScriptToken* CTokenList::peekToken(){
	if(m_currToken<0){
		return m_tokens[0];
	}
	return m_tokens[m_currToken];
}

void CTokenList::seek(int cnt,OS::EStreamSeek mode){
	switch (mode)
	{
	case OS::ESeek_Cur:
		m_currToken+=cnt;
		break;
	case OS::ESeek_Set:
		m_currToken=cnt;
		break;
	case OS::ESeek_End:
		m_currToken=m_tokens.size()-cnt;
		break;
	}
	m_currToken=math::clamp<int>(m_currToken,0,m_tokens.size());
}


}
}
