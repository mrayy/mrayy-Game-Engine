#include "stdafx.h"

#include "mrayGLPlugin.h"
#include "GLDev.h"
#include "Win32GLSupport.h"


namespace mray{
namespace plugins{


GCPtr< video::mrayGLDeviceFactory> s_deviceFactory;
	
mrayGLPlugin::mrayGLPlugin()
{
}
mrayGLPlugin::~mrayGLPlugin(){
}

 core::string mrayGLPlugin::getName(){
	 return mT("mrayGL Renderer Plugin");
}

void mrayGLPlugin::install(){
	if(isInstalled())return;
	IPlugin::install();
	s_deviceFactory= new video::mrayGLDeviceFactory();
	video::DeviceFactory::getInstance().addCreater(s_deviceFactory);

	new video::Win32GLSupport();

}
void mrayGLPlugin::uninstall(){
	if(!isInstalled())return;
	IPlugin::uninstall();

	video::DeviceFactory::getInstance().delCreater(s_deviceFactory);

	delete video::Win32GLSupport::GetInstance();
}


extern "C" MRAY_GL_DLL IPlugin* dllCreatePlugin(){
	return new mrayGLPlugin();

}


}
}
