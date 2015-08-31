

#include "stdafx.h"
#pragma warning(X:4005)

#include "RobotSerialPort.h"
#include "Tserial_event.h"
#include "agent.h"
#include "movingAverage.h"
#include "NissanRobotDLL.h"
#include "modbus.h"

float testPosx = 100.00;
float testPosy = 100.00; 

	// used for serial sending
#define RUN		1
#define STOP	0
#define BASE	0
#define HEAD	1

#define ROBOT_CENTER 300
#define ROBOT_Y_SOFTLIMIT 185

bool debug_print = false;
bool threadStart = false; 
bool upCount = true;



DWORD RobotSerialPort::timerThreadHead(RobotSerialPort *robot, LPVOID pdata){
	int count = 20;
	while (true){
		if (threadStart){
			//printf("thread h: %d \r", count++);
			robot->head_control(-robot->roll, -robot->pan, robot->tilt);
		}

		
		Sleep(5);
	}
}


DWORD RobotSerialPort::timerThreadBase(RobotSerialPort *robot, LPVOID pdata){
	int count = 0;
	while (true){
		if (threadStart){
			//printf("thread b: %d \r", count++);
			robot->iai_pos(X_AXIS, WRITE_HOLD_REG, PCMD, 0x02, 0x04, robot->robotX);
			Sleep(5); 
			robot->iai_pos(Y_AXIS, WRITE_HOLD_REG, PCMD, 0x02, 0x04, robot->robotY);
			Sleep(5);

			//if (upCount){
			//	robot->iai_pos(X_AXIS, WRITE_HOLD_REG, PCMD, 0x02, 0x04, count);
			//	count += 3;
			//}
			//else{
			//	robot->iai_pos(X_AXIS, WRITE_HOLD_REG, PCMD, 0x02, 0x04, count);
			//	count -= 3;
			//}

			//if (count > 200)
			//	upCount = false;
			//else if (count < 20)
			//	upCount = true;
		}
	}
}


class RobotSerialPortImpl
	{
	public:
		Tserial_event *comROBOT;	// Serial Port
		Tserial_event *comHEAD;		// Serial Port
		MovAvg *mvRobot[2][3];		// 1 - base, 2 - head moving avarage 

		ITelubeeRobotListener* listener;

		void NotifyCollision(float l, float r)
		{
			if (listener)
			{
				listener->OnCollisionData(l, r);
			}
		}
};



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
				((RobotSerialPort*)com->userData)->RobotDataArrived(buffer, size);
				com->dataHasBeenRead();
				break;
			}
		}


	}


	void head_SerialEventManager(uint32 object, uint32 event){
		char *buffer;
		int   size;
		Tserial_event *com;

		com = (Tserial_event *)object;
		if (com != 0){
			switch (event){
			case  SERIAL_CONNECTED:
				break;
			case  SERIAL_DATA_ARRIVAL:
				size = com->getDataInSize();
				buffer = com->getDataInBuffer();
				((RobotSerialPort*)com->userData)->HeadDataArrived(buffer, size);
				com->dataHasBeenRead();
				break;
			}
		}


	}


RobotSerialPort::RobotSerialPort()
{

	m_impl = new RobotSerialPortImpl();
	m_impl->listener = 0;
	load_parameters();

	for (int i = 0; i < 3; i++){
		m_impl->mvRobot[BASE][i] = new MovAvg();
		m_impl->mvRobot[HEAD][i] = new MovAvg();
	}

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)timerThreadHead, this, NULL, NULL);
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)&timerThreadBase, this, NULL, NULL);

	ConnectRobot();

}

RobotSerialPort::~RobotSerialPort()
{
	IAI_destroy();
	DisconnectRobot();
	delete m_impl;
}


