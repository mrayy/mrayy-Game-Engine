
/********************************************************************
	created:	2009/01/09
	created:	9:1:2009   21:47
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\StreamLogger.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	StreamLogger
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___StreamLogger___
#define ___StreamLogger___



#include "GCPtr.h"
#include "ILogDevice.h"
#include "IStream.h"
#include "StreamWriter.h"

#include <stack>


namespace mray{
	namespace OS{

		class IMutex;
	}

//! log print class
class MRAY_DLL StreamLogger:public ILogDevice
{
	OS::IStreamPtr m_stream;
	bool m_withTimeStamp;
	int m_maxMessages;

	OS::IMutex* m_mutex;

	std::vector<core::string> m_buffer;

	std::stack<core::string> m_sections;
	GCPtr<OS::StreamWriter> m_writer;

	void addToBuffer(const core::string&msg);

	core::string getTimeStamp();

	void recordMessage(const core::string&msg,ELogLevel level);
public:


	StreamLogger(bool withTimeStamp);
	virtual~StreamLogger();
	bool setStream(OS::IStreamPtr stream);
	void log(const core::string &msg,const core::string &info,ELogLevel level);
	void log(const core::string &msg,ELogLevel level);

	void startSection(const core::string &name);
	void endSection(bool Success);

	void flush();

	void close();
};


};//mray



#endif //___StreamLogger___



