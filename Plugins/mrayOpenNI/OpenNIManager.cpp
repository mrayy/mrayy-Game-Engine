


#include "stdafx.h"
#include "OpenNIManager.h"

#include "ILogManager.h"
#include "OpenNICaptureDevice.h"
#include "IOSystem.h"
#include "IFileSystem.h"
#include "IDirOS.h"
#include <NiTE.h>

namespace mray
{
	class OpenNIManagerImpl
	{
	public:
		bool m_isOpen;
		openni::Array<openni::DeviceInfo> m_deviceInfoList;

		OpenNIManagerImpl()
		{
			m_isOpen=false;
		}
		virtual~OpenNIManagerImpl()
		{
			Close();
		}
		int Init(int argc,char**argv)
		{
			Close();

			{
				GCPtr<OS::IDirOS> os = OS::IOSystem::getInstance().createDirSystem();
				os->changeDir(gFileSystem.getAppPath());
			}

			openni::Status rc = openni::OpenNI::initialize();
			if (rc != openni::STATUS_OK)
			{
				gLogManager.log(core::string("Failed to initialize OpenNI")+ core::StringConverter::toString(openni::OpenNI::getExtendedError()),ELL_WARNING);
				return rc;
			}

			openni::OpenNI::enumerateDevices(&m_deviceInfoList);


			nite::NiTE::initialize();

			core::string msg="Nite version: ";
			msg+=core::StringConverter::toString(nite::NiTE::getVersion().major);
			msg+=core::StringConverter::toString(nite::NiTE::getVersion().minor);
			gLogManager.log(msg,ELL_INFO);

			m_isOpen=true;
			return openni::STATUS_OK;
		}

		bool IsInited()
		{
			return m_isOpen;
		}

		void Close()
		{
			if(!m_isOpen)
				return;
			try{

				nite::NiTE::shutdown();
				openni::OpenNI::shutdown();
			}
			catch (...)
			{
			}
		}


		int GetDeviceCount()
		{
			return m_deviceInfoList.getSize();
		}


		OpenNICaptureDevice* CreateDeviceByIndex(int index)
		{
			if(index>=m_deviceInfoList.getSize())
			{
				printf("COpenNI::OpenDevice() - device index [%d] out of range!",index);
				return 0;
			}
			return new OpenNICaptureDevice(m_deviceInfoList[index].getUri());
		}

		OpenNICaptureDevice* CreateDeviceByURI(const char* uri)
		{
			return new OpenNICaptureDevice(uri);
		}

		OpenNICaptureDevice* CreateDeviceAny()
		{
			return new OpenNICaptureDevice(openni::ANY_DEVICE);
		}

	};

OpenNIManager::OpenNIManager()
{
	m_impl=new OpenNIManagerImpl();
}
OpenNIManager::~OpenNIManager()
{
	delete m_impl;
}

int OpenNIManager::Init(int argc,char**argv){
	return m_impl->Init(argc,argv);
}

bool OpenNIManager::IsInited(){
	return m_impl->IsInited();
}

void OpenNIManager::Close(){
	m_impl->Close();
}


int OpenNIManager::GetDeviceCount(){
	return m_impl->GetDeviceCount();
}


OpenNICaptureDevice* OpenNIManager::CreateDeviceByIndex(int idx){
	return m_impl->CreateDeviceByIndex(idx);
}

OpenNICaptureDevice* OpenNIManager::CreateDeviceByURI(const char* uri){
	return m_impl->CreateDeviceByURI(uri);
}

OpenNICaptureDevice* OpenNIManager::CreateDeviceAny()
{
	return m_impl->CreateDeviceAny();
}

}
