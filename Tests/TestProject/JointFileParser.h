

/********************************************************************
	created:	2009/10/10
	created:	10:10:2009   22:00
	filename: 	i:\Programing\GameEngine\mrayEngine\testFrameWork\TestProject\JointFileParser.h
	file path:	i:\Programing\GameEngine\mrayEngine\testFrameWork\TestProject
	file base:	JointFileParser
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___JointFileParser___
#define ___JointFileParser___

#include <GCPtr.h>
#include <IStream.h>
#include <ILexical.h>

namespace mray{

class JointFileParser
{
private:
protected:
	enum EJointLex{
		LEX_Joint_ID		=script::ERT_Count+1,
		LEX_Pos_ID,
		LEX_Rotate_ID,
		LEX_Constrain_ID,
		LEX_OpenBrace_ID,
		LEX_ClosedBrace_ID,
		LEX_OpenBraket_ID,
		LEX_ClosedBraket_ID,

		LEX_JointLexCount
	};


	GCPtr<script::ILexical> m_lex;
	GCPtr<script::CTokenList> m_tokenList;

	script::ScriptToken* nextToken();
	script::ScriptToken* peekToken();
	bool lookAhead(const mchar*string);

	static script::TokenDefine tokenDefintion[];

public:
	JointFileParser();
	virtual~JointFileParser();

	void loadJointsScript(GCPtr<OS::IStream> stream);
	void addDefinitions(script::TokenDefine*definitions);

};

}


#endif //___JointFileParser___
