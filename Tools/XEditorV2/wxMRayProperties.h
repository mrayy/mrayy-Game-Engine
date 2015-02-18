

/********************************************************************
	created:	2012/01/22
	created:	22:1:2012   15:25
	filename: 	d:\Development\mrayEngine\Tools\XEditorV2\wxMRayProperties.h
	file path:	d:\Development\mrayEngine\Tools\XEditorV2
	file base:	wxMRayProperties
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __wxMRayProperties__
#define __wxMRayProperties__


#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/advprops.h>

namespace mray
{

class wxVector3Property : public wxPGProperty
{
	WX_PG_DECLARE_PROPERTY_CLASS(wxVectorProperty)
public:

	wxVector3Property( const wxString& label = wxPG_LABEL,
		const wxString& name = wxPG_LABEL,
		const math::vector3df& value = math::vector3df() );
	virtual ~wxVector3Property();

	virtual wxVariant ChildChanged( wxVariant& thisValue,
		int childIndex,
		wxVariant& childValue ) const;
	virtual void RefreshChildren();

protected:
};
class wxVector2Property : public wxPGProperty
{
	WX_PG_DECLARE_PROPERTY_CLASS(wxColorProperty)
public:

	wxVector2Property( const wxString& label = wxPG_LABEL,
		const wxString& name = wxPG_LABEL,
		const math::vector2df& value = math::vector2df() );
	virtual ~wxVector2Property();

	virtual wxVariant ChildChanged( wxVariant& thisValue,
		int childIndex,
		wxVariant& childValue ) const;
	virtual void RefreshChildren();

protected:
};


}

#endif
