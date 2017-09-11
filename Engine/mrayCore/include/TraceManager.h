
#ifndef ___TraceManager___
#define ___TraceManager___

#include "ISingleton.h"
#include "mString.h"

#include <stack>

namespace mray{
class MRAY_CORE_DLL TraceFunctionProfile;

class MRAY_CORE_DLL  TraceManager:public ISingleton<TraceManager>
{
	typedef std::stack<TraceFunctionProfile*> FuctionsList;

	FuctionsList m_functions;
public:

	TraceManager();
	virtual~TraceManager();

	void pushFunction(TraceFunctionProfile*f);
	void popFunction(TraceFunctionProfile*f);

	void flushToLogger();
};

class MRAY_CORE_DLL  TraceFunctionProfile
{
public:
	core::stringc m_name;
	core::string m_catagory;
	bool m_timeStumb;
	double m_startTime;
	double m_endTime;

	TraceFunctionProfile(const char*name,const mchar*catagory,bool timeStumb=0);
	void enterFunction();
	void exitFunction();
};
class MRAY_CORE_DLL  TraceFunction
{
	TraceFunctionProfile*m_tf;
	bool m_exit;
public:
	TraceFunction(TraceFunctionProfile*tf);
	~TraceFunction();
	void exit();
};
#define gTraceManager mray::TraceManager::getInstance()

}

#if 0
//#ifdef _DEBUG

#define traceFunction(catagory) static TraceFunctionProfile __s_Profile(__FUNCTION__,mT(#catagory));\
	TraceFunction __t_entrance(&__s_Profile);

#define forceExitFunction() __t_entrance.exit();
#else

#define traceFunction(catagory) 
#define forceExitFunction()

#endif

#endif


