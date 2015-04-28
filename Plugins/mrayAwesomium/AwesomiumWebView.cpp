

#include "stdafx.h"
#include "AwesomiumWebView.h"
#include "AwesomiumSurfaceFactory.h"
#include "AwesomiumHelpers.h"

using namespace Awesomium;

namespace mray
{
namespace web
{

AwesomiumWebView::AwesomiumWebView(WebView* view)
{
	m_view = view;
}
AwesomiumWebView::~AwesomiumWebView()
{

}

video::ITexturePtr AwesomiumWebView::GetSurface()
{
	AwesomiumSurface* s=(AwesomiumSurface*) m_view->surface();
	if (!s)
		return video::ITexturePtr::Null;
	return s->GetTexture();
}

void AwesomiumWebView::LoadURL(const core::string& url)
{
	m_view->LoadURL(WebURL( AwesomiumHelpers::ToString(url)));
}
void AwesomiumWebView::LoadFile(const core::string& path)
{

}

}
}

