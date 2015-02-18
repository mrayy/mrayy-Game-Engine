

/********************************************************************
	created:	2013/01/22
	created:	22:1:2013   14:57
	filename: 	C:\Development\mrayEngine\Engine\mrayEngine\include\StreamReaderEx.h
	file path:	C:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	StreamReaderEx
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __StreamReaderEx__
#define __StreamReaderEx__

#include "mString.h"


namespace mray
{

namespace OS
{
class StreamReader;

class StreamReaderEx
{
protected:
	StreamReader* m_reader;
public:
	StreamReaderEx(StreamReader* rdr):m_reader(rdr)
	{}
	virtual~StreamReaderEx(){}

	StreamReaderEx& operator>>(bool& v);
	StreamReaderEx& operator>>(int& v);
	StreamReaderEx& operator>>(float& v);
	StreamReaderEx& operator>>(core::string& v);

	StreamReaderEx& operator>>(math::vector2d& v);
	StreamReaderEx& operator>>(math::vector3d& v);
	StreamReaderEx& operator>>(math::vector4d& v);
	StreamReaderEx& operator>>(math::quaternion& v);
	StreamReaderEx& operator>>(math::matrix3x3& v);
	StreamReaderEx& operator>>(math::matrix4x4& v);
	StreamReaderEx& operator>>(math::line2d& v);
	StreamReaderEx& operator>>(math::line3d& v);
	StreamReaderEx& operator>>(math::box3d& v);

};

}
}


#endif
