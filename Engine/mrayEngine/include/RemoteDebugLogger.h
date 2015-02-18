
/********************************************************************
	created:	2009/06/30
	created:	30:6:2009   0:14
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\RemoteDebugLogger.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	RemoteDebugLogger
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___RemoteDebugLogger___
#define ___RemoteDebugLogger___


#include "GCPtr.h"
#include "ILogDevice.h"
#include "StreamWriter.h"
#include "IRemoteDebugger.h"


namespace mray{
namespace debug{

class MRAY_DLL RemoteDebugLogger:public ILogDevice
{
private:
protected:
	GCPtr<IRemoteDebugger> m_rd;
public:
	RemoteDebugLogger(GCPtr<IRemoteDebugger>rd);
	virtual~RemoteDebugLogger();

	void log(const core::string &msg,const core::string &info,ELogLevel level);
	void log(const core::string &msg,ELogLevel level);

	void startSection(const core::string &name);
	void endSection(bool Success);

	void flush();

	void close();
};

}
}


#endif //___RemoteDebugLogger___
