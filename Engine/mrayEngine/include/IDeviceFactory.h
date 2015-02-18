/********************************************************************
	created:	2008/12/25
	created:	25:12:2008   16:47
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\IDeviceFactory.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine
	file base:	IDeviceFactory
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___IDeviceFactory___
#define ___IDeviceFactory___

#include "ISingleton.h"
#include "IVideoDevice.h"

//#include "EDeviceType.h"

namespace mray{
namespace video{

class IDeviceFactoryCreater
{
public:
	virtual GCPtr<IVideoDevice> create()=0;

	virtual bool canCreate(const core::string& type)=0;

	virtual core::string getType()=0;
	//virtual EDeviceType getDeviceType()=0;
	virtual core::string getDescription()=0;

	virtual void getConfigParams(OptionContainer& params)=0;
};

class MRAY_DLL DeviceFactory:public ISingleton<DeviceFactory>
{
	typedef std::vector<GCPtr<IDeviceFactoryCreater>> DeviceCreaters;
	DeviceCreaters m_creaters;
public:

	virtual~DeviceFactory();

	GCPtr<IVideoDevice> createDevice(const core::string& type);
	//GCPtr<IVideoDevice> createDevice(EDeviceType type,uint window,math::Point2di size,int bits,bool vsync,int multiSample,bool stencilBuffer);

	void addCreater(GCPtr<IDeviceFactoryCreater> creater);
	void delCreater(GCPtr<IDeviceFactoryCreater> creater);

	int getCreatersCount();
	GCPtr<IDeviceFactoryCreater> getCreater(int i);

	GCPtr<IDeviceFactoryCreater> getCreater(const core::string& type);
	//GCPtr<IDeviceFactoryCreater> getCreater(EDeviceType type);
};

}
}

#endif //___IDeviceFactory___

