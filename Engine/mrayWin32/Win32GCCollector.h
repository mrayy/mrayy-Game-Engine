
/********************************************************************
	created:	2009/03/07
	created:	7:3:2009   19:41
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayWin32\Win32GCCollector.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayWin32
	file base:	Win32GCCollector
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___Win32GCCollector___
#define ___Win32GCCollector___

#include <IGCCollector.h>
#include "CompileConfig.h"

namespace mray{

class MRAY_Win32_DLL Win32GCCollector:public IGCCollector
{
protected:

	virtual void inner_eraseAddress(void*addr);
	virtual void inner_addToDeadList(IInfoTemplate*info);

public:
	Win32GCCollector();
	virtual~Win32GCCollector();

	virtual bool isMultiThreaded();
	virtual GCInfo*getInfo(const void* addr);
	virtual void addInfo(GCInfo*info);

	virtual bool collect();

	virtual int activeSize();
	virtual int deadSize();
};

}


#endif //___Win32GCCollector___
