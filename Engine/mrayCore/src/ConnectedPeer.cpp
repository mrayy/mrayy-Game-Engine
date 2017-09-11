#include "stdafx.h"

#include "ConnectedPeer.h"


namespace mray{
namespace network{




ConnectedPeer::ConnectedPeer(ISocket*s){
	state=EPCS_NONE;
	nextPingTime=0;
	lastMsgTime=0;
	lastPingTime=0;
	pingTime=0;
	m_socket=s;
	m_sendQueue=new SendPacketQueue(m_socket);
	removed=0;
}
ConnectedPeer::~ConnectedPeer(){
}
}
}