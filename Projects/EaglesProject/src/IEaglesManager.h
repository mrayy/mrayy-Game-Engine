
/********************************************************************
	created:	2009/04/01
	created:	1:4:2009   19:37
	filename: 	i:\Programing\GameEngine\mrayEngine\EaglesProject\IEaglesManager.h
	file path:	i:\Programing\GameEngine\mrayEngine\EaglesProject
	file base:	IEaglesManager
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___IEaglesManager___
#define ___IEaglesManager___

#include <ISingleton.h>
#include <IVideoDevice.h>
#include <GUIManager.h>
#include <ISceneManager.h>
#include <ISoundManager.h>
#include <SVideoMode.h>
#include <mrayWin.h>

namespace mray{
namespace eagles{

class IEaglesManager:public ISingleton<IEaglesManager>
{
private:
protected:
public:
	IEaglesManager(){}
	virtual~IEaglesManager(){}

	virtual void setActiveGUIManager(GCPtr<GUI::GUIManager> mngr)=0;
	virtual GCPtr<GUI::GUIManager> getActiveGUIManager()=0;

	virtual void setActiveSceneManager(GCPtr<scene::ISceneManager> mngr)=0;
	virtual GCPtr<scene::ISceneManager> getActiveSceneManager()=0;

	virtual void setActiveSoundManager(GCPtr<sound::ISoundManager> mngr)=0;
	virtual GCPtr<sound::ISoundManager> getActiveSoundManager()=0;

	virtual GCPtr<video::IVideoDevice> getVideoDevice()=0;

	virtual core::array<video::VMode> &getVideoModesList()=0;

	virtual void redraw()=0;

	virtual void exitApplication()=0;

	virtual mrayWin* getAppWindow()=0;

	//////////////////////////////////////////////////////////////////////////
	virtual const core::string &getSoundManagerType()=0;
};

}
}


#endif //___IEaglesManager___
