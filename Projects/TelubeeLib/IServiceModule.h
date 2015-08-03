

#ifndef __ISERVICEMODULE__
#define __ISERVICEMODULE__

#include "ListenerContainer.h"
#include "ServiceContext.h"
#include "RTTI.h"

#undef StartService

namespace mray
{
	namespace xml
	{
		class XMLElement;
	}
namespace TBee
{

	enum class EServiceStatus
	{
		Idle,
		Inited,
		Running,
		Stopped,
		Shutdown
	};

	class IServiceModule;


	class IServiceModuleListener
	{
	public:
		virtual void OnServiceStarted(IServiceModule* s){};
		virtual void OnServiceStopped(IServiceModule* s){};
	};
	
class IServiceModule:public ListenerContainer<IServiceModuleListener*>
{
	DECLARE_RTTI;
protected:
public:
	IServiceModule(){}
	virtual ~IServiceModule(){}

	virtual std::string GetServiceName() = 0;
	virtual EServiceStatus GetServiceStatus() = 0;
	
	virtual void InitService(ServiceContext* context) = 0;
	virtual EServiceStatus StartService(ServiceContext* context) = 0;
	virtual bool StopService() = 0;
	virtual void DestroyService() = 0;

	virtual void Update(float dt) = 0;
	virtual void Render(ServiceRenderContext* contex) = 0;
	virtual void DebugRender(ServiceRenderContext* contex) = 0;

	virtual bool LoadServiceSettings(xml::XMLElement* e) = 0;
	virtual void ExportServiceSettings(xml::XMLElement* e) = 0;

	static core::string ServiceStatusToString(EServiceStatus s);
};

}
}


#endif