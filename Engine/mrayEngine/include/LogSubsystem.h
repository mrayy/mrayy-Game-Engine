

/********************************************************************
	created:	2011/01/02
	created:	2:1:2011   23:10
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\LogSubsystem.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	LogSubsystem
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	this will reformat the log string to a certain class based on the system type
*********************************************************************/

#ifndef LogSubsystem_h__
#define LogSubsystem_h__

#include "ILogManager.h"
#include "ILogDevice.h"


namespace mray
{

class MRAY_DLL LogSubsystem
{
private:
protected:
	core::string m_systemType;
	std::list<ILogDevicePtr> m_logDevices;
public:
	LogSubsystem(const core::string &type);
	virtual ~LogSubsystem();

	const core::string& GetType(){return m_systemType;}

	virtual void log(const core::string &msg,ELogLevel level,EVerbosLevel vl=EVL_Normal);

	virtual void AddLogDevice(const ILogDevicePtr&logger);
	virtual void RemoveLogDevice(const ILogDevicePtr&logger);
	virtual void ClearLogDevices();
};

#define DECLARE_LOGGER_SYSTEM(SystemName)\
class MRAY_DLL LoggerSystem##SystemName:public ISingleton<LoggerSystem##SystemName>,public LogSubsystem\
{\
public:\
	 LoggerSystem##SystemName():LogSubsystem(mT(#SystemName)){}\
	virtual ~LoggerSystem##SystemName (){}\
};

}
#endif // LogSubsystem_h__

