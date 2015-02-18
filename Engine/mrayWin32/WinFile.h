

#ifndef ___WinFile___
#define ___WinFile___

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <mString.h>
#include <IStream.h>
#include "compileConfig.h"


namespace mray{
namespace OS{

using namespace mray::core;

//! the file class
class MRAY_Win32_DLL WinFile:public IStream
{

protected:
	//HANDLE m_fp;
	FILE* m_fp;
	virtual uint calcSizeInternal();
public:
	WinFile();

	WinFile(const  core::string&name,FILE_MODE mode=BIN_READ);

	virtual~WinFile();

	bool open(const  core::string&name,FILE_MODE _mode=BIN_READ);
	virtual bool reopen(FILE_MODE _mode);

	virtual bool canRead();
	virtual bool canWrite();
	virtual bool canSeek(){return true;}

	bool reopen();

	virtual bool isOpen();
	//! reads a block of data
	virtual int read(void*data,size_t size);
	//! writes a block of data
	virtual int write(const void*data,size_t size);
	//! seek file for number of bytes
	//! \param mode:seek type ( SEEK_SET,SEEK_CUR,SEEK_END)
	virtual int seek(int bytes,EStreamSeek mode);

	virtual void close();

	
	
	//! return the current file pos
	virtual int getPos();

	//! have we reach End Of File
	virtual bool eof();
	
	/*
	//! writes a string
	virtual int print(const mchar*text);
	virtual void putChar(int c);

	virtual int getChar();
	
	virtual int getInt();
	virtual float getFloat();
	virtual int getString(string&out,int max);

	//! read complete line from file
	virtual int readLine(char*out);
	//! reads a string
	int readString(string &out);
	//! read complete line non commented
	int readNonCommentedLine(char*out,char comment='#');
	*/
};


};//OS
};//mray

#endif











