#include "stdafx.h"

#include "LogSubsystem.h"


namespace mray
{

LogSubsystem::LogSubsystem(const core::string &type):m_systemType(type)
{
}
LogSubsystem::~LogSubsystem()
{
	ClearLogDevices();
}

void LogSubsystem::log(const core::string &msg,ELogLevel level,EVerbosLevel vl)
{
	core::string logMsg=mT("[")+m_systemType+mT("]:")+msg;
	gLogManager.log(logMsg,level,vl);

	std::list<ILogDevicePtr>::iterator it=m_logDevices.begin();
	for(;it!=m_logDevices.end();++it)
	{
		(*it)->log(msg,level);
	}
}

void LogSubsystem::AddLogDevice(const ILogDevicePtr&logger)
{
	m_logDevices.push_back(logger);
}
void LogSubsystem::RemoveLogDevice(const ILogDevicePtr&logger)
{
	std::list<ILogDevicePtr>::iterator it=m_logDevices.begin();
	for(;it!=m_logDevices.end();++it)
	{
		if(logger==*it)
		{
			m_logDevices.erase(it);
			return;
		}
	}
}
void LogSubsystem::ClearLogDevices()
{/*
	std::list<ILogDevicePtr>::iterator it=m_logDevices.begin();
	for(;it!=m_logDevices.end();++it)
	{
		delete *it;
	}*/
	m_logDevices.clear();
}

}

