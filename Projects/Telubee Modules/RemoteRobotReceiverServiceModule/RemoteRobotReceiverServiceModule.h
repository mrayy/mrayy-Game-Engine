

#ifndef __RemoteRobotReceiverServiceModule__
#define __RemoteRobotReceiverServiceModule__

#include "IServiceModule.h"
#undef StartService


namespace mray
{
namespace TBee
{

	class RemoteRobotReceiverServiceModuleImpl;
class RemoteRobotReceiverServiceModule :public IServiceModule
{
public:
	static const std::string ModuleName;

	DECLARE_RTTI
protected:
	RemoteRobotReceiverServiceModuleImpl* m_impl;

public:
	RemoteRobotReceiverServiceModule();
	virtual ~RemoteRobotReceiverServiceModule();

	virtual std::string GetServiceName();
	virtual EServiceStatus GetServiceStatus();

	virtual void InitService(ServiceContext* context);
	virtual EServiceStatus StartService(ServiceContext* context);
	virtual bool StopService();
	virtual void DestroyService();

	virtual void Update(float dt);
	virtual void Render(ServiceRenderContext* contex);
	virtual void DebugRender(ServiceRenderContext* contex);

	virtual bool LoadServiceSettings(xml::XMLElement* e);
	virtual void ExportServiceSettings(xml::XMLElement* e);
};

}
}


#endif
