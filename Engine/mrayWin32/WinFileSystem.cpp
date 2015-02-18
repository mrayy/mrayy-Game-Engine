#include "stdafx.h"

#include "WinFileSystem.h"
#include "ILogManager.h"
#include "FileResourceManager.h"

#define WIN32_LEAN_AND_MEAN
#include <io.h>
#include <direct.h>
#include <windows.h>
#include <shlobj.h>

#include "CPakLoader.h"
#include "CZipLoader.h"
#include "WinFile.h"
#include "CMemoryStream.h"
#include "TraceManager.h"
#include "ArchiveManager.h"
#include "StringUtil.h"

#ifdef UNICODE
#else
#endif
 
namespace mray{


namespace OS{


const int gBufferSize=2048;
 mchar g_globalBuffer[gBufferSize];

WinFileSystem::WinFileSystem()
{
	#ifdef ___DEBUG___
	setDebugName("WinFileSystem");
	#endif

	
	getExecutableFullLocation(appPath);
	int i=appPath.findlast('\\');
	if(i!=-1){
		appName=appPath.substr(i,appPath.length()-i);
		appPath.resize(i+1);
	}

}


WinFileSystem::~WinFileSystem()
{
	dirsPaths.clear();
}

IStreamPtr WinFileSystem::openFile(const  core::string&filename,FILE_MODE mode)
{

	IStreamPtr f;

	if(FileResourceManager::isExist())
	{
		f=gFileResourceManager.getResource(filename);
		if(f){
			f->reopen(mode);
			f->seek(0,ESeek_Set);
			return f;
		}
	}

	string str;
	if(mode==BIN_READ || mode==TXT_READ){
		//search in archives
		if(ArchiveManager::isExist())
		{
			f= ArchiveManager::getInstance().searchForFile(filename,mode);
			if(f){
				if(FileResourceManager::isExist())
					gFileResourceManager.addResource(f,filename);
				return f;
			}
		}

		getCorrectFilePath(filename,str);
		if(mode!=BIN_APPEND && mode!=TXT_APPEND){
			if(str==mT(""))
			{
				gLogManager.log(mT("WinFileSystem::openFile - file not found: ")+ filename,ELL_WARNING);
				return 0;
			}
		}else if(str==mT(""))
			str=filename;
	}else
		str=filename;

	f= new WinFile(str,mode);
	if(f && FileResourceManager::isExist())
	{
		gFileResourceManager.addResource(f,filename);
	}
	if(f.isNull())
	{
		gLogManager.log(mT("Failed to open file: ")+filename,ELL_WARNING);
	}
	return f;
}

bool WinFileSystem::fileExist(const core::string&fileName){

	HANDLE fp=CreateFile(fileName.c_str(),GENERIC_READ, FILE_SHARE_READ, NULL,OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(fp!=INVALID_HANDLE_VALUE){
		CloseHandle(fp);
		return true;
	}
	return false;
}

bool WinFileSystem::getUserHomeDirectory(core::string&dir){
	
	if(! SHGetSpecialFolderPath( NULL, g_globalBuffer, CSIDL_PERSONAL, false ) ){
		if(! SHGetSpecialFolderPath( NULL, g_globalBuffer, CSIDL_COMMON_DOCUMENTS, false ) )
		{
			return false;
		}
	}
	core::StringUtil::NormalizePathSlashes(g_globalBuffer);
	dir=g_globalBuffer;
	return true;
}

bool WinFileSystem::getTempDirectory(core::string&dir){

	GetTempPath(gBufferSize,g_globalBuffer);
	core::StringUtil::NormalizePathSlashes(g_globalBuffer);
	dir=g_globalBuffer;
	return true;
}

void WinFileSystem::copyFile(const core::string& src,const core::string& dst,bool overWrite){
	CopyFile(src.c_str(),dst.c_str(),overWrite);
}

void WinFileSystem::getExecutableFullLocation(core::string &loc){
	GetModuleFileName(0,g_globalBuffer,gBufferSize);
	core::StringUtil::NormalizePathSlashes(g_globalBuffer);
	loc=g_globalBuffer;
}
core::string WinFileSystem::getCurrPath(){
	GetCurrentDirectory(gBufferSize,g_globalBuffer);
	return g_globalBuffer;
}

int WinFileSystem::getCurrDrive(){
	
	return _getdrive();
}
core::string WinFileSystem::getCurrDriveName(){
	GetVolumeInformation(0,g_globalBuffer,MAX_PATH,0,0,0,0,0);
	return g_globalBuffer;
}

core::string WinFileSystem::getDriveVolume(char drv){
	mchar txt[MAX_PATH];
	mchar drvL[8];
	wsprintf(drvL,mT("%c:\\"),drv);
	if(GetVolumeInformation(drvL,txt,MAX_PATH,0,0,0,0,0))
		return txt;
	return mT("");
}

void WinFileSystem::getAllDrivesLetters(std::vector<char>&out){
	out.clear();

	long drv=GetLogicalDrives();
	for(int i=0;i<25;++i)
		if((1<<i) & drv)
			out.push_back((char)'A'+(char)i);

}



bool WinFileSystem::createDirectory(const  core::string&dirName)
{
	SECURITY_ATTRIBUTES sec;
	bool ret=CreateDirectory(dirName.c_str(),&sec)!=0;
	return ret;
}


core::string WinFileSystem::getShortFileName(const  core::string&name){
	core::string str;
	str=name;
	int p;
	p=str.findlast('\\');
	core::string str2;
	str2=str.substr(p+1,str.length()-p+1);
	return str2.c_str();
}



const core::string& WinFileSystem::getAppPath(){
	return appPath;
}
const core::string& WinFileSystem::getAppName(){
	return appName;
}


void  WinFileSystem::addPath(const  core::string& path)
{
	strCopy(g_globalBuffer,path.c_str());
	core::StringUtil::NormalizePathSlashes(g_globalBuffer);
	for(int i=0;i<dirsPaths.size();++i){
		if(dirsPaths[i].equals_ignore_case(g_globalBuffer))
			return;
	}
	dirsPaths.push_back(g_globalBuffer);
}
void WinFileSystem::removePath(const  core::string&path)
{
	strCopy(g_globalBuffer,path.c_str());
	core::StringUtil::NormalizePathSlashes(g_globalBuffer);
	std::vector<core::string>::iterator it= dirsPaths.begin();
	for(;it!=dirsPaths.end();++it){
		if((*it).equals_ignore_case(g_globalBuffer)){
			dirsPaths.erase(it);
			break;
		}
	}
}
std::vector<string>* WinFileSystem::getPathes()
{
	return &dirsPaths;
}
void WinFileSystem::getCorrectFilePath(const  core::string&name,core::string &fileName)
{
	if(name[0]==0)
	{
		fileName=mT("");
		return;
	}
	//first check if file is exist
	fileName=name;
	fileName.replaceChar('/','\\');
	
	if(fileExist(fileName.c_str()))
		return;
	fileName=appPath+fileName;
	if(fileExist(fileName.c_str())){
		return;
	}
	int i=0;
	bool ok=false;
	while(1)
	{
		if(i<dirsPaths.size()){
			fileName=appPath;
			fileName+=dirsPaths[i];
			if(fileName[fileName.length()-1]!='\\')
				fileName+=string(mT("\\"));
			fileName+=string(name);
			if(fileExist(fileName.c_str())){
				ok=true;
				break;
			}
			i++;
		}else
			break;
	}
	//if its not exist then exit
	if(!ok){
// 		core::string text=mT("file : ")+core::string(name)+mT(" is not exist");
// 		gLogManager.log(text,ELL_WARNING);
		fileName=mT("");
		return;
	}
}

void WinFileSystem::createDirs(const core::string&path){
	core::string dirPath = path;
	if(dirPath==mT(""))return;
	for(int i=0;i<dirPath.length();++i){
		if(dirPath[i]=='\\'){
			dirPath[i]='\0';
			CreateDirectory(dirPath.c_str(),0);
			dirPath[i]='\\';
		}
	}
	if(dirPath[dirPath.length()-1]!='\\')
		CreateDirectory(dirPath.c_str(),0);
}

IStreamPtr WinFileSystem::createBinaryFileReader(const  core::string&fname){

	return openFile(fname,BIN_READ);
}
IStreamPtr WinFileSystem::createBinaryFileWriter(const  core::string&fname){
	return new WinFile(fname,BIN_WRITE);
}
IStreamPtr WinFileSystem::createTextFileReader(const  core::string&fname){
	return openFile(fname,TXT_READ);
}
IStreamPtr WinFileSystem::createTextFileWriter(const  core::string&fname){
	return new WinFile(fname,TXT_WRITE);
}

IStreamPtr WinFileSystem::createBinaryBufferReader(const  core::string&fname,byte*data,int size,bool releaseAtEnd){
	return new CMemoryStream(fname,data,size,releaseAtEnd,BIN_READ);
}
IStreamPtr WinFileSystem::createBinaryBufferWriter(const  core::string&fname,byte*data,int size,bool releaseAtEnd){
	return new CMemoryStream(fname,data,size,releaseAtEnd,BIN_WRITE);
}
IStreamPtr WinFileSystem::createTextBufferReader(const  core::string&fname,byte*data,int size,bool releaseAtEnd){
	return new CMemoryStream(fname,data,size,releaseAtEnd,TXT_READ);
}
IStreamPtr WinFileSystem::createTextBufferWriter(const  core::string&fname,byte*data,int size,bool releaseAtEnd){
	return new CMemoryStream(fname,data,size,releaseAtEnd,TXT_WRITE);
}
IStreamPtr WinFileSystem::createMemoryBufferStream(IStreamPtr stream){

	CMemoryStream* buff=new CMemoryStream();
	int size=stream->length();
	stream->seek(0,ESeek_Set);
	byte*data=new byte[size];
	stream->read(data,size);
	buff->setData(data,size);
	buff->setDeleteAtEnd(true);
	return buff;

}

bool WinFileSystem::deleteFile(const core::string& path)
{
	return DeleteFile(path.c_str());
}


void WinFileSystem::getPathFromXML(xml::XMLElement *elem){
	if(!elem)return;
	xml::XMLAttribute* attr=0;
	if(elem->getName().equals_ignore_case(mT("SearchPath"))){
		if(attr=elem->getAttribute(mT("path"))){
			addPath(attr->value.c_str());
		}
	}
}






}
}