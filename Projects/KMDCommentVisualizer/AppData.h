

#ifndef AppData_h__
#define AppData_h__

#include "ISingleton.h"
#include "ISoundManager.h"

#define USE_LEAP 0

namespace mray
{
	class Application;
#if USE_LEAP
	namespace nui
	{
		class LeapDevice;
	}
#endif
namespace kmd
{
	class SessionContainer;
	class CSubProject;
	class KMDComment;

	class ISubProjectChangeListener
	{
	public:
		virtual void _OnSubProjectChange(CSubProject* sp){}
	};

	class ICommentsListener
	{
	public: 
		virtual void OnCommentsLoaded(const std::vector<KMDComment*>& comments){}
	};

class AppData :public ISingleton<AppData>
{
protected:
	script::CSettingsFile s_values;
public:
	AppData();
	virtual~AppData();


	virtual void Load(const core::string& path);
	virtual void Save(const core::string& path);

	void SetValue(const core::string&catagory, const core::string&name, const core::string& v);
	core::string GetValue(const core::string&catagory, const core::string&name, const core::string &defaultVal = "");

	bool Debugging ;

public:

	Application* app;

#if USE_LEAP
	nui::LeapDevice* leapDevice;
#endif

	SessionContainer* sessions;

#if 0
	sound::ISoundManager* soundManager;
#endif
	class SubProjectChangeCallback :public ListenerContainer<ISubProjectChangeListener*>
	{
		DECLARE_FIRE_METHOD(_OnSubProjectChange, (CSubProject* sp), (sp));

		CSubProject* m_subProject;
	public:
		SubProjectChangeCallback(){ m_subProject = 0; }

		CSubProject* GetActiveSubProject(){ return m_subProject; }

		void OnSubProjectChange(CSubProject* p)
		{
			m_subProject = p;
			FIRE_LISTENR_METHOD(_OnSubProjectChange, (p));
		}
	}subProjectChange;

	class CommentsLoadedCallback :public ListenerContainer<ICommentsListener*>
	{
		DECLARE_FIRE_METHOD(OnCommentsLoaded, (const std::vector<KMDComment*>& comments), (comments));
	public:

		void OnCommentsLoaded(const std::vector<KMDComment*>& comments)
		{
			FIRE_LISTENR_METHOD(OnCommentsLoaded, (comments));
		}
	}commentsCallback;
};

#define gAppData kmd::AppData::getInstance()

}
}

#endif // AppData_h__
