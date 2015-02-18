
/********************************************************************
	created:	2009/01/02
	created:	2:1:2009   16:42
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\IFileSystem.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	IFileSystem
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___IFileSystem___
#define ___IFileSystem___


#include "mString.h"

#include "ISingleton.h"
#include "GCPtr.h"
#include "IStream.h"

#include "XMLElement.h"

namespace mray{
namespace OS{
	class CMemoryStream;

//! has common methods to work with files and directories
class MRAY_DLL IFileSystem:public ISingleton<IFileSystem>
{

public:
	IFileSystem(){}
	virtual~IFileSystem(){}

	virtual IStreamPtr openFile(const core::string& filename,FILE_MODE mode=BIN_READ)=0;

	virtual bool createDirectory(const core::string& dirName)=0;

	virtual void getExecutableFullLocation(core::string &loc)=0;

	virtual bool fileExist(const core::string&fileName)=0;

	virtual core::string getCurrPath()=0;
	virtual int getCurrDrive()=0;
	virtual core::string  getCurrDriveName()=0;

	virtual  core::string getShortFileName(const core::string& name)=0;

	virtual bool getUserHomeDirectory(core::string&dir)=0;
	virtual bool getTempDirectory(core::string&dir)=0;

	//! copy file from src-->dst
	virtual void copyFile(const core::string& src,const core::string& dst,bool overWrite=0)=0;

	virtual void getAllDrivesLetters(std::vector<char>&out)=0;
	virtual core::string getDriveVolume(char drv)=0;


	virtual const  core::string&getAppPath()=0;
	virtual const  core::string&getAppName()=0;
	virtual void addPath(const  core::string&path)=0;
	virtual void removePath(const  core::string&path)=0;
	virtual std::vector<core::string>*getPathes()=0;
	virtual void getCorrectFilePath(const  core::string&name,core::string &fileName)=0;

	virtual void createDirs(const core::string&dirPath)=0;

	virtual IStreamPtr createBinaryFileReader(const  core::string&fname)=0;
	virtual IStreamPtr createBinaryFileWriter(const  core::string&fname)=0;
	virtual IStreamPtr createTextFileReader(const  core::string&fname)=0;
	virtual IStreamPtr createTextFileWriter(const  core::string&fname)=0;

	virtual IStreamPtr createBinaryBufferReader(const  core::string&name,byte*data,int size,bool releaseAtEnd)=0;
	virtual IStreamPtr createBinaryBufferWriter(const  core::string&fname,byte*data,int size,bool releaseAtEnd)=0;
	virtual IStreamPtr createTextBufferReader(const  core::string&fname,byte*data,int size,bool releaseAtEnd)=0;
	virtual IStreamPtr createTextBufferWriter(const  core::string&fname,byte*data,int size,bool releaseAtEnd)=0;
	virtual IStreamPtr createMemoryBufferStream(IStreamPtr stream)=0;

	virtual bool deleteFile(const core::string& path) = 0;

	virtual void getPathFromXML(xml::XMLElement*elem)=0;


};

#define gFileSystem mray::OS::IFileSystem::getInstance()

}
}

#endif //___IFileSystem___
