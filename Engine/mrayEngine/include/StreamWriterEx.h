

/********************************************************************
	created:	2013/01/22
	created:	22:1:2013   14:50
	filename: 	C:\Development\mrayEngine\Engine\mrayEngine\include\StreamWriterEx.h
	file path:	C:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	StreamWriterEx
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __StreamWriterEx__
#define __StreamWriterEx__

#include "StreamWriter.h"


namespace mray
{
namespace OS
{
	class StreamWriter;

class StreamWriterEx
{
protected:
	StreamWriter* m_writer;
public:
	StreamWriterEx(StreamWriter* wrtr):m_writer(wrtr)
	{}
	virtual~StreamWriterEx(){}

	StreamWriterEx& operator<<(int v);
	StreamWriterEx& operator<<(float v);
	StreamWriterEx& operator<<(const core::string& v);

	template <class T>
	StreamWriterEx& operator<<(const T& v)
	{
		m_writer->writeValue(v);
		return *this;
	}

	StreamWriterEx& operator<<(const math::vector2d& v);
	StreamWriterEx& operator<<(const math::vector3d& v);
	StreamWriterEx& operator<<(const math::vector4d& v);
	StreamWriterEx& operator<<(const math::quaternion& v);
	StreamWriterEx& operator<<(const math::matrix3x3& v);
	StreamWriterEx& operator<<(const math::matrix4x4& v);
	StreamWriterEx& operator<<(const math::line2d& v);
	StreamWriterEx& operator<<(const math::line3d& v);
	StreamWriterEx& operator<<(const math::box3d& v);

};

}
}

#endif
