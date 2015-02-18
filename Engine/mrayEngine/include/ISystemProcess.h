

/********************************************************************
	created:	2011/06/22
	created:	22:6:2011   11:27
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\ISystemProcess.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	ISystemProcess
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __ISystemProcess__
#define __ISystemProcess__



#include "mtypes.h"
#include "mString.h"

namespace mray
{
namespace OS
{

	class IStreamWriterListener;
	class StreamWriter;
	class StreamReader;

	struct ProcessStartupInfo
	{
		ProcessStartupInfo():pos(0),size(0),showWindow(true),
			inputStream(0),outputStream(0)
		{
		}
		math::Point2d<ushort> pos;
		math::Point2d<ushort> size;
		core::string title;
		bool showWindow;

		StreamReader* inputStream;
		StreamWriter* outputStream;
	};

class ISystemProcess
{
private:
protected:
public:
	ISystemProcess(){}
	virtual~ISystemProcess(){}

	virtual void Close()=0;

	virtual void SetApplicationName(const core::string& name)=0;
	virtual const core::string& GetApplicationName()=0;

	virtual void SetCurrentDirectory(const core::string& name)=0;
	virtual const core::string& GetCurrentDirectory()=0;

	virtual std::vector<core::string>& GetArguments()=0;

	virtual ProcessStartupInfo& GetProcessStartupInfo()=0;

	virtual bool Run()=0;


};

}
}

#endif
