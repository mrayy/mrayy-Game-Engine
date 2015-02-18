


/********************************************************************
	created:	2011/09/27
	created:	27:9:2011   9:53
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\NetworkStream.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	NetworkStream
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __NetworkStream__
#define __NetworkStream__

#include "IStream.h"

namespace mray
{
namespace network
{
class ISocket;

class NetworkStream:public OS::IStream
{
private:
protected:
	ISocket* m_socket;
public:
	NetworkStream(ISocket*socket);
	virtual~NetworkStream();

	bool canRead();
	bool canWrite();
	bool canSeek();
	int length();
	int getPos();

	bool reopen(OS::FILE_MODE mode){return true;}

	bool isOpen();
	int read(void*data,size_t size);
	int write(const void*data,size_t size);
	int seek(int bytes,OS::EStreamSeek mode);
	bool eof();
	void close();

	int writeByte(uchar v);
	uchar readByte();
};


}
}

#endif
