#ifndef ___WinFileSystem___
#define ___WinFileSystem___



#include <IFileSystem.h>
#include <mString.h>
#include <mArray.h>
#include <GCPtr.h>
#include <IStream.h>

#include <XMLElement.h>
#include "CompileConfig.h"

namespace mray{
namespace OS{

	class CMemoryStream;

//! has common methods to work with files and directories
class MRAY_Win32_DLL WinFileSystem:public IFileSystem
{
	std::vector<core::string> dirsPaths;
	core::string appPath;
	core::string appName;

public:
	WinFileSystem();
	virtual~WinFileSystem();

	IStreamPtr openFile(const core::string& filename,FILE_MODE mode=BIN_READ);

	bool createDirectory(const core::string& dirName);

	void getExecutableFullLocation(core::string &loc);

	bool fileExist(const core::string&fileName);

	core::string getCurrPath();
	int getCurrDrive();
	core::string  getCurrDriveName();

	core::string getShortFileName(const  core::string&name);

	virtual bool getUserHomeDirectory(core::string&dir);
	virtual bool getTempDirectory(core::string&dir);

	//! copy file from src-->dst
	void copyFile(const core::string& src,const core::string& dst,bool overWrite=0);

	core::string getDriveVolume(char drv);
	void getAllDrivesLetters(std::vector<char>&out);
	
	const core::string& getAppPath();
	const core::string& getAppName();
	void addPath(const core::string& path);
	void removePath(const  core::string&path);
	std::vector<core::string>*getPathes();
	void getCorrectFilePath(const core::string& name,core::string &fileName);

	void createDirs(const core::string&dirPath);

	IStreamPtr createBinaryFileReader(const core::string& fname);
	IStreamPtr createBinaryFileWriter(const core::string& fname);
	IStreamPtr createTextFileReader(const core::string& fname);
	IStreamPtr createTextFileWriter(const core::string& fname);
	
	IStreamPtr createBinaryBufferReader(const core::string& name,byte*data,int size,bool releaseAtEnd);
	IStreamPtr createBinaryBufferWriter(const core::string& fname,byte*data,int size,bool releaseAtEnd);
	IStreamPtr createTextBufferReader(const core::string& fname,byte*data,int size,bool releaseAtEnd);
	IStreamPtr createTextBufferWriter(const core::string& fname,byte*data,int size,bool releaseAtEnd);
	IStreamPtr createMemoryBufferStream(IStreamPtr stream);

	bool deleteFile(const core::string& path) ;

	void getPathFromXML(xml::XMLElement*elem);
};

};//OS
};//mray

#endif






