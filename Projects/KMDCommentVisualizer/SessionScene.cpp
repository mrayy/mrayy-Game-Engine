

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
#include "KMDComment.h"
#include "SessionContainer.h"
#include "SessionDetails.h"

#include "CommentNode.h"
#include "ProjectNode.h"
#include "IThreadFunction.h"
#include "IThreadManager.h"
#include "GUISceneSpacePanel.h"
#include "GUIElementRegion.h"

#include "GUISessionSidePanel.h"
#include "GUISpeakerDetailsPanel.h"
#include "PointerNode.h"
#include "Application.h"
#include "RenderWindow.h"

#include "FontResourceManager.h"
#include "IGUIRenderer.h"
#include "IDBHandler.h"
#include "GenericRenderLayer.h"
#include "WallpaperLayer.h"

namespace mray
{
namespace kmd
{

	/*

	class SessionRendererCallback
	{
		GUI::GUISessionSidePanel* m_sidePanel;
		CSubProject* m_selectedSpeaker;
	public:

		SessionRendererCallback()
		{
			m_selectedSpeaker = 0;
		}
		CSubProject* GetSpeaker(){ return m_selectedSpeaker; }

		void SetSidePanel(GUI::GUISessionSidePanel* sp)
		{
			sp->OnSpeakerChange += CreateObjectDelegate(SessionRendererCallback, this, _OnSubProjectChange);
		}
		void _OnSubProjectChange(IObject* sender, PVOID param)
		{
			m_selectedSpeaker = (CSubProject*)param;
			gAppData.subProjectChange.__FIRE__OnSubProjectChange(m_selectedSpeaker);
		}

	}g_sessionCallback;*/

	class TwitterProviderListener :public OS::IThreadFunction
	{
	protected:
		float m_lastTime;
		OS::IThreadPtr m_thread;
		kmd::IDType m_sinceID;
	public:
		scene::SessionRenderer* r;
		float interval;
	public:

		TwitterProviderListener()
		{
			interval = 1000;
			m_sinceID = -1;
		}
		~TwitterProviderListener()
		{
			Stop();
		}

