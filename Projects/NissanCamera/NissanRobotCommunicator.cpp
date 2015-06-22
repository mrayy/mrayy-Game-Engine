
#include "stdafx.h"
#include "NissanRobotCommunicator.h"

#include "NissanRobotDLL.h"
//#include "IRobotController.h"
#include "StringUtil.h"


namespace mray
{
namespace NCam
{
	
	class CQuatAverageWindow
	{
		std::vector<math::vector3d> values;
		int maxLen;
	public:

		CQuatAverageWindow()
		{
			maxLen = 3;
		}
		void SetLength(int len)
		{
			maxLen = len;
		}

		math::vector3d AddValue(math::vector3d q)
		{
			values.push_back(q);
			if (values.size() > maxLen)
				values.erase(values.begin());

			math::vector3d res;
			for (int i = 0; i < values.size(); ++i)
				res += values[i];
			return res / (float)values.size();
		}

	};
	class CDebugRobot
	{

		OS::IStreamPtr m_targetFile;
		OS::StreamWriter m_targetWrtr;

		math::vector3d targetValue;
		math::vector3d currentValue;
	public:

		CDebugRobot()
		{
			m_targetFile = gFileSystem.createTextFileWriter("TargetAngles.xls");

			m_targetWrtr.setStream(m_targetFile);
		}

		~CDebugRobot()
		{
			m_targetFile->close();
		}

		void Output()
		{
			std::stringstream ss;
			float t = gEngine.getTimer()->getSeconds();
			ss << t << "\t" << targetValue.x << "\t" << targetValue.y << "\t" << targetValue.z 
				<< "\t" << currentValue.x << "\t" << currentValue.y << "\t" << currentValue.z << "\n";;
			m_targetWrtr.writeString(ss.str());
		}

		void TargetAngles(math::vector3d a)
		{
			targetValue = a;
			Output();
		}

		void CurrentAngles(math::vector3d a)
		{
			currentValue = a;
			Output();
		}
	};

class NissanRobotCommunicatorImpl:public ITelubeeRobotListener
{
protected:
	CNissanRobotDLL* m_robot;
	RobotStatus m_robotStatus;

	typedef std::map<core::string, core::string> DataMap;
	DataMap m_recvData;

	CQuatAverageWindow m_headRotation;


	CDebugRobot m_debugger;

public:
	NissanRobotCommunicatorImpl()
	{
		m_robot = new CNissanRobotDLL();
		m_robot->GetRobotController()->SetListener(this);

	}
	virtual~NissanRobotCommunicatorImpl()
	{
		delete m_robot;
	}

	virtual void OnRotationArrived(float z, float y, float x)
	{
		math::vector3d a=m_headRotation.AddValue(math::vector3d(x,-y,-z));
		//printf("c: %s\n", core::StringConverter::toString(m_headRotation).c_str());

		m_debugger.CurrentAngles(a);
		m_recvData["HeadRotation"] = core::StringConverter::toString(a);
	}

	bool IsHoming()
	{
		return m_robotStatus.homing;
	}
	virtual bool Connect(const core::string& ip, int port)
	{
		m_robotStatus.connected = true;
		return true;
	}

	virtual void Disconnect()
	{
		m_robotStatus.connected = false;
	}
	virtual bool IsConnected()
	{
		return m_robotStatus.connected;
	}

	virtual void SetUserID(const core::string& userID)
	{
	}
	virtual void ConnectUser(bool c)
	{
	}
	virtual void ConnectRobot(bool c)
	{
		m_robotStatus.connected = c;
		if (m_robotStatus.connected)
			m_robot->GetRobotController()->ConnectRobot();
		else
			m_robot->GetRobotController()->DisconnectRobot();
	}

	virtual void Update(float dt)
	{
	}

	virtual void LoadFromXml(xml::XMLElement* e)
	{
	}

	core::string GetData(const core::string& key)
	{
		DataMap::iterator it= m_recvData.find(key);
		if (it == m_recvData.end())
			return "";
		return it->second;
	}


