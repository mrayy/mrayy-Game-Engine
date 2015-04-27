

#include "stdafx.h"
#pragma warning(X:4005)

#include "RobotSerialPort.h"
#include "Tserial_event.h"
#include "agent.h"
#include "movingAverage.h"
#include "TelubeeRobotDLL.h"
#include "Point3d.h"
#include "quaternion.h"
#include "RoombaController.h"
#include "OmniBaseController.h"

float testPosx = 100.00;
float testPosy = 100.00; 

	// used for serial sending
#define RUN		1
#define STOP	0
#define BASE	0
#define HEAD	1

#define ROBOT_CENTER 325

bool debug_print = true;
bool threadStart = false;
bool isDone = false;
bool upCount = true;

using namespace mray;

DWORD RobotSerialPort::timerThreadHead(RobotSerialPort *robot, LPVOID pdata){
	int count = 0;
	while (!isDone){
		if (threadStart){
			//printf("thread h: %d \r", count++);	
			robot->head_control(-robot->pan*yAxis, robot->tilt*xAxis, -robot->roll*zAxis);
/*
			if(upCount)
				robot->head_control(count+=1, 0, 0);
			else
				robot->head_control(count-=1, 0, 0);


			if (count > 80)
				upCount = false;
			else if (count < -80)
				upCount = true;*/
		}

		
		Sleep(1);
	}
	return 0;
}


DWORD RobotSerialPort::timerThreadBase(RobotSerialPort *robot, LPVOID pdata){
	int count = 0;
	while (true){
		if (threadStart){
			robot->omni_control(-robot->robot_vx, -robot->robot_vy,-robot->robot_rot, robot->baseConnected ? RUN:STOP);
			//printf("thread b: %d \r", count++);
			//robot->yamahaXY_control(robot->robotX, robot->robotY, RUN);
		}

		Sleep(30);
	}
}

#define ROOMBA_CONTROLLER

class RobotSerialPortImpl
	{
	public:
#ifdef ROOMBA_CONTROLLER
		mray::RoombaController* m_baseController;
#else 
		mray::OmniBaseController* m_baseController;
#endif
		Tserial_event *comHEAD;		// Serial Port
		MovAvg *mvRobot[2][3];		// 1 - base, 2 - head moving avarage 

		ITelubeeRobotListener* listener;
		RobotSerialPortImpl()
		{
#ifdef ROOMBA_CONTROLLER
			m_baseController = new mray::RoombaController;
#else 
			m_baseController = new mray::OmniBaseController;
#endif
			comHEAD = 0;
			listener = 0;
		}
		~RobotSerialPortImpl()
		{
			delete m_baseController;
		}
		void NotifyCollision(float l, float r)
		{
			if (listener)
			{
				listener->OnCollisionData(l, r);
			}
		}
};

	void head_OnDataArrival(int size, char *buffer){

		//todo:handle return
		//comROBOT->sendData("S", 2);

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
				head_OnDataArrival(size, buffer);
				com->dataHasBeenRead();
				break;
			}
		}


	}




RobotSerialPort::RobotSerialPort()
{

	m_impl = new RobotSerialPortImpl();
	m_impl->listener = 0;
	baseConnected = 0;
	load_parameters();

	for (int i = 0; i < 3; i++){
		m_impl->mvRobot[BASE][i] = new MovAvg();
		m_impl->mvRobot[HEAD][i] = new MovAvg();
	}

	m_headThread=CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)timerThreadHead, this, NULL, NULL);
	m_baseThread=CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)&timerThreadBase, this, NULL, NULL);

	ConnectRobot();

}

RobotSerialPort::~RobotSerialPort()
{
	isDone = true;
	Sleep(100);
	TerminateThread(m_baseThread, 0);
	TerminateThread(m_headThread, 0);
	DisconnectRobot();
	delete m_impl;
}


