

/********************************************************************
	created:	2012/11/23
	created:	23:11:2012   14:15
	filename: 	D:\Development\mrayEngine\Projects\VirtualTelesar\VTLib\GUIMotorConnector.h
	file path:	D:\Development\mrayEngine\Projects\VirtualTelesar\VTLib
	file base:	GUIMotorConnector
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/
#ifndef _GUIMotorConnector_
#define _GUIMotorConnector_

#include <IGUIPanelElement.h>

namespace mray
{
namespace VT
{
	class MotorConnection;
}
namespace GUI
{
	class IGUIStaticText;
	class IGUIComboList;

class GUIMotorConnector:public IGUIPanelElement
{
protected:
	VT::MotorConnection*	m_connector;
	IGUIStaticText*			m_TargetText;
	IGUIComboList*			m_SourceText;

	math::rectf				m_connectedRect;
	enum EMouseState
	{
		EMS_MouseOn,
		EMS_MouseOut,
		EMS_Clicked
	};

	EMouseState		m_mouseState;

	virtual bool _OnMouseEvent(MouseEvent*evt);
	virtual bool _OnKeyboardEvent(KeyboardEvent*evt);

	void CalculateSizes();

	void m_SourceText_SelectionChanged(IObject*,PVOID);
public:
	static const GUID ElementType;

	typedef DelegateEvent1<GUIMotorConnector*> OnSourceChangedEvent;
	OnSourceChangedEvent OnSourceChanged;
public:
	GUIMotorConnector(GUI::IGUIManager* crtr);
	virtual~GUIMotorConnector();

	VT::MotorConnection* GetConnector(){return m_connector;}
	void SetConnector(VT::MotorConnection* c);

	virtual void Draw(const math::rectf*vp);
	virtual void Update(float dt);

	void SetSourceList(const std::vector<core::string>& lst);
};


}
}

#endif