void RobotSerialPort::ConnectRobot()
{
	DisconnectRobot();
	int ret=0;
	m_impl->comROBOT = new Tserial_event();
	m_impl->comHEAD = new Tserial_event();
	if (m_impl->comROBOT != 0){
		m_impl->comROBOT->userData = this;
		m_impl->comROBOT->setManager(robot_SerialEventManager);
		ret = m_impl->comROBOT->connect(robotCOM, robot_baudRate, SERIAL_PARITY_NONE, 8, FALSE, FALSE);
		if (!ret){
			printf("Robot Connected!\n", ret);
			m_impl->comROBOT->setRxSize(50);
			IAI_Initialize();		// initialize IAI robot
		}
		else{
			printf("Robot not connected (%ld)\n", ret);
			printf("Robot baud (%ld)\n", ret);
			m_impl->comROBOT->disconnect();
			delete m_impl->comROBOT;
			m_impl->comROBOT = 0;
		}
	}

	if (m_impl->comHEAD != 0){
		m_impl->comHEAD->userData = this;

		m_impl->comHEAD->setManager(head_SerialEventManager);
		ret = m_impl->comHEAD->connect(headCOM, head_baudRate, SERIAL_PARITY_NONE, 8, FALSE, FALSE);
		if (!ret){
			printf("Head Connected!\n", ret);
			m_impl->comHEAD->setRxSize(21);


			int packet_size;
			char sCommand[128];
			
			sprintf_s(sCommand, 128, "d\r\n");

			packet_size = strlen(sCommand);
	//		m_impl->comHEAD->sendData(sCommand, packet_size);
		}
		else{
			printf("Head not connected (%ld)\n", ret);
			m_impl->comHEAD->disconnect();
			delete m_impl->comHEAD;
			m_impl->comHEAD = 0;
		}
	}

}

bool RobotSerialPort::IsConnected()
{
	return m_impl->comHEAD != 0 || m_impl->comROBOT != 0;
}

void RobotSerialPort::DisconnectRobot()
{
//	return; 
	int ret = 0;
	//IAI_destroy();

	printf("Disconnecting Robot\n", ret);
	if (m_impl->comROBOT != 0){
		m_impl->comROBOT->disconnect();
	}

	if (m_impl->comHEAD != 0){
		m_impl->comHEAD->setManager(0);
		m_impl->comHEAD->disconnect();
	}
	delete m_impl->comROBOT;
	delete m_impl->comHEAD;
	m_impl->comROBOT = 0;
	m_impl->comHEAD = 0;


}

int RobotSerialPort::omni_control(int velocity_x, int velocity_y, int rotation, int control){
	int packet_size;
	char sCommand[128];
	if (!m_impl->comROBOT)
		return FALSE;

	if (control == RUN)
		sprintf_s(sCommand,128, "V,%d,%d,%d\r\n", velocity_x, velocity_y, rotation);
	else if (control == STOP)
		sprintf_s(sCommand, 128, "q\r\n");

	packet_size = strlen(sCommand);
	m_impl->comROBOT->sendData(sCommand, packet_size);

	return true;

}


int RobotSerialPort::yamahaXY_control(float pos_x, float pos_y, int control){
	int packet_size;
	char sCommand[128];

	if (!m_impl->comROBOT)
		return FALSE;

	if (control == RUN)
		sprintf_s(sCommand, 128, "@DRIVE(1,%3.2f),(2,%3.2f),XY\r\n", pos_x, pos_y);

	else if (control == STOP)
		sprintf_s(sCommand, 128, "@SERVO OFF\r\n");

	packet_size = strlen(sCommand);
	m_impl->comROBOT->sendData(sCommand, packet_size);
	printf(sCommand);

	//Sleep(40); 

	return true;

}

int RobotSerialPort::yamahaInitialize(){
	char sCommand[128];
	if (!m_impl->comROBOT)
		return FALSE;

	sprintf_s(sCommand, 128, "@TORQUE(1)=0\r\n");
	m_impl->comROBOT->sendData(sCommand, strlen(sCommand));

	sprintf_s(sCommand, 128, "@TORQUE(2)=0\r\n");
	m_impl->comROBOT->sendData(sCommand, strlen(sCommand));

	sprintf_s(sCommand, 128, "@TRQTIME(1)=10\r\n");
	m_impl->comROBOT->sendData(sCommand, strlen(sCommand));

	sprintf_s(sCommand, 128, "@TRQTIME(2)=10\r\n");
	m_impl->comROBOT->sendData(sCommand, strlen(sCommand));

	return true;

}

int RobotSerialPort::IAI_Initialize(){
	char sCommand[128];
	if (!m_impl->comROBOT)
		return FALSE;

	iai_command_send(X_AXIS, WRITE_DO, MODBUS_MODE, ON);	//PMOPD ON, X-Axis	
	iai_command_send(Y_AXIS, WRITE_DO, MODBUS_MODE, ON);	//PMOPD ON, Y-Axis
	iai_command_send(X_AXIS, WRITE_DO, SERVO, ON);			//Servo ON, X-Axis
	iai_command_send(Y_AXIS, WRITE_DO, SERVO, ON);			//Servo ON, Y-Axis

	printf("Initialization done...\r\n");
	iai_command_send(X_AXIS, WRITE_DO, JOG_MODE, ON);			//Set Jogging
	iai_command_send(Y_AXIS, WRITE_DO, JOG_MODE, ON);			//Set Jogging
	iai_command_send(X_AXIS, WRITE_DO, SAFETY, OFF);			//Safety OFF X-Axis
	iai_command_send(Y_AXIS, WRITE_DO, SAFETY, OFF);			//Safety OFF Y-Axis
	printf("Full speed enabled...\r\n");

	return true;

}


