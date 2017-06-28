

#include "stdafx.h"
#include "ThreeAxisHead.h"
#include "StringUtil.h"




namespace mray
{


	void head_SerialEventManager(uint32 object, uint32 event){
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


	}

ThreeAxisHead::ThreeAxisHead()
{
	connected = false;
	comROBOT = 0;
}
ThreeAxisHead::~ThreeAxisHead()
{
	Disconnect();
	delete comROBOT;
}


bool ThreeAxisHead::Connect(const core::string& port)
{
	Disconnect();

	comROBOT = new Tserial_event();
	comROBOT->setManager(head_SerialEventManager);
	connected = comROBOT->connect((char*)port.c_str(), 115200, SERIAL_PARITY_ODD, 8, FALSE, TRUE) == 0;
	if (!comROBOT->isconnected())
	{
	//	printf("Failed to connect robot\n");
		delete comROBOT;
		comROBOT = 0;
	}
	else
	{
		comROBOT->setRxSize(15);
		//_sendCommand("#ea");//enable angle logging
	}
	return comROBOT && comROBOT->isconnected();
}
bool ThreeAxisHead::IsConnected()
{
	return comROBOT != 0;
}
void ThreeAxisHead::Disconnect()
{
	if (!comROBOT)
		return;
	_sendCommand("#q");

	SetRotation(0);
	comROBOT->disconnect();
	connected = FALSE;
	delete comROBOT;
	comROBOT = 0;
}

void ThreeAxisHead::_sendCommand(const std::string& cmd)
{
	std::string str = cmd + " \n\r";
	comROBOT->sendData((char*)str.c_str(), str.length());
}
void ThreeAxisHead::SetRotation(const math::vector3d& rotation)
{
	if (!comROBOT)
		return;
	int packet_size;
	char sCommand[128];
	sprintf_s(sCommand, 128, "#d %d %d %d", (int)(rotation.y * 100), (int)(rotation.x * 100), (int)(rotation.z * 100));
	_sendCommand(sCommand);
	m_rotation = rotation;
}
math::vector3d ThreeAxisHead::GetRotation()
{
	return m_rotation;
}

void ThreeAxisHead::_onSerialData(int size, char *buffer)
{
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