std::string RobotSerialPort::ScanePorts()
{
	char portName[64];
	Tserial_event evt;
	for (int i = 0; i < 20; ++i)
	{
		sprintf(portName, "COM%d", i);
		int ret=evt.connect(portName, head_baudRate, SERIAL_PARITY_ODD, 8, FALSE, TRUE);

		if (!ret){
			evt.sendData("v\r\n",3);
		}
		evt.disconnect();
	}
	return "";
}
void RobotSerialPort::InitializeRobot(IRobotStatusProvider* robotStatusProvider)
{

}
void RobotSerialPort::ConnectRobot()
{

	int ret=0;

	m_impl->comHEAD = new Tserial_event();

	
	ret=m_impl->m_baseController->Connect(robotCOM);
	if (ret){

		if (debug_print)
			printf("Robot Connected!\n", ret);
		
#ifndef ROOMBA_CONTROLLER	
		m_impl->m_baseController->GetComEvent()->setRxSize(15);
#endif
//		yamahaInitialize();		// initialize yamaha robot
	}
	else{

		if (debug_print)
		{
			printf("Robot not connected (%ld)\n", ret);
			printf("Robot baud (%ld)\n", ret);
		}
		m_impl->m_baseController->Disconnect();
	}

	if (m_impl->comHEAD != 0){
		m_impl->comHEAD->setManager(head_SerialEventManager);
		ret = m_impl->comHEAD->connect(headCOM, head_baudRate, SERIAL_PARITY_NONE, 8, FALSE, FALSE);
		if (!ret){

			if (debug_print)
				printf("Head Connected!\n", ret);
			m_impl->comHEAD->setRxSize(15);
		}
		else{

			if (debug_print)
				printf("Head not connected (%ld)\n", ret);
			m_impl->comHEAD->disconnect();
			delete m_impl->comHEAD;
			m_impl->comHEAD = 0;
		}
	}

}

bool RobotSerialPort::IsConnected()
{
	return m_impl->comHEAD != 0 || m_impl->m_baseController->IsConnected();
}

void RobotSerialPort::DisconnectRobot()
{
	int ret = 0;
	if (debug_print)
		printf("Disconnecting Robot\n", ret);
	m_impl->m_baseController->DriveStop();
	m_impl->m_baseController->Disconnect();

	if (m_impl->comHEAD != 0){
		m_impl->comHEAD->disconnect();
	}
	delete m_impl->comHEAD;
	m_impl->comHEAD = 0;



}

int RobotSerialPort::omni_control(int velocity_x, int velocity_y, int rotation, int control){

	static int counter = 0;

	if (!m_impl->m_baseController->IsConnected())
		return FALSE;
	if (abs(rotation) < 5)
		rotation = 0;
	if (control == RUN)
		m_impl->m_baseController->Drive(mray::math::vector2di(velocity_x, velocity_y) * 2 / 3, rotation * 2 / 3);
	else if (control == STOP)
		m_impl->m_baseController->DriveStop();

	if (counter == 2)
	{
		counter = 0;
		m_impl->m_baseController->UpdateSensors();
	}
	counter++;

	return true;

}

/*
int RobotSerialPort::yamahaXY_control(float pos_x, float pos_y, int control){
	int packet_size;
	char sCommand[128];

	if (!m_impl->m_baseController->IsConnected())
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

}*/


int RobotSerialPort::head_control(float pan, float tilt, float roll){
	int packet_size;
	char sCommand[128];
	if (!m_impl->comHEAD)
		return FALSE;

	sprintf_s(sCommand, 128, "@%3.2f,%3.2f,%3.2f,\r\n", pan, tilt, roll);

	packet_size = strlen(sCommand);
	if (m_impl->comHEAD)
		m_impl->comHEAD->sendData(sCommand, packet_size);
//	if (debug_print)
	//	printf(sCommand);

	return true;

}



void MatrixtoXYZ(double matrix[16], double *a, double *b, double *c)
{

	*c = atan2(matrix[4], matrix[0]);
	*b = -asin(matrix[8]);
	if (cos(*b) != 0){
		*a = asin((matrix[9] / cos(*b)));
		if (matrix[10] < 0)*a = mray::math::PI64 - *a;
		*c = *c;
		*b = *b;
	}
	else{
		*a = 0;
		*c = atan2(matrix[1], matrix[5]);
	}

	*a = mray::math::toDeg(*a);
	*b = mray::math::toDeg(*b);
	*c = mray::math::toDeg(*c);

}

typedef double Matrix[4][4];

void
qtomatrix(Matrix m, const mray::math::quaternion& q)
/*
* Convert quaterion to rotation sub-matrix of 'm'.
* The left column of 'm' gets zeroed, and m[3][3]=1.0, but the
* translation part is left unmodified.
*
* m = q
*/
{
#define X q.x 
#define Y q.y 
#define Z q.z 
#define W q.w

	float    x2 = X * X;
	float    y2 = Y * Y;
	float    z2 = Z * Z;

	m[0][0] = 1 - 2 * (y2 + z2);
	m[0][1] = 2 * (X * Y + W * Z);
	m[0][2] = 2 * (X * Z - W * Y);
	m[0][3] = 0.0;

	m[1][0] = 2 * (X * Y - W * Z);
	m[1][1] = 1 - 2 * (x2 + z2);
	m[1][2] = 2 * (Y * Z + W * X);
	m[1][3] = 0.0;

	m[2][0] = 2 * (X * Z + W * Y);
	m[2][1] = 2 * (Y * Z - W * X);
	m[2][2] = 1 - 2 * (x2 + y2);
	m[2][3] = 0.0;

	m[3][3] = 1.0;
}

