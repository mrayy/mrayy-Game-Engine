

/********************************************************************
	created:	2011/10/20
	created:	20:10:2011   15:22
	filename: 	d:\Development\mrayEngine\Engine\mrayNet\Win32WebStream.h
	file path:	d:\Development\mrayEngine\Engine\mrayNet
	file base:	Win32WebStream
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __Win32WebStream__
#define __Win32WebStream__

#include "IStream.h"

namespace mray
{
namespace network
{

	class Win32WebRequest;

class Win32WebStream:public OS::IStream
{
private:
protected:
	Win32WebRequest* m_owner;

	uint calcSizeInternal();
public:
	Win32WebStream(Win32WebRequest*owner);
	~Win32WebStream();

	void SetURL(const core::string& url){m_streamName=url;}

	void Refresh();

	bool canRead();
	bool canWrite();
	bool canSeek();
	int getPos();

	bool isStream(){return true;}

	bool isOpen();
	int read(void*data,size_t size);
	int write(const void*data,size_t size);
	int seek(int bytes,OS::EStreamSeek mode);
	bool eof();
	void close();
};

}
}

#endif
