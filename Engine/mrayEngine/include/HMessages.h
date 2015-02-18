

#ifndef ___HMessages___
#define ___HMessages___

#include "CompileConfig.h"
#include <LogManager.h>

namespace mray{
namespace network{
namespace messages{


const int HMSG_NET_START		= 0;
const int HMSG_SOCKET_OPEN		= 1;
const int HMSG_TCP_SOCKET_CREATE	= 2;
const int HMSG_UDP_SOCKET_CREATE	= 3;
const int HMSG_GETHOST		= 4;
const int HMSG_BIND			= 5;
const int HMSG_GETSOCKNAME	= 6;
const int HMSG_ACCEPT		= 7;
const int HMSG_LISTEN		= 8;
const int HMSG_FIND_REMOTE_HOST		= 9;
const int HMSG_CREATE_CONNECTION	= 10;
const int HMSG_SEND			= 11;
const int HMSG_RECIVE		= 12;
const int HMSG_SERVER_LOST	=13;
const int HMSG_CANOT_CONNECT =14;
const int HMSG_MAX_CLIENTS =15;
const int HMSG_CLIENT_LOST =16;
const int HMSG_SYNCRONIZE =17;
const int HMSG_SETBROADCAST =18;
const int HMSG_BROADCAST =19;
const int HMSG_TIMEOUT	=20;
const int HMSG_UNREACHABLEHOST	=21;

const int HMSG_UNKOWN		=100;

MRAY_DLL const mchar* getMessage(int msgCode);
MRAY_DLL void logMessage(int msgCode,ELogLevel level);
MRAY_DLL void logMessage(const mchar*msg,ELogLevel level);


}
}
}



#endif



