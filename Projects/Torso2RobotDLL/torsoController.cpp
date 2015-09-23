
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


#define GetCurrentDir _getcwd
using namespace std;

bool threadStart = false;
bool isDone = false;
bool upCount = true;



class torsoControllerImpl
{
public:
	MovAvg *mvRobot[6];		// 6 DoF moving avarage 

	ITelubeeRobotListener* listener;
	torsoController* _c;
	torsoControllerImpl(torsoController* c)
	{
		_c = c;
		listener = 0;
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

	int avgCnt = 3;
	for (int i = 0; i < 3; ++i)
	{
		m_posAvg[i] = new MovAvg(avgCnt);
		m_rotAvg[i] = new MovAvg(avgCnt);
	}

	//change the roll value
	delete m_rotAvg[0];
	m_rotAvg[0] = new MovAvg(20);


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
	//CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)&timerThread2, NULL, NULL, NULL);

	torsoHeadPos[0] = torsoHeadPos[1] = torsoHeadPos[2] = 0;
	torsoHeadOri[0] = torsoHeadOri[1] = torsoHeadOri[2] = torsoHeadOri[3]= 0;

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

	m_caps.enabledMotion.set(false, false, false);
	m_caps.enabledRotation.set(false, false, false);
	m_caps.headLimits[0].set(-180, -180, -180);
	m_caps.headLimits[1].set(180, 180, 180);
}
ERobotControllerStatus torsoController::GetRobotStatus() {
	return robotState;
}


bool torsoController::GetJointValues(std::vector<float>& values){
	values.resize(12);
	for (int i = 0; i<12; i++) 
		values[i]=(robotJointData[i]);

// 	printf("IK values      : %3.2f, %3.2f, %3.2f, %3.2f, %3.2f, %3.2f \n", robotJointData[0], robotJointData[2], robotJointData[4], robotJointData[6], robotJointData[8], robotJointData[10]);
// 	printf("Realtime values: %3.2f, %3.2f, %3.2f, %3.2f, %3.2f, %3.2f \n", robotJointData[1], robotJointData[3], robotJointData[5], robotJointData[7], robotJointData[9], robotJointData[11]);

	return true; 
}


void torsoController::ConnectRobot()
{
	manualMode = false;

	if ((robotState == EDisconnected) && torsoInitialized && ControlMode!=2)
	{
		ControlMode = 2;
		printf("Connecting Robot\n");
	}

	return;
}

void torsoController::ManualControlRobot()
{
	manualMode = true; 

	if ((robotState == EDisconnected) && torsoInitialized && ControlMode != 2)
		ControlMode = 2;

	return;
}

void torsoController::DisconnectRobot()
{
	if (robotState == EConnected) ControlMode = 4;
	
	return;
}

void torsoController::ShutdownRobot(){

	if (robotState == EDisconnected && ControlMode != 5)
		ControlMode = 5;

	return;
}


void torsoController::InitializeRobot(IRobotStatusProvider* robotStatusProvider)
{
	m_robotStatusProvider = robotStatusProvider;
	if (robotState==EStopped)
		torsoInitializatiion = InitializeTorsoRobot(false);
	return;
}
void QuatToEuler(float* q, float* euler)
{/*
 float w, x, y, z;

 w = quaternion.w;
 x = quaternion.x;
 y = quaternion.y;
 z = quaternion.z;

 double sqw = w*w;
 double sqx = x*x;
 double sqy = y*y;
 double sqz = z*z;

 euler.z = (float) math::toDeg(atan2(2.0 * (x*y + z*w), (sqx - sqy - sqz + sqw)) );
 euler.x = (float)math::toDeg(atan2(2.0 * (y*z + x*w), (-sqx - sqy + sqz + sqw)) );
 euler.y = (float)math::toDeg(asin(-2.0 * (x*z - y*w)) );
 */

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
#if 0
void QuatToEuler(float* q, float* euler)
{
// 	math::quaternion qq(q[0], q[1], q[2], q[3]);
// 	return;
	float w = q[0];
	float x = q[1];
	float y = q[2];
	float z = q[3];

	double sqw =w*w;
	double sqx = x*x;
	double sqy = y*y;
	double sqz = z*z;

	euler[0] = (float)(atan2(2.0 * (y*z + x*w), (-sqx - sqy + sqz + sqw)));
	euler[1] = (float)(asin(-2.0 * (x*z - y*w)));
	euler[2] = (float)(atan2(2.0 * (x*y + z*w), (sqx - sqy - sqz + sqw)));
	/*
	// rotation about x-axis
	euler[0] = (float)(atan2(2.0 * (y*z + x*w), (-sqx - sqy + sqz + sqw)));

	// rotation about y-axis
	euler[1] = (float)(asin(2.0 * (x*z + y*w)));

	// rotation about z-axis
	euler[2] = (float)(atan2(2.0 * (x*y + z*w), (sqx - sqy - sqz + sqw)));*/

}
#endif
void torsoController::UpdateRobotStatus(const RobotStatus& st)

{
	Quaternion targetQuat;
// 	targetQuat[0] = st.headRotation[0];
// 	targetQuat[1] = st.headRotation[3];
// 	targetQuat[2] = st.headRotation[1];
// 	targetQuat[3] = -st.headRotation[2];

	targetQuat[0] = st.headRotation[0];
	targetQuat[1] = st.headRotation[3];
	targetQuat[2] = st.headRotation[1];
	targetQuat[3] = st.headRotation[2];

	torsoHeadPos[0] = st.headPos[2];	//Front Axis
	torsoHeadPos[1] = -st.headPos[0];	//Side Axis
	torsoHeadPos[2] = st.headPos[1];	//Up Axis


	float rotTmp[3];
	QuatToEuler(targetQuat, rotTmp);
// 	torsoHeadOri[0] = -rotTmp[0];
// 	torsoHeadOri[1] = -rotTmp[1];
// 	torsoHeadOri[2] = -rotTmp[2];

// 	torsoHeadOri[0] = -rotTmp[2];	// Roll
// 	torsoHeadOri[1] = -rotTmp[0];	// Pitch
//	torsoHeadOri[2] = rotTmp[1];	// Yaw

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
	while (!isDone){
		if (threadStart){
			robot->_processData();
			robot->controlStateMachine();
		}
		else
			Sleep(1);
	}

	return 0;
}


DWORD torsoController::timerThread2(torsoController *robot, LPVOID pdata){
	int count2 = 0;
	while (!isDone){
		if (threadStart){
		}
		else 
			Sleep(1);
	}

	return 0;
}




int torsoController::ReInitializeTorsoRobot(void){

	if (!torsoInitialized)
		return false;
	return true;
}





int torsoController::InitializeTorsoRobot(bool debug){

	if (torsoInitialized)
		return torsoInitializatiion;

	if (robotState != ERobotControllerStatus::EStopped)
		return torsoInitializatiion;

	printf("Initing Robot.\n");
	robotState = ERobotControllerStatus::EIniting;
	threadStart = false;
	Sleep(100);
	torsoInitializatiion = false; 

	char  temp[100], ctemp;
	std::string Name_ParameterFile_M[6];

	ifstream SetupDataFile,                 // Setup data file.
		ParameterFile_M[6];
	/*
	FILE     *OutputDataFile;
	OutputDataFile = fopen("LogLog.dat","wt");
	*/

	MothorCLK.Acquire_Frequency();          // Acquire and set the MothorCLK_Frequency.
	MothorCLK.Start();                      // Acquire and set the MothorCLK_Frequency.


	//-----[Step1] Load setup parameters to LinearArm.
	printf("Loading setup parameters: ");
	std::string path = std::string(cCurrentPath) + "SetUp_Torso.prm";
	SetupDataFile.open(path);
	if (!SetupDataFile) {
		cout << " FileOpenError(SetupDataFile1):" << path << endl;
		return 0;
	}
	else
		printf("done\n");

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

	printf("Initialize NCord Interface: ");
	//-----[Step2] Initialize Torso_02.

	//Added by Yamen: Fix the initialization bug
	int runCounter = 0;
	for (int i = 0; i < 7; ++i)
	{
		printf("Test Run: %d/7\n", i);
		Torso_02 = new TexART_NCord000_Interface(0);
		ret = Torso_02->Initialize();
		if (ret) {
			printf("Initialize error = %d \n", ret);
			motorForceShutdown();
			Sleep(100);
		}
		else
		{
			runCounter++;
			if (runCounter<3)
			{
				printf("Purging data\n");
				motorForceShutdown();
				Sleep(100);
			}
			else
			{
				printf("done.\n");
				break;
			}
		}
	}
	if (!runCounter)
	{
		return false;
	}

	printf("Start NCord Interface: ");
	//-----[Step3] Start the interface object Torso_02.
	ret = Torso_02->Start();
	if (ret)
	{
		printf("Start error = %d \n", ret);
		motorForceShutdown(); // danger overriding device diagnostic
	}
	else
		printf("done.\n");

	printf("Initialize ServoMotor Control: ");
	//-----[Step4] Initialize ServoMotor.
	for (int i = 0; i<6; i++) {
		ParameterFile_M[i].open(Name_ParameterFile_M[i]);
		if (!ParameterFile_M[i].is_open()) {
			cout << " FileOpenError(ParameterFile_M[" << i << "])" << endl;
			motorForceShutdown();
		}
	}
	printf("done.\n");

	ServoMotor = new TexART_ServoMotor2015[6];
	for (int i = 0; i<6; i++) ServoMotor[i].Initialize(ParameterFile_M[i], 0, 0, 0, Torso_02); // Load initial data file.
	for (int i = 0; i<6; i++) ParameterFile_M[i].close();                                      // Close initial data file.

	//-----[Check stage] Search DeadendPotentioValue except J3 by manual motion.
	// [Danger!] Cut the motor power.
	/*cout << "\n Search DeadendPotentioValue of J1 \n"; ServoMotor[0].SearchDeadendPotentioValue();
	cout << "\n Search DeadendPotentioValue of J2 \n"; ServoMotor[1].SearchDeadendPotentioValue();
	cout << "\n Search DeadendPotentioValue of J4 \n"; ServoMotor[3].SearchDeadendPotentioValue();
	cout << "\n Search DeadendPotentioValue of J5 \n"; ServoMotor[4].SearchDeadendPotentioValue();
	cout << "\n Search DeadendPotentioValue of J6 \n"; ServoMotor[5].SearchDeadendPotentioValue();*/

	//switch (ControlMode) {
	//case 0: cout << "Control Mode = 0 : Each joint displacement incremental by keyboard \n\n";  break;
	//case 1: cout << "Control Mode = 1 : Head 6D incremental motion by keyboard \n\n";           break;
	//case 2: cout << "Control Mode = 2 : Head 6D semi-absolute motion by 6D joystick \n\n";      break;
	//case 3: cout << "Control Mode = 3 : Head 6D absolute motion following HMD. (to be determined) \n\n"; break;
	//}
	//cout << "Push  <i>  key to initialize. \nPush <Esc> key to exit from this program \n";

	//-----[Step6] Calibrate AngularDisplacement for present position of each ServoMotor except M3.
	printf("Calibrating Current Angular Displacement Except M3: ");
	ServoMotor[0].CaliAngularDisplacementByPotentio();
	ServoMotor[1].CaliAngularDisplacementByPotentio();
	ServoMotor[3].CaliAngularDisplacementByPotentio();
	ServoMotor[4].CaliAngularDisplacementByPotentio();
	ServoMotor[5].CaliAngularDisplacementByPotentio();
	printf("done.\n");

	//-----[Check stage] Check JointDisplacement by manual motion.
	// [Danger!] Cut the motor power.
	/*
	while(1) {
	for(i=0; i<6; i++) ServoMotor[i].CalcPIDControl();
	for(i=0; i<6; i++) ServoMotor[i].TorqueCut();
	Torso_02->ContactPeriodic();
	Console::locate(0,7);

	cout << "ServoMotor[0].PotentioValue = " << ServoMotor[0].ReadPotentioValue() << " \n";
	cout << "J1.AngularDisplacement_potentio = " << ServoMotor[0].ReadAngularDisplacement_potentio()/GearRatio_J[0]/PI*180 << " [deg]\n";
	cout << "J1.AngularDisplacement_encoder  = " << ServoMotor[0].ReadAngularDisplacement_encoder() /GearRatio_J[0]/PI*180 << " [deg]\n";
	cout << "ServoMotor[1].PotentioValue = " << ServoMotor[1].ReadPotentioValue() << " \n";
	cout << "J2.AngularDisplacement_potentio = " << ServoMotor[1].ReadAngularDisplacement_potentio()/GearRatio_J[1]/PI*180 << " [deg]\n";
	cout << "J2.AngularDisplacement_encoder  = " << ServoMotor[1].ReadAngularDisplacement_encoder() /GearRatio_J[1]/PI*180 << " [deg]\n";
	cout << "ServoMotor[2].PotentioValue = No potentio \n";
	cout << "J3.AngularDisplacement_potentio = " << ServoMotor[2].ReadAngularDisplacement_potentio()/GearRatio_J[2]/PI*180 << " [m]\n";
	cout << "J3.AngularDisplacement_encoder  = " << ServoMotor[2].ReadAngularDisplacement_encoder() /GearRatio_J[2]/PI*180 << " [m]\n";
	cout << "ServoMotor[3].PotentioValue = " << ServoMotor[3].ReadPotentioValue() << " \n";
	cout << "J4.AngularDisplacement_potentio = " << ServoMotor[3].ReadAngularDisplacement_potentio()/GearRatio_J[3]/PI*180 << " [deg]\n";
	cout << "J4.AngularDisplacement_encoder  = " << ServoMotor[3].ReadAngularDisplacement_encoder() /GearRatio_J[3]/PI*180 << " [deg]\n";
	cout << "ServoMotor[4].PotentioValue = " << ServoMotor[4].ReadPotentioValue() << " \n";
	cout << "J5.AngularDisplacement_potentio = " << ServoMotor[4].ReadAngularDisplacement_potentio()/GearRatio_J[4]/PI*180 << " [deg]\n";
	cout << "J5.AngularDisplacement_encoder  = " << ServoMotor[4].ReadAngularDisplacement_encoder() /GearRatio_J[4]/PI*180 << " [deg]\n";
	cout << "ServoMotor[5].PotentioValue = " << ServoMotor[5].ReadPotentioValue() << " \n";
	cout << "J6.AngularDisplacement_potentio = " << ServoMotor[5].ReadAngularDisplacement_potentio()/GearRatio_J[5]/PI*180 << " [deg]\n";
	cout << "J6.AngularDisplacement_encoder  = " << ServoMotor[5].ReadAngularDisplacement_encoder() /GearRatio_J[5]/PI*180 << " [deg]\n";

	cout << "Displacement_J[0] = "
	<< CalcJointDisplacement(&(ServoMotor[0]), LimitDisplacement_p_J[0], LimitDisplacement_n_J[0])/PI*180 << " [deg]      \n";
	cout << "Displacement_J[1] = "
	<< CalcJointDisplacement(&(ServoMotor[1]), LimitDisplacement_p_J[1], LimitDisplacement_n_J[1])/PI*180 << " [deg]      \n";
	cout << "Displacement_J[2] = "
	<< CalcJointDisplacement(&(ServoMotor[2]), LimitDisplacement_p_J[2], LimitDisplacement_n_J[2])        << " [m]        \n"
	<< ServoMotor[2].AngularVelocity                                                                     << " [rad/s]    \n";
	cout << "Displacement_J[3] = "
	<< CalcJointDisplacement(&(ServoMotor[3]), LimitDisplacement_p_J[3], LimitDisplacement_n_J[3])/PI*180 << " [deg]      \n";
	cout << "Displacement_J[4] = "
	<< CalcJointDisplacement(&(ServoMotor[4]), LimitDisplacement_p_J[4], LimitDisplacement_n_J[4])/PI*180 << " [deg]      \n";
	cout << "Displacement_J[5] = "
	<< CalcJointDisplacement(&(ServoMotor[5]), LimitDisplacement_p_J[5], LimitDisplacement_n_J[5])/PI*180 << " [deg]      \n";
	}
	*/
	//-----[Step7] Raise up Torso in 5 seconds, and calibrate AngularDisplacement of ServoMotor M3.
	printf("Raising J3: Calibrating Current Angular Displacement For J3: ");

	AngularDisplacement_start_M[0] = ServoMotor[0].AngularDisplacement;
	AngularDisplacement_start_M[1] = ServoMotor[1].AngularDisplacement;
	AngularDisplacement_start_M[3] = ServoMotor[3].AngularDisplacement;
	AngularDisplacement_start_M[4] = ServoMotor[4].AngularDisplacement;
	AngularDisplacement_start_M[5] = ServoMotor[5].AngularDisplacement;

	AngularDisplacement_goal_M[0] = CalcMotorDisplacement(&(ServoMotor[0]), 0.0, LimitDisplacement_p_J[0], LimitDisplacement_n_J[0]);
	AngularDisplacement_goal_M[1] = CalcMotorDisplacement(&(ServoMotor[1]), 0.0, LimitDisplacement_p_J[1], LimitDisplacement_n_J[1]);
	AngularDisplacement_goal_M[3] = CalcMotorDisplacement(&(ServoMotor[3]), 0.0, LimitDisplacement_p_J[3], LimitDisplacement_n_J[3]);
	AngularDisplacement_goal_M[4] = CalcMotorDisplacement(&(ServoMotor[4]), 0.0, LimitDisplacement_p_J[4], LimitDisplacement_n_J[4]);
	AngularDisplacement_goal_M[5] = CalcMotorDisplacement(&(ServoMotor[5]), 0.0, LimitDisplacement_p_J[5], LimitDisplacement_n_J[5]);

	for (int i = 0; i<6; i++) ServoMotor[i].ZeroLimitAngularDisplacement(); // Cancel limitation by AngularDisplacement.
	ServoMotor[2].ChangePGain_V(0.0008);								// Give PGain_V to M3 for GoDeadEndPosition.
	Time_start = MothorCLK.Elapsed();

	while (0
		+ ServoMotor[0].GoNextPositionOnTime2(AngularDisplacement_start_M[0], AngularDisplacement_goal_M[0], 5.0, Time_start, MothorCLK.Elapsed())
		+ ServoMotor[1].GoNextPositionOnTime2(AngularDisplacement_start_M[1], AngularDisplacement_goal_M[1], 5.0, Time_start, MothorCLK.Elapsed())
		+ ServoMotor[2].GoDeadEndPosition(-0.02*GearRatio_J[2], 0.0001*GearRatio_J[2], 0.2, ServoMotor[2].ReadDeadendAngularDisplacement_n(),
		Time_start, MothorCLK.Elapsed())
		+ ServoMotor[3].GoNextPositionOnTime2(AngularDisplacement_start_M[3], AngularDisplacement_goal_M[3], 5.0, Time_start, MothorCLK.Elapsed())
		+ ServoMotor[4].GoNextPositionOnTime2(AngularDisplacement_start_M[4], AngularDisplacement_goal_M[4], 5.0, Time_start, MothorCLK.Elapsed())
		+ ServoMotor[5].GoNextPositionOnTime2(AngularDisplacement_start_M[5], AngularDisplacement_goal_M[5], 5.0, Time_start, MothorCLK.Elapsed())
	> 0)
	{
		Torso_02->ContactPeriodic();
		if (GetAsyncKeyState(0x1b) & 0x8000) break;
	}
	printf("done. \n");


	for (int i = 0; i<6; i++) ServoMotor[i].ReloadLimitAngularDisplacement(); // Reset limitation by AngularDisplacement.
	ServoMotor[2].ZeroPGain_V();                                       // Zero PGain_V of M3.

	//-----[Step8] Move J3 to neutral position in 5 seconds.
	printf("Move J3 to neutral position in 5 seconds: ");

	AngularDisplacement_start_M[2] = ServoMotor[2].AngularDisplacement;
	AngularDisplacement_goal_M[2] = CalcMotorDisplacement(&(ServoMotor[2]), 0.0, LimitDisplacement_p_J[2], LimitDisplacement_n_J[2]);

	ServoMotor[2].ZeroLimitAngularDisplacement();                      // Cancel limitation by AngularDisplacement of J3.
	Time_start = MothorCLK.Elapsed();

	while (0
		+ ServoMotor[2].GoNextPositionOnTime2(AngularDisplacement_start_M[2], AngularDisplacement_goal_M[2], 5.0, Time_start, MothorCLK.Elapsed())
	> 0)
	{
		ServoMotor[0].Fix2(AngularDisplacement_goal_M[0]);
		ServoMotor[1].Fix2(AngularDisplacement_goal_M[1]);
		ServoMotor[3].Fix2(AngularDisplacement_goal_M[3]);
		ServoMotor[4].Fix2(AngularDisplacement_goal_M[4]);
		ServoMotor[5].Fix2(AngularDisplacement_goal_M[5]);

		Torso_02->ContactPeriodic();
		if (GetAsyncKeyState(0x1b) & 0x8000) break;
	}
	printf("done.\n");

	printf("Move Robot to Parking Positiion..");
	// moving the robot to parked positin
	for (int i = 0; i<6; i++) AngularDisplacement_start_M[i] = ServoMotor[i].AngularDisplacement;

	AngularDisplacement_goal_M[0] = CalcMotorDisplacement(&(ServoMotor[0]), 0.0 / 180.0*PI, LimitDisplacement_p_J[0], LimitDisplacement_n_J[0]);
	AngularDisplacement_goal_M[1] = CalcMotorDisplacement(&(ServoMotor[1]), 30.0 / 180.0*PI, LimitDisplacement_p_J[1], LimitDisplacement_n_J[1]);
	AngularDisplacement_goal_M[2] = CalcMotorDisplacement(&(ServoMotor[2]), 0.0, LimitDisplacement_p_J[2], LimitDisplacement_n_J[2]);
	AngularDisplacement_goal_M[3] = CalcMotorDisplacement(&(ServoMotor[3]), 45.0 / 180.0*PI, LimitDisplacement_p_J[3], LimitDisplacement_n_J[3]);
	AngularDisplacement_goal_M[4] = CalcMotorDisplacement(&(ServoMotor[4]), 0.0 / 180.0*PI, LimitDisplacement_p_J[4], LimitDisplacement_n_J[4]);
	AngularDisplacement_goal_M[5] = CalcMotorDisplacement(&(ServoMotor[5]), 0.0 / 180.0*PI, LimitDisplacement_p_J[5], LimitDisplacement_n_J[5]);

	ServoMotor[1].ZeroLimitAngularDisplacement(); // Cancel limitation by AngularDisplacement of J2.
	ServoMotor[3].ZeroLimitAngularDisplacement(); // Cancel limitation by AngularDisplacement of J4.
	Time_start = MothorCLK.Elapsed();

	while (0
		+ ServoMotor[0].GoNextPositionOnTime2(AngularDisplacement_start_M[0], AngularDisplacement_goal_M[0], 5.0, Time_start, MothorCLK.Elapsed())
		+ ServoMotor[1].GoNextPositionOnTime2(AngularDisplacement_start_M[1], AngularDisplacement_goal_M[1], 5.0, Time_start, MothorCLK.Elapsed())
		+ ServoMotor[2].GoNextPositionOnTime2(AngularDisplacement_start_M[2], AngularDisplacement_goal_M[2], 5.0, Time_start, MothorCLK.Elapsed())
		+ ServoMotor[3].GoNextPositionOnTime2(AngularDisplacement_start_M[3], AngularDisplacement_goal_M[3], 5.0, Time_start, MothorCLK.Elapsed())
		+ ServoMotor[4].GoNextPositionOnTime2(AngularDisplacement_start_M[4], AngularDisplacement_goal_M[4], 5.0, Time_start, MothorCLK.Elapsed())
		+ ServoMotor[5].GoNextPositionOnTime2(AngularDisplacement_start_M[5], AngularDisplacement_goal_M[5], 5.0, Time_start, MothorCLK.Elapsed())
	> 0)
	{
		Torso_02->ContactPeriodic();
	}
	
	printf("done.\n");
	ServoMotor[2].ReloadLimitAngularDisplacement();   // Reset limitation by AngularDisplacement of J3 after every Zero Limit Cancellation
	
	// making sure the Desired Displacement it Initialized to Zero for Keyboard Input
	for (int i = 0; i < 6; i++) {
		DesiredDisplacement_J[i] = CalcJointDisplacement(&(ServoMotor[i]), LimitDisplacement_p_J[i], LimitDisplacement_n_J[i]);
		DesiredCoordinate_Head[i] = 0.0;
	}
	
	DesiredCoordinate_Head[2] = 0.6 + 0.055;

	torsoInitialized = true;
	ControlMode = 0;

	robotState = ERobotControllerStatus::EDisconnected;
	threadStart = true; 

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
	for (int i = 0; i<6; i++) 
		ServoMotor[i].TorqueCut();

	Torso_02->Contact();

	return true;
}


int torsoController::motorForceShutdown(){
	Torso_02->Free();
	delete Torso_02;
	Torso_02 = 0;

	return true;
}



int torsoController::controlStateMachine(){

	if (!torsoInitialized)
		return false;
	

	switch (ControlMode) {
		case 0:		//parking mode
			robotState = EDisconnected;

			ServoMotor[0].Fix2(AngularDisplacement_goal_M[0]);
			ServoMotor[1].Fix2(AngularDisplacement_goal_M[1]);
			ServoMotor[2].Fix2(AngularDisplacement_goal_M[2]);
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
				+ ServoMotor[2].GoNextPositionOnTime2(AngularDisplacement_start_M[2], AngularDisplacement_goal_M[2], 2.0, Time_start, MothorCLK.Elapsed())
				+ ServoMotor[3].GoNextPositionOnTime2(AngularDisplacement_start_M[3], AngularDisplacement_goal_M[3], 2.0, Time_start, MothorCLK.Elapsed())
				+ ServoMotor[4].GoNextPositionOnTime2(AngularDisplacement_start_M[4], AngularDisplacement_goal_M[4], 2.0, Time_start, MothorCLK.Elapsed())
				+ ServoMotor[5].GoNextPositionOnTime2(AngularDisplacement_start_M[5], AngularDisplacement_goal_M[5], 2.0, Time_start, MothorCLK.Elapsed())
			> 0)
			{
				Torso_02->ContactPeriodic();
			}

			for (int i = 0; i<6; i++) ServoMotor[i].ReloadLimitAngularDisplacement(); // Reload Limit Angles

			if (manualMode)
				ControlMode = 1; 
			else
				ControlMode = 3;

			break;

		case 3:		// realtime mode

			robotState = EConnected; 

			//Realtime data verify overshoot
			DesiredCoordinate_Head[0] = torsoHeadPos[0];     // X-coordinate of point Glabella.	 Start at 0.0 [m]
			DesiredCoordinate_Head[1] = torsoHeadPos[1];	 // Y-coordinate of point Glabella.  Start at 0.0[m]
			DesiredCoordinate_Head[2] = torsoHeadPos[2];	 // Z-coordinate of point Glabella.  Start at 0.655 [m]
			DesiredCoordinate_Head[3] = torsoHeadOri[0];     // Roll  of head.					 Start at 0.0 [rad]
			DesiredCoordinate_Head[4] = torsoHeadOri[1];     // Pitch  of head.					 Start at 0.0 [rad]
			DesiredCoordinate_Head[5] = torsoHeadOri[2];     // Yaw  of head.					 Start at 0.0 [rad]		
			gamma = 0.0;		// Virtual turning displacement.	Start at 0.0[rad]
			
			
			// initial J3 value
#define  J3MiddleValue  0.655

			//DesiredCoordinate_Head[2] = DesiredCoordinate_Head[2] + 0.6 + 0.055;
			DesiredCoordinate_Head[2] = DesiredCoordinate_Head[2] + J3MiddleValue;

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


		//	DesiredDisplacement_J[2] = 0;
		//	DesiredDisplacement_J[2] = 0.0;
			break; 


		case 4:		// fade disconnect

			robotState = EDisconnecting;

			for (int i = 0; i<6; i++)
				AngularDisplacement_start_M[i] = ServoMotor[i].AngularDisplacement;

			AngularDisplacement_goal_M[0] = CalcMotorDisplacement(&(ServoMotor[0]), 0.0 / 180.0*PI, LimitDisplacement_p_J[0], LimitDisplacement_n_J[0]);
			AngularDisplacement_goal_M[1] = CalcMotorDisplacement(&(ServoMotor[1]), 30.0 / 180.0*PI, LimitDisplacement_p_J[1], LimitDisplacement_n_J[1]);
			//AngularDisplacement_goal_M[2] = CalcMotorDisplacement(&(ServoMotor[2]), 0.0 / 180.0*PI, LimitDisplacement_p_J[2], LimitDisplacement_n_J[2]);
			AngularDisplacement_goal_M[2] = CalcMotorDisplacement(&(ServoMotor[2]), 0.00, LimitDisplacement_p_J[2], LimitDisplacement_n_J[2]);
			AngularDisplacement_goal_M[3] = CalcMotorDisplacement(&(ServoMotor[3]), 45.0 / 180.0*PI, LimitDisplacement_p_J[3], LimitDisplacement_n_J[3]);
			AngularDisplacement_goal_M[4] = CalcMotorDisplacement(&(ServoMotor[4]), 0.0 / 180.0*PI, LimitDisplacement_p_J[4], LimitDisplacement_n_J[4]);
			AngularDisplacement_goal_M[5] = CalcMotorDisplacement(&(ServoMotor[5]), 0.0 / 180.0*PI, LimitDisplacement_p_J[5], LimitDisplacement_n_J[5]);

			ServoMotor[1].ZeroLimitAngularDisplacement(); // Cancel limitation by AngularDisplacement of J2.
			ServoMotor[3].ZeroLimitAngularDisplacement(); // Cancel limitation by AngularDisplacement of J4.
			Time_start = MothorCLK.Elapsed();

			while (0
				+ ServoMotor[0].GoNextPositionOnTime2(AngularDisplacement_start_M[0], AngularDisplacement_goal_M[0], 5.0, Time_start, MothorCLK.Elapsed())
				+ ServoMotor[1].GoNextPositionOnTime2(AngularDisplacement_start_M[1], AngularDisplacement_goal_M[1], 5.0, Time_start, MothorCLK.Elapsed())
				+ ServoMotor[2].GoNextPositionOnTime2(AngularDisplacement_start_M[2], AngularDisplacement_goal_M[2], 5.0, Time_start, MothorCLK.Elapsed())
				+ ServoMotor[3].GoNextPositionOnTime2(AngularDisplacement_start_M[3], AngularDisplacement_goal_M[3], 5.0, Time_start, MothorCLK.Elapsed())
				+ ServoMotor[4].GoNextPositionOnTime2(AngularDisplacement_start_M[4], AngularDisplacement_goal_M[4], 5.0, Time_start, MothorCLK.Elapsed())
				+ ServoMotor[5].GoNextPositionOnTime2(AngularDisplacement_start_M[5], AngularDisplacement_goal_M[5], 5.0, Time_start, MothorCLK.Elapsed())
			> 0)
			{
				Torso_02->ContactPeriodic();
			}

			ControlMode = 0; // keep parking

			break;

		case 5:
			robotState = EStopping;
			motorSafeShutdown();
			torsoInitialized = false;
			robotState = EStopped;
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

	// add J3 Displacement limits
//  	if (DesiredDisplacement_J[2] > -0.01) DesiredDisplacement_J[2] = -0.01;
// 	if (DesiredDisplacement_J[2] < -0.08) DesiredDisplacement_J[2] = -0.08;
	//DesiredDisplacement_J[2] = 0.0f;
	//-----[Step14] Set Refernce_D and calculate commands of PID control for each ServoMotor.
	for (int i = 0; i<6; i++){
		ServoMotor[i].SetReference_D(CalcMotorDisplacement(&(ServoMotor[i]), DesiredDisplacement_J[i], LimitDisplacement_p_J[i], LimitDisplacement_n_J[i]));
		ServoMotor[i].CalcPIDControl();
	}
 
contactNCord:
	Torso_02->ContactPeriodic();

	int j = 0;

	for (int i = 0; i < 6; i++){
		if (i != 2)
			robotJointData[j] = DesiredDisplacement_J[i] * 180 / PI;
		else
			robotJointData[j] = DesiredDisplacement_J[i];
		robotJointData[j + 1] = ServoMotor[i].AngularDisplacement;
		j+=2;
	}
	
	return true; 

}


int torsoController::printRealtimeInfo(){

	LoopCount++;

	if (Display == 1 && LoopCount % 1000 == 0){
		Console::locate(0, 6);
		printf("LoopCount = %4d x 1000\n", LoopCount / 1000);
		printf("Virtual turning; gamma = %8.4f [deg]\n", gamma / PI*180.0);
		for (int i = 0; i < 6; i++)  cout << "DesiredCoordinate_Head[" << i << "] = " << DesiredCoordinate_Head[i] << " \n";
		for (int i = 0; i < 6; i++)  cout << "DesiredDisplacement_J[" << i << "] = " << DesiredDisplacement_J[i] << " \n";
	}

	return LoopCount;

}

