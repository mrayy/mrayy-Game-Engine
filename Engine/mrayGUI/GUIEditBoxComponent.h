


/********************************************************************
	created:	2011/12/11
	created:	11:12:2011   16:05
	filename: 	d:\Development\mrayEngine\Engine\mrayGUI\GUIEditBoxComponent.h
	file path:	d:\Development\mrayEngine\Engine\mrayGUI
	file base:	GUIEditBoxComponent
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __GUIEditBoxComponent__
#define __GUIEditBoxComponent__

#include "IGUIComponent.h"

namespace regex
{
	class Pattern;
}
namespace mray
{
namespace GUI
{


class GUIEditBoxComponent:public IGUIComponent
{
public:
	class IListener
	{
	public:
		virtual void OnTextChangeCallback(GUIEditBoxComponent*caller)=0;
	};
private:
protected:
	float m_currentTint;
public:

	bool acceptsChars;
	bool acceptsNumbers;


	int m_CursorPos;
	int m_MarkBegin;
	int m_MarkEnd;
	bool m_MouseMark;
	int m_startPos;

	regex::Pattern* m_regexExpression;
	core::string m_regexExpStr;

	bool m_background;
	bool m_border;
	core::string m_HideChar;
	bool m_HideText;

	video::SColor m_selectionColor;

	IListener* listener;


	GUIEditBoxComponent();
	virtual~GUIEditBoxComponent();

	void Update(float dt);
	virtual void Draw();

	bool OnKeyboardEvent(KeyboardEvent* e);
	bool OnMouseEvent(MouseEvent* e);


	bool SetText(const core::UTFString& cap);
	bool CheckIsValideText(const core::UTFString&t);
	core::string getSelectedText();
	void _RemoveSelectedText();
};

}
}

#endif