	void SetData(const core::string &name, const core::string &value, bool statusData)
	{
		std::vector<core::string> vals;
		vals = core::StringUtil::Split(value, ",");
		if (name == "Speed" && vals.size() == 2)
		{
			m_robotStatus.speedX = atof(vals[0].c_str());
			m_robotStatus.speedY = atof(vals[1].c_str());
			//limit the speed
			m_robotStatus.speedX = math::clamp<float>(m_robotStatus.speedX, -1, 1);
			m_robotStatus.speedY = math::clamp<float>(m_robotStatus.speedY, -1, 1);
		}
		else if (name == "HeadRotation" && vals.size() == 4)
		{

			math::quaternion q;
			q.w = atof(vals[0].c_str());
			q.x = atof(vals[1].c_str());
			q.y = atof(vals[2].c_str());
			q.z = atof(vals[3].c_str());

			math::vector3d angles;
			q.toEulerAngles(angles);


			m_debugger.TargetAngles(angles);

			m_robotStatus.tilt = angles.x;
			m_robotStatus.yaw = angles.y;
			m_robotStatus.roll = angles.z;

			//do head limits
			m_robotStatus.tilt = math::clamp(m_robotStatus.tilt, -80.0f, 80.0f);
			m_robotStatus.yaw = math::clamp<float>(m_robotStatus.yaw, -80.0f, 80.0f);
			m_robotStatus.roll = math::clamp(m_robotStatus.roll, -80.0f, 80.0f);
		}
		else if (name == "HeadPosition" && vals.size() == 3)
		{	
			m_robotStatus.X = atof(vals[0].c_str());
			m_robotStatus.Y = atof(vals[1].c_str());
			m_robotStatus.Z = atof(vals[2].c_str());

		}
		else if (name == "Rotation" && vals.size() == 1)
		{
			m_robotStatus.rotation = atof(vals[0].c_str());
			m_robotStatus.rotation = math::clamp<float>(m_robotStatus.rotation, -1, 1);
		}
		else if (name == "Connect" && vals.size() == 3)
		{
			int videoPort = atoi(vals[1].c_str());
			int audioPort = atoi(vals[2].c_str());
			network::NetAddress addr = network::NetAddress(vals[0], videoPort);
			//if (addr.address != m_userStatus.address.address || addr.port!=m_userStatus.address.port)
			m_robotStatus.connected = true;
		}
		else if (name == "Disconnect" && vals.size() == 2)
		{
			m_robotStatus.connected = false;
		}
		else if (name == "Homing" )
		{
			m_robotStatus.homing = true;
		}

		m_robot->GetRobotController()-> UpdateRobotStatus(m_robotStatus);
	}
	void RemoveData(const core::string &key)
	{
	}
	void ClearData(bool statusValues)
	{
	}
};



NissanRobotCommunicator::NissanRobotCommunicator()
{
	m_impl = new NissanRobotCommunicatorImpl();
}
NissanRobotCommunicator::~NissanRobotCommunicator()
{
	delete m_impl;
}


bool NissanRobotCommunicator::Connect(const core::string& ip, int port)
{
	return m_impl->Connect(ip, port);
}
void NissanRobotCommunicator::Disconnect()
{
	m_impl->Disconnect();
}
bool NissanRobotCommunicator::IsConnected()
{
	return m_impl->IsConnected();
}

void NissanRobotCommunicator::SetUserID(const core::string& userID)
{
	m_impl->SetUserID(userID);
}
void NissanRobotCommunicator::ConnectUser(bool c)
{
	m_impl->ConnectUser(c);
}
void NissanRobotCommunicator::ConnectRobot(bool c)
{
	m_impl->ConnectRobot(c);
}

void NissanRobotCommunicator::Update(float dt)
{
	m_impl->Update(dt);

}

void NissanRobotCommunicator::LoadFromXml(xml::XMLElement* e)
{
	m_impl->LoadFromXml(e);
}

core::string NissanRobotCommunicator::GetData(const core::string& key)
{
	return m_impl->GetData(key);
}

void NissanRobotCommunicator::SetData(const core::string &key, const core::string &value, bool statusData)
{
	m_impl->SetData(key, value, statusData);
}
void NissanRobotCommunicator::RemoveData(const core::string &key)
{
	m_impl->RemoveData(key);
}
void NissanRobotCommunicator::ClearData(bool statusValues)
{
	m_impl->ClearData(statusValues);
}
bool NissanRobotCommunicator::IsHoming()
{
	return m_impl->IsHoming();
}
}
}

