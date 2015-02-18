
#ifndef ___NetAddress___
#define ___NetAddress___


#include <mTypes.h>
#include <mstring.h>
#include "CompileConfig.h"



namespace mray{
namespace network{


enum EAdressType
{
	EAT_BroadCast,
	EAT_Localhost,
	EAT_None,
	EAT_Any
};


class MRAY_DLL NetAddress{
protected:
public:
	ulong address;
	int port;

	NetAddress();
	NetAddress(EAdressType type,int port=0);
	NetAddress(const core::string& ipStr,int port=0);

	bool operator == (const NetAddress &o)const;
	bool operator < (const NetAddress &o)const;


	void getIP(int ip[4]) const;
	void setIP(int ip[4]);
	void setIP(const core::string& ipStr);

	core::string toString() const;

	
static const NetAddress BroadcastAddr;
static const NetAddress LocalHostAddr;
static const NetAddress NoneAddr;
static const NetAddress AnyAddr;
};


}
}




#endif


