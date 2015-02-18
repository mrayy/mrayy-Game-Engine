

/********************************************************************
	created:	2014/01/29
	created:	29:1:2014   14:05
	filename: 	C:\Development\mrayEngine\Engine\mrayEngine\include\IMonitorFileInformation.h
	file path:	C:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	IMonitorFileInformation
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __IMonitorFileInformation__
#define __IMonitorFileInformation__


#include "mString.h"

namespace mray
{
namespace OS
{

	enum class EMonitorAction
	{
		None,
		Delete,
		Create,
		Change
	};
class IMonitorFileInformation
{
protected:
public:
	IMonitorFileInformation(){}
	virtual~IMonitorFileInformation(){}

	virtual core::string GetDirectory()const = 0;
	virtual core::string GetPath()const = 0;
	virtual core::string GetFileName()const = 0;
	virtual int GetFileSize()const = 0;
	virtual bool IsFileExist()const = 0;
	virtual bool IsSystem()const = 0;
	virtual bool IsReadOnly()const = 0;
	virtual bool IsHidden()const = 0;
	virtual bool IsNormal()const = 0;
	virtual bool IsArchive()const = 0;
	virtual bool IsDirectory()const = 0;
	virtual bool IsCurrentRoot()const = 0;
	virtual bool IsRoot()const = 0;
	virtual bool IsParentDirectory()const = 0;
	virtual bool IsTemporary()const = 0;
	virtual bool IsActualFile()const = 0;
	virtual bool IsOk()const = 0;
};

typedef std::list<IMonitorFileInformation*> MonitorFileList;

}
}


#endif
