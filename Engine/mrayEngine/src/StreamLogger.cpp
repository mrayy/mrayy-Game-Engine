#include "stdafx.h"
#include "StreamLogger.h"
#include "IFileSystem.h"
#include "IStream.h"
#include "TraceManager.h"
#include "IErrorDialog.h"
#include "ILogManager.h"
#include "StringConverter.h"
#include "IThreadManager.h"
#include "IMutex.h"
#include <time.h>

namespace mray{


StreamLogger::StreamLogger(bool withTimeStamp){
	m_withTimeStamp=withTimeStamp;

	m_maxMessages=10;
	m_buffer.reserve(m_maxMessages);

	m_writer=new OS::StreamWriter();

	m_mutex=OS::IThreadManager::getInstance().createMutex();
}
StreamLogger::~StreamLogger(){
	flush();
	m_stream=0;
	m_buffer.clear();

	delete m_mutex;
}

core::string StreamLogger::getTimeStamp(){
   time_t TempTime;
   ::tm *Time;
   time(&TempTime);
   Time = localtime(&TempTime);
   
   core::string TimeStamp;
   TimeStamp=mT("[");
   TimeStamp+=core::StringConverter::toString(Time->tm_hour);
   TimeStamp+=mT(":");
   TimeStamp+=core::StringConverter::toString(Time->tm_min);
   TimeStamp+=mT(":");
   TimeStamp+=core::StringConverter::toString(Time->tm_sec);
   TimeStamp+=mT("]");

   return TimeStamp;
}


bool StreamLogger::setStream(OS::IStreamPtr stream)
{
	if(m_stream && m_stream->isOpen())m_stream->close();
	//m_stream=gFileSystem.createTextFileWriter(logFile.c_str());
	m_stream=stream;
	m_writer->setStream(stream);
	if(m_stream){
		if(m_withTimeStamp){
			core::string time=getTimeStamp();
			m_writer->writeString((mT("****************** Logger Started at: ")+time+mT(" ******************\n")).c_str());
		}
		m_stream->close();
		return 1;
	}
	return 0;
}

void StreamLogger::addToBuffer(const core::string&msg){
	m_mutex->lock();
	m_buffer.push_back(msg);
	m_mutex->unlock();
	if(m_buffer.size()>=m_maxMessages){
		flush();
	}
}
void StreamLogger::recordMessage(const core::string&msg,ELogLevel level){
	
	core::string finalMsg=mT("");
	if(m_withTimeStamp){
		finalMsg=getTimeStamp();
		finalMsg+=mT(": ");
	}
	finalMsg+=msg;
	addToBuffer(finalMsg);
	if(level==ELL_ERROR){
		flush();
		if(TraceManager::isExist())
			gTraceManager.flushToLogger();
		
		if(IErrorDialog::isExist())
			IErrorDialog::getInstance().run(core::StringConverter::toString(msg));

		exit(0);
	}
}

void StreamLogger::log(const core::string &msg,const core::string &info,ELogLevel level)
{
	core::string finalMsg;

	finalMsg=gLogManager.getLogLevelStr(level);

	int d=m_sections.size();
	core::string tab;
	for(int i=0;i<d;++i)
		tab+=mT("\t");

	finalMsg+=mT("-->")+tab+msg+mT(":");
	finalMsg+=info+mT("\n");
	
	recordMessage(finalMsg,level);


}
void StreamLogger::log(const core::string &msg,ELogLevel level)
{
	core::string finalMsg;

	int d=m_sections.size();
	core::string tab;
	for(int i=0;i<d;++i)
		tab+=mT("\t");


	finalMsg=gLogManager.getLogLevelStr(level);
	finalMsg+=mT("-->")+tab+msg+mT("\n");
	
	recordMessage(finalMsg,level);
}
void StreamLogger::close(){
	if(!m_stream)return;
	if(m_stream->isOpen())
		m_stream->close();
}

void StreamLogger::startSection(const core::string &name){
	m_sections.push(name);

	log(mT("------ ")+name+mT(" ------"),ELL_INFO);
}
void StreamLogger::endSection(bool Success){
	FATAL_ERROR(m_sections.empty(),mT("endSection Without startSection!"));
	core::string last=m_sections.top();
	log(mT("------ ")+last+mT(" [Done] ------"),ELL_INFO);
	m_sections.pop();
}

void StreamLogger::flush(){
	if(!m_stream)return;
	if(m_stream->reopen(OS::TXT_APPEND))
	{
		m_mutex->lock();
		for(int i=0;i<m_buffer.size();++i){
			m_writer->writeString(m_buffer[i].c_str());
		}
		m_mutex->unlock();
		m_stream->close();
	}
	m_buffer.clear();
}


}