#include "stdafx.h"

#include "WinFile.h"
#include <WinBase.h>



namespace mray{
namespace OS{

	

#ifdef UNICODE
#define openFile _wfopen_s
#define printText fwprintf
#define readText fwscanf
#else
#define openFile fopen_s
#define printText fprintf
#define readText fscanf
#endif

using namespace mray::core;

WinFile::WinFile():IStream(),m_fp(0)
{}

WinFile::WinFile(const  core::string&name,FILE_MODE mode):IStream(name,mode),m_fp(0){
	open(name,mode);
}

WinFile::~WinFile(){
	close();
}
uint WinFile::calcSizeInternal(){
	return sizeof(WinFile);
}

bool WinFile::open(const  core::string&name,FILE_MODE _mode){
	if(name[0]=='\0')return 0;
	m_mode=_mode;
	bool state=0;

	switch(m_mode){
		case BIN_READ:
			//m_fp=CreateFile(name.c_str(),GENERIC_READ,FILE_SHARE_READ,0,OPEN_EXISTING,0,0);
			state=openFile(&m_fp,name.c_str(),mT("rb"));
			break;
		case BIN_WRITE:
			//m_fp=CreateFile(name.c_str(),GENERIC_WRITE,FILE_SHARE_WRITE,0,CREATE_ALWAYS,0,0);
			state=openFile(&m_fp,name.c_str(),mT("wb"));
			break;
		case BIN_APPEND:
			//m_fp=CreateFile(name.c_str(),GENERIC_READ | GENERIC_WRITE,FILE_SHARE_WRITE|FILE_SHARE_READ,0,OPEN_ALWAYS,0,0);
			state=openFile(&m_fp,name.c_str(),mT("ab"));
			//state=openFile(&fp,name.c_str(),mT("ab"));
			break;
		case TXT_READ:
			//m_fp=CreateFile(name.c_str(),GENERIC_READ,FILE_SHARE_READ,0,OPEN_EXISTING,0,0);
			state=openFile(&m_fp,name.c_str(),mT("rb"));
			//state=openFile(&fp,name.c_str(),mT("rb"));//mT("rt"));
			break;
		case TXT_WRITE:
			//m_fp=CreateFile(name.c_str(),GENERIC_WRITE,FILE_SHARE_WRITE,0,CREATE_ALWAYS,0,0);
			state=openFile(&m_fp,name.c_str(),mT("wb"));
			//state=openFile(&fp,name.c_str(),mT("wt"));
			break;
		case TXT_APPEND:
			//m_fp=CreateFile(name.c_str(),GENERIC_READ | GENERIC_WRITE,FILE_SHARE_WRITE|FILE_SHARE_READ,0,OPEN_ALWAYS,0,0);
			state=openFile(&m_fp,name.c_str(),mT("ab"));
			//if(m_fp!=(HANDLE)-1){
			//state=openFile(&fp,name.c_str(),mT("a"));
			break;
			
	}
	//if(m_fp==(HANDLE)-1)return 0;
	if(m_fp==0)return 0;
	m_streamName=name;

	//seek(0,ESeek_Set);
	//m_length=GetFileSize(m_fp,0);

	seek(0,ESeek_End);
	m_length=ftell(m_fp);
	seek(0,ESeek_Set);
	return 1;
}
bool WinFile::reopen(FILE_MODE _mode){
	close();
	return open(m_streamName.c_str(),_mode);
}
bool WinFile::reopen(){
	if(m_streamName==mT(""))return 0;
	return open(m_streamName);

}
bool WinFile::canRead(){
	return !eof() && (m_mode==TXT_READ || m_mode==BIN_READ || m_mode==TXT_APPEND || m_mode==BIN_APPEND);
}
bool WinFile::canWrite(){
	return m_fp!=0 && (m_mode==TXT_WRITE || m_mode==BIN_WRITE || m_mode==TXT_APPEND || m_mode==BIN_APPEND);
}

bool WinFile::isOpen(){
	return m_fp!=0;
}
//! reads a block of data
int WinFile::read(void*data,size_t size){
	if(m_fp==0 )return 0;

	//DWORD cnt=0;
	//if(mode==TXT_READ || mode==TXT_APPEND){
	//	cnt=readText(m_fp,mT("%s"),(const wchar_t*)data);
	//}else
	{
		return fread(data,1,size,m_fp);
	}
	//ReadFile(m_fp,data,size,&cnt,0);

	//return cnt;//fread(data,1,size,m_fp);
}
//! writes a block of data
int WinFile::write(const void*data,size_t size){
	if(m_fp==0 )return 0;
	//DWORD cnt=0;
// 	if(mode==TXT_WRITE || mode==TXT_APPEND){
// 		cnt=printText(m_fp,(const wchar_t*)data);
// 	}else
	{
		int cnt= fwrite(data,1,size,m_fp);
		m_length += cnt;
		return cnt;
	}
	//WriteFile(m_fp,data,size,&cnt,0);
	//return cnt;//fwrite(data,1,size,m_fp);

}

//! seek file for number of bytes
//! \param mode:seek type ( SEEK_SET,SEEK_CUR,SEEK_END)
int WinFile::seek(int bytes,EStreamSeek mode){
	if(m_fp==0)return 0;
	int smode;
	switch(mode){
	case ESeek_Set:
		smode=SEEK_SET;
		//smode=FILE_BEGIN;
		break;
	case ESeek_Cur:
		smode=SEEK_CUR;
		//smode=FILE_CURRENT;
		break;
	case ESeek_End:
		smode=SEEK_END;
		//smode=FILE_END;
		break;
	default:
		return 0;
	}
	return fseek(m_fp,bytes,smode);
	//return SetFilePointer(m_fp,bytes,0,smode);
}


void WinFile::close(){
	if(isOpen())
		fclose(m_fp);
		//CloseHandle(m_fp);
	m_fp=0;
}



//! return the current file pos
int WinFile::getPos(){
	if(m_fp==0)return 0;

	return ftell(m_fp);
	//return SetFilePointer(m_fp,0,0,FILE_CURRENT);
}



//! have we reach End Of File
bool WinFile::eof(){
	if(m_fp==0)return 1;
	return feof(m_fp);
	//return getPos()==length();
}
/*
//! writes a string
int WinFile::print(const mchar*text){
if(m_fp==(HANDLE)-1)return 0;
if(mode==TXT_WRITE || mode ==TXT_APPEND){
DWORD cnt=0;
core::stringc s;
core::string_to_char(text,s);
WriteFile(m_fp,s.c_str(),s.length(),&cnt,0);
return cnt;// printText(m_fp,text);
}
return 0;
}
void WinFile::putChar(int c){
if(m_fp==(HANDLE)-1)return;
char ch=c;
DWORD cnt=0;
WriteFile(m_fp,&ch,sizeof(char),&cnt,0);
}

int WinFile::getChar(){
if(m_fp==(HANDLE)-1)return 0;
char ch;
DWORD cnt=0;
ReadFile(m_fp,&ch,sizeof(char),&cnt,0);
return ch;
}
//! reads a string
int WinFile::readString(string &out){
	if(m_fp==(HANDLE)-1 || mode!=TXT_READ)return 0;
	char buffer[512];
	int res=fscanf(fp,"%s",buffer);
	out=buffer;
	return res;
}

int WinFile::getInt(){
	int o;
	fscanf(fp,"%d",&o);
	return o;
}
float WinFile::getFloat(){
	float o;
	fscanf(fp,"%f",&o);
	return o;
}
int WinFile::getString(string&out,int max){
	char *txt=new char[max];
	int c=fscanf(fp,"%s",txt);
	out=txt;
	delete [] txt;		
	return c;
}


//! read complete line from file
int WinFile::readLine(char*out)
{
	if(m_fp==(HANDLE)-1 || mode!=TXT_READ)return 0;
	char ch;
	int n=0;
	while(!feof(fp)){
		if(!fread(&ch,1,sizeof(char),fp))
			break;
		if(ch=='\n' || ch=='\0'){
			if(n==0)continue;
			break;
		}
		out[n]=ch;
		n++;
	}
	out[n]=0;
	return n;
}

//! read complete line non commented
int WinFile::readNonCommentedLine(char*out,char comment){
	if(m_fp==(HANDLE)-1 || mode!=TXT_READ)return 0;
	char ch;
	int n=0;
	bool flag;
	out[0]='\0';
	do{
		flag=0;
		n=0;
		while(!feof(fp)){
			if(!fread(&ch,1,sizeof(char),fp))break;
			if(ch=='\n' || ch=='\0')
				break;
			if(ch==comment){
				flag=1;
				out[n]='\0';
			}
			if(!flag)
				out[n]=ch;
			n++;
		}
	}
	while(out[0]=='\0' && !feof(fp));///until read one line at least

	if(!flag)
		out[n]='\0';
	return n;
}
*/

};//OS
};//mray

