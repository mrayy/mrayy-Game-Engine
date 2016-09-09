
#include "stdafx.h"
//#pragma warning(X:4005)

#include "torsoController.h"
#include "movingAverage.h"
#include "Torso2RobotDLL.h"

#include <stdio.h>
#include <windows.h> // for GetAsyncKeyState
#include <iostream>
#include <fstream>
#include <math.h>
#include "vectors.h"
#include <direct.h>

#include <stdio.h>
#include <stdlib.h>
#include "plc_config.h"
#include "Socket.h"

#include "IUDPClient.h"
#include "INetwork.h"


#define GetCurrentDir _getcwd
using namespace std;

bool threadStart = false;
bool isDone = false;
bool upCount = true;
bool tuningEnabled = false; 


FILE     *OutputLogFile;


using namespace mray;


void logString(const char* format, ...)
{
	va_list arglist;

	char Buffer[512];

	va_start(arglist, format);
	vprintf(format, arglist);
	vsprintf(Buffer, format, arglist);
	va_end(arglist);
	OutputLogFile = fopen("TorsoLog.txt", "a+");
	fprintf(OutputLogFile, "Msg --> %s", Buffer);
	printf("%s", Buffer);
	fclose(OutputLogFile);
}

bool isnan(double var){
	volatile double d = var;
	return d != d;
}



class torsoControllerImpl
{
public:
	ITelubeeRobotListener* listener;
	torsoController* _c;

	// initialize udp thread for PLC Data
	//MCClient *mc;

	mc_buff mcWriteBuf;
	mc_buff mcReadBuf;

	network::IUDPClient *mPLCSender;

	torsoControllerImpl(torsoController* c)
	{
		_c = c;
		listener = 0;
		mPLCSender = 0;
	}
	~torsoControllerImpl()
	{
		delete mPLCSender;
	}
	void NotifyCollision(float l, float r)
	{
		if (listener)
		{
			listener->OnCollisionData(_c,l, r);
		}
	}
};



torsoController::torsoController()
{
	if (GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)))
	{
		int len=strlen(cCurrentPath);
		cCurrentPath[len ] = '\\'; /* not really required */
		cCurrentPath[len + 1] = '\0'; /* not really required */

	}
	// Position & Orientation Data Moving Average
	int avgCnt = 2;
	for (int i = 0; i < 3; ++i)
	{
		m_posAvg[i] = new MovAvg(avgCnt);
		m_rotAvg[i] = new MovAvg(avgCnt);
	}

	// Current Sensing Moving Average
	int currentAvgCnt = 500;
	for (int j = 0; j < 6; ++j)
		m_mCurrentAvg[j] = new MovAvg(currentAvgCnt);

	robotState = EStopped;

	m_impl = new torsoControllerImpl(this);
	m_impl->listener = 0;
	m_robotStatusProvider = 0;

	threadStart = false;
	torsoInitializatiion = false;

	_setupCaps();

	// Thread routine
	hThread= CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)&timerThread1, this, NULL, NULL);
	SetThreadPriority(hThread, THREAD_PRIORITY_TIME_CRITICAL);	// Make the thread max priority, important to keep the robot stable
	pThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)&timerThread2, this, NULL, NULL);

	torsoHeadPos[0] = torsoHeadPos[1] = torsoHeadPos[2] = 0;
	torsoHeadOri[0] = torsoHeadOri[1] = torsoHeadOri[2] = torsoHeadOri[3]= 0;



	logString("Connecting to PLCWriter Service\r\n");

	Sleep(100);

	memset(&m_impl->mcWriteBuf, 0, sizeof(m_impl->mcWriteBuf)); // initializing test write data
	memset(&m_impl->mcReadBuf, 0, sizeof(m_impl->mcReadBuf)); // initializing test read data

	m_impl->mPLCSender = network::INetwork::getInstance().createUDPClient();//ip hard coded now
	m_impl->mPLCSender->Open();

	// create connection to Melsec PC via MC Protocol 
	//mc = new MCClient(PLC_PORT_TCP_TORSO, MELSEC_PLC);

}

torsoController::~torsoController()
{
	DisconnectRobot();
	delete m_impl;
	isDone = true;
	for (int i = 0; i < 3; ++i)
	{
		delete m_posAvg[i];
		delete m_rotAvg[i];
	}

	for (int j = 0; j < 6; ++j)
		delete m_mCurrentAvg[j];


	TerminateThread(hThread, 0);
	
}


void torsoController::_setupCaps()
{
	m_caps.hasBattery = false;
	m_caps.hasDistanceSensors = false;
	m_caps.hasBumpSensors = false;
	m_caps.canMove = false;
	m_caps.canRotate = false;
	m_caps.hasParallaxMotion = true;

	m_caps.distanceSensorCount = 0;
	m_caps.bumpSensorCount = 0;
	m_caps.bodyJointsCount = 3 + 3;	//Body: 3 , Head: 3

	// For future use by Yamen
	//m_caps.enabledMotion.set(false, false, false);
	//m_caps.enabledRotation.set(false, false, false);
	//m_caps.headLimits[0].set(-180, -180, -180);
	//m_caps.headLimits[1].set(180, 180, 180);
}
ERobotControllerStatus torsoController::GetRobotStatus() {
	return robotState;
}


bool torsoController::GetJointValues(std::vector<float>& values){
	values.resize(12);
	for (int i = 0; i<12; i++) 
		values[i]=(robotJointData[i]);

	return true; 
}


void torsoController::ConnectRobot()
{
	manualMode = false;
	tuningEnabled = false;

	if ((robotState == EDisconnected) && torsoInitialized && ControlMode!=2)
	{
		_setMode(2);
		logString("Connecting Robot\n");
	}

	return;
}

void torsoController::ManualControlRobot()
{
	manualMode = true; 
	tuningEnabled = false;

	if ((robotState == EDisconnected) && torsoInitialized && ControlMode != 2)
		_setMode(2);

	return;
}

void torsoController::DisconnectRobot()
{
	logString("torsoController::DisconnectRobot()\n");
	tuningEnabled = false;
	if (robotState == EConnected) _setMode(4);;
	
	return;
}

void torsoController::ShutdownRobot(){

	logString("torsoController::ShutdownRobot()\n");
	tuningEnabled = false;
	//if (robotState == EDisconnected && ControlMode != 5)
	_setMode(5);

	return;
}
void torsoController::_setMode(int mode)
{
	if (ControlMode == mode)
		return;
	ControlMode = mode;
	switch (ControlMode)
	{
	case 0:
		logString("Switching to Parking\r\n");
		break;
	case 1:
		logString("Switching to Connected KB\r\n");
		break;
	case 2:
		logString("Switching to Connecting\r\n");
		break;
	case 3:
		logString("Switching to Connected RT\r\n");
		break;
	case 4:
		logString("Switching to Disconnecting\r\n");
		break;
	case 5:
		logString("Switching to Shutdown\r\n");
		break;

	}
}

void torsoController::tuningMode(){

	if (robotState == EStopped)
		tuningEnabled = true; 

	return;
}

bool torsoController::softInterlockProcess(){
	if (ServoMotor[0].AngularDisplacement >= 19)
		m_impl->mcWriteBuf.torso.collisionYBM = 1;
	else
		m_impl->mcWriteBuf.torso.collisionYBM = 0;


	return 0;

}



bool torsoController::softCurrentSense(){

	motorCurrent[0] = texArt_ncord->Terminal[0].AD_Value[4] * 0.000415;
	motorCurrent[1] = texArt_ncord->Terminal[0].AD_Value[5] * 0.000415;
#ifdef J3Enabled
	motorCurrent[2] = texArt_ncord->Terminal[0].AD_Value[6] * 0.000415;
#else 
	motorCurrent[2]=0;
#endif
	motorCurrent[3] = (texArt_ncord->Terminal[1].AD_Value[4] - 0x8000)*0.000424;
	motorCurrent[4] = (texArt_ncord->Terminal[1].AD_Value[5] - 0x8000)*0.000424;
	motorCurrent[5] = (texArt_ncord->Terminal[1].AD_Value[6] - 0x8000)*0.000424;

	for (int i = 0; i < 6; ++i)
		motorCurrent[i] = m_mCurrentAvg[i]->getNext(motorCurrent[i]);

	jointTorque[0] = 37.7*motorCurrent[0] * 60 / 1000;
	jointTorque[1] = 37.7*motorCurrent[1] * 60 / 1000;
#ifdef J3Enabled
	jointTorque[2] = 37.7*motorCurrent[2] * 1 / 2000;
#else
	jointTorque[2] = 0;
#endif
	jointTorque[3] = 24.1*motorCurrent[3] * 55 / 2000;
	jointTorque[4] = 24.1*motorCurrent[4] * 50 / 1000;
	jointTorque[5] = 24.1*motorCurrent[5] * 21 / 1000;

	// Current Limitations
	if ((motorCurrent[0] > 2.0) || 
		(motorCurrent[1] > 2.0) ||
		(motorCurrent[2] > 2.0) ||
		(motorCurrent[3] > 0.6) || 
		(motorCurrent[4] > 0.6) || 
		(motorCurrent[5] > 0.6))
		return true; 


	return false; 

}


