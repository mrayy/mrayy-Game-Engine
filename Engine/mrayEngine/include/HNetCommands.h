

#ifndef ___HNetCommands___
#define ___HNetCommands___

#include <mTypes.h>
#include "CompileConfig.h"


namespace mray{
namespace network{

namespace commands{
	typedef uint NTCMD;

	const NTCMD	NET_CMD_UNKOWN				=0;
	const NTCMD	NET_CMD_CONNECT_REQUEST		=1;
	const NTCMD	NET_CMD_CONNECT_ACCEPT		=2;
	const NTCMD	NET_CMD_CONNECT_FAILED		=3;
	const NTCMD	NET_CMD_CONNECTED			=4;
	const NTCMD	NET_CMD_DISCONNECT			=5;
	const NTCMD	NET_CMD_NOPROPLEM			=6;
	const NTCMD	NET_CMD_MAXCLIENTSREACH		=7;
	const NTCMD	NET_CMD_CLIENTID			=8;
	const NTCMD	NET_CMD_BANNED				=9;
	const NTCMD	NET_CMD_REPLAY				=10;
	const NTCMD	NET_CMD_PING				=11;
	const NTCMD	NET_CMD_PONG				=12;
	const NTCMD NET_CMD_PEERINFO			=13;
	const NTCMD NET_CMD_PEERINFO_REPLAY		=14;
	const NTCMD NET_CMD_INVALID_PASSWORD	=15;
	const NTCMD	NET_CMD_PEER_DISCONNECT		=16;
	const NTCMD	NET_CMD_REMOVE_SEQ			=17;
	const NTCMD	NET_CMD_CAN_CONNECT_REQ		=18;
	const NTCMD	NET_CMD_CAN_CONNECT_ANS		=19;
	const NTCMD	NET_CMD_CAN_CONNECT_FAILED	=20;
	const NTCMD	NET_CMD_SERVER_FULL_INFO_REQ	=21;
	const NTCMD	NET_CMD_SERVER_FULL_INFO_REPLAY	=22;

	const NTCMD	NET_CMD_USER				=30;		
	
	MRAY_DLL NTCMD getCommand(const mchar*cmd,NTCMD(*callback)(const mchar*cmd));
	MRAY_DLL const mchar*getCommand(NTCMD,const mchar*(*callback)(NTCMD));
}

}
}



#endif