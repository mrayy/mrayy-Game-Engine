
#include "stdafx.h"
#pragma warning(X:4005)

#include "torsoController.h"
#include "movingAverage.h"
#include "TorsoRobotDLL.h"

#include "RealTorso.h"
#include "CPUTimer.h"
#include "interfaceboard.h"
#include "variables.h"
#include <conio.h>
#include <direct.h>

#include <stdio.h>
#include <stdlib.h>


#define GetCurrentDir _getcwd
using namespace std;
bool threadStart = false;
bool initDone = false; 
bool isDone = false;
bool upCount = true;
bool userConnected = false;




class torsoControllerImpl
{
public:
	MovAvg *mvRobot[6];		// 6 DoF moving avarege 

	ITelubeeRobotListener* listener;

	torsoController* owner;
	torsoControllerImpl(torsoController* o)
	{
		owner = o;
		listener = 0;
	}
	void NotifyCollision(float l,float r)
	{
		if (listener)
		{
			listener->OnCollisionData(owner,l,r);
		}
	}
};



torsoController::torsoController()
{

	if (GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)))
	{
		int len = strlen(cCurrentPath);
		cCurrentPath[len] = '\\'; /* not really required */
		cCurrentPath[len + 1] = '\0'; /* not really required */

	}

	m_connectFlag = false;
	//m_isConnected = false;
	robotState = ERobotControllerStatus::EStopped;
	memset(m_headPos, 0, sizeof(m_headPos));
	memset(m_offset, 0, sizeof(m_offset));

	m_impl = new torsoControllerImpl(this);
	m_impl->listener = 0;
	m_robotStatusProvider = 0;

	memset(targetRotMat, 0, sizeof(targetRotMat));
	targetRotMat[0] = targetRotMat[5] = targetRotMat[10] = targetRotMat[15] = 1;

	//for (int i = 0; i < 6; i++){ 
		//m_impl->mvRobot[BASE][i] = new MovAvg();
		//m_impl->mvRobot[HEAD][i] = new MovAvg();
	//}
	threadStart = false;
	m_calibrated = false;
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)timerThreadHead, this, NULL, NULL);
	//CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)&timerThreadBase, this, NULL, NULL); 

	m_state = EIdle;
}

