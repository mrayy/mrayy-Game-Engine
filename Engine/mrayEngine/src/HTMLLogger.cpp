#include "stdafx.h"

#include "HTMLLogger.h"
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



HTMLLogger::HTMLLogger(bool withTimeStamp){
	m_withTimeStamp=withTimeStamp;

	m_mutex=OS::IThreadManager::getInstance().createMutex();
	m_maxMessages=10;
	m_buffer.reserve(m_maxMessages);

	m_writer=new OS::StreamWriter();
}
HTMLLogger::~HTMLLogger(){
	flush();
	if(m_stream)
		m_writer->writeString(mT("</body>\n</html>\n"));
	m_stream=0;
	m_buffer.clear();
}
void HTMLLogger::addToBuffer(const core::string&msg){
	m_mutex->lock();
	m_buffer.push_back(msg);
	m_mutex->unlock();
	if(m_buffer.size()>=m_maxMessages){
		flush();
	}
}

core::string&HTMLLogger::getTimeStamp(){
	time_t TempTime;
	::tm *Time;
	time(&TempTime);
	Time = localtime(&TempTime);

	static core::string TimeStamp;
	TimeStamp=mT("[");
	TimeStamp+=Time->tm_hour;
	TimeStamp+=mT(":");
	TimeStamp+=Time->tm_min;
	TimeStamp+=mT(":");
	TimeStamp+=Time->tm_sec;
	TimeStamp+=mT("]");

	return TimeStamp;
}


bool HTMLLogger::setStream(OS::IStreamPtr stream)
{
	if(m_stream && m_stream->isOpen())m_stream->close();
	//m_stream=gFileSystem.createTextFileWriter(logFile.c_str());
	m_stream=stream;
	m_writer->setStream(stream);
	if(m_stream){
		m_stream->reopen(OS::BIN_WRITE);
		m_stream->seek(0,OS::ESeek_Set);
		m_writer->writeString(mT("<html>\n"));
			m_writer->writeString(mT("<head>\n"));
				m_writer->writeString(mT("<title>"));
				m_writer->writeString(mT("M-RAY Engine Log file"));
				m_writer->writeString(mT("</title>\n"));
			m_writer->writeString(mT("</head>\n"));
			m_writer->writeString(mT("<body>\n"));
		if(m_withTimeStamp){
			core::string time=getTimeStamp();
			m_writer->writeString((mT("<strong><em>Logger Started at:")+time+mT("</em></strong>\n")).c_str());

		}
		m_stream->close();
		return 1;
	}
	return 0;
}

void HTMLLogger::recordMessage(const core::string&msg,ELogLevel level){

	core::string finalMsg=mT("");

	if(m_withTimeStamp){

		finalMsg=(mT("<strong><em>")+getTimeStamp()+mT("</em></strong> : ")).c_str();
	}
	finalMsg+=msg;
	finalMsg+=mT("</br>");
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

void HTMLLogger::log(const core::string &msg,const core::string &info,ELogLevel level)
{
	core::string finalMsg;

	finalMsg=mT("<span style=\"color: ");
	switch(level){
		case ELL_INFO:
			finalMsg+=mT("#0033ff");//blue
			break;
		case ELL_SUCCESS:
			finalMsg+=mT("#009900");//green
			break;
		case ELL_WARNING:
			finalMsg+=mT("#ffcc00");//orange
			break;
		case ELL_ERROR:
			finalMsg+=mT("#cc0000");//red
			break;
	}

	finalMsg+=mT("\">");
	finalMsg+=gLogManager.getLogLevelStr(level);
	finalMsg+=mT(" : </span>");

	int d=m_sections.size();
	core::string tab;
	for(int i=0;i<d;++i)
		tab+=mT("\t");

	finalMsg+=mT("-->")+tab+msg+mT(":");
	finalMsg+=info;

	recordMessage(finalMsg,level);


}
void HTMLLogger::log(const core::string &msg,ELogLevel level)
{
	core::string finalMsg;

	int d=m_sections.size();
	core::string tab;
	for(int i=0;i<d;++i)
		tab+=mT("\t");


	finalMsg=mT("<span style=\"color: ");
	switch(level){
		case ELL_INFO:
			finalMsg+=mT("#0033ff");//blue
			break;
		case ELL_SUCCESS:
			finalMsg+=mT("#009900");//green
			break;
		case ELL_WARNING:
			finalMsg+=mT("#ffcc00");//orange
			break;
		case ELL_ERROR:
			finalMsg+=mT("#cc0000");//red
			break;
	}


	finalMsg+=mT("\">");
	finalMsg+=gLogManager.getLogLevelStr(level);
	finalMsg+=mT(" : </span>");

	finalMsg+=mT("-->")+tab+msg+mT("\n");

	recordMessage(finalMsg,level);
}
void HTMLLogger::close(){
	if(!m_stream)return;
	if(m_stream->isOpen())
		m_stream->close();
}

void HTMLLogger::startSection(const core::string &name){
	log(mT(" ---------- ")+name+mT(" ---------- "),ELL_INFO);
	m_sections.push(name);
}
void HTMLLogger::endSection(bool Success){
	FATAL_ERROR(m_sections.empty(),mT("endSection Without startSection!"));
	core::string last=m_sections.top();
	m_sections.pop();
	log(mT(" ---------- ")+last+mT(" [Done] ---------- "),ELL_INFO);
}

void HTMLLogger::flush(){
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
