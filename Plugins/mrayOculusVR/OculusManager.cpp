

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



	struct OculusMessage
	{
	public:
		OVR::DeviceHandle device;
		OVR::MessageType message;
		OculusMessage(const OVR::MessageType& msg,const OVR::DeviceHandle &dev):device(dev),message(msg)
		{}
		OculusMessage(){}
		~OculusMessage()
		{
			device.Clear();
		}
	};

class OculusManagerImpl:public MessageHandler
{
protected:
	OVR::DeviceManager* m_deviceManager;
	DeviceEnumerator<HMDDevice> m_deviceEnum;
	std::queue<OculusMessage> m_messageQueue;
	OculusManager* m_owner;
	std::list<OculusDevice*> m_devices;

public:
	OculusManagerImpl(OculusManager*o)
	{
		m_owner=o;
		m_deviceManager=0;

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
		m_deviceEnum.Clear();
		m_deviceManager->Release();
		OVR::System::Destroy();
	}
	void OnMessage(const Message& msg)
	{
		if (msg.Type == Message_DeviceAdded || msg.Type == Message_DeviceRemoved)
		{
			if (msg.pDevice == m_deviceManager)
			{
				const MessageDeviceStatus& statusMsg =
					static_cast<const MessageDeviceStatus&>(msg);

				{ // limit the scope of the lock
					Lock::Locker lock(m_deviceManager->GetHandlerLock());
					m_messageQueue.push(OculusMessage(statusMsg.Type, statusMsg.Handle));
				}

				switch (statusMsg.Type)
				{
				case OVR::Message_DeviceAdded:
					gLogManager.log("DeviceManager reported device added.",ELL_INFO);
					break;

				case OVR::Message_DeviceRemoved:
					gLogManager.log("DeviceManager reported device removed.",ELL_INFO);
					break;

				default: OVR_ASSERT(0); // unexpected type
				}
			}
		}
	}

	bool Init()
	{
		OVR::System::Init(OVR::Log::ConfigureDefaultLog(OVR::LogMask_All));
		m_deviceManager=DeviceManager::Create();
		if(!m_deviceManager)
		{
			gLogManager.log("OculusManager:Init()- Failed to Create DeviceManager!",ELL_WARNING);
			return false;
		}
		m_deviceManager->SetMessageHandler(this);
		EnumDevices();
		return true;
	}

	void EnumDevices()
	{
		if(!m_deviceManager)
			Init();
		m_deviceEnum=m_deviceManager->EnumerateDevices<HMDDevice>(true);

	}

	void RemoveDevice(OculusDevice* d)
	{
		std::list<OculusDevice*>::iterator it= m_devices.begin();
		for (;it!=m_devices.end();++it)
		{
			if(*it==d)
			{
				m_devices.erase(it);
				
				//delete *it;
				break;;
			}
		}
	}


	int GetDevicesCount()
	{
		EnumDevices();
		DeviceEnumerator<HMDDevice> it= m_deviceEnum;
		int count=0;
		do 
		{
			if(it.IsAvailable())
				count++;
		} while(it.Next());
		return count;
	}

	OculusDevice* CreateDevice(int index)
	{
		EnumDevices();
		DeviceEnumerator<HMDDevice> it= m_deviceEnum;
		do 
		{
			if(it.IsAvailable())
			{
				if(0==index)
				{
					HMDDevice* dev= it.CreateDevice();
					if(!dev)
						return NULL;
					else	
					{
						OculusDevice* d=new OculusDevice(dev,m_owner);
						m_devices.push_back(d);
						return d;
					}
				}
				else
					--index;
			}
		} while(it.Next());
		return 0;
	}


	void Update(float dt)
	{
		bool queueDone=false;
		while(!m_messageQueue.empty())
		{
			OculusMessage msg;
			{
				Lock::Locker lock(m_deviceManager->GetHandlerLock());
				msg=m_messageQueue.front();
				m_messageQueue.pop();
			}
			bool wasCreated=msg.device.IsCreated();
			if(msg.message==Message_DeviceAdded)
			{
				switch(msg.device.GetType())
				{
				case Device_Sensor:
					{
						SensorInfo ifo;
						msg.device.GetDeviceInfo(&ifo);
						core::string msg="OculusManager - Device sensor was connected: ";
						msg+=ifo.ProductName;
						gLogManager.log(msg,ELL_INFO);
						break;
					}
				case Device_LatencyTester:
					{
						core::string msg="OculusManager - Latency tester was connected";
						gLogManager.log(msg,ELL_INFO);
						break;
					}
				case Device_HMD:
					{
						HMDInfo ifo;
						msg.device.GetDeviceInfo(&ifo);
						core::string msg="OculusManager - Device sensor was connected: ";
						msg+=ifo.ProductName;
						gLogManager.log(msg,ELL_INFO);
						break;
					}
				}
			}else if(msg.message==Message_DeviceRemoved)
			{
				//handle removing the device
			}
		}
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

