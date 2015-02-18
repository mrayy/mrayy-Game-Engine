


#include "stdafx.h"
#include "StreamWriterEx.h"



namespace mray
{
namespace OS
{


StreamWriterEx& StreamWriterEx::operator<<(int v)
{
	m_writer->binWriteInt(v);
	return *this;
}

StreamWriterEx& StreamWriterEx::operator<<(float v)
{
	m_writer->binWriteFloat(v);
	return *this;
}

StreamWriterEx& StreamWriterEx::operator<<(const core::string& v)
{
	m_writer->binWriteString(v);
	return *this;
}


StreamWriterEx& StreamWriterEx::operator<<(const math::vector2d& v)
{
	(*this)<<v.x<<v.y;
	return *this;
}

StreamWriterEx& StreamWriterEx::operator<<(const math::vector3d& v)
{
	(*this)<<v.x<<v.y<<v.z;
	return *this;

}

StreamWriterEx& StreamWriterEx::operator<<(const math::vector4d& v)
{
	(*this)<<v.x<<v.y<<v.z<<v.w;
	return *this;

}

StreamWriterEx& StreamWriterEx::operator<<(const math::quaternion& v)
{
	(*this)<<v.x<<v.y<<v.z<<v.w;
	return *this;

}

StreamWriterEx& StreamWriterEx::operator<<(const math::matrix3x3& v)
{
	for(int i=0;i<9;++i)
		(*this)<<v.mat[i];
	return *this;

}

StreamWriterEx& StreamWriterEx::operator<<(const math::matrix4x4& v)
{
	for(int i=0;i<16;++i)
		(*this)<<v.mat[i];
	return *this;

}

StreamWriterEx& StreamWriterEx::operator<<(const math::line2d& v)
{
	(*this)<<v.pStart<<v.pEnd;
	return *this;
}

StreamWriterEx& StreamWriterEx::operator<<(const math::line3d& v)
{
	(*this)<<v.pStart<<v.pEnd;
	return *this;
}

StreamWriterEx& StreamWriterEx::operator<<(const math::box3d& v)
{
	(*this)<<v.MinP<<v.MaxP;
	return *this;
}

}
}
