/********************************************************************
	created:	2009/01/09
	created:	9:1:2009   21:45
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\ILogDevice.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine
	file base:	ILogDevice
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___ILogDevice___
#define ___ILogDevice___

#include "mstring.h"
#include "GCPtr.h"

namespace mray{
	enum ELogLevel;

class ILogDevice
{
public:
	virtual ~ILogDevice(){}
	virtual void log(const core::string &msg,const core::string &info,ELogLevel level)=0;
	virtual void log(const core::string &msg,ELogLevel level)=0;

	virtual void startSection(const core::string &name)=0;
	virtual void endSection(bool Success)=0;

	virtual void close()=0;

	virtual void flush()=0;
};
MakeSharedPtrType(ILogDevice)

}


#endif //___ILogDevice___

