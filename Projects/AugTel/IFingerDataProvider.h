

/********************************************************************
	created:	2014/01/17
	created:	17:1:2014   0:29
	filename: 	C:\Development\mrayEngine\Projects\AugTel\IFingerDataProvider.h
	file path:	C:\Development\mrayEngine\Projects\AugTel
	file base:	IFingerDataProvider
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __IFingerDataProvider__
#define __IFingerDataProvider__




namespace mray
{
namespace AugTel
{

class IFingerDataProvider
{
protected:
public:
	IFingerDataProvider(){}
	virtual~IFingerDataProvider(){}

	virtual bool SetHand(int hand) = 0; //hand=0: left, 1: right
	virtual bool Start() = 0;
	virtual void Stop() = 0;
	virtual float GetFingerValue(int finger, int part)=0;
	virtual void Update(float dt){};

	virtual void SendMessage(const core::string& msg, const core::string& value) {};
};

}
}


#endif
