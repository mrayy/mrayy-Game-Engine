
#include "stdafx.h"
#include "AwesomiumManager.h"
#include "AwesomiumWebView.h"
#include "AwesomiumSurfaceFactory.h"


namespace mray
{
namespace web
{

AwesomiumManager::AwesomiumManager()
{
	m_core = 0;
}


AwesomiumManager::~AwesomiumManager()
{
	Shutdown();
}



bool AwesomiumManager::Init(WebConfig &conf)
{
	if (m_core)
		return false;
	m_core = WebCore::Initialize(conf);

	m_surfFactory = new AwesomiumSurfaceFactory();

	m_core->set_surface_factory(m_surfFactory);

	return m_core != 0;
}


void AwesomiumManager::Shutdown()
{
	if (!m_core)
		return;
	WebCore::Shutdown();
	delete m_surfFactory;
	m_core =0;
}

AwesomiumWebView* AwesomiumManager::CreateView()
{
	if (!m_core)
		return 0;

	AwesomiumWebView* view;

	WebView* v = m_core->CreateWebView(1, 1);

	view = new AwesomiumWebView(v);
	return view;
}


void AwesomiumManager::Update()
{
	if (!m_core)
		return;
	m_core->Update();
}


}
}
