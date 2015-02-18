
#include "stdafx.h"
#include "SerializationManager.h"
#include "IClassSerialize.h"
#include "GenericClassTypes.h"


namespace mray
{
namespace serialize
{

SerializationManager::SerializationManager()
{
	AddClass(&g_stringClassSerialize);
	AddClass(&g_intClassSerialize);
	AddClass(&g_uintClassSerialize);
	AddClass(&g_floatClassSerialize);
	AddClass(&g_doubleClassSerialize);
	AddClass(&g_longClassSerialize);
	AddClass(&g_ulongClassSerialize);
	AddClass(&g_boolClassSerialize);
	AddClass(&g_planeClassSerialize);
	AddClass(&g_box3dClassSerialize);
	AddClass(&g_rectfClassSerialize);
	AddClass(&g_line3dClassSerialize);
	AddClass(&g_line2dClassSerialize);
	AddClass(&g_matrix4x4ClassSerialize);
	AddClass(&g_quaternionClassSerialize);
	AddClass(&g_vector2dClassSerialize);
	AddClass(&g_vector2diClassSerialize);
	AddClass(&g_vector3dClassSerialize);
	AddClass(&g_vector3diClassSerialize);
	AddClass(&g_vector4dClassSerialize);
	AddClass(&g_vector4diClassSerialize);
}
SerializationManager::~SerializationManager()
{
	m_classes.clear();
}

void SerializationManager::AddClass(IClassSerialize*c)
{
	m_classes[c->GetName()]=c;
}
IClassSerialize* SerializationManager::GetClass(const core::string&name)
{
	SerializationMap::iterator it=m_classes.find(name);
	if(it==m_classes.end())return 0;
	return it->second;
}


xml::XMLElement* SerializationManager::Serialize(void*obj,const core::string&type)
{
	xml::XMLElement*result=new xml::XMLElement(type);

	IClassSerialize*c=GetClass(type);
	if(c)
	{
		c->Serialize(obj,result);
	}

	return result;
}

}
}
