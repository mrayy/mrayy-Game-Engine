
#ifndef SessionScene_h__
#define SessionScene_h__

#include "IRenderingScene.h"
#include "ISound.h"
#include "AppData.h"

namespace mray
{
	namespace GUI
	{
		class IGUIPanelElement;
		class GUIManager;
		class GUIScreenLayoutImplV2;
	}
	namespace scene
	{
		class SessionRenderer;
		class PointerNode;
	}
namespace kmd
{
	class GenericRenderLayer;
	class WallpaperLayer;
class TwitterProviderListener;


class SessionScene :public IRenderingScene,public ISubProjectChangeListener,public ICommentsListener
{
protected:
	GUI::IGUIPanelElement* m_guiroot;
	GUI::GUIManager* m_guiMngr;
	GUI::GUIScreenLayoutImplV2* m_screenLayout;

	scene::ISceneManager* m_sceneManager;

	scene::ViewPort* m_vp;

	scene::SessionRenderer* m_sessionRenderer;
	TwitterProviderListener* m_providerListener;
	scene::PointerNode* m_pointer;

#if 0
	sound::ISoundPtr m_bgm;
#endif
	std::vector<kmd::CSubProject*> m_subProjects;
	int m_activeSubProject;

	bool m_isTimeBased;
	float m_timerStart;


	WallpaperLayer* m_wallpaperLayer;
	GenericRenderLayer* m_commentsLayer;
	GenericRenderLayer* m_sceneLayer;
	GenericRenderLayer* m_statusLayer;
	std::list<GenericRenderLayer*> m_layerOrder;


	void MakeLayerTop(GenericRenderLayer* layer);
	void MakeLayerBottom(GenericRenderLayer* layer);
	void RefreshLayerAlpha();

	void SetTimerBased(bool timer);
public:
	SessionScene();
	virtual~SessionScene();

	GUI::GUIScreenLayoutImplV2* GetScreenLayout(){ return m_screenLayout; }

	virtual void Init();
	virtual void OnEnter() ;
	virtual void OnExit() ;

	virtual bool OnEvent(Event* e, const math::rectf& rc) ;
	virtual void Update(float dt) ;
	virtual video::IRenderTarget* Draw(const math::rectf& rc);

	void SetNextProject();
	void SetPrevProject();

	void SetCurrentSubProject(int p);
	virtual void _OnSubProjectChange(CSubProject* sp);
	virtual void OnCommentsLoaded(const std::vector<KMDComment*>& comments);
};

}
}

#endif // SessionScene_h__
