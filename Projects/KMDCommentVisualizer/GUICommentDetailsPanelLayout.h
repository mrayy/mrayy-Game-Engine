#ifndef __GUICommentDetailsPanelLayout__
#define __GUICommentDetailsPanelLayout__
#include "IGUISchemeBase.h"
#include "GUIPanel.h"
#include "GUIProfilePicture.h"
#include "GUIStaticText.h"
namespace mray{

using namespace GUI;
class GUICommentDetailsPanelLayout:public GUI::IGUISchemeBase
{

public:
	GUIPanel* Root;
	GUIProfilePicture* TwitterImage;
	GUIStaticText* TwitterID;
	GUIStaticText* Details;
	GUIStaticText* ProjectID;
	GUIStaticText* CommentTime;

public:

	GUICommentDetailsPanelLayout():Root(0),TwitterImage(0),TwitterID(0),Details(0),ProjectID(0),CommentTime(0)
	{		
		m_elementsMap["Root"]=(IGUIElement**)&Root;
		m_elementsMap["TwitterImage"]=(IGUIElement**)&TwitterImage;
		m_elementsMap["TwitterID"]=(IGUIElement**)&TwitterID;
		m_elementsMap["Details"]=(IGUIElement**)&Details;
		m_elementsMap["ProjectID"]=(IGUIElement**)&ProjectID;
		m_elementsMap["CommentTime"]=(IGUIElement**)&CommentTime;

	}

};
}
#endif
