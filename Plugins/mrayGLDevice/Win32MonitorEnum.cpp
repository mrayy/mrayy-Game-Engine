

#include "StdAfx.h"
#include "Win32MonitorEnum.h"


#include "VideoLoggerSystem.h"


namespace mray
{
namespace video
{


Win32MonitorEnum::Win32MonitorEnum(const DISPLAY_DEVICE &device,int index)
{
	m_index=index;
	m_device=device;
	m_monitor.cb=sizeof(DISPLAY_DEVICE);
	Refresh();
}

Win32MonitorEnum::~Win32MonitorEnum()
{
}



bool Win32MonitorEnum::Refresh()
{
 	EnumDisplayDevices(m_device.DeviceName,0,&m_monitor,0);
	m_name=core::string(m_monitor.DeviceName);
 	if(!EnumDisplaySettings(m_device.DeviceName,ENUM_CURRENT_SETTINGS,&m_deviceMode))
		return false;
	m_size.set(m_deviceMode.dmPelsWidth,m_deviceMode.dmPelsHeight);
	m_rect.ULPoint.set(m_deviceMode.dmPosition.x,m_deviceMode.dmPosition.y);
	m_rect.BRPoint=m_rect.ULPoint+m_size;
	return true;
}


std::vector<IMonitorDevice*> Win32MonitorEnum::EnumMonitors()
{
	std::vector<IMonitorDevice*>ret;

	DISPLAY_DEVICE d;
	d.cb=sizeof(DISPLAY_DEVICE);
	int monitor=0;
	int i=0;
	int result;

	do
	{

		result=EnumDisplayDevices(0,i,&d,0);
		if(d.StateFlags &DISPLAY_DEVICE_ACTIVE)
		{
			Win32MonitorEnum* e=new Win32MonitorEnum(d,ret.size());
			ret.push_back(e);
			gLogManager.log(mT("Found Monitor: ")+e->GetName(),ELL_INFO,EVL_Heavy);
		}
		++i;
	}while(result);
	return ret;
}
bool Win32MonitorEnum::ChangeSettings(int width,int height,int colorDepth,int freq)
{

	DEVMODE dm;
	bool found=false;
	for(int i=0;!found && EnumDisplaySettings(m_device.DeviceName,i,&dm) ;++i)
	{
		found=(dm.dmPelsWidth==width && dm.dmPelsHeight==height  && dm.dmBitsPerPel==colorDepth);
	}

	if(!found)
	{
		gVideoLoggerSystem.log(mT("ChangeDisplaySettings : couldn't find user specific display settings."),ELL_WARNING);
		return false;
	}
	dm.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
	bool changed=false;

	if (freq!=0)
	{
		dm.dmDisplayFrequency = freq;
		dm.dmFields |= DM_DISPLAYFREQUENCY;
		if (ChangeDisplaySettingsEx(m_device.DeviceName,&dm,0, CDS_FULLSCREEN | CDS_TEST,0) != DISP_CHANGE_SUCCESSFUL)
		{
			gVideoLoggerSystem.log(mT("ChangeDisplaySettings : Failed to set user display frequency."),ELL_WARNING);
			dm.dmFields ^= DM_DISPLAYFREQUENCY;
		}
	}
	if (ChangeDisplaySettingsEx(m_device.DeviceName,&dm,0, CDS_FULLSCREEN,0) != DISP_CHANGE_SUCCESSFUL)
	{
		gVideoLoggerSystem.log(mT("ChangeDisplaySettings: Failed"),ELL_WARNING);
		return false;
	}

	IMonitorDeviceManager::getInstance().Refresh(false);
	return true;
}
void Win32MonitorEnum::RestoreSettings()
{
	ChangeDisplaySettingsEx(m_device.DeviceName,0,0,0,0);
	IMonitorDeviceManager::getInstance().Refresh(false);

}


Win32MonitorEnumManager::Win32MonitorEnumManager()
{

	Refresh(true);
}

Win32MonitorEnumManager::~Win32MonitorEnumManager()
{
	Clear();
}

void Win32MonitorEnumManager::Clear()
{
	for (int i=0;i<m_monitors.size();++i)
	{
		delete m_monitors[i];
	}
	m_monitors.clear();
}
IMonitorDevice* Win32MonitorEnumManager::GetMonitorFromPoint(const math::vector2di& pos)
{
	for (int i=0;i<m_monitors.size();++i)
	{
		if(m_monitors[i]->GetRect().IsPointInside(pos))
			return m_monitors[i];
	}
	return 0;
}
void Win32MonitorEnumManager::Refresh(bool clearCheck)
{
	if(clearCheck)
	{
		Clear();
		m_monitors=Win32MonitorEnum::EnumMonitors();
	}else
	{
		for(int i=0;i<m_monitors.size();++i)
			m_monitors[i]->Refresh();
	}
}
IMonitorDevice* Win32MonitorEnumManager::GetMonitor(int index)
{
	if(index>=m_monitors.size())
		return 0;
	return m_monitors[index];

}

}
}

