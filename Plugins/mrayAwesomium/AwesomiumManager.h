

#ifndef __AWESOMIUMMANAGER__
#define __AWESOMIUMMANAGER__

#include "Awesomium/WebCore.h"
#include "Awesomium/WebConfig.h"
#include "ISingleton.h"

using namespace Awesomium;

namespace mray
{
namespace web
{
	class AwesomiumWebView;
	class AwesomiumSurfaceFactory;
	
class AwesomiumManager :public ISingleton<AwesomiumManager>
{
protected:
	WebCore* m_core;
	AwesomiumSurfaceFactory* m_surfFactory;
public:
	AwesomiumManager();
	virtual ~AwesomiumManager();

	bool Init(WebConfig &conf);
	void Shutdown();

	AwesomiumWebView* CreateView();

	void Update();
};

}
}


#endif
