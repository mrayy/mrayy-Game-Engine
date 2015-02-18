

#include "stdafx.h"
#include "wxMRayProperties.h"


using namespace mray;
using namespace math;
using namespace video;
WX_PG_DECLARE_VARIANT_DATA(vector3df)
WX_PG_DECLARE_VARIANT_DATA(vector2df)

WX_PG_IMPLEMENT_VARIANT_DATA_DUMMY_EQ(vector3df)
WX_PG_IMPLEMENT_VARIANT_DATA_DUMMY_EQ(vector2df)


namespace mray
{

WX_PG_IMPLEMENT_PROPERTY_CLASS(wxVector3Property,wxPGProperty,
							   vector3df,const vector3df&,TextCtrl);

wxVector3Property::wxVector3Property( const wxString& label,
								   const wxString& name,
								   const vector3df& value )
								   : wxPGProperty(label,name)
{
	SetValue( WXVARIANT(value) );
	AddPrivateChild( new wxFloatProperty(wxT("X"),wxPG_LABEL,value.x) );
	AddPrivateChild( new wxFloatProperty(wxT("Y"),wxPG_LABEL,value.y) );
	AddPrivateChild( new wxFloatProperty(wxT("Z"),wxPG_LABEL,value.z) );
}

wxVector3Property::~wxVector3Property() { }

void wxVector3Property::RefreshChildren()
{
	if ( !GetChildCount() ) return;
	const vector3df& vector = vector3dfRefFromVariant(m_value);
	Item(0)->SetValue( vector.x );
	Item(1)->SetValue( vector.y );
	Item(2)->SetValue( vector.z );
}

wxVariant wxVector3Property::ChildChanged( wxVariant& thisValue,
										 int childIndex,
										 wxVariant& childValue ) const
{
	vector3df vector;
	vector << thisValue;
	switch ( childIndex )
	{
	case 0: vector.x = childValue.GetDouble(); break;
	case 1: vector.y = childValue.GetDouble(); break;
	case 2: vector.z = childValue.GetDouble(); break;
	}
	wxVariant newVariant;
	newVariant << vector;
	return newVariant;
}


//////////////////////////////////////////////////////////////////////////


WX_PG_IMPLEMENT_PROPERTY_CLASS(wxVector2Property,wxPGProperty,
							   vector2df,const vector2df&,TextCtrl);

wxVector2Property::wxVector2Property( const wxString& label,
								   const wxString& name,
								   const vector2df& value )
								   : wxPGProperty(label,name)
{
	SetValue( WXVARIANT(value) );
	AddPrivateChild( new wxFloatProperty(wxT("X"),wxPG_LABEL,value.x) );
	AddPrivateChild( new wxFloatProperty(wxT("Y"),wxPG_LABEL,value.y) );
}

wxVector2Property::~wxVector2Property() { }

void wxVector2Property::RefreshChildren()
{
	if ( !GetChildCount() ) return;
	const vector2df& vector = vector2dfRefFromVariant(m_value);
	Item(0)->SetValue( vector.x );
	Item(1)->SetValue( vector.y );
}

wxVariant wxVector2Property::ChildChanged( wxVariant& thisValue,
										 int childIndex,
										 wxVariant& childValue ) const
{
	vector2df vector;
	vector << thisValue;
	switch ( childIndex )
	{
	case 0: vector.x = childValue.GetDouble(); break;
	case 1: vector.y = childValue.GetDouble(); break;
	}
	wxVariant newVariant;
	newVariant << vector;
	return newVariant;
}
}