#include "stdafx.h"



#include "CLimitedStream.h"





namespace mray{
namespace OS{

CLimitedStream::CLimitedStream(GCPtr<IStream>masterFile,int limitSize,int startPos,const  mchar*name,FILE_MODE mode)
	:IStream(name,mode),m_masterStream(masterFile),m_limitSize(limitSize),m_startPos(startPos),opened(true),
	pos(0)
{
	m_length=limitSize;
}

CLimitedStream::~CLimitedStream(){
	close();
}


uint CLimitedStream::calcSizeInternal(){
	return sizeof(CLimitedStream);
}


void CLimitedStream::close(){
	m_masterStream=0;
	opened=false;
}

bool CLimitedStream::isOpen(){
	return opened;
}


bool CLimitedStream::canRead(){
	return m_masterStream->canRead() && pos<m_limitSize;
}
bool CLimitedStream::canWrite(){
	return m_masterStream->canWrite() && pos<m_limitSize;
}

int CLimitedStream::read(void*_data,size_t _size){
	int fPos=m_masterStream->getPos();

	if(pos>=m_limitSize)return 0;

	m_masterStream->seek(m_startPos + pos,ESeek_Set);
	int cnt=_size;
	if(pos+cnt>m_limitSize)
		cnt=m_limitSize-pos;
	pos+=cnt;

	int ret= m_masterStream->read(_data,cnt);
	m_masterStream->seek(fPos,ESeek_Set);

	return ret;
}

int CLimitedStream::write(const void*_data,size_t _size){
	int fPos=m_masterStream->getPos();

	if(pos>=m_limitSize)return 0;

	m_masterStream->seek(m_startPos + pos,ESeek_Set);
	int cnt=_size;
	if(pos+cnt>m_limitSize)
		cnt=m_limitSize-pos;
	pos+=cnt;

	int ret= m_masterStream->write(_data,cnt);
	m_masterStream->seek(fPos,ESeek_Set);

	return ret;
}

int CLimitedStream::seek(int bytes,EStreamSeek smode){
	switch(smode){
	case ESeek_Set:
		pos=bytes;
		break;
	case ESeek_Cur:
		pos+=bytes;
		break;
	case ESeek_End:
		pos=m_limitSize-bytes;
		if(pos<0)pos=0;
		return pos;
	}
	if(pos>m_limitSize)
		pos=m_limitSize;
	return pos;
}
int CLimitedStream::getPos(){
	return pos;
}
bool CLimitedStream::eof(){
	return pos==m_limitSize;
}





}
}





