

#ifndef __LOCALNETWORKPORTASSIGNER__
#define __LOCALNETWORKPORTASSIGNER__

#include "INetworkPortAssigner.h"

namespace mray
{
namespace network
{
class MRAY_DLL LocalNetworkPortAssigner:public INetworkPortAssigner
{
protected:
public:
	LocalNetworkPortAssigner();
	virtual ~LocalNetworkPortAssigner();
	virtual unsigned short RequestPort(const core::string& name, network::EProtoType type);
	virtual unsigned short AssignPort(const core::string& name, network::EProtoType type, unsigned short port);
	virtual bool UnassignPort(const core::string& name, network::EProtoType type);


};

}
}


#endif
