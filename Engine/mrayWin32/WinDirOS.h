
#ifndef ___WinDirOS___
#define ___WinDirOS___

#include <IDirOS.h>
#include "CompileConfig.h"

namespace mray{
namespace OS{

class MRAY_Win32_DLL WinDirOS:public IDirOS
{
protected:

	core::string m_directory;
	std::vector<SFileData> m_files;
	char m_driveLetter;
	core::string m_driveVolume;

	/*
	core::string cpath;


	core::string tempPath;*/


	void refreshFiles();
public:
	WinDirOS();
	virtual ~WinDirOS();

	
	const  core::string&  getCurrPath();
	const  std::vector<SFileData>&getFiles();

	const  core::string& getShortFileName(int index);
	core::string getFullFileName(int index);
	int getFileSize(int index);
	bool isFileDirectory(int index);

	bool changeDir(const  core::string& path);
	bool changeDir(int index);
	bool changeDir();

	int getFileIndex(const  core::string& fName);
	int getFilesCount();

	//! return file extension
	core::string getFileType(int index);

	char getCurrDriveLetter();
	core::string getCurrDriveVolume();

	
	bool createDirectory(const  core::string& dirName);
	
};

}
}

#endif




