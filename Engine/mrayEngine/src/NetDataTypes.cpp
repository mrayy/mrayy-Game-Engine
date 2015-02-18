#include "stdafx.h"

#include "NetDataTypes.h"
#include "SendPacketQueue.h"
#include "mraySystem.h"


namespace mray{
namespace network{





SRequestPeer::SRequestPeer(){
	requestCount=0;
	password[0]=0;
}


void SRequestPeer::setPassword(const char*pass,int len){
	if(len>255)len=255;
	mraySystem::memCopy(password,pass,len);
	passLen=len & 0xff;
}




SRecivedMessage::SRecivedMessage(){
	m_remove=0;
	m_inUse=1;
}
SRecivedMessage::~SRecivedMessage(){
	msg.clear();
}
void SRecivedMessage::use(){
	m_inUse++;
}
void SRecivedMessage::release(){
	m_inUse--;
}


}
}
	
