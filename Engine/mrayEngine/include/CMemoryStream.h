

#ifndef ___CMemoryStream___
#define ___CMemoryStream___

#include "IStream.h"
#include "mTypes.h"

namespace mray{
namespace OS{

class MRAY_DLL CMemoryStream:public IStream
{
protected:
	byte*data;
	int pos;
	bool bDeleteAtEnd;
	virtual uint calcSizeInternal();
public:
	CMemoryStream();
	CMemoryStream(const  core::string&name,byte*data,int size,bool deleteAtEnd,FILE_MODE _mode=BIN_READ);
	virtual ~CMemoryStream();

	virtual bool reopen(FILE_MODE mode);

	virtual bool canRead();
	virtual bool canWrite();
	bool canSeek();

	void setDeleteAtEnd(bool set);

	bool isDeleteAtEnd();

	void setData(byte*data,int size);
	byte *getData();

	bool open(const  core::string&name,byte*data,int size,FILE_MODE _mode);
	virtual void close();
	virtual bool isOpen();

	virtual int read(void*_data,size_t _size);
	virtual int write(const void*_data,size_t _size);
	virtual int seek(int bytes,EStreamSeek smode);
	virtual int getPos();
	virtual bool eof();
	
};

}
}

#endif



