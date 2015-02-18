

#include "stdafx.h"
#include "LParser.h"

namespace mray
{
namespace parsers
{

	script::TokenDefine LParser::TokenDefineTable[]=
	{
		script::TokenDefine(mT("[A]"),LEX_Atom_ID),
		script::TokenDefine(mT("[R]"),LEX_Rule_ID),
		script::TokenDefine(mT(":"),LEX_Colon_ID),
		script::TokenDefine(mT(";"),LEX_RuleEnd_ID),

		script::TokenDefine::EndDefineToken

	};


LParser::LParser()
{
	m_nextAtomID=0;
}
LParser::~LParser()
{
}

bool LParser::_parseAtom(script::CTokenList*tokens)
{
	script::ScriptToken* token;
	token=tokens->getNextToken();
	if(!token)
		return false;
	LAtom atom;
	atom.ID=m_nextAtomID++;
	atom.name=token->lex;
	m_atoms.push_back(atom);
	return true;
}

bool LParser::_parseRule(script::CTokenList*tokens)
{
	script::ScriptToken* token;
	LRule rule;
	//collect left side
	while(true)
	{
		token=tokens->getNextToken();
		if(!token || token->token==LEX_Colon_ID)
			break;
		
		int id=GetAtomID(token->lex);
		if(id==-1)
			return false;
		rule.leftTerm.push_back(id);
	}

	if(!token || rule.leftTerm.size()==0)
		return false;

	//collect right side
	while(true)
	{
		token=tokens->getNextToken();
		if(!token || token->token==LEX_RuleEnd_ID)
			break;

		int id=GetAtomID(token->lex);
		if(id==-1)
			return false;
		rule.rightTerm.push_back(id);
	}

	m_longestPattern=math::Max<int>(m_longestPattern,rule.leftTerm.size());

	m_rules.push_back(rule);
	return true;
}

bool LParser::LoadFromFile(const core::string& path)
{
	OS::IStreamPtr stream=gFileSystem.openFile(path);
	if(!stream)
		return false;
	
	return LoadFromStream(stream.pointer());
}
bool LParser::LoadFromStream(OS::IStream* stream)
{

	m_nextAtomID=0;
	m_longestPattern=0;
	m_atoms.clear();
	m_rules.clear();

	script::ILexical* parser=new script::ILexical();
	script::CTokenList* tokens=new script::CTokenList();
	parser->loadFromStream(stream);
	parser->setTokenDefinitionTable(TokenDefineTable,false);

	parser->setDelimChars(mT("\t\n\r :;"));

	script::CTokenTranslator translater;

	tokens->fromLexical(parser,&translater,true,true,mT("#"));

	bool res=true;

	script::ScriptToken* token;
	while(!tokens->isDone())
	{
		token=tokens->getNextToken();
		if(token->token==script::ERT_EOF)
			break;
		if(token->token==LEX_Atom_ID)
		{
			if(!_parseAtom(tokens))
			{
				res=false;
				break;
			}
		}else if(token->token==LEX_Rule_ID)
		{
			if(!_parseRule(tokens))
			{
				res=false;
				break;
			}
		}else
		{
			res=false;
			break;
		}
	}

	delete tokens;
	delete parser;

	return res;
}

int LParser::GetAtomID(const core::string& name)
{
	for(int i=0;i<m_atoms.size();++i)
	{
		if(m_atoms[i].name.equals_ignore_case(name))
			return m_atoms[i].ID;
	}
	return -1;
}

std::vector<int> LParser::ApplyRules(const std::vector<int>& atoms)
{
	std::vector<int> result;
	for(int i=0;i<atoms.size();++i)
	{
		bool found=false;
		for(int j=math::Min<int>(i+m_longestPattern,atoms.size())-1;
			j>=i && !found;--j)
		{
			for(int r=0;r<m_rules.size();++r)
			{
				if(m_rules[r].Matches(&atoms[i],j-i+1))
				{
					found =true;
					for(int k=0;k<m_rules[r].rightTerm.size();++k)
						result.push_back(m_rules[r].rightTerm[k]);
					i=j;
					break;
				}
			}
		}
		
		if(!found)
		{
			result.push_back(atoms[i]);
		}
	}
	return result;
}

}
}
