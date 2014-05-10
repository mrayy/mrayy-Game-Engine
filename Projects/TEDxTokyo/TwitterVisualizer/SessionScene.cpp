

#include "stdafx.h"
#include "SessionScene.h"

#include "GUIScreenLayoutImplV2.h"
#include "GUIManager.h"
#include "GUIThemeManager.h"
#include "GUIOverlayManager.h"
#include "GUIOverlay.h"

#include "ParsedShaderPP.h"
#include "ViewPort.h"
#include "SceneManager.h"

#include "SessionRenderer.h"
#include "AppData.h"
#include "TwitterProvider.h"
#include "TwitterTweet.h"

#include "TweetNode.h"

namespace mray
{
namespace ted
{


	class TwitterProviderListener :public ted::ITwitterProviderListener
	{
	public:
		scene::SessionRenderer* r;
		virtual void OnTweetsLoaded(const std::vector<ted::TwitterTweet*>& tweets)
		{
			printf("Tweets Loaded: %d\n", tweets.size());
			std::vector<scene::TweetNode*> nodes;
			for (int i = 0; i < tweets.size(); ++i)
			{
				if (true||tweets[i]->text.find(L"#TEDxTokyo") != -1)
				{
					scene::TweetNode* n = new scene::TweetNode(0, tweets[i]);
					nodes.push_back(n);
				}else
					delete tweets[i];
			}
			r->AddTweetsNodes(nodes);
		}
	}g_cb;

SessionScene::SessionScene()
{
}

SessionScene::~SessionScene()
{
}



void SessionScene::Init()
{
	IRenderingScene::Init();
	video::IVideoDevice* dev = Engine::getInstance().getDevice();

	m_guiMngr = new GUI::GUIManager(dev);
	m_sceneManager = new scene::SceneManager(dev);

	{
		video::IRenderTargetPtr rt = dev->createRenderTarget("", dev->createTexture2D(1, video::EPixel_R8G8B8A8, false), 0, 0, 0);
		m_vp = new scene::ViewPort("", m_camera, rt, 0, math::rectf(0, 0, 1, 1), 0);
		m_vp->enablePostProcessing(true);

		video::ParsedShaderPP* pp = new video::ParsedShaderPP(dev);

		pp->LoadXML(gFileSystem.openFile("blur.peff"));
		m_vp->setPostProcessing(pp);
	}

	m_guiMngr->SetActiveTheme(GUI::GUIThemeManager::getInstance().getActiveTheme());

	m_guiroot = (GUI::IGUIPanelElement*)new GUI::IGUIPanelElement(core::string(""), m_guiMngr);
	m_guiroot->SetDocking(GUI::EED_Fill);
	m_guiMngr->SetRootElement(m_guiroot);

	{
		GUI::GUIOverlay* screenOverlay = GUI::GUIOverlayManager::getInstance().LoadOverlay("GUIScreenLayout_V2.gui");
		m_screenLayout = new GUI::GUIScreenLayoutImplV2();
		screenOverlay->CreateElements(m_guiMngr, m_guiroot, 0, m_screenLayout);
	}
	{
		m_sessionRenderer = new scene::SessionRenderer();
		m_sessionRenderer->SetSessions(gAppData.sessions);
	}
	if (true)
	{
		g_cb.r = m_sessionRenderer;
		if (gAppData.tweetProvider->IsAuthorized())
		{
			std::vector<ted::TwitterTweet*> tweets;
			gAppData.tweetProvider->GetTweetsAsynced(L"tedxtokyo", 0, 100, &g_cb);
		}


	}
}

void SessionScene::OnEnter()
{
	gAppData.leapDevice->AddListener(this);
}

void SessionScene::OnExit()
{
	gAppData.leapDevice->RemoveListener(this);
}


bool SessionScene::OnEvent(Event* e, const math::rectf& rc)
{
	return m_guiMngr->OnEvent(e, &rc);
}

void SessionScene::Update(float dt)
{
	m_sceneManager->update(dt);
	m_guiMngr->Update(dt);
	m_sessionRenderer->Update(dt);

}

video::IRenderTarget* SessionScene::Draw(const math::rectf& rc)
{
	IRenderingScene::Draw(rc);

	Engine::getInstance().getDevice()->setRenderTarget(GetRenderTarget());
	m_sessionRenderer->Draw();
	m_guiMngr->DrawAll(&rc);
	return GetRenderTarget();
}


}
}
