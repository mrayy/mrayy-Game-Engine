
/********************************************************************
	created:	2009/02/26
	created:	26:2:2009   22:36
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\WinThreadManager.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	WinThreadManager
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___WinThreadManager___
#define ___WinThreadManager___

#include <IThreadManager.h>
#include "CompileConfig.h"

namespace mray{
namespace OS{

class MRAY_Win32_DLL WinThreadManager:public IThreadManager
{
	ulong m_tlsID;
public:
	WinThreadManager();
	~WinThreadManager();
	IThread* createThread(IThreadFunction*f,EThreadPriority priority=ETP_Normal);
	IMutex* createMutex();
	ICondition* createCondition();
	IThreadEvent* createEvent();

	IThread*getCurrentThread();

	void sleep(uint ms);

	ulong getTlsID(){return m_tlsID;}

	int WaitForEvents(IThreadEvent**events,int count,bool waitForAll,ulong ms);
};

}
}


#endif //___WinThreadManager___
