




#ifndef ___CLimitedStream___
#define ___CLimitedStream___

#include "IStream.h"

#include "GCPtr.h"

namespace mray{
namespace OS{


class CLimitedStream:public IStream
{
protected:
	int m_limitSize;
	int m_startPos;
	int pos;

	bool opened;

	GCPtr<IStream> m_masterStream;
	virtual uint calcSizeInternal();
public:
	CLimitedStream(GCPtr<IStream>masterStream,int limitSize,int startPos,const  mchar*name,FILE_MODE mode);
	virtual ~CLimitedStream();

	virtual bool canRead();
	virtual bool canWrite();
	bool canSeek(){return true;}
	
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


