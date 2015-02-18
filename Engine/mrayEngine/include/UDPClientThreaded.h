

/********************************************************************
	created:	2012/07/15
	created:	15:7:2012   20:52
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\UDPClientThreaded.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	UDPClientThreaded
	file ext:	h
	author:		MHD YAMEN SARAIJI
	
	purpose:	
*********************************************************************/
#ifndef ___UDPClientThreaded___
#define ___UDPClientThreaded___



namespace mray
{
	namespace OS
	{
		class IThread;
	}
namespace network
{

	class IUDPClient;

class MRAY_DLL UDPClientThreaded
{
protected:
	IUDPClient* m_client;
	OS::IThread* m_thread;
	
public:
	UDPClientThreaded();
	virtual~UDPClientThreaded();

	void Start();
	void Stop() ;
	
};

}
}

#endif
