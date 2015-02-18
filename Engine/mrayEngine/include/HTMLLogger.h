
/********************************************************************
	created:	2009/02/28
	created:	28:2:2009   23:38
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\HTMLLogger.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	HTMLLogger
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___HTMLLogger___
#define ___HTMLLogger___

#include "GCPtr.h"
#include "ILogDevice.h"
#include "IStream.h"
#include "StreamWriter.h"

#include <stack>


namespace mray{

class MRAY_DLL HTMLLogger:public ILogDevice
{
	OS::IStreamPtr m_stream;
	bool m_withTimeStamp;
	int m_maxMessages;

	GCPtr<OS::IMutex> m_mutex;
	std::vector<core::string> m_buffer;

	GCPtr<OS::StreamWriter> m_writer;

	std::stack<core::string> m_sections;

	core::string&getTimeStamp();
	void addToBuffer(const core::string&msg);

	void recordMessage(const core::string&msg,ELogLevel level);
public:


	HTMLLogger(bool withTimeStamp);
	virtual~HTMLLogger();
	bool setStream(OS::IStreamPtr stream);
	void log(const core::string &msg,const core::string &info,ELogLevel level);
	void log(const core::string &msg,ELogLevel level);

	void startSection(const core::string &name);
	void endSection(bool Success);

	void flush();

	void close();
};


}


#endif //___HTMLLogger___
