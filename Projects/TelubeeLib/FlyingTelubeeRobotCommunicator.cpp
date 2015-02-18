

#include "stdafx.h"
#include "FlyingTelubeeRobotCommunicator.h"
#include "shmem.h"
#include "IUDPClient.h"
#include "INetwork.h"
#include "IThreadFunction.h"
#include "IThread.h"
#include "IThreadManager.h"
#include "StringUtil.h"

namespace mray
{
	namespace TBee
	{
#define USE_UDPCLIENT
//#define USE_OPTI
		class FlyingTelubeeRobotCommunicatorData;
		class FlyingTelubeeRobotCommunicatorThread :public OS::IThreadFunction
		{
			FlyingTelubeeRobotCommunicatorData* m_owner;
		public:
			FlyingTelubeeRobotCommunicatorThread(FlyingTelubeeRobotCommunicatorData* o)
			{
				m_owner = o;
			}
			virtual void execute(OS::IThread*caller, void*arg);
		};
		struct FlyingTelubeeStatus
		{
			math::vector3d head;
			math::vector2d speed;
			bool connected;
		};
		class FlyingTelubeeRobotCommunicatorData
		{
		public:
			FlyingTelubeeStatus data;
			FlyingTelubeeRobotCommunicator* owner;
			network::IUDPClient* client;
			network::NetAddress addr;
			OS::IMutex* dataMutex;
			OS::IThread* thread;
			bool connected;
			struct DataInfo
			{
				core::string value;
				bool statusData;
			};
			std::map<core::string, DataInfo> values;

			core::string outputValues;

			FlyingTelubeeRobotCommunicatorData(FlyingTelubeeRobotCommunicator* o)
			{
				owner = o;
				connected = false;
				client = network::INetwork::getInstance().createUDPClient();
				client->Open();

				dataMutex = OS::IThreadManager::getInstance().createMutex();
				memset(&data, 0, sizeof(data));
			}
			~FlyingTelubeeRobotCommunicatorData()
			{
				if (client)
				{
					client->Close();
					delete client;
					client = 0;
				}
				delete dataMutex;
			}
			void CleanData(bool statusValues)
			{
				dataMutex->lock();
				if (statusValues == true)
					values.clear();
				else
				{
					std::map<core::string, DataInfo>::iterator it = values.begin();
					std::map<core::string, DataInfo>::iterator it2;
					for (; it != values.end();)
					{
						it2 = it;
						it2++;
						if (!it->second.statusData)
						{
							values.erase(it);
						}
						it = it2;
					}

				}
				dataMutex->unlock();
			}
			void SetData(const core::string &name, const core::string &value, bool statusData)
			{
				DataInfo di;
				di.value = value;
				di.statusData = statusData;
				dataMutex->lock();

				std::vector<core::string> vals;
				vals = core::StringUtil::Split(value, ",");
				if (name == "Speed" && vals.size() == 2)
				{
					data.speed.x = atof(vals[0].c_str());
					data.speed.y = atof(vals[1].c_str());
					//limit the speed
					data.speed.x = math::clamp<float>(data.speed.x, -1, 1);
					data.speed.y = math::clamp<float>(data.speed.y, -1, 1);
				}
				else if (name == "HeadRotation" && vals.size() == 3)
				{
					data.head.x = atof(vals[0].c_str());
					data.head.y = atof(vals[1].c_str());
					data.head.z = atof(vals[2].c_str());

				}
				else if (name == "HeadPosition" && vals.size() == 3)
				{

				}
				else if (name == "Rotation" && vals.size() == 1)
				{
				}
				else if (name == "Connect" && vals.size() == 3)
				{
					int videoPort = atoi(vals[1].c_str());
					int audioPort = atoi(vals[2].c_str());
					network::NetAddress addr = network::NetAddress(vals[0], videoPort);
				}
				else if (name == "Disconnect" && vals.size() == 2)
				{
				}
				outputValues = core::StringConverter::toString(data.head.x) + "," + core::StringConverter::toString(data.head.y) + "," +
					core::StringConverter::toString(data.head.z)+"\n";
				dataMutex->unlock();
			}
		};


		void FlyingTelubeeRobotCommunicatorThread::execute(OS::IThread*caller, void*arg)
		{
			while (caller->isActive())
			{
				if (m_owner->connected)
				{
					m_owner->owner->Update(0);
				}
				if (m_owner->data.connected && m_owner->connected)
				{
					OS::IThreadManager::getInstance().sleep(33);
				}
				else
				{
					OS::IThreadManager::getInstance().sleep(100);
				}
			}
		}
		FlyingTelubeeRobotCommunicator::FlyingTelubeeRobotCommunicator()
		{
			m_data = new FlyingTelubeeRobotCommunicatorData(this);
			m_data->thread = OS::IThreadManager::getInstance().createThread(new FlyingTelubeeRobotCommunicatorThread(m_data));
			m_data->thread->start(0);
		}
		FlyingTelubeeRobotCommunicator::~FlyingTelubeeRobotCommunicator()
		{
			OS::IThreadManager::getInstance().killThread(m_data->thread);
			delete m_data;
		}

		bool FlyingTelubeeRobotCommunicator::Connect(const core::string& ip, int port)
		{
			if (m_data->connected)
				Disconnect();
			bool res = false;
			m_data->addr = network::NetAddress(ip, port);
			if (m_data->client->Connect(m_data->addr))
			{
				m_data->connected = true;
				res = true;
			}

			return res;
		}
		void FlyingTelubeeRobotCommunicator::Disconnect()
		{
			if (!m_data->connected)
				return;
			m_data->client->Disconnect();
			m_data->connected = false;
		}
		bool FlyingTelubeeRobotCommunicator::IsConnected()
		{
			return m_data->connected;
		}

		void FlyingTelubeeRobotCommunicator::SetUserID(const core::string& userID)
		{
		}

		void FlyingTelubeeRobotCommunicator::ConnectUser(bool c)
		{
			Update(0);
		}
		void FlyingTelubeeRobotCommunicator::ConnectRobot(bool c)
		{
			m_data->data.connected = c;
			Update(0);
		}

		void FlyingTelubeeRobotCommunicator::SetData(const core::string &key, const core::string &value, bool statusData)
		{
			m_data->SetData(key, value, statusData);
		}
		void FlyingTelubeeRobotCommunicator::RemoveData(const core::string &key)
		{
			m_data->dataMutex->lock();
			std::map<core::string, FlyingTelubeeRobotCommunicatorData::DataInfo>::iterator it = m_data->values.find(key);
			if (m_data->values.end() != it)
			{
				m_data->values.erase(it);
			}
			m_data->dataMutex->unlock();
		}
		void FlyingTelubeeRobotCommunicator::ClearData(bool statusValues)
		{
			m_data->CleanData(statusValues);
		}
		void FlyingTelubeeRobotCommunicator::Update(float dt)
		{
			//return ;
			if (!m_data->connected)
				return;

			//construct the xml data fields


			m_data->dataMutex->lock();
			core::string data = m_data->outputValues;
			m_data->dataMutex->unlock();
			m_data->client->SendTo(&m_data->addr, data.c_str(), data.length() + 1);
			m_data->CleanData(false);
		}

		void FlyingTelubeeRobotCommunicator::LoadFromXml(xml::XMLElement* e)
		{
		}

	}
}



