
/********************************************************************
	created:	2009/02/20
	created:	20:2:2009   15:28
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\EffectScript.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	EffectScript
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___EffectScript___
#define ___EffectScript___


#include <GCPtr.h>
#include <IStream.h>
#include <ILexical.h>
#include <ISingleton.h>

namespace mray{
namespace video{
	class SColor;
}
namespace script{

enum{
	LEX_Effect_ID		=script::ERT_Count+1,

	LEX_OpenBrace_ID		,
	LEX_ClosedBrace_ID		,
	LEX_OpenBraket_ID		,
	LEX_ClosedBraket_ID	,

	LEX_EFFECT_COUNT
};

class IEffectScriptCallback 
{
public:
};

class EffectScript: public ISingleton<EffectScript>
{
	GCPtr<script::ILexical> m_lex;
	GCPtr<script::CTokenList> m_tokenList;

	script::ScriptToken* nextToken();
	script::ScriptToken* peekToken();
	bool lookAhead(const mchar*string);

	bool parseColor(video::SColor&c);
	bool parseFloat(float&c);
	bool parseBool(bool&c);

	std::vector<GCPtr<IEffectScriptCallback>> m_callback;

	friend class IEffectScriptCallback;
public:
	EffectScript();
	virtual ~EffectScript();

	int loadEffectScript(OS::IStreamPtr stream);
	void addDefinitions(script::TokenDefine*definitions);

	void addCallback(GCPtr<IEffectScriptCallback> callback);
};

}
}


#endif //___EffectScript___