bool torsoController::writeRobotData(){

	m_impl->mcWriteBuf.torso.J1_ik_angle = (int)(DesiredDisplacement_J[0] * 180 * 100 / PI);
	m_impl->mcWriteBuf.torso.J2_ik_angle = (int)(DesiredDisplacement_J[1] * 180 * 100 / PI);
	m_impl->mcWriteBuf.torso.J3_ik_disp = (int)(DesiredDisplacement_J[2] * 100);
	m_impl->mcWriteBuf.torso.J4_ik_angle = (int)(DesiredDisplacement_J[3] * 180 * 100 / PI);
	m_impl->mcWriteBuf.torso.J5_ik_angle = (int)(DesiredDisplacement_J[4] * 180 * 100 / PI);
	m_impl->mcWriteBuf.torso.J6_ik_angle = (int)(DesiredDisplacement_J[5] * 180 * 100 / PI);

	m_impl->mcWriteBuf.torso.J1_rt_angle = (int)(ServoMotor[0].AngularDisplacement * 100);
	m_impl->mcWriteBuf.torso.J2_rt_angle = (int)(ServoMotor[1].AngularDisplacement * 100);
	m_impl->mcWriteBuf.torso.J3_rt_disp = (int)(ServoMotor[2].AngularDisplacement * 100);
	m_impl->mcWriteBuf.torso.J4_rt_angle = (int)(ServoMotor[3].AngularDisplacement * 100);
	m_impl->mcWriteBuf.torso.J5_rt_angle = (int)(ServoMotor[4].AngularDisplacement * 100);
	m_impl->mcWriteBuf.torso.J6_rt_angle = (int)(ServoMotor[5].AngularDisplacement * 100);

	m_impl->mcWriteBuf.torso.J1_current = (int)(motorCurrent[0] * 100);
	m_impl->mcWriteBuf.torso.J2_current = (int)(motorCurrent[1] * 100);
	m_impl->mcWriteBuf.torso.J3_current = (int)(motorCurrent[2] * 100);
	m_impl->mcWriteBuf.torso.J4_current = (int)(motorCurrent[3] * 100);
	m_impl->mcWriteBuf.torso.J5_current = (int)(motorCurrent[4] * 100);
	m_impl->mcWriteBuf.torso.J6_current = (int)(motorCurrent[5] * 100);

	m_impl->mcWriteBuf.torso.J1_torque = (int)(jointTorque[0] * 100);
	m_impl->mcWriteBuf.torso.J2_torque = (int)(jointTorque[1] * 100);
	m_impl->mcWriteBuf.torso.J3_torque = (int)(jointTorque[2] * 100);
	m_impl->mcWriteBuf.torso.J4_torque = (int)(jointTorque[3] * 100);
	m_impl->mcWriteBuf.torso.J5_torque = (int)(jointTorque[4] * 100);
	m_impl->mcWriteBuf.torso.J6_torque = (int)(jointTorque[5] * 100);

	m_impl->mcWriteBuf.torso.status = robotState; 
	m_impl->mcWriteBuf.torso.robotConnected = torsoInitialized;

	char buffer[sizeof(m_impl->mcWriteBuf.torso) + 10];
	const char TORSO_DATA = 5;
	buffer[0] = TORSO_DATA;
	memcpy(&buffer + 1, &m_impl->mcWriteBuf.torso, sizeof(m_impl->mcWriteBuf.torso));
	int len = 1 + sizeof(m_impl->mcWriteBuf.torso);

	network::NetAddress toAddr;
	toAddr.setIP("192.168.2.1");
	toAddr.port = 8010;
	uint outlen = 0;;
	network::UDPClientError err= m_impl->mPLCSender->SendTo(&toAddr, buffer, len,&outlen);
	if (err != network::UDP_SOCKET_ERROR_NONE)
	{
		logString("Failed to send\n");
	}
	
	uint inLen = sizeof(m_impl->mcReadBuf);
	//now wait for reply message
	if (m_impl->mPLCSender->RecvFrom((char*)&m_impl->mcReadBuf, &inLen,0,0)<0)
	{
		if (ret < 0){
			int err = WSAGetLastError();
			logString("Failed to receive: %d\n", err);
		}
	}

	// Write TORSO Data to PLC
	//mc->batch_write("W", SELECT_TORSO, 0xA0, &mcWriteBuf, 0x20); Sleep(1);

	// Read Interlock Flags	
	//mc->batch_read("W", SELECT_INTERLOCK, 0x0360, &mcReadBuf, 0x06); Sleep(1);


	return true; 

}









void torsoController::InitializeRobot(IRobotStatusProvider* robotStatusProvider)
{
	logString("torsoController::InitializeRobot()\n");

	m_robotStatusProvider = robotStatusProvider;
	if (robotState==EStopped)
		torsoInitializatiion = InitializeTorsoRobot(false);
	return;
}
void QuatToEuler(float* q, float* euler)
{

	float w = q[0];
	float x = q[1];
	float y = q[2];
	float z = q[3];

	float q0 = w;
	float q1 = y;
	float q2 = x;
	float q3 = z;

	euler[1] = ((float)atan2(2 * (q0 * q1 + q2 * q3), 1 - 2 * (q1*q1 + q2*q2)));
	euler[0] = ((float)asin(2 * (q0 * q2 - q3 * q1)));
	euler[2] = ((float)atan2(2 * (q0 * q3 + q1 * q2), 1 - 2 * (q2*q2 + q3*q3)));

}

void torsoController::UpdateRobotStatus(const RobotStatus& st)

{
	Quaternion targetQuat;

	targetQuat[0] = st.headRotation[0];
	targetQuat[1] = st.headRotation[3];
	targetQuat[2] = st.headRotation[1];
	targetQuat[3] = st.headRotation[2];

	torsoHeadPos[0] = st.headPos[2];	//Front Axis
	torsoHeadPos[1] = -st.headPos[0];	//Side Axis
	torsoHeadPos[2] = st.headPos[1];	//Up Axis


	float rotTmp[3];
	QuatToEuler(targetQuat, rotTmp);

	torsoHeadOri[0] = -rotTmp[0];	// Roll
	torsoHeadOri[1] = -rotTmp[1];	// Pitch
	torsoHeadOri[2] = rotTmp[2];	// Yaw


	if (true)
	{
		static const float PosPrecision = 100000;
		static const float RotPrecision = 1000;
		for (int i = 0; i < 3; ++i)
		{
			torsoHeadPos[i] = ((int)(torsoHeadPos[i] * PosPrecision)) / PosPrecision;
			torsoHeadOri[i] = ((int)(torsoHeadOri[i] * RotPrecision)) / RotPrecision;
		}
	}

	if (!st.connected)
	{
		torsoHeadPos[0] = torsoHeadPos[1] = torsoHeadPos[2] = 0.0;
		torsoHeadOri[0] = torsoHeadOri[1] = torsoHeadOri[2] = 0.0;
	}

	for (int i = 0; i < 3; ++i)
	{
		torsoHeadPos[i] = m_posAvg[i]->getNext(torsoHeadPos[i]);
		torsoHeadOri[i] = m_rotAvg[i]->getNext(torsoHeadOri[i]);
	}

}


void torsoController::_processData()
{
	if (m_robotStatusProvider)
	{
		RobotStatus st;
		m_robotStatusProvider->GetRobotStatus(st);
	}
}
void torsoController::SetListener(ITelubeeRobotListener* l)
{
	m_impl->listener = l;
}


