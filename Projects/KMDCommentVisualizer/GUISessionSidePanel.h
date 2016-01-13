#ifndef GUISessionSidePanel_h__
#define GUISessionSidePanel_h__

#include "IGUIPanelElement.h"
#include "GUIElementFactoryDef.h"
#include "GUISessionSidePanelLayout.h"
#include "AppData.h"


namespace mray
{
namespace kmd
{
	class SessionContainer;
	class SessionDetails;
}
namespace GUI
{

class GUISessionSidePanel :public IGUIPanelElement, public GUISessionSidePanelLayout, public kmd::ISubProjectChangeListener
{
	DECLARE_RTTI;
public:
	static const GUID ElementType;
	static const core::string ProfileRedneringShader;
protected:
	enum EState
	{
		Expand,
		Shrink
	};

	struct ProjectImageIfo
	{
		ProjectImageIfo() :t(0), project(0)
		{}
		kmd::SessionDetails* project;
		video::ITexturePtr texture;
		float t;
	};

	std::vector<ProjectImageIfo> m_projects;

	EState m_state;

	float m_maxWidth;
	float m_minWidth;

	float m_szT;

	float m_speakerTransition;
	int m_currentSpeaker;

	kmd::SessionContainer* m_sessions;

public:
	GUISessionSidePanel(IGUIManager* m);
	virtual~GUISessionSidePanel();

	
	kmd::SessionDetails* GetActiveProject(){
		return m_projects[m_currentSpeaker].project;
	}
	void SetSessionContainer(kmd::SessionContainer* s);

	int GetCurrentProjectID(){ return m_currentSpeaker; }
	void SetCurrentProject(int id);
	int GetSpeakersCount(){ return m_projects.size(); }

	bool IsActive(){ return m_state == Expand; }

	virtual bool OnEvent(Event* e);

	virtual void Update(float dt);
	virtual void Draw(const math::rectf*vp);
	virtual void _OnSubProjectChange(kmd::CSubProject* sp);
	
	virtual void LoadXMLSettings(xml::XMLElement*e);
};
DECLARE_ELEMENT_FACTORY(GUISessionSidePanel);

}
}

#endif // GUISessionSidePanel_h__
