

/********************************************************************
	created:	2013/03/01
	created:	1:3:2013   12:45
	filename: 	C:\Development\mrayEngine\Projects\TELUBee\DynamicMemoryStream.h
	file path:	C:\Development\mrayEngine\Projects\TELUBee
	file base:	DynamicMemoryStream
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __DynamicMemoryStream__
#define __DynamicMemoryStream__


#include "IStream.h"

namespace mray
{
namespace OS
{

class DynamicMemoryStream:public IStream
{
protected:
	std::vector<char> m_data;
	int m_pos;
	virtual uint calcSizeInternal(){return sizeof(DynamicMemoryStream);}

public:
	DynamicMemoryStream();
	virtual~DynamicMemoryStream();


	virtual bool canRead(){return true;}
	virtual bool canWrite(){return true;}
	virtual bool canSeek(){return true;}
	//! get size of file
	virtual int length(){return m_data.size();}
	virtual int getPos(){return m_pos;}

	virtual bool isOpen(){return true;}
	virtual int read(void*data,size_t size);
	virtual int write(const void*data,size_t size);
	virtual int seek(int bytes,EStreamSeek mode);
	virtual bool eof();
	virtual void close();

	void Resize(int sz){m_data.resize(sz);}
	char* GetDataPtr()
	{
		if(m_data.size()==0)
			return 0;
		return &m_data[0];
	}
};

}
}


#endif
