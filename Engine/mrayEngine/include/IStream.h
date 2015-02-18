

#ifndef ___IStream___
#define ___IStream___


#include "mString.h"
#include "IResource.h"

namespace mray{
namespace OS{

enum EStreamSeek{
	ESeek_Set,
	ESeek_Cur,
	ESeek_End
};

//! file open types
enum FILE_MODE
{
//! open binary file for reading
	BIN_READ=0,
//! open binary file for writting
	BIN_WRITE,
//! open binary file for appending
	BIN_APPEND,
//! open text file for reading
	TXT_READ,
//! open text file for writting
	TXT_WRITE,
//! open text file for appending
	TXT_APPEND
};

class MRAY_DLL IStream:public IResource
{
protected:
	core::string m_streamName;
	FILE_MODE m_mode;
	int m_length;
public:
	IStream();
	IStream(const  core::string&name,FILE_MODE mode=BIN_READ);

	virtual ~IStream();

	
	void setMode(FILE_MODE mode);

	virtual bool canRead()=0;
	virtual bool canWrite()=0;
	virtual bool canSeek()=0;
	//! get size of file
	virtual int length();
	virtual int getPos()=0;
	// is it a continus stream (like network stream)
	virtual bool isStream(){return false;}


	const  core::string& getStreamName();

	virtual bool reopen(FILE_MODE mode){return true;}

	virtual bool isOpen()=0;
	virtual int read(void*data,size_t size)=0;
	virtual int write(const void*data,size_t size)=0;
	virtual int seek(int bytes,EStreamSeek mode)=0;
	virtual bool eof()=0;
	virtual void close()=0;
	int readToEnd(void**data);

	virtual int writeByte(uchar v);
	virtual uchar readByte();
	/*
	virtual void putChar(int c)=0;
	virtual int getChar()=0;
	virtual int getInt();
	virtual float getFloat();
	virtual int getString(core::string&out,int max);
	
	virtual int readLine(mchar*out);
	virtual int print(const mchar*text)=0;

	virtual int readNonCommentedLine(char*out,char comment='#');
	*/
};

MakeSharedPtrType(IStream)

};//OS
};//mray

#endif