torsoController::~torsoController()
{
	DisconnectRobot();
	delete m_impl;
	isDone = true;
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

// 
// bool torsoController::IsConnected()
// {
// 	
// 	return m_isConnected;
// }

void torsoController::DisconnectRobot()
{
	m_connectFlag = false;
	return;
}

void torsoController::_processData()
{
	if (m_robotStatusProvider)
	{
		RobotStatus st;
		m_robotStatusProvider->GetRobotStatus(st);
	}
}
void torsoController::InitializeRobot(IRobotStatusProvider* robotStatusProvider)
{
	if (m_state == EInitlize || m_calibrated)
		return;
	m_robotStatusProvider = robotStatusProvider;
	realTorso.SetProvider(robotStatusProvider);
	m_state = EInitlize;
	return;
}

void swap(float &a, float &b)
{
	float t = a;
	a = b;
	b = t;
}

void torsoController::ConvertToMatrix(const Quaternion& q, const float* pos, double* mat)
{
	Matrix m;
	/*
	float euler[3];
	qtoeuler(q, euler[0], euler[1], euler[2]);

	printf("%0.3f, %0.3f, %0.3f\n", euler[0], euler[1], euler[2]);

	double tmp1[16];
	double tmp2[16];
	double tmp3[16];
	double tmp[16];
	MakeMatrix(axisX, euler[0], 0, 0, 0, tmp1);
	MakeMatrix(axisY, euler[1], 0, 0, 0, tmp2);
	MultMatrix4(tmp, tmp1, tmp2);
	MakeMatrix(axisZ, euler[2], 0, 0, 0, tmp1);
	MultMatrix4(mat, tmp, tmp1);
	*/
	qtomatrix(m, q);


	//added by yamen: include head position in the matrix
	m[3][0] = pos[0];
	m[3][1] = pos[1];
	m[3][2] = pos[2];


#if 1
	mat[0] = m[0][0];
	mat[1] = m[1][0];
	mat[2] = m[2][0];
	mat[3] = 0;

	mat[4] = m[0][1];
	mat[5] = m[1][1];
	mat[6] = m[2][1];
	mat[7] = 0;

	mat[8] = m[0][2];
	mat[9] = m[1][2];
	mat[10] = m[2][2];
	mat[11] = 0;
	

#elif 1


	mat[0] = m[0][0];
	mat[1] = m[0][1];
	mat[2] = m[0][2];
	mat[3] = 0;

	mat[4] = m[1][0];
	mat[5] = m[1][1];
	mat[6] = m[1][2];
	mat[7] = 0;

	mat[8] =  m[2][0];
	mat[9] =  m[2][1];
	mat[10] = m[2][2];
	mat[11] = 0;

#endif

	mat[12] = m[3][0];
	mat[13] = m[3][1];
	mat[14] = m[3][2];
	mat[15] = 1;
	/*
#define SWAP(x,y) swap(mat[x],mat[y])

	SWAP(1, 11);
	SWAP(2, 7);
	SWAP(5, 10);*/


}

void torsoController::UpdateRobotStatus(const RobotStatus& st)
{
	Quaternion targetQuat;
	targetQuat[0] = st.headRotation[0];
	targetQuat[1] = st.headRotation[3];
	targetQuat[2] = st.headRotation[1];
	targetQuat[3] = -st.headRotation[2];

	m_headPos[0] = -st.headPos[2];
	m_headPos[1] = -st.headPos[0];
	m_headPos[2] = st.headPos[1];

	//printf("HEAD POSITION = %f,%f,%f\n", m_headPos[0], m_headPos[1], m_headPos[2]);

	float p[3];
	p[0] = m_headPos[0] + m_offset[0];
	p[1] = m_headPos[1] + m_offset[1];
	p[2] = m_headPos[2] + m_offset[2];
	
	if (st.connected)
		userConnected = true;
	else
		userConnected = true;


	ConvertToMatrix(targetQuat, p, targetRotMat);

}


void torsoController::SetListener(ITelubeeRobotListener* l)
{
	m_impl->listener = l;
}



void torsoController::ConnectRobot()
{
	m_connectFlag = true;
}


DWORD torsoController::timerThreadHead(torsoController *robot, LPVOID pdata){
	int count = 0;
	while (!isDone){
		if (robot->m_calibrated)
			robot->_processData();

		if (threadStart){
			robot->_innerProcessRobot();
		}

		Sleep(100);
	}

	return 0;
}


DWORD torsoController::timerThreadBase(torsoController *robot, LPVOID pdata){
	int count = 0;
	while (!isDone){
		if (threadStart){


		}
	}

	return 0;
}



void torsoController::_innerProcessRobot()
{

	if (m_state == EInitlize)
	{

		initRobot(false);
		robotState = EDisconnected;
		printf("initialize end\n");
		m_state = EIdle;
	}

	if (!m_calibrated || !userConnected)
		return;
	double tmp_tangles[Torso_DOF] = { 0 }, torque[Torso_DOF] = { 0 };	// �e���|�����ڕW�p�i�[�z��A�g���N�i�[�z��
	double debug_tangles[Torso_DOF], debug_nangles[Torso_DOF];

	bool endflag2,endflag;
	int count;
	int looptime;
	int bufftime;
	char key;
	int ret;

	endflag2 = count = 0;
	looptime = 0.0;
	bufftime = 0;
	///////////////////////////////////////////////////////////////////////////////////
	//////////////////////////  should be moved to different thread ///////////////
	///////////////////////////////////////////////////////////////////////////////////
	printf("Entering real-time mode..\n");
	while (!endflag2){


		printf("�@press [enter] when ready.\n");
		printf("�@press [q] for finish experience.\n");
		count++;
		while (!_kbhit() && !m_connectFlag)
		{
			Sleep(100);
		}
		if (!m_connectFlag)
		{
			key = _getch();
			if (key == 'q' || key == '9') endflag2 = 1;
		}
		if (endflag2 == true)
		{
			continue;;
		}

		endflag = 0;
		robotState = ERobotControllerStatus::EConnecting;
		//m_isConnected = true;
		m_connectFlag = true;

		realTorso.ClearParameter();					// �p�����[�^�Q������
		MainTimer.Start();							// �^�C�}���������E�X�^�[�g
		SetZeroPos();					// �g���\�����p������
		printf("TORSO Initialization.\n");
		printf("press [q] for exit.\n");
		realTorso.GetCounterValue();				// �G���R�[�_�擾(�_�~�[�����A�����}�㏸����̂��߁A�K�v�Ȃ�����)
		ret = FirstMoving();						// �����쓮
		if (ret != 0){
			printf("Initialization stopped.\n");
			endflag = 1;
		}

		printf("Entering real-time mode..\n");
		printf("press [q] for exit.\n");
		robotState = ERobotControllerStatus::EConnected;

		MainTimer.Count();
		//----- ���C�����[�v -----
		while (!endflag){
			looptime = (double)(MainTimer.Elapsed() - bufftime) / 1000000;	// ���[�v�^�C���v��(�m�F�p)
			bufftime = MainTimer.Elapsed();			// ���[�v�^�C���v��(�m�F�p)
			realTorso.GetCounterValue();			// Read Encorder

			realTorso.SetTargetMatrix(targetRotMat);	// Data Aquesition
			realTorso.CalcTargetAngles(tmp_tangles);			// IK Calculation
			realTorso.SetTargetAngles(tmp_tangles);				// IK Solution to the robot

			realTorso.SendToGLprogram();			// GL Check
			realTorso.GetTorques(torque);			// PID Conbtroller
			realTorso.SetDAValue(torque);			// Torque Output

			realTorso.GetTargetAngles(debug_tangles);

#ifdef FILE_OUT_TORSO_JOINT_DATA
			fprintf(fp_torso, "%f, ", looptime);
			fprintf(fp_torso, "%f, %f, %f, %f, %f, %f,,",
				debug_tangles[0], debug_tangles[1], debug_tangles[2],
				debug_tangles[3], debug_tangles[4], debug_tangles[5]);
#endif
			realTorso.GetNowAngles(debug_nangles);
#ifdef FILE_OUT_TORSO_JOINT_DATA
			fprintf(fp_torso, "%f, %f, %f, %f, %f, %f\n",
				debug_nangles[0], debug_nangles[1], debug_nangles[2],
				debug_nangles[3], debug_nangles[4], debug_nangles[5]);
#endif
			if (_kbhit()) { key = _getch(); if (key == 'q' || key == '9') endflag = 1; }
			if (!m_connectFlag)
				endflag = 1;

			//Sleep(1);
			//MainTimer.CountAndWait(500);
		}
		robotState = ERobotControllerStatus::EDisconnecting;
		//m_isConnected = false;

		printf("Exiting real-time mode.\n");
		printf("To kill process, press [e].\n");
		FinishMoving();
		robotState = ERobotControllerStatus::EDisconnected;

		printf("move to next visitor. Visitor [count:%d]\n", count);
	}
	robotState = ERobotControllerStatus::EDisconnected;
	//m_isConnected = false;

	for (int i = 0; i < Torso_DOF; i++) torque[i] = 0.0;
	realTorso.SetDAValue(torque);					// �S�`�����l��0�g���N�o��
}




int torsoController::initRobot(bool debug){
	int  endflag = 0;				
	char sel_key;					
	//threadStart = false;
	std::string paramfile = std::string(cCurrentPath) + "parameterfiles/Parameters.txt";
	printf("Initializing Torso start:%s\n",paramfile.c_str());
	if (realTorso.InitTorso(paramfile.c_str()) != 0) return -1;
	MainTimer.Start();

	if (!debug)
	{
		mainRoutine(AUTOCALIBMODE);
		//mainRoutine(MANUALCALIBMODE); DO NOT USE
		//debugRoutine();

		printf("1. Main routine (AutoCalib) started...\n\n\n");
	}else
	{
		debugRoutine();

		printf("1. debug routine started...\n\n\n");
	}
	printf("Initializing Torso done\n");

	//robotState = ERobotControllerStatus::EStopped;
	initDone = true;
	//threadStart = true;

	return 0;

}


int torsoController::FirstMoving(void){
	double start_theta[Torso_DOF] = { 0 };
	double end_theta[Torso_DOF] = { 0 };
	double tmp_refD[Torso_DOF] = { 0 };
	double torque[Torso_DOF] = { 0 };
	long long int start_time = 0;
	int    i;

	double TimeSpan = TIMESPAN;						// ��A����ɂ����鎞�Ԃ��`[s]

	realTorso.ChangeLimitRange(1);					// ���͈͂������ɂ���Ă���̂ŏ����쓮���ɁA�͈͂������L���Ă���

	//----- �����쓮�̂��߂̎n�_�p�ƏI�_�p�̎擾 -----
	realTorso.GetCounterValue();					// �G���R�[�_�擾
	realTorso.GetNowAngles(start_theta);			// ����ꂽ�p�x���쓮�n�_�p�Ƃ��ă��[�J���ϐ��ɑ��
	start_theta[2] = 0.0;
	realTorso.SetInitialAngles(start_theta);		// �����p�x�̕ۑ�
	realTorso.SetTargetMatrix(targetRotMat);			// adding the input data Matrix

	realTorso.CalcTargetAngles(end_theta);			// �t�^���w�v�Z
	realTorso.SendToGLprogram();					// GL�`�F�b�N�v���O�����փf�[�^���M

	for (i = 0; i<Torso_DOF; i++) torque[i] = 0.0;
	realTorso.SetDAValue(torque);					// �S�`�����l��0�g���N�o��

	start_time = MainTimer.Elapsed();				// �쓮�̂��߂̃^�C���X�p���������߂邽�߂̃^�C�}

	//----- �n�_�p����I�_�p�܂ł�A���I��PD���� -----
	while ((double)(MainTimer.Elapsed() - start_time) / 1000000 < TimeSpan){
		if (_kbhit()) if (_getch() == 'q') {			// �I�������A[q]�L�[�ŏI��
			for (i = 0; i<Torso_DOF; i++) torque[i] = 0.0;
			realTorso.SetDAValue(torque);			// �S�`�����l��0�g���N�o��
			return 1;
		}
		//----- �����ő��c�҂������ɓ������Ƃ��Ă��ŏI�p�x�����킹��悤�ɏ��ADL1������荞�� -----
		realTorso.GetCounterValue();				// �G���R�[�_�擾
		realTorso.SetTargetMatrix(targetRotMat);	// adding the input data Matrix
		realTorso.CalcTargetAngles(end_theta);		// �t�^���w�v�Z

		//----- �e�֐߂��ꎞ�Q�Ɗp�x�ɂ��킹��悤�ɓ��� -----

		for (i = 0; i<Torso_DOF; i++){
			tmp_refD[i] = (end_theta[i] - start_theta[i])	// �e���|�����ȎQ�Ɗp���v�Z
				* (double)(MainTimer.Elapsed() - start_time) / 1000000
				/ TimeSpan + start_theta[i];
		}
		realTorso.SetTargetAngles(tmp_refD);		// �e���|�����Q�Ɗp���Z�b�g����
		realTorso.SetTargetAngles(tmp_refD);		// �e���|�����Q�Ɗp���Z�b�g����(�_�~�[�A�������ł����Ȃ�̂�h��)
		realTorso.SendToGLprogram();				// GL�o�͊m�F
		realTorso.GetTorques(torque);				// PID����
		realTorso.SetDAValue(torque);				// �g���N�o��
		// ---------------------------------------------------------------------------------------------------
	}

	for (i = 0; i<Torso_DOF; i++) torque[i] = 0.0;
	realTorso.SetDAValue(torque);					// �S�`�����l��0�g���N�o��

	realTorso.ChangeLimitRange(0);					// ���͈͂�߂�

	return 0;
}


// �I���쓮
int torsoController::FinishMoving(void)
{
	double start_theta[Torso_DOF] = { 0 };
	double end_theta[Torso_DOF] = { 0 };
	double tmp_refD[Torso_DOF] = { 0 };
	double torque[Torso_DOF] = { 0 };
	long long int start_time = 0;
	int    i;

	double TimeSpan = TIMESPAN;						// ��A����ɂ����鎞�Ԃ��`[s]

	realTorso.ChangeLimitRange(1);					// �I���쓮���ɁA���͈͂������L���Ă���

	//----- �����쓮�̂��߂̎n�_�p�ƏI�_�p�̎擾 -----
	realTorso.GetCounterValue();					// �G���R�[�_�擾
	realTorso.GetNowAngles(start_theta);			// ����ꂽ�p�x���쓮�n�_�p�Ƃ��ă��[�J���ϐ��ɑ��
	realTorso.GetInitialAngles(end_theta);
	realTorso.SendToGLprogram();					// GL�`�F�b�N�v���O�����փf�[�^���M

	for (i = 0; i<Torso_DOF; i++) torque[i] = 0.0;
	realTorso.SetDAValue(torque);					// �S�`�����l��0�g���N�o��

	start_time = MainTimer.Elapsed();				// �쓮�̂��߂̃^�C���X�p���������߂邽�߂̃^�C�}

	//----- �n�_�p����I�_�p�܂ł�A���I��PD���� -----
	while ((double)(MainTimer.Elapsed() - start_time) / 1000000 < TimeSpan){
		if (_kbhit()) if (_getch() == 'e') {			// �I�������A[e]�L�[�ŏI��(�듮��h�~�̂��߁A�ʃL�[�ɂ���)
			for (i = 0; i<Torso_DOF; i++) torque[i] = 0.0;
			realTorso.SetDAValue(torque);			// �S�`�����l��0�g���N�o��
			return 1;
		}
		realTorso.GetCounterValue();				// �G���R�[�_�擾

		//----- �e�֐߂��ꎞ�Q�Ɗp�x�ɂ��킹��悤�ɓ��� -----
		for (i = 0; i<Torso_DOF; i++){
			tmp_refD[i] = (end_theta[i] - start_theta[i])	// �e���|�����ȎQ�Ɗp���v�Z
				* (double)(MainTimer.Elapsed() - start_time) / 1000000
				/ TimeSpan + start_theta[i];
		}
		realTorso.SetTargetAngles(tmp_refD);		// �e���|�����Q�Ɗp���Z�b�g����
		realTorso.SetTargetAngles(tmp_refD);		// �e���|�����Q�Ɗp���Z�b�g����(�_�~�[�A�������ł����Ȃ�̂�h��)
		realTorso.SendToGLprogram();				// GL�o�͊m�F
		realTorso.GetTorques(torque);				// PID����
		realTorso.SetDAValue(torque);				// �g���N�o��
		// ---------------------------------------------------------------------------------------------------
	}

	//----- �I�_�p�Ɏ�������܂�PD���䑱����(TimeSpan�̐ϐ��͊��o�Őݒ肵�����́CPC�ɂ��ς��) -----
	while ((double)(MainTimer.Elapsed() - start_time) / 1000000 < TimeSpan*1.9){
		if (_kbhit()) if (_getch() == 'e') {			// �I�������A[e]�L�[�ŏI��(�듮��h�~�̂��߁A�ʃL�[�ɂ���)
			for (i = 0; i<Torso_DOF; i++) torque[i] = 0.0;
			realTorso.SetDAValue(torque);			// �S�`�����l��0�g���N�o��
			return 1;
		}
		realTorso.GetCounterValue();				// �G���R�[�_�擾

		//----- �e�֐߂��ꎞ�Q�Ɗp�x�ɂ��킹��悤�ɓ��� -----
		for (i = 0; i<Torso_DOF; i++)	tmp_refD[i] = end_theta[i];
		realTorso.SetTargetAngles(tmp_refD);		// �e���|�����Q�Ɗp���Z�b�g����
		realTorso.SetTargetAngles(tmp_refD);		// �e���|�����Q�Ɗp���Z�b�g����(�_�~�[�A�������ł����Ȃ�̂�h��)
		realTorso.SendToGLprogram();				// GL�o�͊m�F
		realTorso.GetTorques(torque);				// PID����
		// �񂪏k�񂶂Ⴄ���ɑΏ�
		if (realTorso.GetDIOValue(4) == 1) torque[2] = 0.0;
		realTorso.SetDAValue(torque);				// �g���N�o��
		// ---------------------------------------------------------------------------------------------------
	}

	// �񂪐L�т��Ⴄ���ɑΏ�
	int psig = realTorso.GetDIOValue(4);
	while (psig == 0){
		for (i = 0; i<Torso_DOF; i++) torque[i] = 0.0;
		torque[2] = -0.35*2.0;
		realTorso.SetDAValue(torque);
		psig = realTorso.GetDIOValue(4);
	}

	for (i = 0; i<Torso_DOF; i++) torque[i] = 0.0;
	realTorso.SetDAValue(torque);					// �S�`�����l��0�g���N�o��

	realTorso.ChangeLimitRange(0);					// ���͈͂�߂�


	return 0;
}


//---------- PD����A�}�X�^�ɍ��킹�ăX���[�u�𓮂������� ---------------
int torsoController::mainRoutine(int CalibSelect)
{
	printf("Starting main routine\n");
	//m_isConnected = false;
	//robotState = ERobotControllerStatus::EDisconnected;
	m_offset[0] = m_offset[1] = m_offset[2] = 0;
	double tmp_tangles[Torso_DOF] = { 0 }, torque[Torso_DOF] = { 0 };	// �e���|�����ڕW�p�i�[�z��A�g���N�i�[�z��
	int endflag, endflag2;							// �I�������t���O(���C�����[�v/�^�[�����[�v�p)
	int i, ret, count, dbflag, key;
	double looptime;
	long long int bufftime;
	double debug_tangles[Torso_DOF], debug_nangles[Torso_DOF];

	for (i = 0; i<Torso_DOF; i++) torque[i] = 0.0;
	//realTorso.SetDAValue(torque);					// �S�`�����l��0�g���N�o��

	printf("Communication with HMD started ...");
	// data aqusition									// �ڑ������肷��܂ŁA�҂�(�K��)
	printf("success�I\n\n\n");

	printf("[data debug term]\n\n");
	printf("Encoder / Photo Interupter / HMD 1 raw value check\n");
	printf("press enter key\n");
	_getch();
	dbflag = 0;
	double angle[Torso_DOF];
	int photo[Torso_DOF];
	while (dbflag == 0){
		system("cls");
		printf("<Encoder / Photo Switch success>\n\n");
		printf("by moving each axis manually�C\n please check the Value of the encoder / photo switch\n\n");
		realTorso.GetCounterValue();
		realTorso.GetNowAngles(angle);
		for (i = 0; i<Torso_DOF; i++)	printf("Angle[%d]=%lf[rad], Angle[%d]=%lf[deg]\n", i, angle[i], i, angle[i] * 180 / PI);
		printf("\n");
		for (i = 0; i<Torso_DOF; i++){
			photo[i] = realTorso.GetDIOValue(i + 1);
			printf("Photo[%d]=%d\n", i, photo[i]);
		}
		printf("press enter Key for confirm\n");
		if (_kbhit()){ _getch(); dbflag = 1; }
	}
	dbflag = 0;
	double adl1_mat[16];
	while (dbflag == 0){
		system("cls");
		printf("<HMD Confirmed>\n\n");
		printf("Move the HMD \n and confirm the HMD values are correct\n");
		printf("(if the values are close to below, there is no problem)\n\n");
		printf("[reference values]\n");
		printf("0.993401, 0.040188, -0.107418, 0.000000,\n");
		printf("-0.049663, 0.994966, -0.087041, 0.000000,\n");
		printf("0.103379, 0.091801, 0.990397, 0.000000,\n");
		printf("0.267195, -0.032874, 0.237793, 1.000000,\n\n");
		realTorso.GetCounterValue();
		realTorso.GetNowAngles(angle);

		RobotStatus st;
		if (m_robotStatusProvider)
		{
			m_robotStatusProvider->GetRobotStatus(st);
			printf("HMD Angles=%f,%f,%f,%f\n", st.headRotation[0], st.headRotation[1], st.headRotation[2], st.headRotation[3]);
		}

		printf("[read values]\n");
		for (i = 0; i<16; i++){
			printf("%lf, ", targetRotMat[i]);
			if ((i + 1) % 4 == 0) printf("\n");
		}
		printf("press enter Key for confirm\n");
		if (_kbhit()){ _getch(); dbflag = 1; }
	}
	printf("If there is no error, press enter key�Cpress [q] and end the program if there are errors\n");
	key = _getch();
	if (key == 'q' || key == '9'){
		//adl1Control.Stop();
		return -1;
	}
	system("cls");

	// AutoCalibMode
	if (CalibSelect == AUTOCALIBMODE){
		printf("[Entering Autocalibration Routine]\n\n");
		printf("/------------------------------------------------------------------/\n");
		printf("Once calibration is finished, TORSO will move to the HMD position\n");
		printf("make sure the HMD is in correct forward position\n");
		printf("/------------------------------------------------------------------/\n\n\n");
		printf("\nPlease adjust the TORSO posture for calibration\n");
		printf("When ready�Cpress enter key\n");
		_getch();
		printf("\n please turn ON the motor power\n");
		printf("When ready�Cpress enter key\n");
		_getch();
		printf("Auto calibration started...\n");
		realTorso.SetCountOffset();
		ret = realTorso.AutoZeroDetect();				// �����[���_���o�֐�
		if (ret != 0){									// �[���_���o�����I������
			printf("Zero point detection failed\n");
			//adl1Control.Stop();
			return 1;
		}
	}
	//ManualCalibMode
	else if (CalibSelect == MANUALCALIBMODE){
		printf("[Manual Calibration mode]\n\n");
		ret = realTorso.ZeroDetect();					// DIO�ǂݍ��݂ɂ��[���_���o
		if (ret != 0){									// �[���_���o�����I������
			printf("Zero point detection failed\n");
			//adl1Control.Stop();
			return 1;
		}
		printf("Calibration complete\n\n\n");

		printf("\nplease turn ON the motor power\n");
		printf("When ready�Cpress enter key\n");
		_getch();
	}

	m_calibrated = true;

	return 1;
}


//---------- �f�o�O ---------------------------------------------------
int torsoController::debugRoutine(void)
{
	char debugmode;
	int endflag = 0, dbflag = 0;
	int i;

	while (!endflag){
		printf("1. Encoder output confirmation\n");
		printf("2. Photo interrupter confirmation\n");
		printf("3. Zero point confirmation�iauto�j\n");
		printf("4. Zero point confirmation�imanual�j\n");
		printf("5. HMD Pos Orientation check\n");
		printf("6. inverse kinematics calculation confirmation\n");
		printf("7. Motor output confirmation\n");
		printf("q. End debug\n");
		debugmode = _getch();
		if (debugmode == '1'){
			dbflag = 0;
			//-------------- �f�o�b�O�P�F�G���R�[�_�l�̊m�F -------------------
			double angle[Torso_DOF];

			while (dbflag == 0){
				system("cls");
				printf("Encoder value acquisition\n");

				realTorso.GetCounterValue();
				realTorso.GetNowAngles(angle);

				for (i = 0; i<Torso_DOF; i++){
					printf("realLink[%d]=%lf[rad], ", i, angle[i]);
					printf("realLink[%d]=%lf[deg]\n", i, angle[i] * 180 / PI);
				}

				printf("Pess Enter Key to finish\n");
				if (_kbhit()){ _getch(); dbflag = 1; }
			}
		}
		else if (debugmode == '2'){
			dbflag = 0;
			//-------------- �f�o�b�O�Q�F�t�H�g�X�C�b�`�̊m�F -------------------
			int photo[Torso_DOF];

			while (dbflag == 0){
				system("cls");
				printf("Photo switch data acquisition\n");

				for (i = 0; i<Torso_DOF; i++){
					photo[i] = realTorso.GetDIOValue(i + 1);
					printf("Photo[%d]=%d\n", i, photo[i]);
				}

				printf("Pess Enter Key to finish\n");
				if (_kbhit()){ _getch(); dbflag = 1; }
			}
		}
		else if (debugmode == '3'){
			dbflag = 0;
			//-------------- �f�o�b�O�R�F�[���_�l���p�x�m�F�i�����j ------------------
			double angle[Torso_DOF];
			int ret;

			ret = realTorso.AutoZeroDetect();
			if (ret != 0){
				printf("Zero point detection failed\n");
				dbflag = 1;
			}
			while (dbflag == 0){
				system("cls");
				printf("Angle acquisition\n");

				realTorso.GetCounterValue();
				realTorso.GetNowAngles(angle);

				for (i = 0; i<Torso_DOF; i++){
					printf("realLink[%d]=%lf[rad], ", i, angle[i]);
					printf("realLink[%d]=%lf[deg]\n", i, angle[i] * 180 / PI);
				}

				printf("Pess Enter Key to finish\n");
				if (_kbhit()){ _getch(); dbflag = 1; }
			}
		}



		else if (debugmode == '4'){
			dbflag = 0;
			//-------------- �f�o�b�O�S�F�[���_�l���p�x�m�F�i�蓮�j ------------------
			double angle[Torso_DOF];
			int ret;

			ret = realTorso.ZeroDetect();
			if (ret != 0){
				printf("Zero point detection failed\n");
				dbflag = 1;
			}
			while (dbflag == 0){
				system("cls");
				printf("Angle acquisition\n");

				realTorso.GetCounterValue();
				realTorso.GetNowAngles(angle);

				for (i = 0; i<Torso_DOF; i++){
					printf("realLink[%d]=%lf[rad], ", i, angle[i]);
					printf("realLink[%d]=%lf[deg]\n", i, angle[i] * 180 / PI);
				}

				printf("Press Enter Key to exit\n");
				if (_kbhit()){ _getch(); dbflag = 1; }
			}
		}




		else if (debugmode == '5'){
			dbflag = 0;
			//-------------- �f�o�b�O�T�FADL1����̊m�F ------------------
			double adl1_mat[16];

			//adl1Control.Start();
			while (dbflag == 0){
				system("cls");
				printf("ADL1 Operation check\n");

				for (i = 0; i<16; i++){
					printf("%lf, ", targetRotMat[i]);
					if ((i + 1) % 4 == 0) printf("\n");
				}

				printf("\nPx  %04.4f  Py  %04.4f  Pz  %04.4f [mm]\n",
					targetRotMat[12] * 1000.0,
					targetRotMat[13] * 1000.0,
					targetRotMat[14] * 1000.0);

				printf("\nRx  %04.4f  Ry  %04.4f  Rz  %04.4f [deg]\n\n",
					atan2(targetRotMat[9], targetRotMat[10])*180.0 / PI,
					asin(targetRotMat[8])*180.0 / PI,
					-atan2(targetRotMat[4], targetRotMat[0])*180.0 / PI);

				printf("Press Enter Key to exit\n");
				if (_kbhit()){ _getch(); dbflag = 1; }
			}
		}
		else if (debugmode == '6'){
			dbflag = 0;
			//-------------- �f�o�b�O�U�F�t�^���w�v�Z�̊m�F ------------------
			double angles[Torso_DOF];

			while (dbflag == 0){
				system("cls");
				printf("inverse Kinematics Calculation confirmation\n");
				realTorso.SetTargetMatrix(targetRotMat);
				realTorso.CalcTargetAngles(angles);
				for (i = 0; i<Torso_DOF; i++)
					printf("realLink[%d]:%lf\n", i, angles[i]);

				printf("Press Enter Key to exit\n");
				if (_kbhit()){ _getch(); dbflag = 1; }
			}
		}
		else if (debugmode == '7'){
			//-------------- �f�o�b�O�V�FDA�o�͊m�F ------------------
			double torque[Torso_DOF];
			double input;
			char c;
			int channel;

			for (i = 0; i<Torso_DOF; i++) torque[i] = 0.0;
			realTorso.SetDAValue(torque);

			printf("DA Output Confirmation\n");

			for (channel = 0; channel<Torso_DOF; channel++){
				input = 0.0;
				printf("ch. %d Confirmation\n", channel + 1);
				printf("Please enter the torque to be output\n");
				scanf("%lf", &input);
				printf("Entered torque value is %f", input);
				printf("Are you sure you want to output�H(y/n)\n");
				c = _getch();
				if (c == 'y'){
					torque[channel] = input;
					realTorso.SetTorqueDirect(channel, torque[channel]);
					Sleep(2000);
					for (i = 0; i<Torso_DOF; i++) torque[i] = 0.0;
					realTorso.SetDAValue(torque);
				}
			}

		}
		else if (debugmode == 'q') endflag = 1;
		else system("cls");
	}
	return 1;
}


void torsoController::SetZeroPos()
{
	m_offset[0] = -m_headPos[0];
	m_offset[1] = -m_headPos[1];
	m_offset[2] = -m_headPos[2] + 0.7;
}
