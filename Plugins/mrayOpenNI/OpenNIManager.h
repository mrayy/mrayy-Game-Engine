



/********************************************************************
	created:	2013/06/18
	created:	18:6:2013   12:16
	filename: 	C:\Development\mrayEngine\Plugins\mrayOpenNI\OpenNIManager.h
	file path:	C:\Development\mrayEngine\Plugins\mrayOpenNI
	file base:	OpenNIManager
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __OpenNIManager__
#define __OpenNIManager__

#include "ISingleton.h"


namespace mray
{

	class OpenNIManagerImpl;
	class OpenNICaptureDevice;
class OpenNIManager:public ISingleton<OpenNIManager>
{
protected:
	OpenNIManagerImpl* m_impl;
public:
	OpenNIManager();
	virtual~OpenNIManager();

	int Init(int argc,char**argv);
	bool IsInited();
	void Close();

	int GetDeviceCount();

	OpenNICaptureDevice* CreateDeviceByIndex(int idx);
	OpenNICaptureDevice* CreateDeviceByURI(const char* uri);
	OpenNICaptureDevice* CreateDeviceAny();

};

}


#endif
