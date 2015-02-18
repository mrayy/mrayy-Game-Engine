


#include "stdafx.h"
#include "GhostProxyManager.h"
#include "IUDPClient.h"
#include "INetwork.h"
#include "IThreadFunction.h"
#include "IThread.h"
#include "NetPacket.h"
#include "BitConverter.h"
#include "CMemoryStream.h"
#include "StreamWriter.h"
#include "StreamReader.h"
#include "IPhysicalNode.h"
#include "IThreadManager.h"
#include "MutexLocks.h"
#include "IMovable.h"
#include "ILogManager.h"

namespace mray
{
namespace game
{

class GhostProxyManagerImpl;

#define UDPBufferSize 4096

	const uint GhostNet_Connect=0x100;
	const uint GhostNet_Disconnect=0x101;
	const uint GhostNet_Objects=0x102;

	class BaseGhostNetPacket
	{
	protected:
		static byte send_buffer[UDPBufferSize];
		static byte rcv_buffer[UDPBufferSize];
	public:
		virtual uint getCommand()=0;

		virtual bool readPacket(network::IUDPClient*client,const network::NetAddress* from)=0;
		virtual void writeData(const network::NetAddress* addr,network::IUDPClient*client)
		{
			int len=core::BitConverter::GetBytes(getCommand(),send_buffer,UDPBufferSize);
			client->SendTo(addr,(char*)send_buffer,len);
		}
	};

	byte BaseGhostNetPacket::send_buffer[];
	byte BaseGhostNetPacket::rcv_buffer[];

	class GhostNetPacketConnect:public BaseGhostNetPacket
	{
	public:

		network::NetAddress addr;

		GhostNetPacketConnect ()
		{
		}
		virtual ~GhostNetPacketConnect ()
		{
		}
		virtual uint getCommand()
		{
			return GhostNet_Connect;
		}
		virtual bool readPacket(network::IUDPClient*client,const network::NetAddress* from)
		{
			if(from)
				addr=*from;
			return true;
		}
		virtual void writeData(const network::NetAddress* addr,network::IUDPClient*client)
		{
			return BaseGhostNetPacket::writeData(addr,client);
		}
	};

	class GhostNetPacketObjects:public BaseGhostNetPacket
	{
	public:

		struct DataPacket
		{
			uint ID;
			math::vector3d pos;
			math::quaternion ori;
		};

		std::vector<DataPacket> data;

		GhostNetPacketObjects ()
		{
		}
		virtual ~GhostNetPacketObjects ()
		{
		}

		virtual uint getCommand()
		{
			return GhostNet_Objects;
		}


		virtual bool readPacket(network::IUDPClient*client,const network::NetAddress* from)
		{
			uint len=UDPBufferSize;
			network::NetAddress addr;
			if(client->RecvFrom((char*)rcv_buffer,&len,&addr,0)!=network::UDP_SOCKET_ERROR_NONE)
			{
				OS::CMemoryStream stream("",rcv_buffer,UDPBufferSize,false,OS::BIN_READ);
				OS::StreamReader rdr(&stream);
				int cnt=rdr.txtReadInt();

				DataPacket dp;

				for(int i=0;i<cnt;++i)
				{
					dp.ID=rdr.binReadInt();

					dp.pos.x=rdr.binReadFloat();
					dp.pos.y=rdr.binReadFloat();
					dp.pos.z=rdr.binReadFloat();

					dp.ori.x=rdr.binReadFloat();
					dp.ori.y=rdr.binReadFloat();
					dp.ori.z=rdr.binReadFloat();
					dp.ori.w=rdr.binReadFloat();

					data.push_back(dp);
				}
			}else
				return false;
			return true;
		}
		virtual void writeData(const network::NetAddress* addr,network::IUDPClient*client)
		{
			BaseGhostNetPacket::writeData(addr,client);
			OS::CMemoryStream stream("",send_buffer,UDPBufferSize,false,OS::BIN_WRITE);
			OS::StreamWriter wrtr(&stream);
			wrtr.binWriteInt(data.size());
			for(int i=0;i<data.size();++i)
			{
				if(stream.getPos()>=UDPBufferSize-sizeof(DataPacket))
				{
					client->SendTo(addr,(char*)send_buffer,stream.getPos());
					BaseGhostNetPacket::writeData(addr,client);
					stream.seek(0,OS::ESeek_Set);
				}
				wrtr.binWriteInt(data[i].ID);

				wrtr.binWriteFloat(data[i].pos.x);
				wrtr.binWriteFloat(data[i].pos.y);
				wrtr.binWriteFloat(data[i].pos.z);

				wrtr.binWriteFloat(data[i].ori.x);
				wrtr.binWriteFloat(data[i].ori.y);
				wrtr.binWriteFloat(data[i].ori.z);
				wrtr.binWriteFloat(data[i].ori.w);
			}
			client->SendTo(addr,(char*)send_buffer,stream.getPos());
		}
	};