DWORD torsoController::timerThread1(torsoController *robot, LPVOID pdata){
	int count1 = 0;
	bool recordData = false;
	std::vector<float> angles;
	std::vector<float> data;
	FILE* dataFile = 0;
	if (recordData)
	{
		dataFile = fopen("TorsoAngles.txt", "w");
		fprintf(dataFile, "starting\n");
		fclose(dataFile);
	}
	while (!isDone){
		if (threadStart){
			robot->_processData();
			robot->controlStateMachine();
			if (recordData)
			{
				robot->GetJointValues(angles);
				if (angles.size() > 11)
				{
					data.push_back(angles[11]);
					if (data.size() > 500){
						float lastAngle = 0;
						dataFile = fopen("TorsoAngles.txt", "a");
						for (int i = 0; i < data.size(); ++i)
						{
							float diff = data[i] - lastAngle;
							fprintf(dataFile, "%f\t%f\n", data[i], diff);
							lastAngle = data[i];
						}
						fclose(dataFile);
						data.clear();
					}
				}
			}
		}
		else
			Sleep(50);
	}


	return 0;
}


DWORD torsoController::timerThread2(torsoController *robot, LPVOID pdata){
	int count2 = 0;
	while (!isDone){
		if (threadStart){
			robot->writeRobotData(); 

// 			printf("current: %4.2f, %4.2f, %4.2f, %4.2f, %4.2f, %4.2f \r\n",
// 				robot->motorCurrent[0], robot->motorCurrent[1], robot->motorCurrent[2], robot->motorCurrent[3], robot->motorCurrent[4], robot->motorCurrent[5]);

			Sleep(50);
		}
		else 
			Sleep(100);
	}

	return 0;
}




int torsoController::ReInitializeTorsoRobot(void){
	logString("torsoController::ReInitializeTorsoRobot()\n");

	if (!torsoInitialized)
		return false;
	return true;
}



class ThreadPrioritySetter
{
public:
	HANDLE thread;
	ThreadPrioritySetter()
	{
		thread = GetCurrentThread();
		SetThreadPriority(thread, THREAD_PRIORITY_HIGHEST);

	}
	~ThreadPrioritySetter()
	{
		SetThreadPriority(thread, THREAD_PRIORITY_NORMAL);

	}
};

