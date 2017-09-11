


#ifndef __INETWORKPORTASSIGNER__
#define __INETWORKPORTASSIGNER__

#include "mString.h"
#include "NetDataTypes.h"
#include "ISingleton.h"

namespace mray
{
	namespace network
{
class MRAY_CORE_DLL  INetworkPortAssigner:public ISingleton<INetworkPortAssigner>
{
protected:
public:

	INetworkPortAssigner(){}
	virtual~INetworkPortAssigner(){}
	
	virtual unsigned short RequestPort(const core::string& name, network::EProtoType type) = 0;
	virtual unsigned short AssignPort(const core::string& name, network::EProtoType type, unsigned short port) = 0;
	virtual bool UnassignPort(const core::string& name, network::EProtoType type) = 0;

};

#define gNetworkPortAssigner mray::network::INetworkPortAssigner::getInstance()

}
}

#endif
