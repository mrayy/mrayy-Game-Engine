/********************************************************************
	created:	2009/01/09
	created:	9:1:2009   21:44
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\LogManager.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine
	file base:	LogManager
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___LogManager___
#define ___LogManager___

#include "ILogManager.h"
#include "GCPtr.h"




namespace mray{


class MRAY_CORE_DLL LogManager:public ILogManager
{
	typedef std::list<ILogDevicePtr> LogDeviceList;
	LogDeviceList m_loggers;

	EVerbosLevel m_verboseLevel;

	struct LogMsg{
		core::string msg;
		ELogLevel level;
	};
	LogMsg m_message;
	bool m_messagePending;

	std::vector<LogMsg> m_logHistory;
	virtual void Output(const core::string& v);
	void _CheckMessagePending();
public:

	LogManager();
	virtual~LogManager();

	virtual void setVerbosLevel(EVerbosLevel vl);
	virtual EVerbosLevel GetVerbosLevel();

	virtual void log(const core::string &msg,const core::string &info,ELogLevel level,EVerbosLevel vl=EVL_Normal);
	virtual void log(const core::string &msg,ELogLevel level,EVerbosLevel vl=EVL_Normal);

	virtual void startSection(const core::string &name);
	virtual void endSection(bool Success);

	virtual void close();

	virtual ILogManager& StartLog(ELogLevel level);

	virtual void addLogDevice(const ILogDevicePtr&logger);
	virtual void removeLogDevice(const ILogDevicePtr&logger);

	virtual void flush();
};

}


#endif //___LogManager___