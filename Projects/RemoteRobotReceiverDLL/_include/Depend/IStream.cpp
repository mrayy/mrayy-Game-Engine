#include "stdafx.h"



#include "IStream.h"
#include <string>

namespace mray{
namespace OS{

IStream::IStream()
{
	m_mode=BIN_READ;
}
IStream::IStream(const  std::string&name,FILE_MODE mode){
	m_streamName=name;
	m_mode=mode;
	m_length = 0;
}

IStream::~IStream(){
}

void IStream::setMode(FILE_MODE mode){
	m_mode=mode;
}
const  std::string& IStream::getStreamName(){
	return m_streamName;
}
//! get size of file
int IStream::length(){
	return m_length;
}

int IStream::writeByte(uchar v){
	return write(&v,sizeof(v));
}
uchar IStream::readByte(){
	uchar v;
	read(&v,sizeof(v));
	return v;
}

int IStream::readToEnd(void**data)
{/*
	if(isStream())
	{
		const int BlockSize=4096;
		std::vector<uchar> res;
		int current=0;
		while(!eof())
		{
			res.increaseUsed(BlockSize);
			int bytes=read(res.pointer()+current,BlockSize);
			current+=bytes;
		}
		res.resize(current);
		*data=res.pointer();
		return res.size();
	}else
	{
		*data=new uchar[length()];
		return read(*data,length());
	}*/
}

};//OS
};//mray







