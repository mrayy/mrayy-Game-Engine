

/********************************************************************
	created:	2011/12/11
	created:	11:12:2011   21:49
	filename: 	d:\Development\mrayEngine\Engine\mrayGUI\GUICheckBoxComponent.h
	file path:	d:\Development\mrayEngine\Engine\mrayGUI
	file base:	GUICheckBoxComponent
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __GUICheckBoxComponent__
#define __GUICheckBoxComponent__


#include "CompileConfig.h"
#include "IGUIComponent.h"

namespace mray
{
namespace GUI
{

class GUICheckBoxComponent:public IGUIComponent
{
public:
	class IListener
	{
	public:
		virtual void OnChangeCheck(GUICheckBoxComponent*caller)=0;
	};
private:
protected:
public:
	enum ECheckState
	{
		EC_Normal,
		EC_MouseOver,
		EC_MouseDown
	}m_state;

	bool m_checked;
	bool m_BGOpaque;
	bool m_pushButton;

	IListener* listener;


	GUICheckBoxComponent()
	{
		listener=0;
		m_checked=0;
		m_BGOpaque=0;
		m_pushButton=0;

		m_state=EC_Normal;
	}
	virtual~GUICheckBoxComponent()
	{
	}

	virtual void Draw();

	bool OnKeyboardEvent(KeyboardEvent* e);
	bool OnMouseEvent(MouseEvent* e);

	void LostFocus();
	void LostMouseOn();

	void ChangeState()
	{
		SetChecked(!m_checked);
	}

	const bool& IsChecked() { return m_checked; }
	bool SetChecked(const bool& c)
	{
		m_checked=c;
		if(listener)
			listener->OnChangeCheck(this);
		return true;
	}


	const bool& IsBackgroundOpaque() { return m_BGOpaque; }
	bool SetBGOpaque(const bool& val) { m_BGOpaque = val; return true;}

	const bool& IsPushButton() { return m_pushButton; }
	bool SetPushButton(const bool&val) { m_pushButton = val; return true;}
};

}
}

#endif

