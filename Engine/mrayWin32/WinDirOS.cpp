#include "stdafx.h"

#include "WinDirOS.h"
#include "WinFileSystem.h"

#include <io.h>
#include <direct.h>
#include <windows.h>

#ifdef UNICODE
#define ChangeDir _wchdir
#else
#define ChangeDir _chdir
#endif

namespace mray{
namespace OS{

WinDirOS::WinDirOS(){

	//tempPath.resize(MAX_PATH);

	refreshFiles();
}
WinDirOS::~WinDirOS(){
//	m_files.clear();
}

void WinDirOS::refreshFiles()
{
	m_driveLetter=gFileSystem.getCurrDrive()+'A'-1;
	m_driveVolume = gFileSystem.getCurrDriveName();
	m_files.clear();
	WIN32_FIND_DATA fd;
	HANDLE hFile;
	SFileData fData;
	if ((hFile = FindFirstFile((m_directory+"*").c_str(), &fd)) != 0)
	{
		do{
			//	char_to_wchar(fname,fd.name);
			fData.name = fd.cFileName;
			fData.size = (int)fd.nFileSizeLow;
			fData.isDir = (_A_SUBDIR & fd.dwFileAttributes) != 0;
			m_files.push_back(fData);

		} while (FindNextFile(hFile, &fd));
		FindClose(hFile);
	}
	/*
	cpath=gFileSystem.getCurrPath();



	
	if( (hFile = FindFirstFile( mT("*"), &fd )) != 0 )
	{
		do{
		//	char_to_wchar(fname,fd.name);
			fData.name=fd.cFileName;
			fData.size=(int)fd.nFileSizeLow;
			fData.isDir=(_A_SUBDIR & fd.dwFileAttributes)!=0;
			m_files.push_back(fData);

		}
		while(FindNextFile(hFile,&fd));
		FindClose(hFile);
	}*/

}
	
const  core::string& WinDirOS::getCurrPath(){
	return m_directory;
}

const std::vector<SFileData> & WinDirOS::getFiles(){
	return m_files;
}


const  core::string& WinDirOS::getShortFileName(int index){
	if (index<0 || index>m_files.size())return core::string::Empty;
	return m_files[index].name;
}

  core::string WinDirOS::getFullFileName(int index){
	if (index<0 || index>m_files.size())
		return core::string::Empty;
	
	//wsprintf(&tempPath[0],mT("%s\\%s"),cpath.c_str(),m_files[index].name.c_str());

	//tempPath=cpath+mT("\\")+m_files[index].name;

	return m_directory+m_files[index].name;
}

int WinDirOS::getFileSize(int index)
{
	if (index<0 || index>m_files.size())return 0;
	return m_files[index].size;
}

bool WinDirOS::isFileDirectory(int index)
{
	if (index<0 || index>m_files.size())return 0;
	return m_files[index].isDir;
}

bool WinDirOS::changeDir(int index)
{
	if (index<0 || index>m_files.size())return 0;
	if(ChangeDir((m_directory+m_files[index].name).c_str())==-1)return 0;
	refreshFiles();
	return 1;
}


bool WinDirOS::changeDir()
{
	if(_chdir("..")==-1)return 0;
	refreshFiles();
	return 1;
}
bool WinDirOS::changeDir(const  core::string& path)
{
	m_directory = path;
	if(ChangeDir(path.c_str())==-1)return 0;
	refreshFiles();
	return 1;
}

int WinDirOS::getFilesCount()
{
	return m_files.size();
}
	



int WinDirOS::getFileIndex(const  core::string& fName){
	for(int i=0;i<m_files.size();++i)
	{
		if(m_files[i].name.equals_ignore_case(fName))
			return i;
	}
	return -1;
}

char WinDirOS::getCurrDriveLetter(){
	return m_driveLetter;
}

core::string  WinDirOS::getCurrDriveVolume(){
	return m_driveVolume;
}


bool WinDirOS::createDirectory(const  core::string& dirName){
	bool ret=gFileSystem.createDirectory(dirName);
	if(ret)
		refreshFiles();
	return ret;
}


//! return file extension
core::string WinDirOS::getFileType(int index){
	if(index<0 || index>m_files.size())return core::string::Empty;
	int p=m_files[index].name.findlast('.');
	if(p==-1)return core::string::Empty;
	return m_files[index].name.substr(p+1,m_files[index].name.length()-p-1);
}


}
}



