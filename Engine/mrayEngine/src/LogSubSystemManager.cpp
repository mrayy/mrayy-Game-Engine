#include "stdafx.h"

#include "LogSubSystemManager.h"

//default Loggers
#include "GenericLoggerSystem.h"
#include "AnimationLoggerSystem.h"
#include "GUILoggerSystem.h"
#include "NetworkLoggerSystem.h"
#include "OSLoggerSystem.h"
#include "PhysicsLoggerSystem.h"
#include "SceneLoggerSystem.h"
#include "ScriptLoggerSystem.h"
#include "SoundLoggerSystem.h"
#include "VideoLoggerSystem.h"

namespace mray
{

LogSubSystemManager::LogSubSystemManager()
{
	RegisterLogger(new LoggerSystemGeneric());
	RegisterLogger(new LoggerSystemAnimation());
	RegisterLogger(new LoggerSystemGUI());
	RegisterLogger(new LoggerSystemNetwork());
	RegisterLogger(new LoggerSystemOS());
	RegisterLogger(new LoggerSystemPhysics());
	RegisterLogger(new LoggerSystemScene());
	RegisterLogger(new LoggerSystemScript());
	RegisterLogger(new LoggerSystemSound());
	RegisterLogger(new LoggerSystemVideo());
}
LogSubSystemManager::~LogSubSystemManager()
{
	std::list<LogSubsystem*>::iterator it= m_loggers.begin();
	for(;it!=m_loggers.end();++it)
	{
		delete *it;
	}
	m_loggers.clear();
}


void LogSubSystemManager::RegisterLogger(LogSubsystem*l)
{
	m_loggers.push_back(l);
}
void LogSubSystemManager::RemoveLogger(LogSubsystem*l)
{
	std::list<LogSubsystem*>::iterator it= m_loggers.begin();
	for(;it!=m_loggers.end();++it)
	{
		if(*it==l)
		{
			m_loggers.erase(it);
			break;
		}
	}
}

}

