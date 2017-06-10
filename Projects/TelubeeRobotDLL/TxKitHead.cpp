

#include "stdafx.h"
#include "TxKitHead.h"
#include "StringUtil.h"

#include <windowsx.h>


namespace mray
{

	const int ServoCODE[] = {
		0x3,
		0x1,
		0x2
	};



TxKitHead::TxKitHead()
{
	connected = false;
	m_serial = 0;
}
TxKitHead::~TxKitHead()
{
	Disconnect();
	if (m_serial)
		delete m_serial;
}


bool TxKitHead::Connect(const core::string& port)
{
	Disconnect();
	gLogManager.log("Connecting", ELL_INFO);
	m_serial = new serial::Serial(port, 115200, serial::Timeout::simpleTimeout(1000),serial::eightbits,serial::parity_even);
	connected = m_serial->isOpen();
	if (!connected)
	{
	//	printf("Failed to connect robot\n");
		delete m_serial;
		m_serial = 0;
	}
	else
	{
		gLogManager.log("Connected", ELL_INFO);
		//comROBOT->setRxSize(15);
		//_sendCommand("#ea");//enable angle logging
	}
	//comROBOT->owner = this;
	return connected;
}
bool TxKitHead::IsConnected()
{
	return connected;
}
void TxKitHead::Disconnect()
{
	if (!m_serial)
		return;

	SetRotation(0);
	_sleep(100);

	char sCommand[] = { 0, 0, 0 };
	for (int i = 0; i < 3; ++i){

		sCommand[0] = 0x80 | ServoCODE[i];//(PosCtrlCMD << 5)
		sCommand[1] = 0;
		sCommand[2] = 0;
		_sendCommand(sCommand, 3 * sizeof(char));
		//	printf("%d,%d,%d,%d\n", value,(int)sCommand[0], (int)sCommand[1], (int)sCommand[2]);
	}

	m_serial->close();
	connected = false;
	delete m_serial;
	m_serial = 0;
}

void TxKitHead::_sendCommand(const char* cmd, int len)
{
	m_serial->flushInput();
	m_serial->flushOutput();
	//comROBOT->sendData((char*)cmd, len);
	m_serial->write((const uint8_t*) cmd, len);
	/*
	uint8_t buf[9];
	int ret=m_serial->read(buf, 6);
	if (ret != 6)
		gLogManager.log("Failed to read 6 bytes: "+core::StringConverter::toString(ret), ELL_INFO);*/
	//_sleep(1);
}

void TxKitHead::SetRotation(const math::vector3d& rotation)
{
	float rot[3];
	rot[0] = 0.5f + math::clamp(rotation.x, -18.0f, 24.0f) / 270.0f; //pitch
	rot[1] = 0.5f + math::clamp(rotation.y, -90.0f, 90.0f) / 270.0f; //yaw
	rot[2] = 0.5f + math::clamp(rotation.z, -20.0f, 20.0f) / 270.0f; //roll

	const int MinValue = 3500;
	const int MaxValue = 11500;

	const int PosCtrlCMD = 0x4;//0b100


	const char bitMask = 0x7F;

	char sCommand[3 ];
	for (int i = 0; i < 3; ++i){
		rot[i] = MinValue + rot[i] * (MaxValue - MinValue);
		unsigned short value = rot[i];

		sCommand[ 0] = 0x80 | ServoCODE[i];//(PosCtrlCMD << 5)
		sCommand[1] = (value >> 7) & bitMask;
		sCommand[ 2] = value & bitMask;
		_sendCommand(sCommand, 3 * sizeof(char));
	//	printf("%d,%d,%d,%d\n", value,(int)sCommand[0], (int)sCommand[1], (int)sCommand[2]);
	}



	m_rotation = rotation;
}
math::vector3d TxKitHead::GetRotation()
{
	return m_rotation;
}

void TxKitHead::_onSerialData(int size, char *buffer)
{
	return;
	char* ptr = buffer;
	buffer[size - 1] = 0;
	while (*ptr)
	{
		if (*ptr == '@')
		{
			++ptr;
			break;
		}
		++ptr;
	}
	if (!*ptr)
		return;
	std::vector<core::string> lst = core::StringUtil::Split(ptr, " ");
	if (lst.size() == 0)
		return;
	if (lst[0] == "angles ")//angles
	{
		m_rotation.x = core::StringConverter::toFloat(lst[1]);
		m_rotation.y = core::StringConverter::toFloat(lst[2]);
		m_rotation.z = core::StringConverter::toFloat(lst[3]);
	}
}

}
