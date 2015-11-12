
#ifndef __CRASHHANDLER__
#define __CRASHHANDLER__

#include "mString.h"

namespace mray
{
	
class CrashHandler:public ISingleton<CrashHandler>
{
protected:
	core::string m_reportFileName;

	static void CrashHandlerFunc(int signal);
	void _handleSignal(int signal);
public:
	CrashHandler(const core::string& reportFile);
	virtual ~CrashHandler();

};

}


#endif