	class GhostProxyUDPThread:public OS::IThreadFunction
	{
	protected:
		GhostProxyManagerImpl* m_owner;
	public:
		GhostProxyUDPThread(GhostProxyManagerImpl* o):m_owner(o)
		{
		}
		void execute(OS::IThread*caller,void*arg);
	};


	struct ClientInfo
	{
	public:
		network::NetAddress address;
	};

	class GhostProxyManagerImpl
	{
	protected:
		
		struct ObjectInfo
		{
		public:
			scene::IMovable* node;
			uint masterID;
		};

		struct NodeInfo
		{
		public:
			physics::IPhysicalNode* node;
			uint ID;
		};

		network::IUDPClient* m_udpClient;
		bool m_isMaster;

		typedef  std::map<uint,uint> IndexMap;

		std::vector<ObjectInfo> m_objects;
		IndexMap m_objectsMap;

		std::vector<NodeInfo> m_masterNodes;
		IndexMap m_masterNodesMap;

		OS::IThread* m_thread;

		OS::IMutex* m_dataMutex;
		typedef std::map<uint,GhostNetPacketObjects::DataPacket> DataPacketMap;
		DataPacketMap m_dataPackets;

		std::vector<ClientInfo> m_connectedClients;

	public:
		GhostProxyManagerImpl()
		{
			m_isMaster=false;
			m_udpClient=network::INetwork::getInstance().createUDPClient();
			m_thread=0;
			m_dataMutex=OS::IThreadManager::getInstance().createMutex();
		}
		virtual~GhostProxyManagerImpl()
		{
			Close();
			delete m_udpClient;
		}

		network::IUDPClient* GetClient(){return m_udpClient;}

		bool OpenPort(int port)
		{
			Close();
			if(m_udpClient->Open(port)==network::UDP_SOCKET_ERROR_NONE)
			{
				m_thread=OS::IThreadManager::getInstance().createThread(new GhostProxyUDPThread(this));
				m_thread->start(0);
				m_isMaster=true;
				return true;
			}
			return false;
		}
		bool Connect(const network::NetAddress& addr)
		{
			Close();
			m_udpClient->Open();
			if(m_udpClient->Connect(addr))
			{
				GhostNetPacketConnect pack;
				pack.writeData(&addr,m_udpClient);
				m_thread=OS::IThreadManager::getInstance().createThread(new GhostProxyUDPThread(this));
				m_thread->start(0);
				m_isMaster=false;
				return true;
			}
			return false;
		}
		bool IsConnected()
		{
			return m_udpClient->IsConnected() || m_udpClient->IsOpen();
		}

		bool Close()
		{
			if(m_udpClient->IsConnected())
			{
				m_udpClient->Disconnect();
			}
			if(m_udpClient->IsOpen())
			{
				m_udpClient->Close();
			}
			if(m_thread)
			{
				OS::IThreadManager::getInstance().killThread(m_thread);
				delete m_thread;
				m_thread=0;
			}
			return false;
		}

		bool IsMaster()
		{
			return m_isMaster && m_udpClient->IsOpen();
		}

		bool AddMasterObject(uint ID,physics::IPhysicalNode* node)
		{
			if(m_masterNodesMap.find(ID)!=m_masterNodesMap.end())
				return false;
			NodeInfo ifo;
			ifo.ID=ID;
			ifo.node=node;
			m_masterNodesMap[ID]=m_masterNodes.size();
			m_masterNodes.push_back(ifo);
			return true;
		}
		bool RemoveMasterObject(uint ID)
		{
			std::map<uint,uint>::iterator it= m_masterNodesMap.find(ID);
			if(it==m_masterNodesMap.end())
				return false;
			m_masterNodes[it->second].ID=-1;
			m_masterNodes[it->second].node=0;
			m_masterNodesMap.erase(it);
			return true;
		}

		bool AttachGhostObject(uint ID,scene::IMovable* node)
		{
			ObjectInfo obj;
			obj.masterID=ID;
			obj.node=node;
			m_objects.push_back(obj);
			return true;
		}
		bool RemoveGhostObject(scene::IMovable* node)
		{
			std::vector<ObjectInfo>::iterator it= m_objects.begin();
			for(;it!=m_objects.end();++it)
			{
				if((*it).node==node)
				{
					m_objects.erase(it);
					return true;
				}
			}
			return false;
		}

		void Clear()
		{
			m_objects.clear();
			m_masterNodesMap.clear();
			m_masterNodes.clear();
		}

