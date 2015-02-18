

/********************************************************************
	created:	2013/03/01
	created:	1:3:2013   12:21
	filename: 	C:\Development\mrayEngine\Projects\TELUBee\UDPDataReceiver.h
	file path:	C:\Development\mrayEngine\Projects\TELUBee
	file base:	UDPDataReceiver
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __UDPDataReceiver__
#define __UDPDataReceiver__


#include "BaseDataReceiver.h"

namespace mray
{
namespace TBee
{

	class UDPDataReceiverImpl;

class UDPDataReceiver:public BaseDataReceiver
{
protected:
	UDPDataReceiverImpl* m_impl;
	friend class UDPDataReceiverImpl;
public:
	UDPDataReceiver();
	virtual~UDPDataReceiver();

	bool Connect(const core::string& ip,int port);
	bool IsConnected();
	virtual void Start();
	virtual void Close();
};

}
}


#endif
