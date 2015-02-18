

#include "stdafx.h"
#include "BasicTypesGUID.h"


namespace mray
{

BasicTypesGUID::TypesMap BasicTypesGUID::m_types;
BasicTypesGUID BasicTypesGUID::m_instance;

BasicTypesGUID::BasicTypesGUID()
{
	AddTypeInfo(BasicTypeInfo(typeof(int),true));
	AddTypeInfo(BasicTypeInfo(typeof(uint),true));
	AddTypeInfo(BasicTypeInfo(typeof(short),true));
	AddTypeInfo(BasicTypeInfo(typeof(ushort),true));
	AddTypeInfo(BasicTypeInfo(typeof(char),false));
	AddTypeInfo(BasicTypeInfo(typeof(uchar),false));
	AddTypeInfo(BasicTypeInfo(typeof(long),true));
	AddTypeInfo(BasicTypeInfo(typeof(float),true));
	AddTypeInfo(BasicTypeInfo(typeof(double),true));
	AddTypeInfo(BasicTypeInfo(typeof(string),false));
	AddTypeInfo(BasicTypeInfo(typeof(vector2d),true));
	AddTypeInfo(BasicTypeInfo(typeof(vector3d),true));
	AddTypeInfo(BasicTypeInfo(typeof(quaternion),true));
	AddTypeInfo(BasicTypeInfo(typeof(line2d),true));
	AddTypeInfo(BasicTypeInfo(typeof(line3d),true));
	AddTypeInfo(BasicTypeInfo(typeof(matrix4x4),true));
	AddTypeInfo(BasicTypeInfo(typeof(matrix3x3),true));
	AddTypeInfo(BasicTypeInfo(typeof(rect),true));
	AddTypeInfo(BasicTypeInfo(typeof(box3d),true));
	AddTypeInfo(BasicTypeInfo(typeof(plane),true));
	AddTypeInfo(BasicTypeInfo(typeof(SColor),true));
	AddTypeInfo(BasicTypeInfo(typeof(enum),false));
	AddTypeInfo(BasicTypeInfo(typeof(array),false));
	AddTypeInfo(BasicTypeInfo(typeof(list),false));
}
void BasicTypesGUID::AddTypeInfo(const BasicTypeInfo& ifo)
{
	m_types[ifo.GetGUID().ID()]=ifo;
}
const BasicTypeInfo* BasicTypesGUID::GetTypeInfo(uint type)
{
	TypesMap::iterator it=m_types.find(type);
	if(it==m_types.end())
		return 0;
	return &it->second;
}


}

