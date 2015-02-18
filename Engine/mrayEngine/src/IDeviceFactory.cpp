#include "stdafx.h"


#include "IDeviceFactory.h"


namespace mray{
namespace video{

DeviceFactory::~DeviceFactory(){
	m_creaters.clear();
}

GCPtr<IVideoDevice> DeviceFactory::createDevice(const core::string& type)
{
	GCPtr<IVideoDevice> dev=0;

	GCPtr<IDeviceFactoryCreater> c=getCreater(type);
	if(c){
		dev=c->create();
	}

	return dev;
}
/*GCPtr<IVideoDevice> DeviceFactory::createDevice(EDeviceType type,uint window,math::Point2di size,int bits,bool vsync,int multisample,bool stencilBuffer)
{
	GCPtr<IVideoDevice> dev=0;

	GCPtr<IDeviceFactoryCreater> c=getCreater(type);
	if(c){
		dev=c->create(window,size,bits,vsync,multisample,stencilBuffer);
	}

	return dev;
}
*/

void DeviceFactory::addCreater(GCPtr<IDeviceFactoryCreater> creater){
	m_creaters.push_back(creater);
}
void DeviceFactory::delCreater(GCPtr<IDeviceFactoryCreater> creater){
	DeviceCreaters::iterator it= m_creaters.begin();
	for(;it!=m_creaters.end();++it){
		if(*it==creater){
			m_creaters.erase(it);
			return;
		}
	}
}

int DeviceFactory::getCreatersCount(){
	return m_creaters.size();
}
GCPtr<IDeviceFactoryCreater> DeviceFactory::getCreater(int i){
	if(i>=m_creaters.size())return 0;
	return m_creaters[i];
}

GCPtr<IDeviceFactoryCreater> DeviceFactory::getCreater(const core::string& type){
	for(int i=0;i<m_creaters.size();++i){
		if(m_creaters[i]->canCreate(type)){
			return m_creaters[i];
		}
	}
	return 0;
}


/*GCPtr<IDeviceFactoryCreater> DeviceFactory::getCreater(EDeviceType type){
	for(int i=0;i<m_creaters.size();++i){
		if(m_creaters[i]->canCreate(type)){
			return m_creaters[i];
		}
	}
	return 0;
}*/

}
}


