



/********************************************************************
created:	2014/04/30
created:	30:4:2014   17:39
filename: 	F:\Development\mrayEngine\Projects\TEDxTokyo\TwitterVisualizer\GUICommentDetailsPanel.h
file path:	F:\Development\mrayEngine\Projects\TEDxTokyo\TwitterVisualizer
file base:	GUICommentDetailsPanel
file ext:	h
author:		MHD Yamen Saraiji

purpose:
*********************************************************************/

#ifndef __GUICommentDetailsPanel__
#define __GUICommentDetailsPanel__



#include "IGUIPanelElement.h"
#include "GUIElementFactoryDef.h"
#include "GUICommentDetailsPanelLayout.h"


namespace mray
{
namespace kmd
{
class SessionDetails;
class KMDComment;
}
namespace GUI
{

class GUISessionSidePanel;

class GUICommentDetailsPanel :public IGUIPanelElement, public GUICommentDetailsPanelLayout
{
	DECLARE_RTTI;
public:
	static const GUID ElementType;
protected:
	bool m_active;

	GUI::GUISessionSidePanel* m_sidePanel;
	kmd::KMDComment* m_Comment;
public:
	GUICommentDetailsPanel(IGUIManager* m);
	virtual ~GUICommentDetailsPanel();

	void SetComment(kmd::KMDComment* t);
	kmd::KMDComment* GetComment(){ return m_Comment; }

	void SetSidePanel(GUISessionSidePanel* p){ m_sidePanel = p; }

	virtual bool OnEvent(Event* e);

	virtual void Update(float dt);
	virtual void Draw(const math::rectf*vp);
	void LoadXMLSettings(xml::XMLElement*e);
};
DECLARE_ELEMENT_FACTORY(GUICommentDetailsPanel);

}
}

#endif
