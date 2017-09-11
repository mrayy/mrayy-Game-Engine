#include "stdafx.h"

#include "logManager.h"
#include "ILogDevice.h"

namespace mray{


LogManager::LogManager():m_verboseLevel(EVL_Normal)
{
	m_messagePending = false;
}

LogManager::~LogManager(){
	/*
	LogDeviceList::iterator it=m_loggers.begin();
	for(;it!=m_loggers.end();++it){
		(*it)=0;
	}*/
	m_loggers.clear();
	m_logHistory.clear();
}

void LogManager::setVerbosLevel(EVerbosLevel vl)
{
	m_verboseLevel=vl;
}
EVerbosLevel LogManager::GetVerbosLevel()
{
	return m_verboseLevel;
}

void LogManager::log(const core::string &msg,const core::string &info,ELogLevel level,EVerbosLevel vl){
// 	LogDeviceList::iterator it=m_loggers.begin();
// 	for(;it!=m_loggers.end();++it){
// 		(*it)->log(msg,info,level);
// 	}

	log(msg+info,level,vl);
}

void LogManager::log(const core::string &msg,ELogLevel level,EVerbosLevel vl){
	_CheckMessagePending();
	if (vl > m_verboseLevel)
		return;

	LogMsg tmp;
	tmp.level=level;
	tmp.msg=msg;
	m_logHistory.push_back(tmp);

	LogDeviceList::iterator it=m_loggers.begin();
	for(;it!=m_loggers.end();++it){
		(*it)->log(msg,level);
	}
	flush();
}


void LogManager::startSection(const core::string &name){
	_CheckMessagePending();
	LogMsg tmp;
	tmp.level=ELL_INFO;
	tmp.msg=mT(" ---------- ")+name+mT(" ---------- ");
	m_logHistory.push_back(tmp);
	LogDeviceList::iterator it=m_loggers.begin();
	for(;it!=m_loggers.end();++it){
		(*it)->startSection(name);
	}
}

void LogManager::endSection(bool Success){
	_CheckMessagePending();
	LogMsg tmp;
	tmp.level=Success ? ELL_SUCCESS:ELL_WARNING;
	tmp.msg=mT(" ---------- [Done] ---------- ");
	m_logHistory.push_back(tmp);
	LogDeviceList::iterator it=m_loggers.begin();
	for(;it!=m_loggers.end();++it){
		(*it)->endSection(Success);
	}
}


void LogManager::close(){
	if (m_messagePending)
	{
		m_logHistory.push_back(m_message);
		m_messagePending = false;
	}
	flush();
	LogDeviceList::iterator it=m_loggers.begin();
	for(;it!=m_loggers.end();++it){
		(*it)->close();
	}
}


void LogManager::addLogDevice(const ILogDevicePtr&logger){
	_CheckMessagePending();
	m_loggers.push_back(logger);

	for (int i=0;i<m_logHistory.size();++i)
	{
		logger->log(m_logHistory[i].msg,m_logHistory[i].level);
	}
}

void LogManager::removeLogDevice(const ILogDevicePtr&logger){
	LogDeviceList::iterator it=m_loggers.begin();
	for(;it!=m_loggers.end();++it){
		if((*it)==logger){
			m_loggers.erase(it);
			return;
		}
	}
}

ILogManager& LogManager::StartLog(ELogLevel level)
{
	_CheckMessagePending();

	m_messagePending = true;
	m_message.level = level;
	return *this;
}

void LogManager::Output(const core::string& v)
{

	if (!m_messagePending)
	{
		m_message.level = ELL_INFO;
		m_messagePending = true;
	}
	m_message.msg += v;
}
void LogManager::_CheckMessagePending()
{

	if (m_messagePending)
	{
		m_logHistory.push_back(m_message);
		LogDeviceList::iterator it = m_loggers.begin();
		for (; it != m_loggers.end(); ++it){
			(*it)->log(m_message.msg, m_message.level);
		}
		m_message.msg = "";
		m_messagePending = false;
	}
}

void LogManager::flush(){
	_CheckMessagePending();
	LogDeviceList::iterator it = m_loggers.begin();
	for(;it!=m_loggers.end();++it){
		(*it)->flush();
	}

}

}