int torsoController::InitializeTorsoRobot(bool debug){

	if (torsoInitialized)
		return torsoInitializatiion;

	if (robotState != ERobotControllerStatus::EStopped)
		return torsoInitializatiion;

	ThreadPrioritySetter _PrioritySetter;//automatically change priority from Hight to Normal on function exit

	logString("Initing Robot.\n");
	robotState = ERobotControllerStatus::EIniting;
	threadStart = false;
	Sleep(3000);	// Sleep for three seconds until threads gets stable
	torsoInitializatiion = false; 

	char  temp[100], ctemp;
	std::string Name_ParameterFile_M[6];

	ifstream SetupDataFile,                 // Setup data file.
	ParameterFile_M[6];
	/*
	OutputDataFile = fopen("LogLog.dat","wt");
	*/

	MothorCLK.Acquire_Frequency();          // Acquire and set the MothorCLK_Frequency.
	MothorCLK.Start();                      // Acquire and set the MothorCLK_Frequency.


	//-----[Step1] Load setup parameters to LinearArm.
	logString("Loading setup parameters: ");
	std::string path = std::string(cCurrentPath) + "SetUp_Torso.prm";
	SetupDataFile.open(path);
	if (!SetupDataFile) {
		cout << " FileOpenError(SetupDataFile1):" << path << endl;
		return 0;
	}
	else
		logString("done\n");

	for (int i = 0; i<10; i++){
		SetupDataFile.get(temp, 100);              // Absorbing comment.
		SetupDataFile.get(ctemp);                  // Absorbing '\n'.
	}

	SetupDataFile >> ControlMode;
	SetupDataFile.get(temp, 100);              // Absorbing comment.
	SetupDataFile.get(ctemp);                  // Absorbing '\n'.

	SetupDataFile >> Display;
	SetupDataFile.get(temp, 100);              // Absorbing comment.
	SetupDataFile.get(ctemp);                  // Absorbing '\n'.

	SetupDataFile.get(temp, 100);                // Absorbing comment.
	SetupDataFile.get(ctemp);                    // Absorbing '\n'.

	for (int i = 0; i<6; i++) {
		SetupDataFile >> Name_ParameterFile_M[i];
		Name_ParameterFile_M[i] = cCurrentPath + (Name_ParameterFile_M[i]);
		SetupDataFile.get(temp, 100);              // Absorbing comment.
		SetupDataFile.get(ctemp);                  // Absorbing '\n'.
	}

	SetupDataFile.get(temp, 100);                // Absorbing comment.
	SetupDataFile.get(ctemp);                    // Absorbing '\n'.

	for (int i = 0; i<6; i++) {
		SetupDataFile >> GearRatio_J[i];
		SetupDataFile.get(temp, 100);              // Absorbing comment.
		SetupDataFile.get(ctemp);                  // Absorbing '\n'.
	}

	SetupDataFile.get(temp, 100);                // Absorbing comment.
	SetupDataFile.get(ctemp);                    // Absorbing '\n'.

	for (int i = 0; i<6; i++) {
		SetupDataFile >> LimitDisplacement_p_J[i];
		SetupDataFile.get(temp, 100);              // Absorbing comment.
		SetupDataFile.get(ctemp);                  // Absorbing '\n'.
		SetupDataFile >> LimitDisplacement_n_J[i];
		SetupDataFile.get(temp, 100);              // Absorbing comment.
		SetupDataFile.get(ctemp);                  // Absorbing '\n'.
	}

	SetupDataFile.get(temp, 100);                // Absorbing comment.
	SetupDataFile.get(ctemp);                    // Absorbing '\n'.

	for (int i = 0; i<6; i++) {
		SetupDataFile >> StepDisplacement_J[i];
		SetupDataFile.get(temp, 100);              // Absorbing comment.
		SetupDataFile.get(ctemp);                  // Absorbing '\n'.
	}

	SetupDataFile.get(temp, 100);                // Absorbing comment.
	SetupDataFile.get(ctemp);                    // Absorbing '\n'.

	for (int i = 0; i<6; i++) {
		SetupDataFile >> StepCoordinate_Head[i];
		SetupDataFile.get(temp, 100);              // Absorbing comment.
		SetupDataFile.get(ctemp);                  // Absorbing '\n'.
	}

	SetupDataFile.close();

	logString("Initialize NCord Interface: ");
	//-----[Step2] Initialize texArt_ncord.
	texArt_ncord = new TexART_NCord000_Interface(0);
	ret = texArt_ncord->Initialize();
		if (ret) {
			logString("Initialize error = %d \n", ret);
			motorForceShutdown();
			Sleep(100);
		}
		else
			logString("done.\n");

		// added by charith, clearing previous driver commands in buffer. 
		texArt_ncord->Free();
		Sleep(100);

		// resetting all terminals
		for (int i = 0; i<8; i++)
			texArt_ncord->ResetTerminal(i);


	logString("Start NCord Interface: ");
	//-----[Step3] Start the interface object texArt_ncord.
	ret = texArt_ncord->Start();

	if (ret)
	{
		logString("TexART Driver communication error #:%d\n", ret);
		motorForceShutdown(); // danger overriding device diagnostic
	}
	else
		logString("TexART Driver communication ok!\n");

	logString("Initialize ServoMotor Control: ");
	//-----[Step4] Initialize ServoMotor.
	for (int i = 0; i<6; i++) {
		ParameterFile_M[i].open(Name_ParameterFile_M[i]);
		if (!ParameterFile_M[i].is_open()) {
			cout << " FileOpenError(ParameterFile_M[" << i << "])" << endl;
			motorForceShutdown();
		}
	}
	logString("done.\n");

	ServoMotor = new TexART_ServoMotor2015[6];
	for (int i = 0; i<6; i++) ServoMotor[i].Initialize(ParameterFile_M[i], 0, 0, 0, texArt_ncord); // Load initial data file.
	for (int i = 0; i<6; i++) ParameterFile_M[i].close();                                      // Close initial data file.

	//-----[Check stage] Search DeadendPotentioValue except J3 by manual motion.
	// [Danger!] Cut the motor power.
	/*cout << "\n Search DeadendPotentioValue of J1 \n"; ServoMotor[0].SearchDeadendPotentioValue();
	cout << "\n Search DeadendPotentioValue of J2 \n"; ServoMotor[1].SearchDeadendPotentioValue();
	cout << "\n Search DeadendPotentioValue of J4 \n"; ServoMotor[3].SearchDeadendPotentioValue();
	cout << "\n Search DeadendPotentioValue of J5 \n"; ServoMotor[4].SearchDeadendPotentioValue();
	cout << "\n Search DeadendPotentioValue of J6 \n"; ServoMotor[5].SearchDeadendPotentioValue();*/

	//-----[Step6] Calibrate AngularDisplacement for present position of each ServoMotor except M3.
	logString("Calibrating Current Angular Displacement Except M3: ");
	ServoMotor[0].CaliAngularDisplacementByPotentio();
	ServoMotor[1].CaliAngularDisplacementByPotentio();
	ServoMotor[3].CaliAngularDisplacementByPotentio();
	ServoMotor[4].CaliAngularDisplacementByPotentio();
	ServoMotor[5].CaliAngularDisplacementByPotentio();
	logString("done.\n");

	//-----[Step7] Raise up Torso in 5 seconds, and calibrate AngularDisplacement of ServoMotor M3.
	logString("Raising J3: Calibrating Current Angular Displacement For J3: ");

	AngularDisplacement_start_M[0] = ServoMotor[0].AngularDisplacement;
	AngularDisplacement_start_M[1] = ServoMotor[1].AngularDisplacement;
	AngularDisplacement_start_M[3] = ServoMotor[3].AngularDisplacement;
	AngularDisplacement_start_M[4] = ServoMotor[4].AngularDisplacement;
	AngularDisplacement_start_M[5] = ServoMotor[5].AngularDisplacement;

	if ((isnan(AngularDisplacement_start_M[3]) || isnan(AngularDisplacement_start_M[4]) || isnan(AngularDisplacement_start_M[5]))){
		logString("Potentiometer Read Error...");
		motorForceShutdown();
	}

	AngularDisplacement_goal_M[0] = CalcMotorDisplacement(&(ServoMotor[0]), 0.0, LimitDisplacement_p_J[0], LimitDisplacement_n_J[0]);
	AngularDisplacement_goal_M[1] = CalcMotorDisplacement(&(ServoMotor[1]), 0.0, LimitDisplacement_p_J[1], LimitDisplacement_n_J[1]);
	AngularDisplacement_goal_M[3] = CalcMotorDisplacement(&(ServoMotor[3]), 0.0, LimitDisplacement_p_J[3], LimitDisplacement_n_J[3]);
	AngularDisplacement_goal_M[4] = CalcMotorDisplacement(&(ServoMotor[4]), 0.0, LimitDisplacement_p_J[4], LimitDisplacement_n_J[4]);
	AngularDisplacement_goal_M[5] = CalcMotorDisplacement(&(ServoMotor[5]), 0.0, LimitDisplacement_p_J[5], LimitDisplacement_n_J[5]);

	if ((isnan(AngularDisplacement_goal_M[3]) || isnan(AngularDisplacement_goal_M[4]) || isnan(AngularDisplacement_goal_M[5]))){
		logString("Goal calculation Error...");
		motorForceShutdown();
	}

	for (int i = 0; i<6; i++) 
		ServoMotor[i].ZeroLimitAngularDisplacement(); // Cancel limitation by AngularDisplacement.
	
	ServoMotor[2].ChangePGain_V(0.0008);								// Give PGain_V to M3 for GoDeadEndPosition.
	
	Time_start = MothorCLK.Elapsed();
	float timespan = 3.0f;//5.0f;
	while (0
		+ ServoMotor[0].GoNextPositionOnTime2(AngularDisplacement_start_M[0], AngularDisplacement_goal_M[0], timespan, Time_start, MothorCLK.Elapsed())
		+ ServoMotor[1].GoNextPositionOnTime2(AngularDisplacement_start_M[1], AngularDisplacement_goal_M[1], timespan, Time_start, MothorCLK.Elapsed())
#ifdef J3Enabled
		+ ServoMotor[2].GoDeadEndPosition(-0.02*GearRatio_J[2], 0.0001*GearRatio_J[2], 0.2, ServoMotor[2].ReadDeadendAngularDisplacement_n(),
		Time_start, MothorCLK.Elapsed())
#endif
		+ ServoMotor[3].GoNextPositionOnTime2(AngularDisplacement_start_M[3], AngularDisplacement_goal_M[3], timespan, Time_start, MothorCLK.Elapsed())
		+ ServoMotor[4].GoNextPositionOnTime2(AngularDisplacement_start_M[4], AngularDisplacement_goal_M[4], timespan, Time_start, MothorCLK.Elapsed())
		+ ServoMotor[5].GoNextPositionOnTime2(AngularDisplacement_start_M[5], AngularDisplacement_goal_M[5], timespan, Time_start, MothorCLK.Elapsed())
	> 0)
	{
		texArt_ncord->ContactPeriodic();
		if (GetAsyncKeyState(0x1b) & 0x8000) break;
	}
	logString("done. \n");


	for (int i = 0; i<6; i++) 
		ServoMotor[i].ReloadLimitAngularDisplacement(); // Reset limitation by AngularDisplacement.

	ServoMotor[2].ZeroPGain_V();                                       // Zero PGain_V of M3.

	//-----[Step8] Move J3 to neutral position in 5 seconds.
	logString("Move J3 to neutral position in 2 seconds: ");

	AngularDisplacement_start_M[2] = ServoMotor[2].AngularDisplacement;
	AngularDisplacement_goal_M[2] = CalcMotorDisplacement(&(ServoMotor[2]), 0.0, LimitDisplacement_p_J[2], LimitDisplacement_n_J[2]);

	ServoMotor[2].ZeroLimitAngularDisplacement();                      // Cancel limitation by AngularDisplacement of J3.
	Time_start = MothorCLK.Elapsed();

	while (0
		+ ServoMotor[2].GoNextPositionOnTime2(AngularDisplacement_start_M[2], AngularDisplacement_goal_M[2], 2.0, Time_start, MothorCLK.Elapsed())
	> 0)
	{
		ServoMotor[0].Fix2(AngularDisplacement_goal_M[0]);
		ServoMotor[1].Fix2(AngularDisplacement_goal_M[1]);
		ServoMotor[3].Fix2(AngularDisplacement_goal_M[3]);
		ServoMotor[4].Fix2(AngularDisplacement_goal_M[4]);
		ServoMotor[5].Fix2(AngularDisplacement_goal_M[5]);

		texArt_ncord->ContactPeriodic();
		if (GetAsyncKeyState(0x1b) & 0x8000) break;
	}
	logString("done.\n");

	logString("Move Robot to Parking Positiion..");
	// moving the robot to parked positin
	for (int i = 0; i<6; i++) AngularDisplacement_start_M[i] = ServoMotor[i].AngularDisplacement;

	AngularDisplacement_goal_M[0] = CalcMotorDisplacement(&(ServoMotor[0]), 0.0 / 180.0*PI, LimitDisplacement_p_J[0], LimitDisplacement_n_J[0]); //-3.0/180.0
	AngularDisplacement_goal_M[1] = CalcMotorDisplacement(&(ServoMotor[1]), 0.0 / 180.0*PI, LimitDisplacement_p_J[1], LimitDisplacement_n_J[1]); //20.0/180.0
#ifdef J3Enabled
	AngularDisplacement_goal_M[2] = CalcMotorDisplacement(&(ServoMotor[2]), 0.0, LimitDisplacement_p_J[2], LimitDisplacement_n_J[2]);			
#else 
	AngularDisplacement_goal_M[2] = 0;
#endif
	AngularDisplacement_goal_M[3] = CalcMotorDisplacement(&(ServoMotor[3]), 45.0 / 180.0*PI, LimitDisplacement_p_J[3], LimitDisplacement_n_J[3]);
	AngularDisplacement_goal_M[4] = CalcMotorDisplacement(&(ServoMotor[4]), 0.0 / 180.0*PI, LimitDisplacement_p_J[4], LimitDisplacement_n_J[4]);
	AngularDisplacement_goal_M[5] = CalcMotorDisplacement(&(ServoMotor[5]), 0.0 / 180.0*PI, LimitDisplacement_p_J[5], LimitDisplacement_n_J[5]);

	ServoMotor[1].ZeroLimitAngularDisplacement(); // Cancel limitation by AngularDisplacement of J2.
	ServoMotor[3].ZeroLimitAngularDisplacement(); // Cancel limitation by AngularDisplacement of J4.
	Time_start = MothorCLK.Elapsed();
	
	
	timespan = 3.0f; //5.0f;

	while (0
		+ ServoMotor[0].GoNextPositionOnTime2(AngularDisplacement_start_M[0], AngularDisplacement_goal_M[0], timespan, Time_start, MothorCLK.Elapsed())
		+ ServoMotor[1].GoNextPositionOnTime2(AngularDisplacement_start_M[1], AngularDisplacement_goal_M[1], timespan, Time_start, MothorCLK.Elapsed())
#ifdef J3Enabled
		+ ServoMotor[2].GoNextPositionOnTime2(AngularDisplacement_start_M[2], AngularDisplacement_goal_M[2], timespan, Time_start, MothorCLK.Elapsed())
#endif
		+ ServoMotor[3].GoNextPositionOnTime2(AngularDisplacement_start_M[3], AngularDisplacement_goal_M[3], timespan, Time_start, MothorCLK.Elapsed())
		+ ServoMotor[4].GoNextPositionOnTime2(AngularDisplacement_start_M[4], AngularDisplacement_goal_M[4], timespan, Time_start, MothorCLK.Elapsed())
		+ ServoMotor[5].GoNextPositionOnTime2(AngularDisplacement_start_M[5], AngularDisplacement_goal_M[5], timespan, Time_start, MothorCLK.Elapsed())
	> 0)
	{
		texArt_ncord->ContactPeriodic();
	}
	
	logString("done.\n");
	ServoMotor[2].ReloadLimitAngularDisplacement();   // Reset limitation by AngularDisplacement of J3 after every Zero Limit Cancellation
	
	// making sure the Desired Displacement it Initialized to Zero for Keyboard Input
	for (int i = 0; i < 6; i++) {
		DesiredDisplacement_J[i] = CalcJointDisplacement(&(ServoMotor[i]), LimitDisplacement_p_J[i], LimitDisplacement_n_J[i]);
		DesiredCoordinate_Head[i] = 0.0;
	}
	
	DesiredCoordinate_Head[2] = 0.6 + 0.055;

	torsoInitialized = true;
	_setMode(0);

	robotState = ERobotControllerStatus::EDisconnected;
	threadStart = true; 

	logString("Finished Initing Torso\n");

	return true;
}



