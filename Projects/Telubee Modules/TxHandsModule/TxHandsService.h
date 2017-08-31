

#ifndef __TxHandsService__
#define __TxHandsService__

#include "IServiceModule.h"
#undef StartService


namespace mray
{
namespace TBee
{

	class TxHandsServiceImpl;
class TxHandsService :public IServiceModule
{
public:
	static const std::string ModuleName;

	DECLARE_RTTI
protected:
	TxHandsServiceImpl* m_impl;

public:
	TxHandsService();
	virtual ~TxHandsService();

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
