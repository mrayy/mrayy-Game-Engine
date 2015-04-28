

#ifndef __WEBSCENE__
#define __WEBSCENE__


#include "IScene.h"
#include "AwesomiumWebView.h"

namespace mray
{
	
class WebScene:public IScene
{
protected:

	GCPtr<web::AwesomiumWebView> m_webview;

public:
	WebScene();
	virtual ~WebScene(); 

	virtual void OnInit() ;
	virtual void OnRender(const math::rectf& rc) ;
	virtual void OnUpdate(float dt) ;
};

}


#endif


