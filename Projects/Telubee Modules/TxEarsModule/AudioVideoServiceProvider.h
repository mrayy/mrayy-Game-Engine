

#ifndef __AUDIOVIDEOSERVICEPROVIDER__
#define __AUDIOVIDEOSERVICEPROVIDER__

#include "IServiceModule.h"

namespace mray
{
namespace TBee
{
	
class AudioVideoServiceProvider:public IServiceModule
{
public:
	static const std::string ServiceName;
	DECLARE_RTTI
protected:

	EServiceStatus m_status;

public:
	AudioVideoServiceProvider();
	virtual ~AudioVideoServiceProvider();

	virtual std::string GetServiceName(){ return ServiceName; }
	virtual EServiceStatus GetServiceStatus();

	virtual EServiceStatus StartService(ServiceContext* context);
	virtual bool StopService();
};

}
}


#endif