///-----Function calculating a joint displacement form ServoMotor.AngularDisplacement.
double torsoController::CalcJointDisplacement(
	TexART_ServoMotor2015 *ServoMotor,  // Interface object for the ServoMotor.
	double LimitDisplacement_p_J,       // Positive limit of Joint Displacement.     [rad or m]
	double LimitDisplacement_n_J)       // Negative limit of Joint Displacement.     [rad or m]
{
	double JointDisplacement;

	JointDisplacement = (LimitDisplacement_p_J - LimitDisplacement_n_J)
		/ (ServoMotor->ReadLimitAngularDisplacement_p() - ServoMotor->ReadLimitAngularDisplacement_n())
		* (ServoMotor->AngularDisplacement - ServoMotor->ReadLimitAngularDisplacement_n())
		+ LimitDisplacement_n_J;

	return JointDisplacement;
}

///-----Function calculating a ServoMotor.AngularDisplacement from joint displacement.
double torsoController::CalcMotorDisplacement(
	TexART_ServoMotor2015 *ServoMotor,  // Interface object for the ServoMotor.
	double JointDisplacement,
	double LimitDisplacement_p_J,       // Positive limit of Joint Displacement.     [rad or m]
	double LimitDisplacement_n_J)       // Negative limit of Joint Displacement.     [rad or m]
{
	double MotorDisplacement;

	MotorDisplacement = (ServoMotor->ReadLimitAngularDisplacement_p() - ServoMotor->ReadLimitAngularDisplacement_n())
		/ (LimitDisplacement_p_J - LimitDisplacement_n_J)
		* (JointDisplacement - LimitDisplacement_n_J)
		+ ServoMotor->ReadLimitAngularDisplacement_n();

	return MotorDisplacement;
}

///-----Function adapting to a complex limitaion.
// cos(*AngularDisplacement_1) * cos(*AngularDisplacement_2) >= cos(AngularDisplacement_limit)
void torsoController::AdaptComplexLimitation(
	double *AngularDisplacement_1,      // 1st joint rotation. [rad]
	double *AngularDisplacement_2,      // 2nd joint rotation. [rad]
	double AngularDisplacement_limit)   // Limit Angle         [rad]
{
	double s1, c1, s2, c2, c2_d, c3;    // parameters.

	c1 = cos(*AngularDisplacement_1);
	c2 = cos(*AngularDisplacement_2);
	c3 = cos(AngularDisplacement_limit);

	if (c1 * c2 >= c3) return; // Satisfy the complex limitation.

	s1 = sin(*AngularDisplacement_1);
	s2 = sin(*AngularDisplacement_2);
	c2_d = sqrt((s1*s1*c2*c2 + s2*s2*c3*c3) / (s1*s1*c2*c2 + s2*s2));

	if (*AngularDisplacement_2 >= 0.0)
		*AngularDisplacement_2 = acos(c2_d);
	else
		*AngularDisplacement_2 = -acos(c2_d);

	if (*AngularDisplacement_1 >= 0.0)
		*AngularDisplacement_1 = acos(c3 / c2_d);
	else
		*AngularDisplacement_1 = -acos(c3 / c2_d);
}


