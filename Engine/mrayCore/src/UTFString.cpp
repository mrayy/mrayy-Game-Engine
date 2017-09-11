#include "stdafx.h"


#include "UTFString.h"



namespace mray
{
namespace core
{
	PoolMemoryAllocator<utf32> UTFString::s_allocator;
	
	const UTFString UTFString::Empty("");

UTFString::UTFString():m_data(0),m_length(0),m_capacity(0)
{
	Set(core::string(mT("")));
}
UTFString::UTFString(const utf8*buf):m_data(0),m_length(0),m_capacity(0)
{
	Set(buf);
}
UTFString::UTFString(const utf32*buf):m_data(0),m_length(0),m_capacity(0)
{
	Set(buf);
}
UTFString::~UTFString()
{
	_Clear();
}

void UTFString::_Clear()
{
	delete m_data;
	//s_allocator.deallocate(m_data);
	m_length=0;
	m_capacity=0;
}

void UTFString::Set(const core::string& str)
{

#ifdef UNICODE
	SetW(str);
#else
	SetA(str);
#endif
}

void UTFString::SetA(const core::stringc& str)
{
	Set((utf8*)str.c_str());
}
void UTFString::SetW(const core::stringw& str)
{
	if (str.length() >= m_capacity)
	{
		Reserve(2 * str.length() + 1);
		m_length = str.length();
	}
	const wchar_t*ptr = &str[0];
	utf32*c = m_data;
	for (; *ptr;)
	{
		*(c++) = *(ptr++);
	}
	*(c) = 0;
}



core::string UTFString::GetAsString()const
{
	core::string res;
#ifdef UNICODE
	return GetAsStringW();
#else
	return GetAsStringA();
#endif
}



core::stringc UTFString::GetAsStringA()const
{
	core::stringc res;
	uint len = UTF32EncodeSize(m_data);
	if (len > 0)
	{
		res.resize(len);
		EncodeUTF32(m_data, (utf8*)&res[0], len);
	}
	return res;
}


core::stringw UTFString::GetAsStringW()const
{
	core::stringw res;
	if (m_length > 0)
	{
		res.resize(m_length);
		const utf32* ptr = m_data;
		wchar_t* c = &res[0];
		for (; *ptr;)
		{
			*(c++) = *(ptr++);
		}
		*(c) = 0;
	}
	else
		res = L"";

	return res;
}


void UTFString::Reserve(uint sz)
{
	_Clear();
	m_capacity=sz;
	m_data=/*s_allocator.allocate(sz);//*/new utf32[sz];
	m_length=0;
}

void UTFString::Set(const utf8*buf)
{
	int len=UTF8EncodeSize(buf);
	if(len+1>m_capacity)
		Reserve(len*2+1);
	m_length=len;
	EncodeUTF8(buf,m_data,m_length+1);

}
void UTFString::Set(const utf32*buf)
{
	int len=UTF32Length(buf);
	if(len+1>m_capacity)
		Reserve(len*2+1);
	m_length=len;
	utf32*ptr=m_data;
	while(*buf)
	{
		*(ptr++)=*(buf++);
	}
	*ptr=0;
}

UTFString& UTFString::operator=(const UTFString&o)
{
	Reserve(o.m_capacity);
	utf32*ptr=o.m_data;
	utf32*ptr2=m_data;
	while(*ptr){
		*(ptr2++)=*(ptr++);
	}
	*ptr2=0;
	m_length=o.m_length;
	return *this;
}
UTFString& UTFString::operator=(const utf8*buf)
{
	Set(buf);
	return *this;
}
UTFString& UTFString::operator=(const utf32*buf)
{
	Set(buf);
	return *this;
}

UTFString& UTFString::operator+=(utf32 c)
{
	Append(c);
	return *this;
}

UTFString& UTFString::operator+=(const UTFString& buf)
{
	int len = buf.Length() + Length()+1;

	utf32* ptr = buf.m_data;
	utf32* dst=0;

	if (m_capacity < len)
	{
		utf32* newD = new utf32[len];
		dst = m_data;
		utf32* p = newD;
		while (*dst)
			(*p++) = (*dst++);
		delete [] m_data;
		m_data = newD;
		m_capacity = len;
	}
	dst = m_data + Length();
	while (*ptr)
	{
		*(dst++)=*(ptr++);
	}
	*dst = 0;
	m_length = len;
	return *this;
}

bool UTFString::operator==(const UTFString& o)const
{
	if(m_length!=o.m_length)return false;
	const utf32* ptr1=m_data;
	const utf32* ptr2=o.m_data;
	for(;*ptr1;++ptr1,++ptr2)
	{
		if(*ptr1!=*ptr2)return false;
	}
	return true;
}
bool UTFString::operator!=(const UTFString& o)const
{
	
	return !(*this==o);
}
bool UTFString::operator <(const UTFString& other) const
{
	for(int i=0; m_data[i] && other.m_data[i]; ++i)
		if (m_data[i] != other.m_data[i])
			return (m_data[i] < other.m_data[i]);

	return(m_length < other.m_length);

}

UTFString UTFString::substr(uint begin,uint len)const
{
	if(len+begin > m_length)
		len = m_length-begin;
	if (len <= 0)
		return UTFString((utf8*)"");

	UTFString o;
	o.Reserve(len+1);

	for (int i=0; i<len; ++i)
		o.m_data[i] = m_data[i+begin];

	o.m_data[len] = 0;
	o.m_length = len+1;

	return o;
}

void UTFString::Insert(utf32 c,uint pos,uint cnt)
{
	int i;
	int len=m_length+cnt;
	if(len>=m_capacity)
	{
		int cap=2*len+1;
		utf32* newData=/*s_allocator.allocate(cap);//*/new utf32[cap];
		for(i=0;i<pos;++i)
		{
			newData[i]=m_data[i];
		}

		for(i=pos;i<pos+cnt;++i)
			newData[i]=c;
		for(i=pos+cnt;i<=m_length;++i)
		{
			newData[i]=m_data[i-1];
		}
		delete m_data;
		//s_allocator.deallocate(m_data);
		m_data=newData;
		m_capacity=cap;
		m_length=len;
		m_data[m_length]=0;
	}else
	{

		for(i=m_length+cnt;i>=pos+cnt;--i)
		{
			m_data[i]=m_data[i-1];
		}
		m_length+=cnt;
		for(i=pos;i<pos+cnt;++i)
			m_data[i]=c;
		m_data[m_length]=0;

	}
}
void UTFString::Erase(uint start,uint end)
{
	if(end<start)return;
	end=(end>m_length)? m_length:end;
	int diff=m_length-end;
	for(int i=0;i<diff;++i)
	{
		m_data[start+i]=m_data[end+i+1];
	}
	m_length-=end-start+1;
}

void UTFString::Append(utf32 c)
{
	Insert(c,m_length,1);
}

uint UTFString::EncodeUTF8(const utf8*buf,utf32*dst,uint dst_capacity)
{
	utf8 c;
	uint len=dst_capacity;
	while(*buf && dst_capacity>0)
	{
		--dst_capacity;
		c=*buf++;
		if(c<0x80)
		{
			*dst=(utf32)c;
		}else if(c<0xE0) // 2 Bytes
		{
			*dst=(c&0x1F)<<6;		//first byte:  110xxxxx
			*dst|=(*buf++)&0x3F;	//second byte: 10xxxxxx
		}else if(c<0xF0) // 3 Bytes
		{
			*dst =(c&0x0F)<<12;			//first byte:  1110xxxx
			*dst|=((*buf++)&0x3F)<<6;	//second byte: 10xxxxxx
			*dst|=((*buf++)&0x3F)   ;	//third byte:  10xxxxxx
		}else
		{
			*dst=(c&0x07)<<18;			//first byte:  11110xxx
			*dst|=((*buf++)&0x3F)<<12;	//second byte: 10xxxxxx
			*dst|=((*buf++)&0x3F)<<6;	//third byte:  10xxxxxx
			*dst|=((*buf++)&0x3F)   ;	//fourth byte: 10xxxxxx
		}
		++dst;
	}
	*dst=0;
	return len-dst_capacity;
}
uint UTFString::EncodeUTF32(const utf32*buf,utf8*dst,uint dst_capacity)
{
	utf32 c;
	uint len=dst_capacity;
	while(*buf && dst_capacity>0)
	{
		c=*buf++;
		if(c<0x80)
		{
			*dst++=(utf8)c;
			--dst_capacity;
		}else if(c<0x0800) // 2 Bytes
		{
			*dst++=(utf8)((c>>6)|0xc0);
			*dst++=(utf8)((c&0x3F)|0x80);
			dst_capacity-=2;
		}else if(c<0x10000) // 3 Bytes
		{
			*dst++=(utf8)((c>>12)|0xE0);
			*dst++=(utf8)((c>>6) |0x3F)|0x80;
			*dst++=(utf8)((c&0x3F)|0x80);
			dst_capacity-=3;
		}else
		{
			*dst++=(utf8)((c>>18)|0xF0);
			*dst++=(utf8)((c>>12)|0x3F)|0x80;
			*dst++=(utf8)((c>>6) |0x3F)|0x80;
			*dst++=(utf8)((c&0x3F)|0x80);
			dst_capacity-=4;
		}
	}
	//*dst=0;
	return len-dst_capacity;
}



uint UTFString::UTF8EncodeSize(const utf8*buf)
{
	utf8 c;
	uint cnt=0;
	while (*buf)
	{
		c=*buf++;
		++cnt;
		if(c<0x80)	//0XXXXXXX,normal ASCII char
		{
		}else if(c<0xE0)//110XXXXX,2 bytes char
		{
			++buf;
		}else if(c<0xF0)//1110XXXX,3 bytes char
		{
			buf+=2;
		}else
		{
			buf+=3;
		}
	}
	return cnt;
}

uint UTFString::UTF32EncodeSize(const utf32*buf)
{
	uint cnt=0;
	while(*buf)
	{
		cnt+=UTF32EncodeSize(*buf++);
	}
	return cnt;
}

uint UTFString::UTF8Length(const utf8*buf)
{
	int len=0;
	while(*buf++)
	{
		++len;
	}
	return len;
}
uint UTFString::UTF32Length(const utf32*buf)
{
	int len=0;
	while(*buf++)
	{
		++len;
	}
	return len;
}


}
}
