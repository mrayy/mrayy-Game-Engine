

/********************************************************************
	created:	2010/10/14
	created:	14:10:2010   23:09
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\ActionAnimationTrack.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	ActionAnimationTrack
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ActionAnimationTrack_h__
#define ActionAnimationTrack_h__

#include "KeyAnimationTrack.h"
#include "IKeyFrame.h"

namespace mray
{
namespace animation
{

	class IActionAnimationListener
	{
	public:
		virtual void OnKeyAction(const core::string&action,const core::string&params)=0;
	};

	class ActionKeyFrame:public IKeyFrame
	{
	public:

		core::string action;
		core::string params;

		ActionKeyFrame();
		ActionKeyFrame(const ActionKeyFrame*o);

		virtual IKeyFrame*duplicate()const;

		virtual xml::XMLElement*  exportXMLSettings(xml::XMLElement* elem);
	};

class MRAY_DLL ActionAnimationTrack:public KeyAnimationTrack
{
	DECLARE_RTTI;
private:
protected:

	IActionAnimationListener* m_listener;
public:
	ActionAnimationTrack(const core::string& name,AnimationTrackGroup*owner=0);
	virtual ~ActionAnimationTrack();

	virtual void addKeyFrame(IKeyFrame*kf);
	IKeyFrame* createKeyFrame(float time,const core::string&action,const core::string&params);

	virtual IAnimationTrack* duplicate();
	virtual void copy(IAnimationTrack* o);
	virtual void loadKeyFrame(xml::XMLElement*elem);

};

}
}

#endif // ActionAnimationTrack_h__

