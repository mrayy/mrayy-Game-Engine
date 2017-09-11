#include "stdafx.h"

#include "HMessages.h"
#include <string>


namespace mray{
namespace network{
namespace messages{


const mchar* getMessage(int msgCode){

	static core::string msgStr;
	switch(msgCode){
		case HMSG_NET_START:
			msgStr+=mT("Network startup.");
			break;
		case HMSG_SOCKET_OPEN:
			msgStr+=mT("Socket Open.");
			break;
		case HMSG_TCP_SOCKET_CREATE:
			msgStr+=mT("TCP Socket Create.");
			break;
		case HMSG_UDP_SOCKET_CREATE:
			msgStr+=mT("UDP Socket Create.");
			break;
		case HMSG_GETHOST:
			msgStr+=mT("Get Host Name.");
			break;
		case HMSG_BIND:
			msgStr+=mT("socket Bind.");
			break;
		case HMSG_GETSOCKNAME:
			msgStr+=mT("Getting socket name.");
			break;
		case HMSG_ACCEPT:
			msgStr+=mT("accept data.");
			break;
		case HMSG_LISTEN:
			msgStr+=mT("Socket Listning.");
			break;
		case HMSG_FIND_REMOTE_HOST:
			msgStr+=mT("Find Remote Host.");
			break;
		case HMSG_CREATE_CONNECTION:
			msgStr+=mT("Creating Connection to Remote Host.");
			break;
		case HMSG_SEND:
			msgStr+=mT("Send Data.");
			break;
		case HMSG_RECIVE:
			msgStr+=mT("Recive Data.");
			break;
		case HMSG_SERVER_LOST:
			msgStr+=mT("Server Lost.");
			break;
		case HMSG_CANOT_CONNECT:
			msgStr+=mT("Cann't connect to server.");
			break;
		case HMSG_MAX_CLIENTS:
			msgStr+=mT("Max Clients Reached.");
			break;
		case HMSG_CLIENT_LOST:
			msgStr+=mT("Client Lost.");
			break;
		case HMSG_SETBROADCAST:
			msgStr+=mT("set Broadcast.");
			break;
		case HMSG_BROADCAST:
			msgStr+=mT("Broadcasting!.");
			break;
		case HMSG_TIMEOUT:
			msgStr+=mT("timeout!.");
			break;
		case HMSG_UNREACHABLEHOST:
			msgStr+=mT("Unreachable remote host!.");
			break;

		default:
			msgStr+=mT("Unkwon.");
			break;
	}


	return msgStr.c_str();
}

void logMessage(int msgCode,ELogLevel level){
	const mchar*str=getMessage(msgCode);
	
	gLogManager.log(str,level);

}

void logMessage(const mchar*msg,ELogLevel level){
	gLogManager.log(msg,level);


}

}
}
}