int RobotSerialPort::IAI_destroy(){
	char sCommand[128];
	if (!m_impl->comROBOT)
		return FALSE;

	iai_command_send(X_AXIS, WRITE_DO, SAFETY, ON);			//Safety OFF X-Axis
	iai_command_send(Y_AXIS, WRITE_DO, SAFETY, ON);			//Safety OFF Y-Axis
	iai_command_send(X_AXIS, WRITE_DO, MODBUS_MODE, ON);	//PMOPD ON, X-Axis	
	iai_command_send(Y_AXIS, WRITE_DO, MODBUS_MODE, ON);	//PMOPD ON, Y-Axis
	iai_command_send(X_AXIS, WRITE_DO, SERVO, OFF);			//Servo ON, X-Axis
	iai_command_send(Y_AXIS, WRITE_DO, SERVO, OFF);			//Servo ON, Y-Axis
	printf("speed disabled...\r\n");

	return true;

}



unsigned int RobotSerialPort::lrc_generator(unsigned int axis, unsigned int fc, unsigned int data, unsigned int range){
	unsigned int checksum, data_most, data_least, range_most, range_least;
	data_least = data & 0x00ff;
	data_most = (data >> 8) & 0x00ff;

	range_least = range & 0x00ff;
	range_most = (range >> 8) & 0x00ff; 

	checksum = 0xFFFFFFFF - (axis + fc + data_most + data_least + range_most + range_least) + 0x01; 
	checksum = checksum & 0x000000ff;

	return checksum;

}

char *RobotSerialPort::iai_command_send(unsigned int axis, unsigned int fc, unsigned int data, unsigned int range){
	char rCommand[128]; 
	sprintf_s(rCommand, 128, ":%02X%02X%04X%04X%02X\r\n", axis, fc, data, range, lrc_generator(axis, fc, data, range));

	if (!m_impl->comROBOT)
		return 0;
	m_impl->comROBOT->sendData(rCommand, strlen(rCommand));
	//printf(rCommand);
	Sleep(10);
	
	return rCommand;
}

char *RobotSerialPort::iai_pos(unsigned int axis, unsigned int fc, unsigned int data, unsigned int range, unsigned int length, float pos){
	
	if (!m_impl->comROBOT)
		return 0;
	char rCommand[128];
	unsigned int checksum, data_msb, data_lsb, range_msb, range_lsb, pos_msb, pos_lsb, position;
	data_lsb = data & 0x00ff;
	data_msb = (data >> 8) & 0x00ff;

	range_lsb = range & 0x00ff;
	range_msb = (range >> 8) & 0x00ff;

	position = pos * 100;

	pos_lsb = position & 0x00ff;
	pos_msb = (position >> 8) & 0x00ff;

	checksum = 0xFFFFFFFF - (axis + fc + data_msb + data_lsb + range_msb + range_lsb + length + pos_msb + pos_lsb) + 0x01;
	checksum = checksum & 0x000000ff;

	sprintf_s(rCommand, 128, ":%02X%02X%04X%04X%02X0000%04X%02X\r\n", axis, fc, data, range, length, position, checksum);

	m_impl->comROBOT->sendData(rCommand, strlen(rCommand));
	//printf(rCommand);
	Sleep(6);

	return rCommand;
}




int RobotSerialPort::head_control(float pan, float tilt, float roll){
	int packet_size;
	char sCommand[128];
	if (!m_impl->comHEAD)
		return FALSE;

	sprintf_s(sCommand, 128, "@%3.2f,%3.2f,%3.2f,\r\n", pan, tilt, roll);

	packet_size = strlen(sCommand);
	m_impl->comHEAD->sendData(sCommand, packet_size);

	//request head values
	sprintf_s(sCommand, 128, "g\r\n");

	packet_size = strlen(sCommand);
	//m_impl->comHEAD->sendData(sCommand, packet_size);
	//printf(sCommand);

	return true;

}



