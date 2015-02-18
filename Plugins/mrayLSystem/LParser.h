


/********************************************************************
	created:	2011/05/06
	created:	6:5:2011   21:48
	filename: 	d:\Development\mrayEngine\Engine\mrayLSystem\LParser.h
	file path:	d:\Development\mrayEngine\Engine\mrayLSystem
	file base:	LParser
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __LParser__
#define __LParser__

#include <ILexical.h>


namespace mray
{
namespace parsers
{

class LParser
{
public:

	enum{
		LEX_LParserStarts_ID	=script::ERT_Count+1,

		LEX_Atom_ID		,
		LEX_Rule_ID		,
		LEX_Colon_ID		,
		LEX_RuleEnd_ID		,

		LEX_LParser_Count
	};


	struct LAtom
	{
		int ID;
		core::string name;
	};
	struct LRule
	{
		std::vector<int> leftTerm;
		std::vector<int> rightTerm;

		bool Matches(const int* terms,int cnt)
		{
			if(leftTerm.size()!=cnt)return false;
			for(int i=0;i<cnt;++i)
			{
				if(leftTerm[i]!=terms[i])
					return false;
			}
			return true;
		}
	};

private:
protected:

	static script::TokenDefine TokenDefineTable[];

	int m_nextAtomID;
	std::vector<LAtom> m_atoms;
	std::vector<LRule> m_rules;

	int m_longestPattern;

	bool _parseAtom(script::CTokenList*tokens);
	bool _parseRule(script::CTokenList*tokens);

public:
	LParser();
	virtual~LParser();

	bool LoadFromFile(const core::string& path);
	bool LoadFromStream(OS::IStream* stream);

	std::vector<int> ApplyRules(const std::vector<int>& atoms);

	int GetAtomID(const core::string& name);

};


}
}

#endif

