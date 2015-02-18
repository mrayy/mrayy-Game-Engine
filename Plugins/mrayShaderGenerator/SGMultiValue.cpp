
#include "stdafx.h"
#include "SGMultiValue.h"
#include "SGDirectValue.h"

namespace mray
{
namespace shader
{

SGMultiValue::SGMultiValue(video::EUniformType type,const core::string&v1)
:SGValue(type)
{
	video::EUniformType st=GetSingleType(type);
	std::vector<SGIValuePtr>vals;
	vals.push_back(new SGDirectValue(st,v1));
	this->SetValues(vals);
}
SGMultiValue::SGMultiValue(video::EUniformType type,const core::string&v1,const core::string&v2)
:SGValue(type)
{
	video::EUniformType st=GetSingleType(type);
	std::vector<SGIValuePtr>vals;
	vals.push_back(new SGDirectValue(st,v1));
	vals.push_back(new SGDirectValue(st,v2));
	this->SetValues(vals);
}
SGMultiValue::SGMultiValue(video::EUniformType type,const core::string&v1,const core::string&v2,const core::string&v3)
:SGValue(type)
{
	video::EUniformType st=GetSingleType(type);
	std::vector<SGIValuePtr>vals;
	vals.push_back(new SGDirectValue(st,v1));
	vals.push_back(new SGDirectValue(st,v2));
	vals.push_back(new SGDirectValue(st,v3));
	this->SetValues(vals);
}
SGMultiValue::SGMultiValue(video::EUniformType type,const core::string&v1,const core::string&v2,const core::string&v3,const core::string&v4)
:SGValue(type)
{
	video::EUniformType st=GetSingleType(type);
	std::vector<SGIValuePtr>vals;
	vals.push_back(new SGDirectValue(st,v1));
	vals.push_back(new SGDirectValue(st,v2));
	vals.push_back(new SGDirectValue(st,v3));
	vals.push_back(new SGDirectValue(st,v4));
	this->SetValues(vals);
}
SGMultiValue::~SGMultiValue()
{
}
video::EUniformType SGMultiValue::GetSingleType(video::EUniformType t)
{
	switch(t)
	{
	case video::EUT_Int1:
	case video::EUT_Int2:
	case video::EUT_Int3:
	case video::EUT_Int4:
		return video::EUT_Int1;
	case video::EUT_Float1:
	case video::EUT_Float2:
	case video::EUT_Float3:
	case video::EUT_Float4:
		return video::EUT_Float1;
	case video::EUT_Matrix2x2:
	case video::EUT_Matrix2x3:
	case video::EUT_Matrix2x4:
	case video::EUT_Matrix3x3:
	case video::EUT_Matrix3x4:
	case video::EUT_Matrix4x2:
	case video::EUT_Matrix4x3:
	case video::EUT_Matrix4x4:
		return video::EUT_Float1;
	}
	return video::EUT_Unkown;
}

}
}