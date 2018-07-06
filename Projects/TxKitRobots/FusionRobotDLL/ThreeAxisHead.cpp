

#include "stdafx.h"
#include "ThreeAxisHead.h"
#include "StringUtil.h"




namespace mray
{


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
	connected = comROBOT->connect((char*)port.c_str(), 115200, SERIAL_PARITY_NONE, 8, FALSE, FALSE) == 0;
	if (!comROBOT->isconnected())
	{
	//	printf("Failed to connect robot\n");
		delete comROBOT;
		comROBOT = 0;
	}
	else
	{
		comROBOT->owner = this;
		comROBOT->setRxSize(10);
		_sendCommand("sa");//disable angle logging
		_sendCommand("es");//enable stabilization
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
	_sendCommand("q");

	SetRotation(0);
	comROBOT->disconnect();
	connected = FALSE;
	delete comROBOT;
	comROBOT = 0;
}

void ThreeAxisHead::_sendCommand(const std::string& cmd)
{
	std::string str = "@"+cmd + "#";
	comROBOT->sendData((char*)str.c_str(), str.length());
}
void ThreeAxisHead::SetRotation(const math::vector3d& rotation)
{
	if (!comROBOT)
		return;
	int packet_size;
	char sCommand[128];
	sprintf_s(sCommand, 128, "d,%d,%d,%d", (int)(rotation.z * 100), (int)(rotation.y * 100), (int)(rotation.x * 100));
	_sendCommand(sCommand);
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
		if (_buffer[i] == '\r')
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
		m_rotation.y = core::StringConverter::toFloat(lst[0]);
		m_rotation.x = core::StringConverter::toFloat(lst[1]);
		m_rotation.z = core::StringConverter::toFloat(lst[2]);
	}
}

}
