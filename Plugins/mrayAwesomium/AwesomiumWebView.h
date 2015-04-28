

#ifndef __AWESOMIUMWEBVIEW__
#define __AWESOMIUMWEBVIEW__

#include "ITexture.h"
#include "Awesomium/WebView.h"

namespace mray
{
namespace web
{
	
class AwesomiumWebView
{
protected:
	Awesomium::WebView* m_view;
public:
	AwesomiumWebView(Awesomium::WebView* view);
	virtual ~AwesomiumWebView();

	Awesomium::WebView* GetView(){ return m_view; }

	video::ITexturePtr GetSurface();

	void LoadURL(const core::string& url);
	void LoadFile(const core::string& path);
};

}
}


#endif
