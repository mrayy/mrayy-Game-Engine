

/********************************************************************
	created:	2010/06/25
	created:	25:6:2010   12:31
	filename: 	i:\Programing\GameEngine\mrayEngine\Tools\XEditor\TextLogDevice.h
	file path:	i:\Programing\GameEngine\mrayEngine\Tools\XEditor
	file base:	TextLogDevice
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef TextLogDevice_h__
#define TextLogDevice_h__

#include "ILogDevice.h"
#include <stack>

namespace mray
{

class TextLogDevice:public ILogDevice
{
private:
protected:
	std::stack<core::string> m_sections;
	void recordMessage(const core::string&msg,ELogLevel level);
	void addToBuffer(const core::string&msg);

	CXTListCtrl* logEditBox;
public:
	TextLogDevice();
	virtual ~TextLogDevice();
	virtual void log(const core::string &msg,const core::string &info,ELogLevel level);
	virtual void log(const core::string &msg,ELogLevel level);

	virtual void startSection(const core::string &name);
	virtual void endSection(bool Success);

	virtual void close();

	virtual void flush();

	void SetEditBox(CXTListCtrl* edt);
	
};

}
#endif // TextLogDevice_h__
