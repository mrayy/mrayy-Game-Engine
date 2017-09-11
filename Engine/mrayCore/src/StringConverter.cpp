#include "stdafx.h"

#include "StringConverter.h"
#include "EnumManager.h"
#include "StringUtil.h"

namespace mray{
namespace core{


core::string StringConverter::toString(const math::Point4d<int>&v,int precision)  
{
	osstream stream;
	stream.precision(precision);
	stream << v.x <<mT(",")<<v.y<<mT(",")<< v.z <<mT(",")<<v.w;

	return stream.str().c_str();
};
core::string StringConverter::toString(const math::Point4d<float>&v,int precision)  
{
	osstream stream;
	stream.precision(precision);
	stream << v.x <<mT(",")<<v.y<<mT(",")<< v.z <<mT(",")<<v.w;

	return stream.str().c_str();
};

core::string StringConverter::toString(const math::Point3d<float>&v,int precision)  
{
	osstream stream;
	stream.precision(precision);
	stream << v.x <<mT(",")<<v.y<<mT(",")<<v.z;

	
	return stream.str().c_str();
};
core::string StringConverter::toString(const math::Point2d<float>&v,int precision)  
{
	osstream stream;
	stream.precision(precision);
	stream << v.x <<mT(",")<<v.y;

	
	return stream.str().c_str();
};
core::string StringConverter::toString(const video::SColor &v)  
{
	osstream stream;
	stream << v.R <<mT(",")<<v.G<<mT(",")<<v.B<<mT(",")<<v.A;

	
	return stream.str().c_str();
};
core::string StringConverter::toString(const math::line2d &v)  
{
	osstream stream;
	stream <<	v.pStart.x <<mT(",")<<v.pStart.y  <<mT("-")<<
				v.pEnd.x <<mT(",")<<v.pEnd.y ;

	
	return stream.str().c_str();
};
core::string StringConverter::toString(const math::line3d &v)  
{
	osstream stream;
	stream <<	v.pStart.x <<mT(",")<<v.pStart.y <<mT(",") <<v.pStart.z <<mT("-")<<
				v.pEnd.x <<mT(",")<<v.pEnd.y <<mT(",")<<v.pEnd.z ;

	
	return stream.str().c_str();
};
core::string StringConverter::toString(const math::box3d &v)  
{
	osstream stream;
	stream <<	v.MinP.x <<mT(",")<<v.MinP.y <<mT(",") <<v.MinP.z <<mT(",")<<
		v.MaxP.x <<mT(",")<<v.MaxP.y <<mT(",")<<v.MaxP.z ;

	
	return stream.str().c_str();
};
core::string StringConverter::toString(const math::Plane&v)  
{
	osstream stream;
	stream << v.Normal.x <<mT(",")<<v.Normal.y<<mT(",")<< v.Normal.z <<mT(",")<<v.D;

	
	return stream.str().c_str();
};

core::string StringConverter::toString(const math::quaternion &v)  
{
	osstream stream;
	stream << v.w <<mT(",")<<v.x<<mT(",")<<v.y<<mT(",")<<v.z;

	
	return stream.str().c_str();
};
core::string StringConverter::toString(const math::matrix3x3 &v)  
{
	osstream stream;
	stream << v(0,0) <<mT(",")<<v(0,1)<<mT(",")<<v(0,2)<<mT(",");
	stream << v(1,0) <<mT(",")<<v(1,1)<<mT(",")<<v(1,2)<<mT(",");
	stream << v(2,0) <<mT(",")<<v(2,1)<<mT(",")<<v(2,2)<<mT(",");

	
	return stream.str().c_str();
}
core::string StringConverter::toString(const math::matrix4x4 &v)  
{
	osstream stream;
	stream << v(0,0) <<mT(",")<<v(0,1)<<mT(",")<<v(0,2)<<mT(",")<<v(0,3)<<mT(",");
	stream << v(1,0) <<mT(",")<<v(1,1)<<mT(",")<<v(1,2)<<mT(",")<<v(1,3)<<mT(",");
	stream << v(2,0) <<mT(",")<<v(2,1)<<mT(",")<<v(2,2)<<mT(",")<<v(2,3)<<mT(",");
	stream << v(3,0) <<mT(",")<<v(3,1)<<mT(",")<<v(3,2)<<mT(",")<<v(3,3);

	
	return stream.str().c_str();
}
core::string StringConverter::toString(const UTFString &v) 
{
	return v.GetAsString();
}

core::string StringConverter::toString(const float&v,int precision) 
{
	osstream stream;
	stream.precision(precision);
	stream << v;

	
	return stream.str().c_str();
};
core::string StringConverter::toString(const bool&v,bool trueFalse) 
{
	osstream stream;
	if(trueFalse)
	{
		if(v)
			stream << mT("true");
		else
			stream << mT("false");
	}else
	{
		if(v)
			stream << mT("1");
		else
			stream << mT("0");
	}

	
	return stream.str().c_str();
};

const core::string& StringConverter::toString(int v,const core::string&enumType){
	return EnumManager::getInstance().getName(enumType,v);
}


//////////////////////////////////////////////////////////////////////////


int StringConverter::toInt(const core::string&str){
	return strToInt(str.c_str());
}
float StringConverter::toFloat(const core::string&str){
	return strToFloat(str.c_str());
}
double StringConverter::toDouble(const core::string&str){
	return strToFloat(str.c_str());
}
uint StringConverter::toUInt(const core::string&str){
	return (uint)strToUL(str.c_str(),0,10);
}
long StringConverter::toLong(const core::string&str){
	return strToL(str.c_str(),0,10);
}
ulong StringConverter::toULong(const core::string&str){
	return strToUL(str.c_str(),0,10);
}
bool StringConverter::toBool(const core::string&str){
	core::string s(str.c_str());
	return(s.equals_ignore_case(mT("true")) || s.equals_ignore_case(mT("1")));
}
math::vector2d StringConverter::toVector2d(const core::string&str){
	std::vector<core::string> tmpArray=core::StringUtil::Split(str,mT(","),2);
	if(tmpArray.size()==1)
		return math::vector2d(toFloat(tmpArray[0]));
	return math::vector2d(toFloat(tmpArray[0]),toFloat(tmpArray[1]));
}
math::vector3d StringConverter::toVector3d(const core::string&str){
	std::vector<core::string> tmpArray=core::StringUtil::Split(str,mT(","),3);
	if(tmpArray.size()==1)
		return math::vector3d(toFloat(tmpArray[0]));
	else if(tmpArray.size()<3)
		return 0;
	return math::vector3d(toFloat(tmpArray[0]),toFloat(tmpArray[1]),toFloat(tmpArray[2]));
}
math::vector4d StringConverter::toVector4d(const core::string&str){
	std::vector<core::string> tmpArray=core::StringUtil::Split(str,mT(","),4);
	if(tmpArray.size()==1)
		return math::vector4d(toFloat(tmpArray[0]));
	else if(tmpArray.size()<4)
		return 0;
	return math::vector4d(toFloat(tmpArray[0]),toFloat(tmpArray[1]),
		toFloat(tmpArray[2]),toFloat(tmpArray[3]));
}
video::SColor StringConverter::toColor(const core::string&str){
	std::vector<core::string> tmpArray=core::StringUtil::Split(str,mT(","),4);
	if(tmpArray.size()==4)
		return video::SColor(toFloat(tmpArray[0]),toFloat(tmpArray[1]),
		toFloat(tmpArray[2]),toFloat(tmpArray[3]));
	else if(tmpArray.size()==3)
		return video::SColor(toFloat(tmpArray[0]),toFloat(tmpArray[1]),
		toFloat(tmpArray[2]),255);
	else if(tmpArray.size()==1){
		return video::SColor(toFloat(tmpArray[0]));
	}

	return video::SColor(0);
}
math::quaternion StringConverter::toQuaternion(const core::string&str){
	std::vector<core::string> tmpArray=core::StringUtil::Split(str,mT(","),4);
	if(tmpArray.size()<4)
		return math::quaternion();
	return math::quaternion(toFloat(tmpArray[0]),toFloat(tmpArray[1]),
		toFloat(tmpArray[2]),toFloat(tmpArray[3]));
}
math::matrix4x4 StringConverter::toMatrix(const core::string&str){
	std::vector<core::string> tmpArray=core::StringUtil::Split(str,mT(","),16);
	if(tmpArray.size()<16)
		return math::matrix4x4();
	math::matrix4x4 m;
	for(int i=0;i<16;++i)
		m(i)=toFloat(tmpArray[i]);
	tmpArray.clear();
	return m;
}
math::line2d StringConverter::toLine2d(const core::string&str){
	std::vector<core::string> tmpArray=core::StringUtil::Split(str,mT(","),4);
	if(tmpArray.size()<4)
		return math::line2d();
	return math::line2d(toFloat(tmpArray[0]),toFloat(tmpArray[1])
		,toFloat(tmpArray[2]),toFloat(tmpArray[3]));
}
math::line3d StringConverter::toLine3d(const core::string&str){
	std::vector<core::string> tmpArray=core::StringUtil::Split(str,mT(","),6);
	if(tmpArray.size()<6)
		return math::line3d();
	return math::line3d(toFloat(tmpArray[0]),toFloat(tmpArray[1]),toFloat(tmpArray[2]),
		toFloat(tmpArray[3]),toFloat(tmpArray[4]),toFloat(tmpArray[5]));
}
math::rectf StringConverter::toRect(const core::string&str){
	std::vector<core::string> tmpArray=core::StringUtil::Split(str,mT(","),4);
	if(tmpArray.size()<4)
		return math::rectf(0,0,0,0);
	return math::rectf(toFloat(tmpArray[0]),toFloat(tmpArray[1]),
		toFloat(tmpArray[2]),toFloat(tmpArray[3]));

}
math::box3d StringConverter::toBox3d(const core::string&str){
	std::vector<core::string> tmpArray=core::StringUtil::Split(str,mT(","),6);
	if(tmpArray.size()<6)
		return math::box3d();
	return math::box3d(toFloat(tmpArray[0]),toFloat(tmpArray[1]),toFloat(tmpArray[2]),
		toFloat(tmpArray[3]),toFloat(tmpArray[4]),toFloat(tmpArray[5]));

}
math::Plane StringConverter::toPlane(const core::string&str){
	std::vector<core::string> tmpArray=core::StringUtil::Split(str,mT(","),4);
	if(tmpArray.size()<4)
		return math::Plane();
	math::Plane p;
	p.Normal=math::vector3d(toFloat(tmpArray[0]),toFloat(tmpArray[1]),toFloat(tmpArray[2]));

	p.D=toFloat(tmpArray[3]);
	return p;

}
core::UTFString StringConverter::toUTFString(const core::string&str)
{
	return core::UTFString(str);
}

int StringConverter::toEnum(const core::string&enumType,const core::string&enumVal){
	return EnumManager::getInstance().getValue(enumType,enumVal);
}

core::string StringConverter::toString(const core::stringc&str){
	core::string res;
	char_to_string(str.c_str(),res);
	return res;
}
core::string StringConverter::toString(const core::stringw&str){
	core::string res;
	wchar_to_string(str.c_str(),res);
	return res;
}
core::string StringConverter::toString(const mchar*str){
	return str;
}

void StringConverter::parse(const core::string&str,char&v){v=(mchar)str[0]; }
void StringConverter::parse(const core::string&str, wchar_t&v){v=(mchar)str[0]; }
void StringConverter::parse(const core::string&str,int&v){v=toInt(str.c_str()); }
void StringConverter::parse(const core::string&str,double&v){v=toDouble(str.c_str()); }
void StringConverter::parse(const core::string&str,float&v){v=toFloat(str.c_str()); }
void StringConverter::parse(const core::string&str,uint&v){v=toUInt(str.c_str()); }
void StringConverter::parse(const core::string&str,long&v){v=toLong(str.c_str()); }
void StringConverter::parse(const core::string&str,ulong&v){v=toULong(str.c_str()); }
void StringConverter::parse(const core::string&str,bool&v){v=toBool(str.c_str()); }
void StringConverter::parse(const core::string&str,math::vector2d&v){v=toVector2d(str.c_str()); }
void StringConverter::parse(const core::string&str,math::vector2di&v){
	math::vector2d t=toVector2d(str.c_str()); 
	v.x=t.x;
	v.y=t.y;
}
void StringConverter::parse(const core::string&str,math::vector3d&v){v=toVector3d(str.c_str()); }
void StringConverter::parse(const core::string&str,math::vector3di&v){
	math::vector3d t=toVector3d(str.c_str()); 
	v.x=t.x;
	v.y=t.y;
	v.z=t.z;
}
void StringConverter::parse(const core::string&str,math::vector4d&v){v=toVector4d(str.c_str()); }
void StringConverter::parse(const core::string&str,math::vector4di&v){
	math::vector4d t=toVector4d(str.c_str()); 
	v.x=t.x;
	v.y=t.y;
	v.z=t.z;
	v.w=t.w;
}
void StringConverter::parse(const core::string&str,video::SColor&v){v=toColor(str.c_str()); }
void StringConverter::parse(const core::string&str,math::quaternion&v){v=toQuaternion(str.c_str()); }
void StringConverter::parse(const core::string&str,math::matrix4x4&v){v=toMatrix(str.c_str()); }
void StringConverter::parse(const core::string&str,math::line2d&v){v=toLine2d(str.c_str()); }
void StringConverter::parse(const core::string&str,math::line3d&v){v=toLine3d(str.c_str()); }
void StringConverter::parse(const core::string&str,math::rect<float>&v){v=toRect(str.c_str()); }
void StringConverter::parse(const core::string&str,math::box3d&v){v=toBox3d(str.c_str()); }
void StringConverter::parse(const core::string&str,math::Plane&v){v=toPlane(str.c_str()); }
void StringConverter::parse(const core::string&str,string&v){v=toString(str.c_str()); }

void StringConverter::parse(const core::string&str,int&v,const core::string&enumType){
	v=toEnum(enumType,str);
}
void StringConverter::parse(const core::string&str, UTFString &v) 
{
	v.Set(str);
}


}
}


