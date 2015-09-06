

/********************************************************************
	created:	2013/10/06
	created:	6:10:2013   14:25
	filename: 	C:\Development\mrayEngine\Plugins\mrayOculusVR\OculusManager.h
	file path:	C:\Development\mrayEngine\Plugins\mrayOculusVR
	file base:	OculusManager
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __OculusManager__
#define __OculusManager__




namespace mray
{
namespace video
{

	class OculusDevice;


	class OculusManagerImpl;
class OculusManager
{
protected:
	OculusManagerImpl* m_impl;
public:
	OculusManager();
	virtual~OculusManager();

	int GetDeviceCount();
	OculusDevice* CreateDevice(int index);
	void Update(float dt);

	void RemoveDevice(OculusDevice* d);

};

}
}


#endif
