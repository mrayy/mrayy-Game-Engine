
#include "stdafx.h"
#include "OmniBaseController.h"
#include "StringUtil.h"


namespace mray
{

OmniBaseController::OmniBaseController()
{
	comROBOT = 0;
	connected = false;
}

OmniBaseController::~OmniBaseController()
{
	Disconnect();
	delete comROBOT;
}


void OmniBaseController::sendCommand(const core::string& cmd)
{
	comROBOT->sendData((char*)cmd.c_str(), cmd.length());
}
void OmniBaseController::_onSerialData(int size, char *buffer)
{
	char* ptr = buffer;
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
	std::vector<core::string> lst= core::StringUtil::Split(ptr, " ");
	if (lst.size() == 0)
		return;
	if (lst[0] == "dist")
	{
		int sensorsCount = core::StringConverter::toInt(lst[1]);
		m_sensors.resize(sensorsCount);
		for (int i = 0; i < sensorsCount; ++i)
		{
			m_sensors[i] = core::StringConverter::toFloat(lst[2+i]);
		}
	}
}


void robot_SerialEventManager(uint32 object, uint32 event){
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
			((OmniBaseController*)com->owner)->_onSerialData(size, buffer);
			com->dataHasBeenRead();
			break;
		}
	}


}

bool OmniBaseController::Connect(const core::string& port)
{
	Disconnect();

	comROBOT = new Tserial_event();
	comROBOT->owner = this;
	comROBOT->setManager(robot_SerialEventManager);
	connected=comROBOT->connect((char*)port.c_str(), 115200, SERIAL_PARITY_ODD, 8, FALSE, TRUE)==0;
	if (!comROBOT->isconnected())
	{
		printf("Failed to connect robot\n");
		delete comROBOT;
		comROBOT = 0;
	}
	else
	{
		comROBOT->setRxSize(15);

		sendCommand("#ed\r\n");
	}
	return comROBOT->isconnected();
}

bool OmniBaseController::IsConnected()
{
	return comROBOT!=0;
}

void OmniBaseController::Disconnect()
{
	if (!comROBOT)
		return;

	Drive(0, 0);
	comROBOT->disconnect();
	connected = FALSE;
	delete comROBOT;
	comROBOT = 0;
}


void OmniBaseController::Drive(const math::vector2di& speed, int rotationSpeed)
{
	if (!comROBOT)
		return;
	int packet_size;
	char sCommand[128];
	sprintf_s(sCommand, 128, "#d %d %d %d\r\n", speed.x , speed.y , rotationSpeed );
// 	
	sendCommand(sCommand);

}
void OmniBaseController::DriveStop()
{
	if (!comROBOT)
		return;

	sendCommand("#q\r\n");
}

void OmniBaseController::UpdateSensors()
{
}

int  OmniBaseController::GetSensorCount()
{
	return m_sensors.size();
}

float OmniBaseController::GetSensorValue(int s)
{
	if (s >= m_sensors.size())
		return 0;
	return m_sensors[s];
}

int  OmniBaseController::GetBatteryLevel()
{
	return 100;
}

}


