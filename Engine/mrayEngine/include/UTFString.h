


/********************************************************************
	created:	2010/12/17
	created:	17:12:2010   15:37
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\UTFString.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	UTFString
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef UTFString_h__
#define UTFString_h__

#include "CompileConfig.h"
#include "mTypes.h"
#include "mstring.h"
#include "PoolMemoryAllocator.h"

namespace mray
{
namespace core
{


class MRAY_DLL UTFString
{
private:
protected:
	static PoolMemoryAllocator<utf32> s_allocator;
	utf32 * m_data;
	uint m_length;
	uint m_capacity;

	void _Clear();
public:
	UTFString();
	UTFString(const core::stringc&str):m_data(0),m_length(0),m_capacity(0)
	{
		SetA(str);
	}
	UTFString(const core::stringw&str) :m_data(0), m_length(0), m_capacity(0)
	{
		SetW(str);
	}
	UTFString(const UTFString&o):m_data(0),m_length(0),m_capacity(0)
	{
		*this=o;
	}
	UTFString(const utf8*buf);
	UTFString(const utf32*buf);

	virtual ~UTFString();

	utf32* c_str(){return m_data;}
	const utf32* c_str()const{return m_data;}
	uint Length()const{return m_length;}

	void Reserve(uint sz);

	void Set(const utf8*buf);
	void Set(const utf32*buf);

	void Set(const core::string& str);
	void SetA(const core::stringc& str);
	void SetW(const core::stringw& str);
	core::string GetAsString()const;
	core::stringc GetAsStringA()const;
	core::stringw GetAsStringW()const;

	utf32 operator [](const uint index) const
	{
		return m_data[index];
	}
	utf32& operator [](const uint index)
	{
		return m_data[index];
	}
	UTFString& operator=(const UTFString&o);
	UTFString& operator=(const utf8*buf);
	UTFString& operator=(const utf32*buf);
	UTFString& operator+=(utf32 c);
	UTFString& operator+=(const UTFString& buf);
	bool operator==(const UTFString& o)const;
	bool operator!=(const UTFString& o)const;
	bool operator <(const UTFString& other) const;


	UTFString substr(uint pos,uint length)const;

	void Insert(utf32 c,uint pos,uint cnt=1);
	void Erase(uint start,uint end);
	void Append(utf32 c);

	static uint EncodeUTF8(const utf8*buf,utf32*dst,uint dst_capacity);
	static uint EncodeUTF32(const utf32*buf,utf8*dst,uint dst_capacity);

	static uint UTF8EncodeSize(const utf8*buf);

	static inline uint UTF32EncodeSize(utf32 c)
	{
		if(c<0x80)return 1;
		else if(c<0x0800)return 2;
		else if(c<0x10000)return 3;
		else return 4;
	}

	static uint UTF32EncodeSize(const utf32*buf);

	static uint UTF8Length(const utf8*buf);
	static uint UTF32Length(const utf32*buf);

	
	static const UTFString Empty;
};

}
}


#endif // UTFString_h__
