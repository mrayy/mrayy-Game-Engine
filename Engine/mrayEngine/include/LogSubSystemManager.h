


/********************************************************************
	created:	2011/01/17
	created:	17:1:2011   0:02
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\LogSubSystemManager.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	LogSubSystemManager
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef LogSubSystemManager_h__
#define LogSubSystemManager_h__

#include "ISingleton.h"
#include "CompileConfig.h"


namespace mray
{
	class LogSubsystem;

class MRAY_DLL LogSubSystemManager:public ISingleton<LogSubSystemManager>
{
private:
protected:
	std::list<LogSubsystem*> m_loggers;
public:
	LogSubSystemManager();
	virtual ~LogSubSystemManager();

	const std::list<LogSubsystem*>& GetLoggers(){return m_loggers;}

	void RegisterLogger(LogSubsystem*l);
	void RemoveLogger(LogSubsystem*l);
	
};

}
#endif // LogSubSystemManager_h__