void RobotSerialPort::UpdateRobotStatus(const RobotStatus& st)
{
// 	if (!IsConnected())
// 		return;
	if (IsConnected() && !st.connected)
	{
		DisconnectRobot();
	}
	else if (!IsConnected() && st.connected)
	{
		ConnectRobot();
	}

	if (st.connected)
		threadStart = true;
	else
		threadStart = false;

	if (!IsConnected())
		return;

	//todo: send the data to control the robot

	int v_scale = 500;
	int r_scale = 400;

	float v_size;

	robot_vx = m_impl->mvRobot[BASE][0]->getNext(st.speed[0]*v_scale);
	robot_vy = m_impl->mvRobot[BASE][1]->getNext(st.speed[1]*v_scale);
	robot_rot = m_impl->mvRobot[BASE][2]->getNext(st.rotation*r_scale);

//	robotX = m_impl->mvRobot[BASE][0]->getNext(st.X * 1000);
//	robotY = m_impl->mvRobot[BASE][1]->getNext(st.Z * 1000);

	//mray::math::Point3d<double> angles;
	mray::math::quaternion q2(st.headRotation[0], st.headRotation[1], st.headRotation[2], st.headRotation[3]);
	mray::math::quaternion q(q2.w,q2.z,q2.x,q2.y);
	//Matrix rotMat;
	//qtomatrix(rotMat, q);
	//MatrixtoXYZ((double*)rotMat, &angles.x, &angles.y, &angles.z);
	math::vector3d angles;
	q.toEulerAngles(angles);

	tilt = m_impl->mvRobot[HEAD][1]->getNext(-angles.y);
	pan = m_impl->mvRobot[HEAD][0]->getNext(-angles.z);
	roll = m_impl->mvRobot[HEAD][2]->getNext(-angles.x);

	baseConnected = st.connected;

	robotX = ROBOT_CENTER - robotX;

	if (robotY > 0)
		robotY = 100.0; 
	else
		robotY = 100.0 - robotY; 

	//v_size = sqrt(static_cast<double>(robot_vx)*static_cast<double>(robot_vx)+static_cast<double>(robot_vy)*static_cast<double>(robot_vy));

	if (debug_print && false){
		//printf("Robot Speed / Rot = %3.2f,%3.2f,%3.2f\n", st.speed.x, st.speed.y, st.rotation);
		printf("Head Position = %3.2f,%3.2f\n", robotX, robotY);
		printf("Head Orientation = %3.2f,%3.2f,%3.2f\n", angles.y, angles.x, angles.z);
		printf("-------------------------------\n");
	}


	//head_control(pan, tilt, roll);
	//yamahaXY_control(robotx, roboty, RUN);

	//while (true){
	//	if (testPosx < 200){
	//		testPosx += 0.1;
	//		testPosy += 0.1;
	//	}
	//	else{
	//		testPosx = 50.00;
	//		testPosy = 50.00;
	//	}

	//	yamahaXY_control(testPosx, testPosy, RUN);	// init position

	//}

	//Sleep(1);

}


void RobotSerialPort::SetListener(ITelubeeRobotListener* l)
{
	m_impl->listener = l;
}

std::string RobotSerialPort::ExecCommand(const std::string& cmd, const std::string& args)
{
	if (!m_impl->m_baseController || !m_impl->m_baseController->IsConnected())
		return "";
	if (cmd == CMD_Start)
	{
		m_impl->m_baseController->Start();
		return "";
	}
	if (cmd == CMD_Stop)
	{
		m_impl->m_baseController->Stop();
		return "";
	}
	 if (cmd == CMD_IsStarted)
	{
		return core::StringConverter::toString(m_impl->m_baseController->IsStarted());
	}
	
	if (cmd == CMD_GetSensorCount)
	{
		return core::StringConverter::toString(m_impl->m_baseController->GetSensorCount());

	}
	else if (cmd == CMD_GetSensorValue)
	{
		return core::StringConverter::toString(m_impl->m_baseController->GetSensorValue(core::StringConverter::toInt(args)));
	}
	else if (cmd == CMD_GetBatteryLevel)
	{
		return core::StringConverter::toString(m_impl->m_baseController->GetBatteryLevel());
	}
	else
		return m_impl->m_baseController->ExecCommand(cmd, args);
	return "";
}

