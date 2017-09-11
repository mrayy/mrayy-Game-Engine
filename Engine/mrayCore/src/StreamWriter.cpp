#include "stdafx.h"

#include "StreamWriter.h"
#include "StringConverter.h"
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
	FIRE_LISTENR_METHOD(OnWriteData,(data,count));
	return m_stream->write(data,count);
}
int StreamWriter::writeLine(const core::string& str){
	core::stringc s2;
	core::string_to_char(str,s2);
	s2+="\n";
	return write(s2.c_str(),sizeof(char)*(s2.length()));
}
int StreamWriter::writeString(const core::string& str){
	core::stringc s2;
	core::string_to_char(str,s2);
	//s2+=' ';
	return write(s2.c_str(),sizeof(char)*(s2.length()));
}
int StreamWriter::binWriteInt(int v){
	if(!m_stream || !m_stream->canWrite())
		return 0;
	return m_stream->write(&v,sizeof(v));
}
int StreamWriter::txtWriteInt(int v){
	core::string txt=core::StringConverter::toString(v);
	return writeString(txt);
}
int StreamWriter::binWriteFloat(float v){
	return write(&v,sizeof(v));

}
int StreamWriter::txtWriteFloat(float v){
	core::string txt=core::StringConverter::toString(v);
	return writeString(txt);
}

int StreamWriter::binWriteString(const core::stringc& str)
{
	int ret=0;
	ret+=binWriteInt(str.length());
	ret+=write(str.c_str() ,str.length()*sizeof(char));
	return ret;
}
int StreamWriter::binWriteString(const core::stringw& str)
{
	int ret=0;
	ret+=binWriteInt(str.length());
	ret+=write(str.c_str() ,str.length()*sizeof(wchar_t));
	return ret;
}

}
}
