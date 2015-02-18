#include "stdafx.h"

#include "RemoteDebugLogger.h"
#include "ILogManager.h"



namespace mray{
namespace debug{


RemoteDebugLogger::RemoteDebugLogger(GCPtr<IRemoteDebugger> rd){
	m_rd=rd;
}
RemoteDebugLogger::~RemoteDebugLogger(){
}

void RemoteDebugLogger::log(const core::string &msg,const core::string &info,ELogLevel level){

	core::string finalMsg;

	finalMsg=gLogManager.getLogLevelStr(level);

	finalMsg+=mT("-->")+msg+mT(":");
	finalMsg+=info+mT("\n");

	m_rd->addLogMsg(finalMsg);
}
void RemoteDebugLogger::log(const core::string &msg,ELogLevel level){
	core::string finalMsg=mT("");
	finalMsg+=msg;
	m_rd->addLogMsg(finalMsg);
}

void RemoteDebugLogger::startSection(const core::string &name){
}
void RemoteDebugLogger::endSection(bool Success){
}

void RemoteDebugLogger::flush(){
}

void RemoteDebugLogger::close(){
}
}
}



