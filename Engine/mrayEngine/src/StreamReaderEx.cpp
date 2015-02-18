

#include "stdafx.h"
#include "StreamReaderEx.h"
#include "StreamReader.h"



namespace mray
{
namespace OS
{


StreamReaderEx& StreamReaderEx::operator>>(bool& v)
{
	v=m_reader->binReadInt();
	return *this;
}
StreamReaderEx& StreamReaderEx::operator>>(int& v)
{
	v=m_reader->binReadInt();
	return *this;
}

StreamReaderEx& StreamReaderEx::operator>>(float& v)
{
	v=m_reader->binReadFloat();
	return *this;
}

StreamReaderEx& StreamReaderEx::operator>>(core::string& v)
{
	v=m_reader->binReadString();
	return *this;
}


StreamReaderEx& StreamReaderEx::operator>>(math::vector2d& v)
{
	(*this)>>v.x>>v.y;
	return *this;
}

StreamReaderEx& StreamReaderEx::operator>>(math::vector3d& v)
{
	(*this)>>v.x>>v.y>>v.z;
	return *this;

}

StreamReaderEx& StreamReaderEx::operator>>(math::vector4d& v)
{
	(*this)>>v.x>>v.y>>v.z>>v.w;
	return *this;

}

StreamReaderEx& StreamReaderEx::operator>>(math::quaternion& v)
{
	(*this)>>v.x>>v.y>>v.z>>v.w;
	return *this;

}

StreamReaderEx& StreamReaderEx::operator>>(math::matrix3x3& v)
{
	for(int i=0;i<9;++i)
		(*this)>>v.mat[i];
	return *this;

}

StreamReaderEx& StreamReaderEx::operator>>(math::matrix4x4& v)
{
	for(int i=0;i<16;++i)
		(*this)>>v.mat[i];
	return *this;

}

StreamReaderEx& StreamReaderEx::operator>>(math::line2d& v)
{
	(*this)>>v.pStart>>v.pEnd;
	return *this;
}

StreamReaderEx& StreamReaderEx::operator>>(math::line3d& v)
{
	(*this)>>v.pStart>>v.pEnd;
	return *this;
}

StreamReaderEx& StreamReaderEx::operator>>(math::box3d& v)
{
	(*this)>>v.MinP>>v.MaxP;
	return *this;
}

}
}
