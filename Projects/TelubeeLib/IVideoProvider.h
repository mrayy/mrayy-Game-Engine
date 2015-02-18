

/********************************************************************
	created:	2013/03/12
	created:	12:3:2013   21:01
	filename: 	D:\Development\mrayEngine\Projects\TELUBee\IVideoProvider.h
	file path:	D:\Development\mrayEngine\Projects\TELUBee
	file base:	IVideoProvider
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/
#ifndef _IVideoProvider_
#define _IVideoProvider_



namespace mray
{
namespace TBee
{

class IVideoProvider
{
protected:
public:
	IVideoProvider(){}
	virtual~IVideoProvider(){}

	virtual void ConnectToCameras(const core::string& ip, int videoPort, int audioPort, int localPort) = 0;
	virtual void Disconnect()=0;
	virtual bool IsConnected()=0;

	virtual bool HasNewImage(int id)=0;
	virtual const video::ImageInfo* GetImage(int id)=0;
	virtual math::rectf	GetTexRect(int id)=0;

	virtual void Update(float dt)=0;
};

}
}

#endif
