#include "stdafx.h"

#include "CZipArchive.h"
#include "IFileSystem.h"
#include "ILogManager.h"
#include <zzip\zzip.h>
#include "IStream.h"



namespace mray{
namespace OS{


uint  CZipArchive::CZipFile::calcSizeInternal(){
			return sizeof(CZipFile);
}
CZipArchive::CZipFile::CZipFile(const  mchar*name,ZZIP_FILE*zipFile,int size):IStream(name)
{
	m_zipfile=zipFile;
	this->m_length=size;
	m_isOpen=1;
}
CZipArchive::CZipFile::~CZipFile(){
	if(m_isOpen)
		close();
	if(m_zipfile){
		zzip_file_close(m_zipfile);
		m_zipfile=0;
	}
}

bool CZipArchive::CZipFile::canRead(){
	return m_zipfile && !eof();
}
bool CZipArchive::CZipFile::canWrite(){
	return false;
}

void CZipArchive::CZipFile::close(){
	m_isOpen=false;
}

bool CZipArchive::CZipFile::isOpen(){
	return m_isOpen;
}
int CZipArchive::CZipFile::read(void*_data,size_t _size){
	return zzip_file_read(m_zipfile,(char*)(_data),_size);
}
int CZipArchive::CZipFile::write(const void*_data,size_t _size){
	return 0;
}
int CZipArchive::CZipFile::seek(int bytes,EStreamSeek smode){
	switch(smode){
	case ESeek_Set:
		return zzip_seek(m_zipfile,bytes,SEEK_SET);
		
	case ESeek_Cur:
		return zzip_seek(m_zipfile,bytes,SEEK_CUR);

	case ESeek_End:
		return zzip_seek(m_zipfile,bytes,SEEK_END);

	}
	return 0;

}
int CZipArchive::CZipFile::getPos(){
	return zzip_tell(m_zipfile);
}
bool CZipArchive::CZipFile::eof(){
    return (zzip_tell(m_zipfile) >= static_cast<zzip_off_t>(length()));
}
/*
virtual int getInt(){
	if(eof())return -1;
	int i;
	if(mode==BIN_READ){
		read(&i,sizeof(i));
	}else{
	}
	return i;
}
virtual float getFloat(){
	if(eof())return -1;
	float f;
	read(&f,sizeof(f));
	return f;
}

virtual int readLine(char*out){
	if(!isOpen())return 0;
	char ch;
	int n=0;
	while(!eof()){
		if(!read(&ch,sizeof(char)))break;
		if(ch=='\n' || ch=='\0')
			break;
		out[n]=ch;
		n++;
	}
	out[n]='\0';
	return n;
}

virtual int getString(core::string&out,int max){
	if(!isOpen())return 0;
	char ch;
	int n=0;
	out.resize(max);
	while(!eof() && n<max){
		if(!read(&ch,sizeof(char)))break;
		if(ch=='\n' || ch=='\0')
			break;
		out[n]=ch;
		n++;
	}
	out[n]='\0';
	out.resize(n);
	return n;
}*/

const mchar* getZzipError(zzip_error_t zzipError) 
{
	static core::string errorMsg;
    switch (zzipError)
    {
    case ZZIP_NO_ERROR:
        break;
    case ZZIP_OUTOFMEM:
        errorMsg = mT("Out of memory.");
        break;            
    case ZZIP_DIR_OPEN:
    case ZZIP_DIR_STAT: 
    case ZZIP_DIR_SEEK:
    case ZZIP_DIR_READ:
        errorMsg = mT("Unable to read zip file.");
        break;            
    case ZZIP_UNSUPP_COMPR:
        errorMsg = mT("Unsupported compression format.");
        break;            
    case ZZIP_CORRUPTED:
        errorMsg = mT("Corrupted archive.");
        break;            
    default:
        errorMsg = mT("Unknown error.");
        break;            
    };

    return errorMsg.c_str();
}


bool CZipArchive::checkForError(int err,const mchar*desc){
	if(err==ZZIP_NO_ERROR)return false;
	core::string error_str=mT("CZipArchive- [");
	error_str+=desc;
	error_str+=mT("] :");
	error_str+= getZzipError((zzip_error_t)err);
	gLogManager.log(error_str.c_str(),ELL_WARNING);

	return true;

}
CZipArchive::CZipArchive(const  core::string&name):IArchive(name),m_zipDir(0){
	loadArchive();
}

CZipArchive::~CZipArchive(){
	unloadArchive();
}

void CZipArchive::unloadArchive()
{
//	for(int i=0;i<m_files.size();++i){
//		GCPtr<IStream>file=m_files[i].fileData;
//	}
	m_files.clear();

	if(m_zipDir){
		zzip_dir_close(m_zipDir);
		m_zipDir=0;
	}
}

bool CZipArchive::loadArchive(){
	static  mchar tempBuff[128];
	if(!m_zipDir){
		zzip_error_t err;
		//m_file=gFileSystem.openFile(getName());
		//if(!m_file || !m_file->getFD())return 0;
		
		//m_zipDir = zzip_dir_fdopen(m_file->getFD()->_file,&err);
		core::stringc fileName;
		core::string_to_char(getName(),fileName);
		m_zipDir = zzip_dir_open(fileName.c_str(),&err);
		if(checkForError(err,mT(" loadArchive()")))return false;

		ZZIP_DIRENT zipEntry;
		sFileEntry entry;
		core::string tstr;
		core::string zipName;
		core::stringc tmpc;
		while(zzip_dir_read(m_zipDir,&zipEntry)){
			zipName=zipEntry.d_name;
			zipName.replaceChar('\\','/');
			int x=zipName.findlast('/');
			if(x==zipName.length()){
// 				entry.dirPath=zipName;
// 				entry.filePath=zipName;
				entry.dirPath=mT("");
				entry.filePath=mT("");
				zipName[zipName.length()]='\0';
				x=zipName.findlast('/');
				tstr=zipName.substr(x,zipName.length()-x+1);
				entry.fileName=tstr;
				entry.isfolder=true;
				entry.fileData=0;
			}else{
				//entry.filePath=zipName;
				entry.dirPath=mT("");
				entry.filePath=mT("");

				tstr=zipName.substr(x+1,zipName.length());
				entry.fileName=tstr;

				core::string_to_char(zipName,tmpc);
				
				zzip_file*zipFile=zzip_file_open(m_zipDir,tmpc.c_str(),ZZIP_ONLYZIP | ZZIP_CASELESS);
				if(!zipFile){
					int zerr = zzip_error(m_zipDir);
					checkForError(zerr,mT("open zip file"));
					entry.fileData=0;
				}else{
					ZZIP_STAT zstate;
					zzip_dir_stat(m_zipDir,tmpc.c_str(),&zstate,ZZIP_CASEINSENSITIVE);

					entry.fileData=new CZipFile(entry.fileName.c_str(),zipFile,zstate.st_size);
				}

				int s=zipName.find('/');
				tstr=zipName.substr(s+1,x-s);
				entry.dirPath=tstr;
				entry.filePath=tstr+entry.fileName;
				entry.isfolder=false;
				

			}

			entry.compressSize=zipEntry.d_csize;
			entry.uncompressSize=zipEntry.st_size;


			m_files.push_back(entry);
		}
	}
	return 1;
}


const IStreamPtr&  CZipArchive::getFile(const  core::string&name,FILE_MODE mode){
	core::string nameStr(name);
	nameStr.replaceChar('\\','/');
	bool withPath=nameStr.find('/')!=-1;

	int sz=m_files.size();
	for(int i=0;i<sz;++i){
		if(withPath){
			if(m_files[i].filePath.equals_ignore_case(nameStr)){
				m_files[i].fileData->setMode(mode);
				return m_files[i].fileData;
			}
		}else{
			if(m_files[i].fileName.equals_ignore_case(nameStr)){
				m_files[i].fileData->setMode(mode);
				return m_files[i].fileData;
			}
		}
	}
	return IStreamPtr::Null;
}
const IStreamPtr& CZipArchive::getFile(int index,FILE_MODE mode){
	if(index<0 || index>=m_files.size())
		return IStreamPtr::Null;
	m_files[index].fileData->setMode(mode);

	return m_files[index].fileData;
}

int CZipArchive::getFileIndex(const  core::string&name){
	core::string nameStr(name);
	nameStr.replaceChar('\\','/');
	bool withPath=nameStr.find('/');

	int sz=m_files.size();
	for(int i=0;i<sz;++i){
		if(withPath){
			if(m_files[i].filePath.equals_ignore_case(nameStr))
				return i;
		}else{
			if(m_files[i].fileName.equals_ignore_case(nameStr))
				return i;
		}
	}
	return -1;
}

void CZipArchive::getFilesList(bool dirs,TFiles_List & outList){

	outList.clear();
	int sz=m_files.size();
	for(int i=0;i<sz;++i){
		if(m_files[i].isfolder){
			if(dirs)
				outList.push_back(m_files[i].filePath);
		}else
			outList.push_back(m_files[i].filePath);
	}
}

int CZipArchive::getCount(bool dir)
{
	if(dir)
		return m_files.size();
	else{
		int sz=m_files.size();
		int cnt=0;
		for(int i=0;i<sz;++i){
			if(!m_files[i].isfolder)
				cnt++;
		}
		return cnt;
	}

}


}
}


