#include "stdafx.h"


#include <mstring.h>
#include "NetAddress.h"

#include <string>
#ifdef WIN32
#include <winsock2.h>
//#pragma comment (lib,"ws2_32.lib")
#endif


namespace mray{
namespace network{
const NetAddress NetAddress::BroadcastAddr(EAT_BroadCast);
const NetAddress NetAddress::LocalHostAddr(EAT_Localhost);
const NetAddress NetAddress::NoneAddr(EAT_None);
const NetAddress NetAddress::AnyAddr(EAT_Any);

NetAddress::NetAddress(){
	address=0;
	port=0;
}
NetAddress::NetAddress(EAdressType type,int port){
	address=0;
	this->port=port;
	switch(type){
		case EAT_None:
			address=0;
			break;
		case EAT_Localhost:
			//127.0.0.1
			address=0x7F000001;//htonl(0x7F000001);
			break;
		case EAT_BroadCast:
			//255.255.255.255
			address=INADDR_BROADCAST;//htonl(INADDR_BROADCAST);
			break;
		case EAT_Any:
			address=INADDR_ANY;//htonl(INADDR_ANY);
			break;
	}
}

NetAddress::NetAddress(const core::string&ipStr,int port){

	this->port=port;
	setIP(ipStr);
}

bool NetAddress::operator == (const NetAddress &o)const{
	return address==o.address && port==o.port;
}
bool NetAddress::operator < (const NetAddress &o)const{
	if(address<o.address)return true;
	if(address==o.address)return port<=o.port;
	return false;
}
core::string NetAddress::toString() const{

	int ip[4];
	getIP(ip);

	char ipStr[32];
	core::string str;
	sprintf_s(ipStr,32,"%d.%d.%d.%d",ip[0],ip[1],ip[2],ip[3]);
	core::char_to_string(ipStr,str);
	return str;
}
void NetAddress::getIP(int ip[4]) const{
	ulong addr= address;//ntohl(NetAddress);

	ip[0]=addr&0xFF;
	ip[1]=(addr>>8) & 0xFF;
	ip[2]=(addr>>16) & 0xFF;
	ip[3]=(addr>>24) & 0xFF;
}

void NetAddress::setIP(int ip[4]){
	ulong addr;
	addr=ip[3]<<24;
	addr+=ip[2]<<16;
	addr+=ip[1]<<8;
	addr+=ip[0];

	
	address=addr;//htonl(addr);
}
void NetAddress::setIP(const core::string& ipStr){
	
	if(ipStr=="")return;
	int ip[4];

	core::stringc str;
	core::string_to_char(ipStr,str);
	if(sscanf_s(str.c_str(),"%d.%d.%d.%d",&ip[0],&ip[1],&ip[2],&ip[3])!=4)return;

	setIP(ip);
}



}
}

