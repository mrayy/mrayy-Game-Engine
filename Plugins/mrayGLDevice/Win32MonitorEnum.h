

/********************************************************************
	created:	2012/09/26
	created:	26:9:2012   15:43
	filename: 	d:\Development\mrayEngine\Plugins\mrayGLDevice\Win32MonitorEnum.h
	file path:	d:\Development\mrayEngine\Plugins\mrayGLDevice
	file base:	Win32MonitorEnum
	file ext:	h
	author:		MHD YAMEN SARAIJI
	
	purpose:	
*********************************************************************/
#ifndef ___Win32MonitorEnum___
#define ___Win32MonitorEnum___


#include <windows.h>
#include "IMonitorDevice.h"
#include "IMonitorDeviceManager.h"

namespace mray
{
namespace video
{


class Win32MonitorEnum:public IMonitorDevice
{
protected:

	DISPLAY_DEVICE m_monitor;
	DEVMODE  m_deviceMode;

	DISPLAY_DEVICE m_device;

	core::string m_name;
	math::recti m_rect;
	math::vector2di m_size;
	int m_index;

public:
	Win32MonitorEnum(const DISPLAY_DEVICE &device,int index);
	virtual~Win32MonitorEnum();

	int GetIndex(){return m_index;};
	const core::string& GetName()const{return m_name;}
	const math::vector2di& GetSize()const{return m_size;}
	const math::vector2di& GetStartPosition()const{return m_rect.ULPoint;}
	const math::recti& GetRect()const{return m_rect;}


	const DISPLAY_DEVICE& GetDisplayMonitor(){return m_monitor;}
	const DISPLAY_DEVICE& GetDisplayDevice(){return m_device;}
	const DEVMODE& GetMonitorMode(){return m_deviceMode;}
	core::string GetMonitorName(){return m_monitor.DeviceName;}
	core::string GetDeviceName(){return m_device.DeviceName;}

	bool Refresh();

	bool ChangeSettings(int width,int height,int colorDepth,int freq);
	void RestoreSettings();


	static std::vector<IMonitorDevice*> EnumMonitors();
	
};

class Win32MonitorEnumManager:public IMonitorDeviceManager
{
protected:
	std::vector<IMonitorDevice*> m_monitors;
public:
	Win32MonitorEnumManager();
	virtual~Win32MonitorEnumManager();

	void Clear();
	void Refresh(bool clearCheck=true);

	IMonitorDevice* GetMonitorFromPoint(const math::vector2di& pos);

	const std::vector<IMonitorDevice*>& GetMonitors(){return m_monitors;}
	IMonitorDevice* GetMonitor(int index);

};

}
}

#endif
