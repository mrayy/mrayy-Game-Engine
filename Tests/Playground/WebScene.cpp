

#include "stdafx.h"
#include "WebScene.h"
#include "AwesomiumManager.h"


namespace mray
{

WebScene::WebScene()
{

}

WebScene::~WebScene()
{

}


void WebScene::OnInit()
{
	m_webview = web::AwesomiumManager::getInstance().CreateView();
	m_webview->LoadURL("https://www.youtube.com/watch?v=wxd4bY_o_4k");
	m_webview->GetView()->Resize(1280, 720);
}

void WebScene::OnRender(const math::rectf& rc)
{
	gEngine.getDevice()->set2DMode();
	
	video::TextureUnit t;
	t.SetTexture(m_webview->GetSurface());
	gEngine.getDevice()->useTexture(0, &t);
	gEngine.getDevice()->draw2DImage(rc, 1);
}

void WebScene::OnUpdate(float dt)
{
	
}


}
