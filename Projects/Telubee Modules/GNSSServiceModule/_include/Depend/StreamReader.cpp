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
	return res;
}
std::string StreamReader::readLine(char comment){
	if(!m_stream || !m_stream->canRead())
		return 0;
	char ch;
	int n=0;
	bool flag;
	bool done=0;
	std::string result;
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
std::string StreamReader::readString()
{
	if(!m_stream || !m_stream->canRead())
		return 0;
	char c;
	std::string out;
	do{
		read(&c,sizeof(c));
	}
	while((isspace(c) || c==0) && m_stream->canRead());
	int cnt=1;
	out=("");

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
std::string	StreamReader::binReadString()
{
	int len=binReadInt();
	if(!len)return "";
	char* data=new char[len+1];
	read(data,len*sizeof(char));
	data[len]=0;
	std::string ret=data;
	delete data;
	return ret;
}


}
}