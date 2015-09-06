

#include "stdafx.h"
#include "OculusManager.h"
#include "OVR.h"
#include "OculusDevice.h"
#include "EventQueue.h"
#include "OculusEvent.h"
#include <queue>


using namespace OVR;

namespace mray
{
namespace video
{



class OculusManagerImpl
{
protected:
	OculusManager* m_owner;
	std::list<OculusDevice*> m_devices;

public:
	OculusManagerImpl(OculusManager*o)
	{
		m_owner=o;

		Init();
	}
	~OculusManagerImpl()
	{
		std::list<OculusDevice*>::iterator it= m_devices.begin();
		for (;it!=m_devices.end();++it)
		{
			delete *it;
		}
		m_devices.clear();
		ovr_Shutdown();
	}

	bool Init()
	{
		ovr_Initialize();
		return true;
	}

	void RemoveDevice(OculusDevice* d)
	{
		std::list<OculusDevice*>::iterator it= m_devices.begin();
		for (;it!=m_devices.end();++it)
		{
			if(*it==d)
			{
				m_devices.erase(it);
				
				break;;
			}
		}
	}

	int GetDevicesCount()
	{
		return ovrHmd_Detect();
	}

	OculusDevice* CreateDevice(int index)
	{
		ovrHmd Hmd = ovrHmd_Create(index);

		if (!Hmd)
		{
			return 0;
		}


		OculusDevice* dev = new OculusDevice(Hmd, m_owner);
		m_devices.push_back(dev);
		return dev;
	}


	void Update(float dt)
	{
		bool queueDone=false;
	}
};


OculusManager::OculusManager()
{
	m_impl=new OculusManagerImpl(this);
}
OculusManager::~OculusManager()
{
	delete m_impl;
}

void OculusManager::RemoveDevice(OculusDevice* d)
{
	m_impl->RemoveDevice(d);
}
int OculusManager::GetDeviceCount()
{
	return m_impl->GetDevicesCount();
}
OculusDevice* OculusManager::CreateDevice(int index)
{
	return m_impl->CreateDevice(index);
}
void OculusManager::Update(float dt)
{
	m_impl->Update(dt);
}

}
}

