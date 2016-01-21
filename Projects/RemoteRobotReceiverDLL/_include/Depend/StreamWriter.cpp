#include "stdafx.h"

#include "StreamWriter.h"
#include "IStream.h"

namespace mray{
namespace OS{

StreamWriter::StreamWriter():m_stream(0)
{
}
StreamWriter::StreamWriter(IStream* stream):m_stream(stream)
{
}
StreamWriter::~StreamWriter(){
	m_stream=0;
}

void StreamWriter::setStream(IStream* s){
	m_stream=s;
}
IStream*	StreamWriter::getStream(){
	return m_stream;
}

int StreamWriter::writeByte(uchar v){
	return write(&v,sizeof(uchar));
}
int StreamWriter::write(const void*data,size_t count){
	if(!m_stream || !m_stream->canWrite())
		return 0;
	return m_stream->write(data,count);
}
int StreamWriter::writeLine(const std::string& str){
	std::string s2=str;
	s2+="\n";
	return write(s2.c_str(),sizeof(char)*(s2.length()));
}
int StreamWriter::writeString(const std::string& str){
	return write(str.c_str(), sizeof(char)*(str.length()));
}
int StreamWriter::binWriteInt(int v){
	if(!m_stream || !m_stream->canWrite())
		return 0;
	return m_stream->write(&v,sizeof(v));
}
int StreamWriter::binWriteFloat(float v){
	return write(&v,sizeof(v));

}

}
}