		void Start()
		{
			m_thread = OS::IThreadManager::getInstance().createThread(this);
			m_thread->start(0);
			m_lastTime = 0;

		}
		void Stop()
		{
			OS::IThreadManager::getInstance().killThread(m_thread);
			m_thread = 0;
		}
		virtual void OnCommentsLoaded(const std::vector<kmd::KMDComment*>& comments)
		{
			if (!comments.size())
				return;
			printf("Comments Loaded: %d\n", comments.size());
			for (int i = 0; i< comments.size(); ++i)
			{
				m_sinceID = math::Max(m_sinceID, comments[i]->ID);
			}
			gAppData.commentsCallback.OnCommentsLoaded(comments);
			//r->AddComments(comments);
		}
		virtual void execute(OS::IThread*caller, void*arg)
		{
			while (caller->isActive())
			{
				float t = gEngine.getTimer()->getSeconds();
				float dt = (t - m_lastTime);
				if (dt > interval)
				{
					std::vector<kmd::KMDComment*> comments;

					comments=IDBHandler::getInstance().LoadComments(m_sinceID);
					OnCommentsLoaded(comments);
					m_lastTime = gEngine.getTimer()->getSeconds();
				}
				else
					OS::IThreadManager::getInstance().sleep(dt / 2);
			}
		}

	};

SessionScene::SessionScene()
{
	m_providerListener = new TwitterProviderListener;
	m_pointer = new scene::PointerNode();

	gAppData.subProjectChange.AddListener(this);

	m_activeSubProject = 0;

	m_screenLayout = 0;
	m_sceneLayer = new GenericRenderLayer();
	m_commentsLayer = new GenericRenderLayer();
	m_statusLayer = new GenericRenderLayer();
	m_wallpaperLayer = new WallpaperLayer();
	m_isTimeBased = false;
}

SessionScene::~SessionScene()
{
	delete m_providerListener;
	delete m_pointer;

	delete m_statusLayer;
	delete m_commentsLayer;
	delete m_sceneLayer;
	delete m_wallpaperLayer;
}



void SessionScene::Init()
{
	IRenderingScene::Init();
	video::IVideoDevice* dev = Engine::getInstance().getDevice();

	m_guiMngr = new GUI::GUIManager(dev);
	m_sceneManager = new scene::SceneManager(dev);

	{
		video::IRenderTargetPtr rt = dev->createRenderTarget("", dev->createTexture2D(1, video::EPixel_R8G8B8A8, false), 0, 0, 0);
		m_vp = new scene::ViewPort("", 0, rt, 0, math::rectf(0, 0, 1, 1), 0);
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
		//load subprojects
		for (int i = 0; i < gAppData.sessions->GetSessions().size(); ++i)
		{
			kmd::SessionDetails* d = gAppData.sessions->GetSessions()[i];
			for (int j = 0; j < d->GetProjects().size(); ++j)
			{
				m_subProjects.push_back(d->GetProjects()[j]);
			}
		}
		m_sessionRenderer = new scene::SessionRenderer();
		m_sessionRenderer->SetSessions(gAppData.sessions);

		//user loaded Comments

		m_sessionRenderer->AddComments(KMDComment::KMDCommentList);
	}
	{
		GUI::GUIOverlay* screenOverlay = GUI::GUIOverlayManager::getInstance().LoadOverlay("GUIScreenLayout_V2.gui");
		m_screenLayout = new GUI::GUIScreenLayoutImplV2();
		screenOverlay->CreateElements(m_guiMngr, m_guiroot, 0, m_screenLayout);
//		g_sessionCallback.SetSidePanel(m_screenLayout->SessionsBar);
		m_screenLayout->ScenePanel->SetSessionRenderer(m_sessionRenderer);


		m_screenLayout->SessionsBar->SetSessionContainer(gAppData.sessions);
		m_screenLayout->StatusPanel->SetProjects(gAppData.sessions);
	}
	{
		m_commentsLayer->InitLayer(m_screenLayout->CommentsPanel);
		m_sceneLayer->InitLayer(m_screenLayout->ScenePanel);
		m_statusLayer->InitLayer(m_screenLayout->StatusPanel);
		m_wallpaperLayer->InitLayer(m_screenLayout->WallpaperLayer);

	//	m_layerOrder.push_front(m_wallpaperLayer);
		m_layerOrder.push_back(m_statusLayer);
		m_layerOrder.push_back(m_sceneLayer);
		m_layerOrder.push_front(m_commentsLayer);
		
		m_statusLayer->SetVisible(false);
	}
#if 0
	{
		m_bgm = gAppData.soundManager->loadSound("sounds//TedXTokyoBGM.mp3",true,sound::ESNDT_2D);
		m_bgm->setLooping(true);
	}
#endif 
	m_providerListener->r = m_sessionRenderer;
	SetTimerBased(false);

}

void SessionScene::OnEnter()
{
#if 0
	m_bgm->play();
#endif
	
	gAppData.commentsCallback.AddListener(m_sessionRenderer);
	gAppData.commentsCallback.AddListener(this);
	m_providerListener->Start();
}

void SessionScene::OnExit()
{
#if 0
	m_bgm->stop();
#endif

	m_providerListener->Stop();
	gAppData.commentsCallback.RemoveListener(m_sessionRenderer);
	gAppData.commentsCallback.RemoveListener(this);

}


bool SessionScene::OnEvent(Event* e, const math::rectf& rc)
{

	if (e->getType() == ET_Mouse )
	{
		MouseEvent *evt = (MouseEvent *)e;
		if (evt->event == MET_MOVED)
		{
			m_pointer->MoveBy(evt->pos - m_pointer->GetPosition());
			//m_pointer->Show();
		}
		if (evt->event == MET_LEFTDOWN)
		{
			scene::IKMDNode* node = m_sessionRenderer->GetNodeFromPosition(evt->pos);
			if (dynamic_cast<scene::CommentNode*>(node))
			{
				scene::CommentNode*tn=dynamic_cast<scene::CommentNode*>(node);
				kmd::KMDComment* t = tn->GetComment();
//				m_screenLayout->CommentDetails->SetComment(t);
			}
			else 
			{
				m_sessionRenderer->SetHoverdItem(node);
			}

		}

		return m_guiMngr->OnEvent(e, &rc);
	}
	else if (e->getType() == ET_Keyboard)
	{

		KeyboardEvent* evt = dynamic_cast<KeyboardEvent*>(e);
		if (evt->press)
		{
			if (evt->key == KEY_DOWN)
			{

				//	if (!m_isTimeBased)
					SetCurrentSubProject(m_activeSubProject + 1);
			}
			if (evt->key == KEY_UP)
			{
			//	if (!m_isTimeBased)
					SetCurrentSubProject(m_activeSubProject - 1);
			}
			if (evt->key == KEY_PGUP)
			{

				//	if (!m_isTimeBased)
					SetPrevProject();
			}
			if (evt->key == KEY_PGDOWN)
			{

				//if (!m_isTimeBased)
					SetNextProject();
			}
			if (evt->key == KEY_T)
			{
				SetTimerBased(!m_isTimeBased);
			}
			if (evt->key == KEY_HOME)
			{

				//	if (!m_isTimeBased)
					SetCurrentSubProject(0);
			}
			if (evt->key == KEY_R)
			{
				m_timerStart = gEngine.getTimer()->getSeconds();
			}
			
			/*
			if (evt->key == KEY_C)
			{
				m_commentsLayer->SetVisible(!m_commentsLayer->IsVisible());
				RefreshLayerAlpha();
			}
			if (evt->key == KEY_V)
			{
				m_sceneLayer->SetVisible(!m_sceneLayer->IsVisible());
				RefreshLayerAlpha();
			}
			if (evt->key == KEY_B)
			{
				m_statusLayer->SetVisible(!m_statusLayer->IsVisible());
				RefreshLayerAlpha();
			}*/
			if (evt->key == KEY_1)
			{
				m_commentsLayer->SetVisible(!m_commentsLayer->IsVisible());
				MakeLayerTop(m_commentsLayer);
				RefreshLayerAlpha();
			}
			if (evt->key == KEY_3)
			{
				//MakeLayerTop(m_statusLayer);
				m_statusLayer->SetVisible(!m_statusLayer->IsVisible());
				m_commentsLayer->SetVisible(!m_statusLayer->IsVisible());
				m_sceneLayer->SetVisible(!m_statusLayer->IsVisible());
				RefreshLayerAlpha();
				/*if (!m_statusLayer->IsVisible())
				{
					MakeLayerTop(m_commentsLayer);
				}else
				{
					MakeLayerTop(m_statusLayer);
				}*/
			}
		}
		return m_guiMngr->OnEvent(e, &rc);
	}

	return false;
}
void SessionScene::SetTimerBased(bool timer)
{
	m_isTimeBased = timer;
	if (!m_screenLayout)
		return;
	m_screenLayout->ScenePanel->TimerIco->SetVisible(m_isTimeBased);

}
void SessionScene::MakeLayerTop(GenericRenderLayer* layer)
{
	if (!layer->GetPanelElement())
		return;
	m_screenLayout->LayersRoot->MoveElementToTop(layer->GetPanelElement());

	std::list<GenericRenderLayer*>::iterator it= m_layerOrder.begin();
	for (; it != m_layerOrder.end(); ++it)
	{
		if (*it == layer)
		{
			m_layerOrder.erase(it);
			break;
		}
	}
	m_layerOrder.push_front(layer);
	
	RefreshLayerAlpha();

}
void SessionScene::MakeLayerBottom(GenericRenderLayer* layer)
{
	if (!layer->GetPanelElement())
		return;
	m_screenLayout->LayersRoot->MoveElementToBottom(layer->GetPanelElement());
	std::list<GenericRenderLayer*>::iterator it = m_layerOrder.begin();
	for (; it != m_layerOrder.end(); ++it)
	{
		if (*it == layer)
		{
			m_layerOrder.erase(it);
			break;
		}
	}
	m_layerOrder.push_back(layer);
	RefreshLayerAlpha();

}
void SessionScene::RefreshLayerAlpha()
{
	int cnt = m_sceneLayer->IsVisible() + m_commentsLayer->IsVisible() + m_statusLayer->IsVisible();

	std::list<GenericRenderLayer*>::iterator it = m_layerOrder.begin();
	for (int i = 0; it != m_layerOrder.end(); ++it)
	{
		if ((*it)->IsVisible())
		{
			(*it)->SetAlpha(1-(float)(i ) / (float)cnt);
			++i;
		}
	}
}


void SessionScene::SetNextProject()
{
	SessionDetails* s= m_subProjects[m_activeSubProject]->GetSession();
	for (int i = m_activeSubProject; i < m_subProjects.size(); ++i)
	{
		if (m_subProjects[i]->GetSession() != s)
		{
			SetCurrentSubProject(i);
			break;;
		}
	}
}
void SessionScene::SetPrevProject()
{

	SessionDetails* s = m_subProjects[m_activeSubProject]->GetSession();
	for (int i = m_activeSubProject; i >=0; --i)
	{
		if (m_subProjects[i]->GetSession() != s)
		{
			SetCurrentSubProject(i);
			break;;
		}
	}
}

void SessionScene::SetCurrentSubProject(int i)
{
	if (i < 0)
	{
		i = 0;
	}
	if (i >= m_subProjects.size())
		i = m_subProjects.size() - 1;
	m_activeSubProject = i;

	gAppData.subProjectChange.OnSubProjectChange(m_subProjects[m_activeSubProject]);

}

void SessionScene::Update(float dt)
{
	m_sceneManager->update(dt);
	m_guiMngr->Update(dt);
	m_sessionRenderer->Update(dt);

	m_pointer->Update(dt);

	{
		scene::ProjectNode* node= m_sessionRenderer->GetCurrentSpeaker();
		if (node)
		{
			int cnt=node->GetSubComments().size();
			if (cnt > 20)cnt = 20;
			float a = (1.0f-(float)cnt / 20.0f)*0.8+0.2f;
			m_screenLayout->ScenePanel->Info->SetTargetAlpha(a);
		}
	}

	m_statusLayer->UpdateLayer(dt);
	m_commentsLayer->UpdateLayer(dt);
	m_sceneLayer->UpdateLayer(dt);
	m_wallpaperLayer->UpdateLayer(dt);

	core::CTime t = core::CTime::Now();
	m_screenLayout->SessionDetails->Time->SetText(core::CTime::ToString(t,true));

	if (m_activeSubProject < m_subProjects.size())
	{
		CSubProject* sp = m_subProjects[m_activeSubProject];
		SessionDetails* s = sp->GetSession();

	}
	if (m_isTimeBased)
	{

		float time = gEngine.getTimer()->getSeconds();
		int span = (time - m_timerStart) / 1000;;
		int hours = floor((span) / (3600.0f));
		int mins = floor((span - hours * 3600.0f)/60.0f);
		int sec = floor(span % 60);
		core::CTime timeLeft(hours, mins, sec);
		m_screenLayout->ScenePanel->Time->SetText(core::CTime::ToString(timeLeft, true));

		if (false && m_activeSubProject < m_subProjects.size())
		{
			CSubProject* sp = m_subProjects[m_activeSubProject];
			SessionDetails* s = sp->GetSession();
			int totalLength = 0;
			for (int i = 0; i<s->GetProjects().size(); ++i)
			{
				if (s->GetProjects()[i]==sp)
					break;;
				totalLength += s->GetProjects()[i]->GetLength();
			}
			core::CTime start = s->GetSessionStartTime();
			int m = start.GetMinute() + totalLength;
			if (m>60)
			{
				start.SetHour(start.GetHour() + 1);
				m -= 60;
			}
			start.SetMinute(m);
			core::CTime end = start;
			m = start.GetMinute() + sp->GetLength();
			if (m > 60)
			{
				end.SetHour(start.GetHour() + 1);
				m -= 60;
			}
			end.SetMinute(m);
			if (t>start && t<end)
				SetCurrentSubProject(m_activeSubProject + 1);
		}
	}
}

video::IRenderTarget* SessionScene::Draw(const math::rectf& rc)
{
	IRenderingScene::Draw(rc);

	Engine::getInstance().getDevice()->setRenderTarget(GetRenderTarget());
	m_guiMngr->DrawAll(&rc);
	m_pointer->Draw();


	GUI::IFont* font = gFontResourceManager.getDefaultFont();

	if (gAppData.Debugging && font)
	{
		GUI::FontAttributes attr;
		attr.fontColor.Set(1, 1, 1, 1);
		attr.fontAligment = GUI::EFA_MiddleLeft;
		attr.fontSize = 20;
		attr.hasShadow = false;
		attr.shadowColor.Set(0, 0, 0, 1);
		attr.shadowOffset = math::vector2d(2);
		attr.spacing = 2;
		attr.wrap = 0;
		attr.RightToLeft = 0;
		core::UTFString msg;

		m_guiMngr->GetRenderQueue()->Flush();/**/
	}
	return GetRenderTarget();
}


void SessionScene::_OnSubProjectChange(CSubProject* sp)
{
	m_screenLayout->FacultyList->SetSubProject(sp);
}

void SessionScene::OnCommentsLoaded(const std::vector<KMDComment*>& comments)
{
	m_screenLayout->CommentsPanel->CommentsLoaded(comments);
}

}
}
