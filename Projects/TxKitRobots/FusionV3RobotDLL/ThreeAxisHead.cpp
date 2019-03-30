

#include "stdafx.h"
#include "ThreeAxisHead.h"
#include "StringUtil.h"




namespace mray
{

	/*
	void head_SerialEventManager(Tserial_event * object, uint32 event){
		char *buffer;
		int   size;
		Tserial_event *com;

		com = (Tserial_event *)object;
		if (com != 0){
			switch (event){
			case  SERIAL_CONNECTED:
				//printf("Com Port Connected! \n");
				break;
			case  SERIAL_DATA_ARRIVAL:
				size = com->getDataInSize();
				buffer = com->getDataInBuffer();
				((ThreeAxisHead*)com->owner)->_onSerialData(size, buffer);
				com->dataHasBeenRead();
				break;
			}
		}


	}*/

ThreeAxisHead::ThreeAxisHead()
{
	connected = false;
	m_serial = 0;
}
ThreeAxisHead::~ThreeAxisHead()
{
	Disconnect();
	delete m_serial;
}

DWORD timerThreadRobot(ThreeAxisHead *robot, LPVOID pdata) {
	int count = 0;
	while (robot->IsConnected()) {
		robot->CheckSerial();
		Sleep(10);
	}
	return 0;
}

bool ThreeAxisHead::Connect(const core::string& port,bool enableAngleLog)
{
	Disconnect();

	m_serial = new serial::Serial(port, 115200);// , serial::Timeout::simpleTimeout(20), serial::eightbits, serial::parity_none, serial::stopbits_one);
	//m_serial->open();
	if (!m_serial->isOpen())
	{
		gLogManager.log("Failed to start head!", ELL_WARNING);
		connected = false;
	//	printf("Failed to connect robot\n");
		delete m_serial;
		m_serial = 0;
	}
	else
	{
		_sendCommand("q");//disable angle logging
		Sleep(50);
		connected = true;
		enableAngleLog = false;
		if (enableAngleLog)
			_sendCommand("ea");//disable angle logging
		else
			_sendCommand("sa");
		_sendCommand("es");//enable stabilization
		if(enableAngleLog)
			m_robotThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)timerThreadRobot, this, NULL, NULL);
		else m_robotThread = 0;
		gLogManager.log("Head Started!", ELL_SUCCESS);

	}
	return m_serial && m_serial->isOpen();
}
bool ThreeAxisHead::IsConnected()
{
	return connected && m_serial != 0 && m_serial->isOpen();
}
void ThreeAxisHead::Disconnect()
{
	if (!m_serial)
		return;
	connected = false;
	if (m_robotThread != 0)
	{
		TerminateThread(m_robotThread, 0);
		Sleep(100);
		m_robotThread = 0;
	}
	SetRotation(0);
	Sleep(100);
	_sendCommand("q");
	Sleep(100);

	if (m_serial != 0)
	{
		m_serial->close();
		delete m_serial;
		m_serial = 0;
	}
}

void ThreeAxisHead::CheckSerial()
{
	std::string str;
// 	if (m_serial->available() == 0)
// 		return;
	_rcvbuffer += m_serial->readline();// m_serial->available());
	std::size_t index = 0;
	bool found = false;
	do {
		index = _rcvbuffer.find_first_of('\n');
		if (index != std::string::npos)
		{
			str = _rcvbuffer.substr(0, index + 1);
			_rcvbuffer = _rcvbuffer.substr(index + 1);
			found = true;
		}
	} while (index != std::string::npos);
	if(found)
		_onSerialData(str.length(), &str[0]);
}
void ThreeAxisHead::_sendCommand(const std::string& cmd)
{
	std::string str = "@"+cmd + "#";
	m_serial->write(str);
}
void ThreeAxisHead::SetRotation(const math::vector3d& rotation)
{
	if (!m_serial)
		return;
	int packet_size;
	char sCommand[128];
	math::vector3d rot=rotation;
	rot.x= math::clamp(rotation.x, -38.0f, 38.0f);
	sprintf_s(sCommand, 128, "d,%d,%d,%d", (int)(rot.z * 100), (int)(rot.y * 100), (int)(rot.x * 100));
	_sendCommand(sCommand);

	//CheckSerial();
	//m_rotation = rotation;
}
math::vector3d ThreeAxisHead::GetRotation()
{
	return m_rotation;
}

void ThreeAxisHead::_onSerialData(int size, char *buffer)
{
	char* ptr = buffer;
	buffer[size ] = 0;
	while (*ptr)
	{
		_buffer+=*ptr;
		++ptr;
	}

	char data[50];
	int cnt = 0;
	int idx = 0;
	int i = 0;
	for (i = 0; i < _buffer.size();++i)
	{	

		if (_buffer[i] == '@')
		{
			if (cnt == 0)
			{
				cnt = 1;
			}
			continue;
		}
		if (cnt == 0)
			continue;
		if (_buffer[i] == '\n')
		{
			if(cnt==1){
				cnt = 2;
				break;
			}
			continue;
		}
		data[idx] = _buffer[i];
		++idx;
	}
	if (cnt!=2)
		return;
	data[idx] = 0;
	_buffer = _buffer.substr(i, _buffer.size() - i );
	std::vector<core::string> lst = core::StringUtil::Split(data, " ");
	if (lst.size() != 2)
		return;
	if (lst[0] == "ang")//angles
	{
		lst = core::StringUtil::Split(lst[1], ",");
		m_rotation.z = -core::StringConverter::toFloat(lst[0]);
		m_rotation.y = -core::StringConverter::toFloat(lst[1]);
		m_rotation.x = -core::StringConverter::toFloat(lst[2]);
	}
}

}