		bool Update()
		{
			if(IsMaster())
			{
				GhostNetPacketObjects packet;
				GhostNetPacketObjects::DataPacket dp;
				for(int i=0;i<m_masterNodes.size();++i)
				{
					if(m_masterNodes[i].node==0)
						continue;
					dp.ID=m_masterNodes[i].ID;
					dp.pos=m_masterNodes[i].node->getGlobalPosition();
					dp.ori=m_masterNodes[i].node->getGlobalOrintation();
					packet.data.push_back(dp);
				}
				if(packet.data.size()>0)
				{
					for(int i=0;i<m_connectedClients.size();++i)
						packet.writeData(&m_connectedClients[i].address,m_udpClient);
				}
			}else
			{
				DataPacketMap dpm;
				
				m_dataMutex->lock();
				dpm=m_dataPackets;
				m_dataPackets.clear();
				m_dataMutex->unlock();

				DataPacketMap::iterator it=dpm.begin();
				for (;it!=dpm.end();++it)
				{
					IndexMap::iterator obIt= m_objectsMap.find(it->first);
					if(obIt!=m_objectsMap.end())
					{
						if(m_objects[obIt->second].node!=0)
						{
							m_objects[obIt->second].node->setPosition(it->second.pos);
							m_objects[obIt->second].node->setOrintation(it->second.ori);
						}
					}
				}

			}
			return true;
		}

		void ReceiveDataPackets(GhostNetPacketObjects& pack)
		{
			OS::ScopedLock lock(m_dataMutex);
			for(int i=0;i<pack.data.size();++i)
			{
				m_dataPackets[pack.data[i].ID]=pack.data[i];
			}
		}

		void ClientConnected(GhostNetPacketConnect& obj)
		{
			ClientInfo ifo;
			ifo.address=obj.addr;
			m_connectedClients.push_back(ifo);
			gLogManager.log("GhostManager- Client Connected:"+obj.addr.toString(),ELL_INFO);
		}
	};


	
	void GhostProxyUDPThread::execute(OS::IThread*caller,void*arg)
	{
		char buffer[UDPBufferSize];
		uint len;
		network::NetAddress addr;
		while(caller->isActive())
		{
			if(!m_owner->IsConnected())
			{
				OS::IThreadManager::getInstance().sleep(100);
				continue;
			}
			len=UDPBufferSize;
			if (m_owner->GetClient()->RecvFrom(buffer, &len, &addr, 0) == network::UDP_SOCKET_ERROR_NONE)
			{
				int packet=core::BitConverter::ToInt32((byte*)buffer);
				switch(packet)
				{
				case GhostNet_Objects:
					{
						GhostNetPacketObjects obj;
						obj.readPacket(m_owner->GetClient(),&addr);
					}
				case GhostNet_Connect:
					{
						GhostNetPacketConnect obj;
						obj.readPacket(m_owner->GetClient(),&addr);
						m_owner->ClientConnected(obj);
					}
				}
			}
		}
	}


	GhostProxyManager::GhostProxyManager()
	{
		m_impl=new GhostProxyManagerImpl();
	}
	GhostProxyManager::~GhostProxyManager()
	{
		delete m_impl;
	}

	void GhostProxyManager::OpenPort(int port)
	{
		if(m_impl->OpenPort(port))
		{
		}
	}
	void GhostProxyManager::Connect(const network::NetAddress& addr)
	{
		if(m_impl->Connect(addr))
		{
			FIRE_LISTENR_METHOD(OnConnected,(this,addr));
		}
	}

	void GhostProxyManager::Connect(const core::string& ip,int port)
	{
		network::NetAddress addr(ip,port);
		if(m_impl->Connect(addr))
		{
			FIRE_LISTENR_METHOD(OnConnected,(this,addr));
		}
	}
	void GhostProxyManager::Close()
	{
		if(m_impl->Close())
		{
			FIRE_LISTENR_METHOD(OnDisconnected,(this));
		}
	}

	bool GhostProxyManager::IsMaster()
	{
		return m_impl->IsMaster();
	}

	void GhostProxyManager::AddMasterObject(uint ID,physics::IPhysicalNode* node)
	{
		m_impl->AddMasterObject(ID,node);
	}
	void GhostProxyManager::RemoveMasterObject(uint ID)
	{
		m_impl->RemoveMasterObject(ID);
	}

	void GhostProxyManager::AttachGhostObject(uint ID,scene::IMovable* node)
	{
		m_impl->AttachGhostObject(ID,node);
	}
	void GhostProxyManager::RemoveGhostObject(scene::IMovable* node)
	{
		m_impl->RemoveGhostObject(node);
	}
	void GhostProxyManager::Clear()
	{
		m_impl->Clear();
	}

	void GhostProxyManager::Update()
	{
		m_impl->Update();
	}
}
}