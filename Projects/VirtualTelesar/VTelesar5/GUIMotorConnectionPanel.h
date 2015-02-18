


/********************************************************************
	created:	2012/11/26
	created:	26:11:2012   17:46
	filename: 	D:\Development\mrayEngine\Projects\VirtualTelesar\VTelesar5\GUIMotorConnectionPanel.h
	file path:	D:\Development\mrayEngine\Projects\VirtualTelesar\VTelesar5
	file base:	GUIMotorConnectionPanel
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/
#ifndef _GUIMotorConnectionPanel_
#define _GUIMotorConnectionPanel_

#include "IGUIPanelElement.h"
#include "ICommunicationLayer.h"

namespace mray
{
namespace VT
{
	class MotorConnection;
}
namespace GUI
{
	class IGUIStackPanel;
	class GUIMotorConnector;
	class GUISliderbarComponent;
class GUIMotorConnectionPanel:public IGUIPanelElement,public VT::ICommunicationLayerListener
{
protected:
	IGUIPanelElement* m_panel;
	IGUIStackPanel* m_stackPanel;
	GUISliderbarComponent* m_slider;

	std::list<GUIMotorConnector*> m_connections;
	std::vector<core::string> m_items;

	enum EPanelStatus
	{
		EHidden,
		EShown
	}m_status;

	float m_maxWidth;

	virtual bool _OnMouseEvent(MouseEvent*evt);
	virtual bool _OnKeyboardEvent(KeyboardEvent*evt);
	void m_panel_OnMouseMove(IObject*,PVOID);
	void m_panel_OnMouseLeave(IObject*,PVOID);
	void creator_OnChangeMouseOn(IGUIElement*);

public:
	static const GUID ElementType;
public:
	GUIMotorConnectionPanel(IGUIManager* creator);
	virtual~GUIMotorConnectionPanel();

	void AddConnection(VT::MotorConnection* connection);

	virtual void Update(float dt);

	virtual void Draw(video::IRenderArea*vp);

	virtual void OnSchemeChanged(VT::ICommunicationLayer* ,const std::vector<core::string>& names );

};


}
}


#endif
