#include "stdafx.h"

#include "CMemoryStream.h"
#include "mraySystem.h"


namespace mray{
namespace OS{


CMemoryStream::CMemoryStream():IStream(),bDeleteAtEnd(0)
{
}
CMemoryStream::CMemoryStream(const  core::string&name,byte*data,int size,bool deleteAtEnd,FILE_MODE _mode):IStream(name,_mode),bDeleteAtEnd(deleteAtEnd){
	open(name,data,size,_mode);
}

CMemoryStream::~CMemoryStream(){
	if(bDeleteAtEnd)
		delete [] data;
}

uint CMemoryStream::calcSizeInternal(){
	uint sz=sizeof(CMemoryStream);
	if(bDeleteAtEnd)
		sz+=length();
	return sz;
}
void CMemoryStream::setDeleteAtEnd(bool set){
	bDeleteAtEnd=set;
}

bool CMemoryStream::isDeleteAtEnd(){
	return bDeleteAtEnd;
}

void CMemoryStream::setData(byte*data,int size){
	this->data=data;
	this->m_length=size;
	pos=0;
}
byte *CMemoryStream::getData()
{
	return data;
}
bool CMemoryStream::reopen(FILE_MODE mode)
{
	pos=0;
	return true;
}
bool CMemoryStream::canRead(){
	return (data!=0 && pos < length());
}
bool CMemoryStream::canWrite(){
	return (data!=0 && pos < length());
}
bool CMemoryStream::canSeek()
{
	return true;
}

bool CMemoryStream::open(const  core::string&name,byte*data,int size,FILE_MODE _mode){
	m_streamName=name;
	m_mode=_mode;
	setData(data,size);
	return 1;
}
void CMemoryStream::close(){
}
bool CMemoryStream::isOpen(){
	return length()!=0 && data!=0;
}

int CMemoryStream::read(void*_data,size_t _size){
	if(pos>=length())
		return 0;
	if(pos + _size>length())
		_size=length()-pos;
	mraySystem::memCopy(_data,data+pos,_size);
	pos+=_size;
	return _size;
	
}
int CMemoryStream::write(const void*_data,size_t _size){
	if(pos>=length())
		return 0;
	if(pos + _size>length())
		_size=length()-pos;
	mraySystem::memCopy(data+pos,_data,_size);
	pos+=_size;
	return _size;
}
int CMemoryStream::seek(int bytes,EStreamSeek smode){
	switch(smode){
	case ESeek_Set:
		pos=bytes;
		break;
	case ESeek_Cur:
		pos+=bytes;
		break;
	case ESeek_End:
		pos=length()-bytes;
		break;
	}
	if(pos>length())
		pos=length();
	return pos;
}
int CMemoryStream::getPos(){
	return pos;
}
bool CMemoryStream::eof(){
	return (pos==length());
}





}
}