///-----Function calculating inverse kinematics of TORSO; Roll-Pitch-Yaw type.
void torsoController::InverseKinematicsTORSO_RPY(
	double G_X,           // X-coordinate of point Glabella [m]
	double G_Y,           // Y-coordinate of point Glabella [m]
	double G_Z,           // Z-Coordinate of point Glabella [m]
	double Roll,          // Roll  of head                  [rad]
	double Pitch,         // Pitch of head                  [rad]
	double Yaw,           // Yaw   of head                  [rad]
	int    *quadrant,     // Code presenting quadrant of J6 rotaiton
	double gamma,         // Virtual turning displacement   [rad]
	double *Theta_1,      // AngularDisplacement of J1      [rad]
	double *Theta_2,      // AngularDisplacement of J2      [rad]
	double *d_3,          // LinearDisplacement  of J3      [m]
	double *Theta_4,      // AngularDisplacement of J4      [rad]
	double *Theta_5,      // AngularDisplacement of J5      [rad]
	double *Theta_6)      // AngularDisplacement of J6      [rad]
{
	double a_X, a_Y, a_Z, // Coordinate of vector a.
		b_X, b_Y, b_Z, // Coordinate of vector b.
		c_X, c_Y, c_Z, // Coordinate of vector c.
		N_X, N_Y, N_Z, // Coordinate of point Neck.
		a_X_d, a_Y_d, a_Z_d,
		b_X_d, b_Y_d, b_Z_d,
		c_X_d, c_Y_d, c_Z_d,
		N_X_d, N_Y_d, N_Z_d,
		N,
		c_1, s_1,     // cos(*Theta_1), sin(*Theta_1)
		c_2, s_2,     // cos(*Theta_2), sin(*Theta_2)
		//       c_4,  s_4,     // cos(*Theta_4), sin(*Theta_4)
		c_5, s_5,     // cos(*Theta_5), sin(*Theta_5)
		//       c_24, s_24,    // cos(*Theta_2 + *Theta_4), sin(*Theta_2 + *Theta_4)
		//       alfa,
		c_g, s_g;     // cos(gamma), sin(gamma)

	a_X = sin(Pitch);
	a_Y = -sin(Roll)*cos(Pitch);
	a_Z = cos(Roll)*cos(Pitch);
	b_X = cos(Pitch)*cos(Yaw);
	b_Y = cos(Roll)*sin(Yaw) + sin(Roll)*sin(Pitch)*cos(Yaw);
	b_Z = sin(Roll)*sin(Yaw) - cos(Roll)*sin(Pitch)*cos(Yaw);
	c_X = -cos(Pitch)*sin(Yaw);
	c_Y = cos(Roll)*cos(Yaw) - sin(Roll)*sin(Pitch)*sin(Yaw);
	c_Z = sin(Roll)*cos(Yaw) + cos(Roll)*sin(Pitch)*sin(Yaw);
	N_X = G_X - 0.055*a_X;
	N_Y = G_Y - 0.055*a_Y;
	N_Z = G_Z - 0.055*a_Z;

	// Virtual turning.
	c_g = cos(gamma);
	s_g = sin(gamma);

	a_X_d = c_g*a_X - s_g*a_Y;
	a_Y_d = s_g*a_X + c_g*a_Y;
	a_Z_d = a_Z;
	b_X_d = c_g*b_X - s_g*b_Y;
	b_Y_d = s_g*b_X + c_g*b_Y;
	b_Z_d = b_Z;
	c_X_d = c_g*c_X - s_g*c_Y;
	c_Y_d = s_g*c_X + c_g*c_Y;
	c_Z_d = c_Z;
	N_X_d = c_g*N_X - s_g*N_Y;
	N_Y_d = s_g*N_X + c_g*N_Y;
	N_Z_d = N_Z;

	N = sqrt(N_X_d*N_X_d + N_Y_d*N_Y_d + N_Z_d*N_Z_d);
	*d_3 = N - 0.6;
	*Theta_2 = asin(N_X_d / N);
	c_2 = cos(*Theta_2);
	s_2 = sin(*Theta_2);
	*Theta_1 = asin(-N_Y_d / (N*c_2));
	c_1 = cos(*Theta_1);
	s_1 = sin(*Theta_1);
	*Theta_5 = asin(-a_Y_d*c_1 - a_Z_d*s_1);
	c_5 = cos(*Theta_5);
	s_5 = sin(*Theta_5);
	*Theta_4 = asin(a_X_d / c_5) - *Theta_2;

	/*     c_4 = cos(*Theta_4);
	s_4 = sin(*Theta_4);
	c_24 = c_2*c_4 - s_2*s_4;
	s_24 = s_2*c_4 + c_2*s_4;
	alfa = atan2(c_24, s_24*s_5);
	Theta_6 = asin(b_X_d / sqrt(s_24*s_24*s_5*s_5 + c_24*c_24)) - alfa; */

	// Seitch quadrant.
	if (*quadrant == 0) {
		// cout << "quadrant = 0 \n";
		*Theta_6 = asin((c_1*b_Y_d + s_1*b_Z_d) / c_5);
		if (*Theta_6 >  50.0 / 180.0*PI) *quadrant = 1;
		else if (*Theta_6 < -50.0 / 180.0*PI) *quadrant = -1;
	}
	else if (*quadrant == 1) {
		//cout << "quadrant = 1 \n";
		*Theta_6 = acos((c_1*c_Y_d + s_1*c_Z_d) / c_5);
		if (*Theta_6 > 140.0 / 180.0*PI) *quadrant = 2;
		else if (*Theta_6 <  40.0 / 180.0*PI) *quadrant = 0;
	}
	else if (*quadrant == 2) {
		//cout << "quadrant = 2 \n";
		*Theta_6 = asin((c_1*b_Y_d + s_1*b_Z_d) / c_5) * -1.0 + PI;
		if (*Theta_6 > 228.0 / 180.0*PI) *Theta_6 = 228.0 / 180.0*PI;
		else if (*Theta_6 < 130.0 / 180.0*PI) *quadrant = 1;
	}
	else if (*quadrant == -1) {
		//cout << "quadrant = -1 \n";
		*Theta_6 = acos((c_1*c_Y_d + s_1*c_Z_d) / c_5) * -1.0;
		if (*Theta_6 < -140.0 / 180.0*PI) *quadrant = -2;
		else if (*Theta_6 >  -40.0 / 180.0*PI) *quadrant = 0;
	}
	else if (*quadrant == -2) {
		//cout << "quadrant = -2 \n";
		*Theta_6 = asin((c_1*b_Y_d + s_1*b_Z_d) / c_5) * -1.0 - PI;
		if (*Theta_6 < -228.0 / 180.0*PI) *Theta_6 = -228.0 / 180.0*PI;
		else if (*Theta_6 > -130.0 / 180.0*PI) *quadrant = -1;
	}
}

int torsoController::motorSafeShutdown(){
	logString("torsoController::motorSafeShutdown()\n");
	for (int i = 0; i < 6; i++)
		ServoMotor[i].TorqueCut();

	texArt_ncord->Contact();

	return true;
}


int torsoController::motorForceShutdown(){
	logString("torsoController::motorForceShutdown()\n");
	texArt_ncord->Free();
	delete texArt_ncord;
	texArt_ncord = 0;

	exit(0);

	return true;
}











