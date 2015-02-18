

#include "stdafx.h"
#include "wxMRAYPropGrid.h"
#include <wx/propgrid/propgrid.h>


namespace mray
{


wxMRAYPropGrid::wxMRAYPropGrid(wxWindow *parent, wxWindowID id /* = wxID_ANY */, const wxPoint& pos /* = wxDefaultPosition */, const wxSize& size /* = wxDefaultSize */, long style /* = wxPGMAN_DEFAULT_STYLE */, const wxString& name /* = wxPropertyGridManagerNameStr */)
	:wxPropertyGridManager(parent,id,	pos,size,style,name)
{
	GetGrid()->SetVerticalSpacing(2);
	AddPage("Properties");
	AddPage("Events");
	m_propPage= GetPage("Properties");
	m_eventPage= GetPage("Events");

	m_object=0;

	Connect(this->GetId(),wxEVT_PG_CHANGED, 
		(wxObjectEventFunction) (wxEventFunction) (wxPropertyGridEventFunction)
		&wxMRAYPropGrid::OnPropertyGridChange );
}
wxMRAYPropGrid::~wxMRAYPropGrid()
{
}

void wxMRAYPropGrid::SetObject(IObject* obj)
{
	m_object=obj;
	m_propPage->Clear();
	m_eventPage->Clear();


	if(!m_object)
		return;

	CPropertieDictionary* dic= obj->GetDictionary();
	const TPropertieGroupList& lst= dic->getGroups();
	TPropertieGroupList::const_iterator it=lst.begin();
	for (;it!=lst.end();++it)
	{
		wxPGProperty *page= m_propPage->AppendCategory(it->second.name.c_str());
		TPropertieList::const_iterator pit= it->second.properties.begin();
		for (;pit!=it->second.properties.end();++pit)
		{
			page->AppendChild(new wxStringProperty(pit->second->getName().c_str(),pit->second->getName().c_str(),pit->second->toString(obj).c_str()));
		}
	}
}


void wxMRAYPropGrid::OnPropertyGridChange(wxPropertyGridEvent& evt)
{

	wxPGProperty* id = evt.GetProperty();

	const wxString& name = evt.GetPropertyName();
	wxVariant value = evt.GetPropertyValue();

	// Don't handle 'unspecified' values
	if ( value.IsNull() || m_object==0)
		return;

	m_object->SetPropertyValue(name.c_str(),value.GetString().c_str());

	id->SetValue(m_object->GetPropertyValue(name.c_str()).c_str());
}


}
