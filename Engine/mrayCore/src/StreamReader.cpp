#include "stdafx.h"


#include "StreamReader.h"
#include "IStream.h"

#include <string>

namespace mray{
namespace OS{


StreamReader::StreamReader(IStream* stream):m_stream(stream)
{
}
StreamReader::StreamReader():m_stream(0)
{
}
StreamReader::~StreamReader(){
	m_stream=0;
}

void StreamReader::setStream(IStream* s){
	m_stream=s;
}
IStream*	StreamReader::getStream(){
	return m_stream;
}

uchar StreamReader::readByte(){
	uchar data;
	read(&data,sizeof(data));
	return data;
}
int StreamReader::read(void*data,size_t count){
	if(!m_stream || !m_stream->canRead())
		return 0;
	int res=m_stream->read(data,count);
	FIRE_LISTENR_METHOD(OnReadData,(data,res));
	return res;
}
core::string StreamReader::readLine(char comment){
	if(!m_stream || !m_stream->canRead())
		return 0;
	char ch;
	int n=0;
	bool flag;
	bool done=0;
	core::string result;
	//do
	{
		flag=0;
		n=0;
		while(m_stream->canRead()){
			if(!m_stream->read(&ch,sizeof(char)))break;
			if(ch=='\n' || ch=='\0'){
				done=true;
				break;
			}
			if(ch==comment){
				flag=1;
			}
			if(!flag)
				result.append(1,(wchar_t) ch);
			n++;
		}
	}
	//while(!done);///until read one line at least

	FIRE_LISTENR_METHOD(OnReadData,(result.c_str(),result.length()*sizeof(char)));
	return result;
}
int StreamReader::txtReadInt()
{
	if(!m_stream || !m_stream->canRead())
		return 0;
	char tmp[20]={0};
	int n=0;
	char c;
	do{
		read(&c,sizeof(c));
	}while(m_stream->canRead() && !isgraph(c));

	n=0;
	for(int i=0;i<20 ;i++,n++){
		if(isgraph((int)c))
			tmp[i]=(char)c;
		else break;
		if(!m_stream->canRead())
			break;
		c=readByte();
	}
	//pos+=sscanf((const mchar*)(data+pos),"%d",&o);
	return atoi(tmp);
}

bool StreamReader::binReadBool()
{
	if(!m_stream || !m_stream->canRead())
		return 0;
	bool v=0;
	read(&v,sizeof(v));
	return v;

}

long StreamReader::binReadLong()
{
	if(!m_stream || !m_stream->canRead())
		return 0;
	long v=0;
	read(&v,sizeof(v));
	return v;

}


int StreamReader::binReadInt()
{

	if(!m_stream || !m_stream->canRead())
		return 0;
	int v=0;
	read(&v,sizeof(v));
	return v;
}
float StreamReader::txtReadFloat()
{
	if(!m_stream || !m_stream->canRead())
		return 0;
	char tmp[20]={0};
	int n=0;
	char c;
	do{
		read(&c,sizeof(c));
	}while(m_stream->canRead() && !isgraph(c));

	n=0;
	bool radix=0;

	if(isgraph((int)c))
		tmp[0]=(char)c;
	else if(c=='.'&&!radix){
		radix=true;
		tmp[0]=(char)c;
	}
	for(int i=1;i<20 ;i++,n++){
		if(!m_stream->canRead())
			break;
		c=readByte();
		if(isgraph((int)c))
			tmp[i]=(char)c;
		else if(c=='.'&&!radix){
			radix=true;
			tmp[i]=(char)c;
		}else
			break;
	}
	return atof(tmp);
}
float StreamReader::binReadFloat()
{
	if(!m_stream || !m_stream->canRead())
		return 0;
	float v=0;
	read(&v,sizeof(v));
	return v;
}
core::string StreamReader::readString()
{
	if(!m_stream || !m_stream->canRead())
		return 0;
	char c;
	core::string out;
	do{
		read(&c,sizeof(c));
	}
	while((isspace(c) || c==0) && m_stream->canRead());
	int cnt=1;
	out=mT("");

	out.append(1,c);
	cnt++;
	while(m_stream->canRead()){
	
		if( read(&c,sizeof(c))==0 )//)
			break;
		if(c==0 ||  isspace(c))
			break;
		out.append(1,c);
	}
	return out;
}
core::stringc	StreamReader::binReadStringC()
{
	int len=binReadInt();
	if(!len)return "";
	char* data=new char[len+1];
	read(data,len*sizeof(char));
	data[len]=0;
	core::stringc ret=data;
	delete data;
	return ret;
}
core::stringw	StreamReader::binReadStringW()
{
	int len=binReadInt();
	if(!len)return L"";
	wchar_t* data=new wchar_t[len+1];
	read(data,len*sizeof(wchar_t));
	data[len]=0;
	core::stringw ret=data;
	delete data;
	return ret;
}
core::string StreamReader::binReadString()
{
#ifdef UNICODE
	return binReadStringW();
#else
	return binReadStringC();
#endif
}


}
}