int torsoController::controlStateMachine(){

	controlLoop++;

	if (softCurrentSense()){
		overCurrent = true;
		_setMode(4);
		m_impl->mcWriteBuf.torso.overCurrent = 1;
	}
	else{
		overCurrent = false;
		m_impl->mcWriteBuf.torso.overCurrent = 0;
	}
		

	softInterlockProcess();



	if (!torsoInitialized)
	{
		if (tuningEnabled){
			displayEncoderValues();
			for (int i = 0; i < 6; i++) ServoMotor[i].CalcPIDControl();
			for (int i = 0; i < 6; i++) ServoMotor[i].TorqueCut();
			texArt_ncord->ContactPeriodic();

			Sleep(10);
		}
		else
			return false;
	}
			

	if (controlLoop > 10000){
		ServoMotor[0].CaliAngularDisplacementByPotentio();
		ServoMotor[1].CaliAngularDisplacementByPotentio();
		ServoMotor[3].CaliAngularDisplacementByPotentio();
		ServoMotor[4].CaliAngularDisplacementByPotentio();
		ServoMotor[5].CaliAngularDisplacementByPotentio();

		controlLoop = 0;
	}



	switch (ControlMode) {
		case 0:		//parking mode
			robotState = EDisconnected;

			ServoMotor[0].Fix2(AngularDisplacement_goal_M[0]);
			ServoMotor[1].Fix2(AngularDisplacement_goal_M[1]);
#ifdef J3Enabled
			ServoMotor[2].Fix2(AngularDisplacement_goal_M[2]);
#endif
			ServoMotor[3].Fix2(AngularDisplacement_goal_M[3]);
			ServoMotor[4].Fix2(AngularDisplacement_goal_M[4]);
			ServoMotor[5].Fix2(AngularDisplacement_goal_M[5]);

			goto contactNCord;
			break;

		case 1:		//Keyboard CoordinateSpace Simulate
			robotState = EConnected;
				// D_X
			if (GetAsyncKeyState('1') & 0x8000) DesiredCoordinate_Head[0] += StepCoordinate_Head[0];
			else if (GetAsyncKeyState('Q') & 0x8000) DesiredCoordinate_Head[0] -= StepCoordinate_Head[0];
			// D_Y
			else if (GetAsyncKeyState('2') & 0x8000) DesiredCoordinate_Head[1] += StepCoordinate_Head[1];
			else if (GetAsyncKeyState('W') & 0x8000) DesiredCoordinate_Head[1] -= StepCoordinate_Head[1];
			// D_Z
			else if (GetAsyncKeyState('3') & 0x8000) DesiredCoordinate_Head[2] += StepCoordinate_Head[2];
			else if (GetAsyncKeyState('E') & 0x8000) DesiredCoordinate_Head[2] -= StepCoordinate_Head[2];
			// Roll
			else if (GetAsyncKeyState('4') & 0x8000) DesiredCoordinate_Head[3] += StepCoordinate_Head[3];
			else if (GetAsyncKeyState('R') & 0x8000) DesiredCoordinate_Head[3] -= StepCoordinate_Head[3];
			// Pitch
			else if (GetAsyncKeyState('5') & 0x8000) DesiredCoordinate_Head[4] += StepCoordinate_Head[4];
			else if (GetAsyncKeyState('T') & 0x8000) DesiredCoordinate_Head[4] -= StepCoordinate_Head[4];
			// Yaw
			else if (GetAsyncKeyState('6') & 0x8000) DesiredCoordinate_Head[5] += StepCoordinate_Head[5];
			else if (GetAsyncKeyState('Y') & 0x8000) DesiredCoordinate_Head[5] -= StepCoordinate_Head[5];

			// limit DesiredCoordinate_Head.
			if (DesiredCoordinate_Head[3] >  (25.0 + 25.0) / 180 * PI) DesiredCoordinate_Head[3] = (25.0 + 25.0) / 180 * PI;
			else if (DesiredCoordinate_Head[3] < -(25.0 + 25.0) / 180 * PI) DesiredCoordinate_Head[3] = -(25.0 + 25.0) / 180 * PI;
			if (DesiredCoordinate_Head[4] >  (40.0 + 25.0) / 180 * PI) DesiredCoordinate_Head[4] = (40.0 + 25.0) / 180 * PI;
			else if (DesiredCoordinate_Head[4] < -(40.0 + 25.0) / 180 * PI) DesiredCoordinate_Head[4] = -(40.0 + 25.0) / 180 * PI;
			if (DesiredCoordinate_Head[5] >       225.0 / 180 * PI) DesiredCoordinate_Head[5] = 225.0 / 180 * PI;
			else if (DesiredCoordinate_Head[5] <      -225.0 / 180 * PI) DesiredCoordinate_Head[5] = -225.0 / 180 * PI;

			InverseKinematicsTORSO_RPY(DesiredCoordinate_Head[0], DesiredCoordinate_Head[1], DesiredCoordinate_Head[2],
				DesiredCoordinate_Head[3], DesiredCoordinate_Head[4], DesiredCoordinate_Head[5],
				&quadrant, gamma,
				&(DesiredDisplacement_J[0]), &(DesiredDisplacement_J[1]), &(DesiredDisplacement_J[2]),
				&(DesiredDisplacement_J[3]), &(DesiredDisplacement_J[4]), &(DesiredDisplacement_J[5]));

			if (DesiredDisplacement_J[0] > (19.0)*PI / 180.0f)
				DesiredDisplacement_J[0] = 19.0*PI / 180.0f;
			else if (DesiredDisplacement_J[0] < (-19.0)*PI / 180.0f)
				DesiredDisplacement_J[0] = -19.0*PI / 180.0f;

			break;

		case 2:		// fade connect

			robotState = EConnecting;

			for (int i = 0; i<6; i++)
				AngularDisplacement_start_M[i] = ServoMotor[i].AngularDisplacement;	// note: angular displacement is in Deg

			for (int i = 0; i<6; i++)
				DesiredDisplacement_J[i] = 0.0; 

			for (int i = 0; i<6; i++)
				AngularDisplacement_goal_M[i] = CalcMotorDisplacement(&(ServoMotor[i]), DesiredDisplacement_J[i], LimitDisplacement_p_J[i], LimitDisplacement_n_J[i]);

			ServoMotor[1].ZeroLimitAngularDisplacement(); // Cancel limitation by AngularDisplacement of J2.
			ServoMotor[3].ZeroLimitAngularDisplacement(); // Cancel limitation by AngularDisplacement of J4.
			Time_start = MothorCLK.Elapsed();

			while (0
				+ ServoMotor[0].GoNextPositionOnTime2(AngularDisplacement_start_M[0], AngularDisplacement_goal_M[0], 2.0, Time_start, MothorCLK.Elapsed())
				+ ServoMotor[1].GoNextPositionOnTime2(AngularDisplacement_start_M[1], AngularDisplacement_goal_M[1], 2.0, Time_start, MothorCLK.Elapsed())
#ifdef J3Enabled
				+ ServoMotor[2].GoNextPositionOnTime2(AngularDisplacement_start_M[2], AngularDisplacement_goal_M[2], 2.0, Time_start, MothorCLK.Elapsed())
#endif
				+ ServoMotor[3].GoNextPositionOnTime2(AngularDisplacement_start_M[3], AngularDisplacement_goal_M[3], 2.0, Time_start, MothorCLK.Elapsed())
				+ ServoMotor[4].GoNextPositionOnTime2(AngularDisplacement_start_M[4], AngularDisplacement_goal_M[4], 2.0, Time_start, MothorCLK.Elapsed())
				+ ServoMotor[5].GoNextPositionOnTime2(AngularDisplacement_start_M[5], AngularDisplacement_goal_M[5], 2.0, Time_start, MothorCLK.Elapsed())
			> 0)
			{
				texArt_ncord->ContactPeriodic();
			}

			for (int i = 0; i<6; i++) ServoMotor[i].ReloadLimitAngularDisplacement(); // Reload Limit Angles

			if (manualMode)
				_setMode(1);
			else
				_setMode(3);

			break;

		case 3:		// realtime mode

			robotState = EConnected; 

			//Realtime data verify overshoot
			DesiredCoordinate_Head[0] = torsoHeadPos[0];     // X-coordinate of point Glabella.	 Start at 0.0 [m]
			DesiredCoordinate_Head[1] = torsoHeadPos[1];	 // Y-coordinate of point Glabella.  Start at 0.0[m]
#ifdef J3Enabled
			DesiredCoordinate_Head[2] = torsoHeadPos[2];	 // Z-coordinate of point Glabella.  Start at 0.655 [m]		
#else 
			DesiredCoordinate_Head[2] = 0;
#endif
			DesiredCoordinate_Head[3] = torsoHeadOri[0];     // Roll  of head.					 Start at 0.0 [rad]
			DesiredCoordinate_Head[4] = torsoHeadOri[1];     // Pitch  of head.					 Start at 0.0 [rad]
			DesiredCoordinate_Head[5] = torsoHeadOri[2];     // Yaw  of head.					 Start at 0.0 [rad]		
			gamma = 0.0;		// Virtual turning displacement.	Start at 0.0[rad]
			
			
			// initial J3 value
#define  IK_height  0.655

			//DesiredCoordinate_Head[2] = DesiredCoordinate_Head[2] + 0.6 + 0.055;
			DesiredCoordinate_Head[2] = DesiredCoordinate_Head[2] + IK_height;



			// limit DesiredCoordinate_Head.
			if (DesiredCoordinate_Head[3] >  (25.0 + 25.0) / 180 * PI) DesiredCoordinate_Head[3] = (25.0 + 25.0) / 180 * PI;
			else if (DesiredCoordinate_Head[3] < -(25.0 + 25.0) / 180 * PI) DesiredCoordinate_Head[3] = -(25.0 + 25.0) / 180 * PI;
			if (DesiredCoordinate_Head[4] >  (40.0 + 25.0) / 180 * PI) DesiredCoordinate_Head[4] = (40.0 + 25.0) / 180 * PI;
			else if (DesiredCoordinate_Head[4] < -(40.0 + 25.0) / 180 * PI) DesiredCoordinate_Head[4] = -(40.0 + 25.0) / 180 * PI;
			if (DesiredCoordinate_Head[5] >       225.0 / 180 * PI) DesiredCoordinate_Head[5] = 225.0 / 180 * PI;
			else if (DesiredCoordinate_Head[5] < -225.0 / 180 * PI) DesiredCoordinate_Head[5] = -225.0 / 180 * PI;

			// Inverse Kinematics
			InverseKinematicsTORSO_RPY(
				DesiredCoordinate_Head[0],
				DesiredCoordinate_Head[1],
				DesiredCoordinate_Head[2],
				DesiredCoordinate_Head[3],
				DesiredCoordinate_Head[4],
				DesiredCoordinate_Head[5],
				&quadrant,
				gamma,
				&(DesiredDisplacement_J[0]),
				&(DesiredDisplacement_J[1]),
				&(DesiredDisplacement_J[2]),
				&(DesiredDisplacement_J[3]),
				&(DesiredDisplacement_J[4]),
				&(DesiredDisplacement_J[5]));


			// Limit Joint Space
			if ((m_impl->mcReadBuf.interlock.torso_ybm_collison == 1) && (DesiredDisplacement_J[0] >= 19.0*PI / 180.0f))
				DesiredDisplacement_J[0] = 19.0*PI / 180.0f;
	

			break; 


		case 4:		// fade disconnect
			disconnect_lbl:
			robotState = EDisconnecting;

			for (int i = 0; i<6; i++)
				AngularDisplacement_start_M[i] = ServoMotor[i].AngularDisplacement;

			AngularDisplacement_goal_M[0] = CalcMotorDisplacement(&(ServoMotor[0]), 0.0 / 180.0*PI, LimitDisplacement_p_J[0], LimitDisplacement_n_J[0]);
			AngularDisplacement_goal_M[1] = CalcMotorDisplacement(&(ServoMotor[1]), 0.0 / 180.0*PI, LimitDisplacement_p_J[1], LimitDisplacement_n_J[1]);
#ifdef J3Enabled
			AngularDisplacement_goal_M[2] = CalcMotorDisplacement(&(ServoMotor[2]), 0.00, LimitDisplacement_p_J[2], LimitDisplacement_n_J[2]);
#else 
			AngularDisplacement_goal_M[2] = 0;
#endif
			AngularDisplacement_goal_M[3] = CalcMotorDisplacement(&(ServoMotor[3]), 45.0 / 180.0*PI, LimitDisplacement_p_J[3], LimitDisplacement_n_J[3]);
			AngularDisplacement_goal_M[4] = CalcMotorDisplacement(&(ServoMotor[4]), 0.0 / 180.0*PI, LimitDisplacement_p_J[4], LimitDisplacement_n_J[4]);
			AngularDisplacement_goal_M[5] = CalcMotorDisplacement(&(ServoMotor[5]), 0.0 / 180.0*PI, LimitDisplacement_p_J[5], LimitDisplacement_n_J[5]);

			ServoMotor[1].ZeroLimitAngularDisplacement(); // Cancel limitation by AngularDisplacement of J2.
			ServoMotor[3].ZeroLimitAngularDisplacement(); // Cancel limitation by AngularDisplacement of J4.
			Time_start = MothorCLK.Elapsed();

			while (0
				+ ServoMotor[0].GoNextPositionOnTime2(AngularDisplacement_start_M[0], AngularDisplacement_goal_M[0], 3.0, Time_start, MothorCLK.Elapsed())
				+ ServoMotor[1].GoNextPositionOnTime2(AngularDisplacement_start_M[1], AngularDisplacement_goal_M[1], 3.0, Time_start, MothorCLK.Elapsed())
#ifdef J3Enabled
				+ ServoMotor[2].GoNextPositionOnTime2(AngularDisplacement_start_M[2], AngularDisplacement_goal_M[2], 3.0, Time_start, MothorCLK.Elapsed())
#endif
				+ ServoMotor[3].GoNextPositionOnTime2(AngularDisplacement_start_M[3], AngularDisplacement_goal_M[3], 3.0, Time_start, MothorCLK.Elapsed())
				+ ServoMotor[4].GoNextPositionOnTime2(AngularDisplacement_start_M[4], AngularDisplacement_goal_M[4], 3.0, Time_start, MothorCLK.Elapsed())
				+ ServoMotor[5].GoNextPositionOnTime2(AngularDisplacement_start_M[5], AngularDisplacement_goal_M[5], 3.0, Time_start, MothorCLK.Elapsed())
			> 0)
			{
				texArt_ncord->ContactPeriodic();
			}

			if (overCurrent){
				_setMode(5); // Immediate Shtdown
				logString("Over Current Shutdown\r\n");
			}
				
			else if (ControlMode==4)
				_setMode(0);// keep parking
			else robotState = EDisconnected;

			break;

		case 5:
			if (robotState != EDisconnected)
			{
				printf("Disconnecting first\n");
				robotState = EDisconnecting;
				goto disconnect_lbl;
			}
			robotState = EStopping;
			motorSafeShutdown();
			torsoInitialized = false;
			robotState = EStopped;
			threadStart = false;
			goto contactNCord;


			break;
		}


	//-----[Step12] Adapt to complex limitaions.
	AdaptComplexLimitation(&(DesiredDisplacement_J[0]), &(DesiredDisplacement_J[1]), 25.0 / 180 * PI);
	AdaptComplexLimitation(&(DesiredDisplacement_J[3]), &(DesiredDisplacement_J[4]), 40.0 / 180 * PI);

	//-----[Step13] Check DesiredDisplacement_J by LimitAngularDisplacement of each ServoMotor.
	for (int i = 0; i<6; i++){
		if (DesiredDisplacement_J[i] > ServoMotor[i].ReadLimitAngularDisplacement_p() / GearRatio_J[i])
			DesiredDisplacement_J[i] = ServoMotor[i].ReadLimitAngularDisplacement_p() / GearRatio_J[i];
		else if (DesiredDisplacement_J[i] < ServoMotor[i].ReadLimitAngularDisplacement_n() / GearRatio_J[i])
			DesiredDisplacement_J[i] = ServoMotor[i].ReadLimitAngularDisplacement_n() / GearRatio_J[i];
	}

	//-----[Step14] Set Refernce_D and calculate commands of PID control for each ServoMotor.
	for (int i = 0; i<6; i++){
		ServoMotor[i].SetReference_D(CalcMotorDisplacement(&(ServoMotor[i]), DesiredDisplacement_J[i], LimitDisplacement_p_J[i], LimitDisplacement_n_J[i]));
		ServoMotor[i].CalcPIDControl();
	}


	int j = 0;

	for (int i = 0; i < 6; i++){
		if (i != 2)
			robotJointData[j] = DesiredDisplacement_J[i] * 180 / PI;
		else
			robotJointData[j] = DesiredDisplacement_J[i];

		robotJointData[j + 1] = ServoMotor[i].AngularDisplacement;

		j += 2;
	}
 
contactNCord:
	texArt_ncord->ContactPeriodic();




	return true; 

}


