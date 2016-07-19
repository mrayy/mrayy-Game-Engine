
#include "stdafx.h"
#include "RobotCommunicator.h"
#include "StringUtil.h"
#include "MutexLocks.h"

namespace mray
{
namespace TBee
{



RobotCommunicator::RobotCommunicator()
{
	m_listener = 0;
		
}

RobotCommunicator::~RobotCommunicator()
{
	StopServer();
}
void RobotCommunicator::_HandleData(network::NetAddress* srcaddr,const core::string& name, const core::string& value)
{

	std::vector<core::string> vals;
	vals=core::StringUtil::Split(value, ",");

	OS::ScopedLock lock(m_dataMutex);

	if (name == "Connect")// && vals.size() == 6)
	{/*
		int videoPort = atoi(vals[1].c_str());
		int audioPort = atoi(vals[2].c_str());
		int handsPort = atoi(vals[3].c_str());
		int clockPort = atoi(vals[4].c_str());
		bool rtcp = core::StringConverter::toBool(vals[5].c_str())
		network::NetAddress addr = network::NetAddress(vals[0], videoPort);;*/
		//if (addr.address != m_userStatus.address.address || addr.port!=m_userStatus.address.port)
		{
			//printf("Connect Message: %s\n", srcaddr->toString().c_str());
			m_userStatus.receivedAddress = *srcaddr;
			m_userStatus.clientAddress = *srcaddr;// .setIP(vals[0]);
			if (m_listener)
			{
				UserConnectionData data;
				data.userData = m_userStatus;
				/*data.videoPort = videoPort;
				data.audioPort = audioPort;
				data.handsPort = handsPort;
				data.clockPort = clockPort;
				data.rtcp = rtcp;*/
				m_listener->OnUserConnected(this, data);
			}
		}
	}
	else if (name == "Disconnect" && vals.size() == 2)
	{
		//printf("Disconnect Message: %s\n", srcaddr->toString().c_str());
		network::NetAddress addr = network::NetAddress(vals[0], atoi(vals[1].c_str()));
		addr.address = srcaddr->address;
		if (addr.address == m_userStatus.clientAddress.address)
		{
			if (m_listener)
			{
				m_listener->OnUserDisconnected(this, m_userStatus.clientAddress);
			}
		}
	}
	else
	{
		if (m_listener)
			m_listener->OnUserMessage(srcaddr, name, value);
	}
}

void RobotCommunicator::StartServer(int port)
{
	StartHandler(port);
}

void RobotCommunicator::StopServer()
{
	StopHandler();
}

}
}