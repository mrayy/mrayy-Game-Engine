/********************************************************************
	created:	2009/01/02
	created:	2:1:2009   16:47
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\IDirOS.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	IDirOS
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___IDirOS___
#define ___IDirOS___

#include "mString.h"


namespace mray{
namespace OS{


struct SFileData
{
	core::string name;
	int size;
	bool isDir;
};
class MRAY_DLL IDirOS
{
public:
	IDirOS(){}
	virtual ~IDirOS(){}


	virtual const  core::string&  getCurrPath()=0;
	virtual const std::vector<SFileData>&getFiles()=0;

	virtual const  core::string& getShortFileName(int index)=0;
	virtual core::string getFullFileName(int index)=0;
	virtual int getFileSize(int index)=0;
	virtual bool isFileDirectory(int index)=0;

	virtual bool changeDir(const  core::string& path)=0;
	virtual bool changeDir(int index)=0;
	virtual bool changeDir()=0;

	virtual int getFileIndex(const  core::string& fName)=0;
	virtual int getFilesCount()=0;

	//! return file extension
	virtual core::string getFileType(int index)=0;

	virtual char getCurrDriveLetter()=0;
	virtual core::string getCurrDriveVolume()=0;


	virtual bool createDirectory(const  core::string& dirName)=0;

};

}
}


#endif //___IDirOS___