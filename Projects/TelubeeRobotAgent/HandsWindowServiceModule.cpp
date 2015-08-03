
#include "stdafx.h"
#include "HandsWindowServiceModule.h"
#include "TBeeServiceContext.h"
#include "HandsWindow.h"

namespace mray
{
namespace TBee
{

	IMPLEMENT_RTTI(HandsWindowServiceModule,IServiceModule)
	const std::string HandsWindowServiceModule::ModuleName("HandsWindowServiceModule");

	class HandsWindowServiceModuleImpl :public IServiceContextListener
	{
	public:

		GCPtr<HandsWindow> m_handsWindow;
		EServiceStatus m_status;
		TBeeServiceContext* m_context;
	public:

		HandsWindowServiceModuleImpl()
		{
			m_handsWindow = new HandsWindow();
			m_status = EServiceStatus::Idle;
			m_context = 0;
		}

		~HandsWindowServiceModuleImpl()
		{
			m_handsWindow = 0;
		}

		void Init(TBeeServiceContext* context)
		{
			m_context = context;
			m_handsWindow->Parse(context->appOptions);
			m_handsWindow->OnInit(context);

			context->AddListener(this);
			m_status = EServiceStatus::Inited;
		}
		void Destroy()
		{
			if (m_status == EServiceStatus::Idle)
				return;
			m_context->RemoveListener(this);

			m_handsWindow->OnClose();

			m_status = EServiceStatus::Idle;

		}

		void Start()
		{
			if (m_status != EServiceStatus::Inited && m_status != EServiceStatus::Stopped)
				return;
			m_handsWindow->OnEnable();

			m_status = EServiceStatus::Running;
		}
		bool Stop()
		{
			if (m_status != EServiceStatus::Running)
				return false;
			m_handsWindow->OnDisable();
			m_status = EServiceStatus::Stopped;
			return true;
		}
		void Update(float dt)
		{
			if (m_status != EServiceStatus::Running)
				return;
			m_handsWindow->OnUpdate(dt);
		}
		void DebugRender(TbeeServiceRenderContext* context)
		{
			if (m_status == EServiceStatus::Idle)
				return;
		}
		void Render(TbeeServiceRenderContext* context)
		{
			if (m_status == EServiceStatus::Idle)
				return;

		}


		//////////////////////////////////////////////////////////////////////////
		/// Listeners
		virtual void OnUserConnected(const UserConnectionData& data)
		{
			m_handsWindow->OnConnected(data.address.toString(), data.handsPort, data.rtcp);
		}
	};


HandsWindowServiceModule::HandsWindowServiceModule()
{
	m_impl = new HandsWindowServiceModuleImpl();
}

HandsWindowServiceModule::~HandsWindowServiceModule()
{
	delete m_impl;
}


std::string HandsWindowServiceModule::GetServiceName()
{
	return HandsWindowServiceModule::ModuleName;
}

EServiceStatus HandsWindowServiceModule::GetServiceStatus()
{
	return m_impl->m_status;
}

void HandsWindowServiceModule::InitService(ServiceContext* context)
{
	m_impl->Init((TBeeServiceContext*)context);
}

EServiceStatus HandsWindowServiceModule::StartService(ServiceContext* context)
{
	m_impl->Start();
	return m_impl->m_status;
}

bool HandsWindowServiceModule::StopService()
{
	return m_impl->Stop();
}

void HandsWindowServiceModule::DestroyService()
{
	m_impl->Destroy();
}


void HandsWindowServiceModule::Update(float dt)
{
	m_impl->Update(dt);
}

void HandsWindowServiceModule::Render(ServiceRenderContext* contex)
{
	m_impl->Render((TbeeServiceRenderContext*)contex);
}

void HandsWindowServiceModule::DebugRender(ServiceRenderContext* contex)
{
	m_impl->DebugRender((TbeeServiceRenderContext*)contex);
}

bool HandsWindowServiceModule::LoadServiceSettings(xml::XMLElement* e)
{
	return true;
}

void HandsWindowServiceModule::ExportServiceSettings(xml::XMLElement* e)
{
}

}
}


