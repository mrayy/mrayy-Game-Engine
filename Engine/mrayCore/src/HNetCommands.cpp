#include "stdafx.h"


#include "HNetCommands.h"

#include <mString.h>



namespace mray{
namespace network{
namespace commands{

NTCMD getCommand(const mchar*cmd,NTCMD(*callback)(const mchar*cmd)){
	core::string str=cmd;
/*	if(strcmp(cmd,"connect"))return NET_CMD_CONNECT;
	if(strcmp(cmd,"disconnect"))return NET_CMD_DISCONNECT;
	if(strcmp(cmd,"maxclients"))return NET_CMD_MAXCLIENTSREACH;

*/
	if(callback)
		return callback(cmd);

	return NET_CMD_UNKOWN;
}
const mchar*getCommand(NTCMD cmd,const mchar*(*callback)(NTCMD)){

	switch (cmd)
	{
	case NET_CMD_CONNECT_REQUEST:
		return mT("Connect Request");
	case NET_CMD_CONNECT_ACCEPT:
		return mT("Connect Accept");
	case NET_CMD_CONNECTED:
		return mT("Connected");
	case NET_CMD_CONNECT_FAILED:
		return mT("Connect Failed");
	case NET_CMD_DISCONNECT:
		return mT("Disconnect");
	case NET_CMD_NOPROPLEM:
		return mT("No Problem");
	case NET_CMD_MAXCLIENTSREACH:
		return mT("Max Clients Reach");
	case NET_CMD_REPLAY:
		return mT("replay");
	case NET_CMD_CLIENTID:
		return mT("Client ID");
	case NET_CMD_BANNED:
		return mT("Banned");
	case NET_CMD_PING:
		return mT("Ping");
	case NET_CMD_PONG:
		return mT("Pong");
	case NET_CMD_PEER_DISCONNECT:
		return mT("peer disconnect");
	case NET_CMD_PEERINFO:
		return mT("Peer Info");
	case NET_CMD_PEERINFO_REPLAY:
		return mT("Peer Info Replay");
	case NET_CMD_INVALID_PASSWORD:
		return mT("Invalid Password");
	case NET_CMD_UNKOWN:
	default:
		return mT("Unkown");
	}
	
	if(callback)
		return callback(cmd);

}

}
}
}


