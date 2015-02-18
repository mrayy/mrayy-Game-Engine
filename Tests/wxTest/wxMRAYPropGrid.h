

/********************************************************************
	created:	2012/11/22
	created:	22:11:2012   19:59
	filename: 	D:\Development\mrayEngine\Tests\wxTest\wxMRAYPropGrid.h
	file path:	D:\Development\mrayEngine\Tests\wxTest
	file base:	wxMRAYPropGrid
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/
#ifndef _wxMRAYPropGrid_
#define _wxMRAYPropGrid_

#include <wx/propgrid/manager.h>

namespace mray
{

class wxMRAYPropGrid:public wxPropertyGridManager
{

protected:
protected:
	wxPropertyGridPage* m_propPage;
	wxPropertyGridPage* m_eventPage;

	IObject* m_object;

	virtual void OnPropertyGridChange( wxPropertyGridEvent& event );
public:
	wxMRAYPropGrid(wxWindow *parent, wxWindowID id = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxPGMAN_DEFAULT_STYLE,
		const wxString& name = wxPropertyGridManagerNameStr);
	virtual~wxMRAYPropGrid();

	void SetObject(IObject* obj);
};


}

#endif
