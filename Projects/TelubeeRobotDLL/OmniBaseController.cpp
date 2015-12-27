
#include "stdafx.h"
#include "OmniBaseController.h"
#include "StringUtil.h"
#include "IThreadManager.h"


namespace mray
{

OmniBaseController::OmniBaseController()
{
	comPort= 0;
	connected = false;

	m_dataMutex = OS::IThreadManager::getInstance().createMutex();
}

OmniBaseController::~OmniBaseController()
{
	Disconnect();
	//delete comPort;
	delete m_dataMutex;
}


void OmniBaseController::_ProcessBuffer()
{
	if (m_buffer.size() == 0)
		return;
	char* ptr = &m_buffer[0];

	while (*ptr)
	{
		if (*ptr == '@')
		{
			++ptr;
			break;
		}
	}
	if (!*ptr)
		return;
	std::vector<core::string> lst = core::StringUtil::Split(ptr, " \t");
	if (lst.size() == 0)
		return;
	if (lst[0] == "dist")
	{
		int sensorsCount = core::StringConverter::toInt(lst[1]);
		m_sensors.resize(sensorsCount);
		for (int i = 0; i < sensorsCount; ++i)
		{
			m_sensors[i] = core::StringConverter::toFloat(lst[2 + i]);
		}
	}
}

void OmniBaseController::_onSerialData(int size, char *buffer)
{
	m_dataMutex->lock();
	char* ptr = buffer;
	buffer[size] = 0;
	gLogManager.log(buffer, ELL_INFO);
	while (*ptr)
	{
		m_buffer.push_back(*ptr);
		if (*ptr == '\n')
		{
			m_buffer.push_back(0);
			_ProcessBuffer();
			m_buffer.clear();
		}
		++ptr;
	}
	m_dataMutex->unlock();
}

void robot_OnDataArrival(int size, char *buffer){

	//todo:handle return
	//comPort->sendData("S", 2);

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
			robot_OnDataArrival(size, buffer);
			if (com->owner)
				((OmniBaseController*)com->owner)->_onSerialData(size, buffer);
			com->dataHasBeenRead();
			break;
		}
	}


}

bool OmniBaseController::Connect(const core::string& port)
{
	Disconnect();
#if 1
	comPort = new Tserial_event();
	comPort->setManager(robot_SerialEventManager);
	comPort->owner = this;
	connected = comPort->connect((char*)port.c_str(), 115200, SERIAL_PARITY_NONE, 8, FALSE, TRUE) == 0;
	if (!comPort->isconnected())
	{
		//printf("Failed to connect robot\n");
		delete comPort;
		comPort = 0;
	}
	else
	{
		comPort->setRxSize(15);
//		sendCommand("#ed");

	}
	return comPort && comPort->isconnected();
#else 

	gLogManager.log("[Base] Trying to connect to:" + port, ELL_INFO);

	try
	{
		comPort = new serial::Serial();
		comPort->Setup(port, 115200);

		comPort->open();
	}
	catch (serial::SerialException* e)
	{
		gLogManager.log(e->what(),ELL_WARNING);
		delete comPort;
	}
	 gLogManager.log("[Base] Finished connecting to:" + port, ELL_INFO);

	 return comPort && comPort->isOpen();
#endif


}

bool OmniBaseController::IsConnected()
{
	return comPort && comPort->isOpen();
	//return comPort != 0;
}

void OmniBaseController::Disconnect()
{
	if (!comPort || !comPort->isOpen())
		return;

	sendCommand("#sd");
	Drive(0, 0);
	sendCommand("#q");
	Sleep(100);
	//comPort->disconnect();
	comPort->close();
	connected = FALSE;
	//delete comPort;
	delete comPort;
	comPort = 0;
}


void OmniBaseController::sendCommand(const core::string& cmd)
{
	if (!comPort || !comPort->isOpen())
		return;
	std::string str = cmd + " \n\r";
	//comPort->sendData((char*)str.c_str(), str.size());
	comPort->write(str);
	//gLogManager.log(cmd, ELL_INFO);

}

void OmniBaseController::Drive(const math::vector2di& speed, int rotationSpeed)
{
	if (!comPort || !comPort->isOpen())
		return;
	int packet_size;
	char sCommand[128];
	if (m_lastSpeed == speed && m_lastRotation == rotationSpeed)
		return;

	m_lastRotation = rotationSpeed;
	m_lastSpeed = speed;
	sprintf_s(sCommand, 128, "#d %d %d %d", speed.x , speed.y , rotationSpeed );
	// 	
	sendCommand(sCommand);

}
void OmniBaseController::DriveStop()
{
	if (!comPort || !comPort->isOpen())
		return;

	int packet_size;
	char sCommand[128];
	 	
	sprintf_s(sCommand, 128, "#q");
	sendCommand(sCommand);

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