void RobotSerialPort::UpdateRobotStatus(RobotStatus& st)
{
	if (!IsConnected())
		return;
	if (IsConnected() && !st.connected && !st.homing)
	{
		DisconnectRobot();
	}
	else if (!IsConnected() && (st.connected || st.homing))
	{
		ConnectRobot();
	}

	if (st.connected)
		threadStart = true;
	else
		threadStart = false;

	if (!IsConnected())
		return;

	if (st.homing){
		iai_command_send(X_AXIS, WRITE_DO, HOME, OFF);			//Homing, X-Axis
		iai_command_send(X_AXIS, WRITE_DO, HOME, ON);			//Homing, X-Axis
		iai_command_send(Y_AXIS, WRITE_DO, HOME, OFF);			//Homing, Y-Axis
		iai_command_send(Y_AXIS, WRITE_DO, HOME, ON);			//Homing, Y-Axis
		st.homing = false; 
	}

	//todo: send the data to control the robot
	int robot_vx, robot_vy, robot_rot;

	int v_scale = 500;
	int r_scale = 400;

	float v_size;

	//robot_vx = m_impl->mvRobot[BASE][0]->getNext(st.speedX*v_scale);
	//robot_vy = m_impl->mvRobot[BASE][1]->getNext(st.speedY*v_scale);
	//robot_rot = m_impl->mvRobot[BASE][2]->getNext(st.rotation*r_scale);

	robotX = m_impl->mvRobot[BASE][0]->getNext(st.X * 1000);
	robotY = m_impl->mvRobot[BASE][1]->getNext(-st.Y * 1000);

	pan = m_impl->mvRobot[HEAD][0]->getNext(st.yaw);
	tilt = m_impl->mvRobot[HEAD][1]->getNext(st.tilt);
	roll = m_impl->mvRobot[HEAD][2]->getNext(st.roll);

	robotX = ROBOT_CENTER - robotX;
	robotY = ROBOT_Y_SOFTLIMIT - robotY;

	if (robotY > ROBOT_Y_SOFTLIMIT)
		robotY = ROBOT_Y_SOFTLIMIT;
	else if (robotY < 0)
		robotY = 0;

	//v_size = sqrt(static_cast<double>(robot_vx)*static_cast<double>(robot_vx)+static_cast<double>(robot_vy)*static_cast<double>(robot_vy));

	if (debug_print){
		//printf("Robot Speed / Rot = %3.2f,%3.2f,%3.2f\n", st.speedX, st.speedY, st.rotation);
		printf("Head Position = %3.2f,%3.2f\n", robotX, robotY);
		printf("Head Orientation = %3.2f,%3.2f,%3.2f\n", st.yaw, st.tilt, st.roll);
		printf("-------------------------------\n");
		Sleep(1); 
	}

}

void RobotSerialPort::RobotDataArrived(char *buffer, int size){

	//todo:handle return
	//comROBOT->sendData("S", 2);

	buffer[size] = 0;

	//printf("%s\n", buffer);
}

void RobotSerialPort::HeadDataArrived(char *inBuff, int size){

	//todo:handle return
	//comROBOT->sendData("S", 2);

	inBuff[size] = 0;

	float cx, cy, cz;
	float tx, ty, tz;

	char* ptr = inBuff;
	/*
	while (*ptr != 0 && *ptr != '#')
	++ptr;

	if (*ptr == 0)
	return;
	sscanf(ptr, "#%f,%f,%f", &cx, &cy, &cz);
	*/

	for (int i = 0; i < size; ++i)
	{
		m_headbBuffer.push_back(inBuff[i]);
	}

	int Start = -1;
	int End = -1;

	char buffer[128];
	for (int i = m_headbBuffer.size()-1; i >=0; --i)
	{
		if (m_headbBuffer[i] == '$')
		{
			End = i;
			break;
		}
	}
	if (End == -1)
		return;

	ptr = buffer;
	for (int i = End - 1; i >= 0; --i)
	{
		if (m_headbBuffer[i] == '$')
		{
			Start = i;
			break;
		}
	}
	if (Start == -1)
		return;

	for (int i = Start + 1; i < End; ++i)
		*ptr++ = m_headbBuffer[i];

	sscanf(buffer, "%f,%f,%f", &tx, &ty, &tz);

	m_headbBuffer.erase(m_headbBuffer.begin(), m_headbBuffer.begin() + (End));

	if (m_impl->listener)
		m_impl->listener->OnRotationArrived(tx, ty, tz);

	//printf("%f,%f,%f\n", tx,ty,tz);
}

void RobotSerialPort::SetListener(ITelubeeRobotListener* l)
{
	m_impl->listener = l;
}