int torsoController::printRealtimeInfo(){

	LoopCount++;

	if (Display == 1 && LoopCount % 1000 == 0){
		Console::locate(0, 6);
		logString("LoopCount = %4d x 1000\n", LoopCount / 1000);
		logString("Virtual turning; gamma = %8.4f [deg]\n", gamma / PI*180.0);
		for (int i = 0; i < 6; i++)  cout << "DesiredCoordinate_Head[" << i << "] = " << DesiredCoordinate_Head[i] << " \n";
		for (int i = 0; i < 6; i++)  cout << "DesiredDisplacement_J[" << i << "] = " << DesiredDisplacement_J[i] << " \n";
	}

	return LoopCount;

}



int torsoController::displayEncoderValues(){

	Console::locate(0, 12);

	for (int i = 0; i < 2; i++)
		logString("J[%d]\t 0x % 05X\t %4.2lf\t %4.2lf \n", i+1 , (int)ServoMotor[i].ReadPotentioValue(),
		ServoMotor[i].ReadAngularDisplacement_potentio() / GearRatio_J[i] / PI * 180,
		ServoMotor[i].ReadAngularDisplacement_encoder() / GearRatio_J[i] / PI * 180);
	logString("J[%d]\t 0x % 05X\t %4.2lf\t %4.2lf \n", 2, 0, 0, 
		ServoMotor[2].ReadAngularDisplacement_encoder() / GearRatio_J[2] / PI * 180);
	for (int i = 3; i < 6; i++)
		logString("J[%d]\t 0x % 05X\t %4.2lf\t %4.2lf \n", i+1, (int)ServoMotor[i].ReadPotentioValue(),
		ServoMotor[i].ReadAngularDisplacement_potentio() / GearRatio_J[i] / PI * 180,
		ServoMotor[i].ReadAngularDisplacement_encoder() / GearRatio_J[i] / PI * 180);

	return true; 

}



// Error code to Error message mapping
TORSO_ERRMAP t_errmap[] = {
	// red alert error list 0x2000 ~ 0x20FF
	{ 0x2000, "Forward(45°) ≦ Xyris", "前方45°≦機体角度"},		
	{ 0x2001, "Backward 40°≦ Xyris", "後方40°≦機体角度" },
	{ 0x2002, "Forward(41°) ≦ Xyris", "前方41°≦機体角度" },
	{ 0x2003, "Left(42°) ≦ Xyris", "左に42°≦機体角度" },

	// yellow alert error list 0x2100 ~ 0x21FF
	{ 0x2100, "(Forward)40° ≦ Xyris＜45°", "前方40°≦機体角度＜前方45°" },
	{ 0x2101, "(Forward)36° ≦ Xyris＜40°", "後方36°≦機体角度＜後方40°" },
	{ 0x2102, "(Right)36°≦ Xyris ＜41°", "右に36°≦機体角度＜右に41°" },
	{ 0x2103, "(Left)37°≦ Xyris ＜42°", "左に37°≦機体角度＜左に42°" },

	// green alert error list 0x2200 ~ 0x22FF
	{ 0x2200, "Backwards(36°)＞Xyris ＜Front(40°)", "後方36°＞機体角度＜前方40°" },
	{ 0x2201, "Left(37°)＞ Xyris ＜ Right(36°)", "左に37°＞機体角度＜右に36°" },

	{ NULL, NULL }
};
