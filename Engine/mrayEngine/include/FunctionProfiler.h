

/********************************************************************
	created:	2011/03/24
	created:	24:3:2011   14:52
	filename: 	i:\Programing\GameEngine\mrayEngine\Engine\mrayEngine\include\FunctionProfiler.h
	file path:	i:\Programing\GameEngine\mrayEngine\Engine\mrayEngine\include
	file base:	FunctionProfiler
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef FunctionProfiler_h__
#define FunctionProfiler_h__

#include "mstring.h"

namespace mray
{

class MRAY_DLL FunctionProfiler
{
	static uint IDCounter;
	uint m_id;
	std::string m_name;
	double m_enterTime;
	double m_executionTime;
	double m_totalExecutionTime;
	double m_subTime;
	int m_level;
	FunctionProfiler* m_caller;
	static FunctionProfiler* currentFunction;
public:
	FunctionProfiler(const std::string& name);

	uint GetID()const;
	const std::string& GetName()const ;
	void OnEnter();
	void OnExit();

	double GetLastExecutionTime()const;
	double GetTotalExecutionTime()const;
	double GetSubExecutionTime()const;
};

class FunctionProfilerCall
{
	FunctionProfiler*m_profile;
public:
	FunctionProfilerCall(FunctionProfiler*p):m_profile(p)
	{
		m_profile->OnEnter();
	}
	~FunctionProfilerCall()
	{
		m_profile->OnExit();
	}
};

}


#define PROFILE_FUNCTION()\
	static mray::FunctionProfiler s_profiler(__FUNCTION__ );\
	mray::FunctionProfilerCall tmpCaller(&s_profiler);

#define PROFILE_BLOCK(name)\
	static mray::FunctionProfiler s_blockProfiler(name);\
	mray::FunctionProfilerCall tmpCaller(&s_blockProfiler);

#endif // FunctionProfiler_h__
