

#include "stdafx.h"
#include "OpenNIService.h"
#include "TBeeServiceContext.h"
#include "IThreadManager.h"
#include "CommunicationMessages.h"
#include "StreamReader.h"
#include "XMLTree.h"
#include "NetworkValueController.h"
#include "StringUtil.h"
#include "INetwork.h"
#include <conio.h>

#include "OpenNIHandler.h"
#include "GeomDepthRect.h"
#include "OpenNIManager.h"
#include "ofxDepthStreamCompression.h"

namespace mray
{
namespace TBee
{

IMPLEMENT_RTTI(OpenNIService, IServiceModule);

const std::string OpenNIService::ModuleName("OpenNIService");

#define PC_COMM_PORT 8010

class OpenNIServiceImpl :public IServiceContextListener
{
public:

	ofxDepthStreamCompression m_compressor;

	EServiceStatus m_status;

	TBeeServiceContext* m_context;
	

	OpenNIHandler* m_openNi;
	GeomDepthRect m_depthRect;
	OpenNIManager *m_openNIMngr;
	bool m_depthSend;

	int _dataSize,_compSize;


public:
	OpenNIServiceImpl()
	{
		m_status = EServiceStatus::Idle;
		m_openNi = 0;
	}
	~OpenNIServiceImpl()
	{
		Destroy();
	}

	void Init(TBeeServiceContext* context)
	{
		if (m_status != EServiceStatus::Idle)
			return;

		m_context = context;

		_RegisterValues();

		context->AddListener(this);
		m_status = EServiceStatus::Inited;


		m_openNIMngr = new OpenNIManager();
		m_openNIMngr->Init(0, 0);
		m_openNi = new TBee::OpenNIHandler;
		m_openNi->Init();

		gLogManager.log("Done Initing.", ELL_INFO);
	}

	void _RegisterValues()
	{

	}

	void StartService()
	{
		m_openNi->Start(320,240);
		m_compressor.setup(320, 240);
		m_status = EServiceStatus::Running;
	}

	bool StopService()
	{
		m_openNi->Close();
		m_status = EServiceStatus::Stopped;
		return true;
	}
	void Destroy()
	{
		if (m_status == EServiceStatus::Idle)
			return;

		if (m_openNi)
			m_openNi->Close();
		delete m_openNi;
		
		if (m_openNIMngr)
			m_openNIMngr->Close();
		delete m_openNIMngr;
		m_status = EServiceStatus::Idle;
	}




	void Update()
	{

		if (m_status != EServiceStatus::Running)
			return;
		

		auto d = m_openNi->GetNormalCalculator().GetDepthFrame();
		if (d->GetSize().x ==0)
			return;

		auto frame = m_compressor.newFrame(d, 1, 1);
		auto c = frame->compressedData();

		_compSize= c.size();
		_dataSize = d->GetRawDataLength(); 

		ofxDepthCompressedFrame f;
		f.fromCompressedData((char*)&c[0], c.size()*sizeof(short));
	}


	void DebugRender(ServiceRenderContext* context)
	{

		core::string msg = "[" + OpenNIService::ModuleName + "] Service Status: " + IServiceModule::ServiceStatusToString(m_status);
		context->RenderText(msg, 0, 0);

		if (m_status != EServiceStatus::Running)
			return;

		msg = "Compression: " + core::StringConverter::toString(_compSize);
		msg += "/" + core::StringConverter::toString(_dataSize);
		msg += " Ratio: " + core::StringConverter::toString((int)(_compSize * 100.0 / (float)_dataSize)) + "%";
		context->RenderText(msg, 0, 0);
	}
	void Render(ServiceRenderContext* context)
	{
		if (m_status == EServiceStatus::Idle)
			return;
	}



	//////////////////////////////////////////////////////////////////////////
	/// Listeners
	virtual void OnUserConnected(const UserConnectionData& data)
	{
		const int BufferLen = 128;
		uchar buffer[BufferLen];
		//tell the client if we are sending stereo or single video images
		OS::CMemoryStream stream("", buffer, BufferLen, false, OS::BIN_WRITE);
		OS::StreamWriter wrtr(&stream);

	}
	virtual void OnUserDisconnected()
	{
	}


	virtual void OnUserMessage(network::NetAddress* addr, const core::string& msg, const core::string& value)
	{

		const int BufferLen = 128;
		uchar buffer[BufferLen];
		//tell the client if we are sending stereo or single video images
		OS::CMemoryStream stream("", buffer, BufferLen, false, OS::BIN_WRITE);
		OS::StreamWriter wrtr(&stream);
		std::vector<core::string> values = core::StringUtil::Split(value, ",");

		if (msg.equals_ignore_case("depthSize"))
		{
			int reply = (int)EMessages::DepthSize;
			int len = stream.write(&reply, sizeof(reply));
			math::vector2di sz = m_openNi->GetSize();
			len += stream.write(&sz, sizeof(sz));
			m_context->commChannel->SendTo(&m_context->remoteAddr, (char*)buffer, len);
		}
		else
		if (msg.equals_ignore_case("depth"))
		{
			math::rectf rc = core::StringConverter::toRect(value);
			TBee::DepthFrame* f = m_openNi->GetNormalCalculator().GetDepthFrame();
			m_depthRect.SetFrame(f, rc);
			int reply = (int)EMessages::DepthData;
			int len = stream.write(&reply, sizeof(reply));
			len += m_depthRect.WriteToStream(&stream);
			m_context->commChannel->SendTo(&m_context->remoteAddr, (char*)buffer, len);
		}

	}

	bool LoadServiceSettings(xml::XMLElement* elem)
	{
		xml::XMLElement* e;
		xml::XMLAttribute*a;
		

		return true;
	}
};

OpenNIService::OpenNIService()
{
	m_impl = new OpenNIServiceImpl();
}

OpenNIService::~OpenNIService()
{
	delete m_impl;
}


std::string OpenNIService::GetServiceName()
{
	return OpenNIService::ModuleName;
}

EServiceStatus OpenNIService::GetServiceStatus()
{
	return m_impl->m_status;
}

void OpenNIService::InitService(ServiceContext* context)
{
	m_impl->Init((TBeeServiceContext*)context);
}

EServiceStatus OpenNIService::StartService(ServiceContext* context)
{
	m_impl->StartService();
	return m_impl->m_status;
}

bool OpenNIService::StopService()
{
	return m_impl->StopService();
}

void OpenNIService::DestroyService()
{
	m_impl->Destroy();
}


void OpenNIService::Update(float dt)
{
	m_impl->Update();
}

void OpenNIService::Render(ServiceRenderContext* contex)
{
	m_impl->Render(contex);
}

void OpenNIService::DebugRender(ServiceRenderContext* contex)
{
	m_impl->DebugRender(contex);
}

bool OpenNIService::LoadServiceSettings(xml::XMLElement* e)
{
	return m_impl->LoadServiceSettings(e);
}

void OpenNIService::ExportServiceSettings(xml::XMLElement* e)
{
}

}
}

