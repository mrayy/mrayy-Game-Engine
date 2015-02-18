

/********************************************************************
	created:	2014/01/29
	created:	29:1:2014   14:17
	filename: 	C:\Development\mrayEngine\Engine\mrayWin32\WinMonitorFileInformation.h
	file path:	C:\Development\mrayEngine\Engine\mrayWin32
	file base:	WinMonitorFileInformation
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __WinMonitorFileInformation__
#define __WinMonitorFileInformation__

#include "IMonitorFileInformation.h"
#include <windows.h>

namespace mray
{
namespace OS
{

class WinMonitorFileInformation:public IMonitorFileInformation
{
protected:
	core::string m_dir;
	WIN32_FIND_DATA m_fd;

public:
	WinMonitorFileInformation(WIN32_FIND_DATA& fd,const core::string &dir);
	virtual~WinMonitorFileInformation();

	core::string GetDirectory()const;
	core::string GetPath()const;
	core::string GetFileName()const;
	int GetFileSize()const ;
	bool IsFileExist()const;
	bool IsSystem()const;
	bool IsReadOnly()const;
	bool IsHidden()const;
	bool IsNormal()const;
	bool IsArchive()const;
	bool IsDirectory()const;
	bool IsCurrentRoot()const;
	bool IsRoot()const ;
	bool IsParentDirectory()const;
	bool IsTemporary()const;
	bool IsActualFile()const;
	bool IsOk()const;

	FILETIME GetLastWriteTime()const;

	bool equlas(const WinMonitorFileInformation* o)const;

	static core::string ConcPath(const core::string& root, const core::string& sub);
	static int EnumFiles(const core::string& root, MonitorFileList &lst);
	static void ClearFiles(MonitorFileList &lst);
	static EMonitorAction Compare(const MonitorFileList &oldLst, const MonitorFileList &newLst, IMonitorFileInformation*& f);
};

}
}


#endif
