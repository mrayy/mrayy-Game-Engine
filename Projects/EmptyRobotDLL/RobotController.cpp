

#include "stdafx.h"
#pragma warning(X:4005)

#include "RobotController.h"
#include "serial.h"
#include "movingAverage.h"
#include "EmptyRobotDLL.h"
#include "Point3d.h"
#include "quaternion.h"
#include "matrix4x4.h"
#include "ThreeAxisHead.h"

void logMessage(const char*msg)
{
	printf("%s\n", msg);
}

	// used for serial sending
#define RUN		1
#define STOP	0
#define BASE	0
#define HEAD	1


bool debug_print = false;
bool threadStart = false;
bool isDone = false;
bool upCount = true;

using namespace mray;

//#define ROOMBA_CONTROLLER

class RobotControllerImpl
	{
	public:
		mray::ThreeAxisHead* m_headController;

		std::string m_headPort;

		//Tserial_event *comHEAD;		// Serial Port
		MovAvg *mvRobot[2][3];		// 1 - base, 2 - head moving avarage 

		ITelubeeRobotListener* listener;
		RobotController* m_owner;
		RobotControllerImpl(RobotController* o)
		{
			m_owner = o;
			m_headController = new mray::ThreeAxisHead();
			listener = 0;
		}
		~RobotControllerImpl()
		{
			delete m_headController;
		}
		void NotifyCollision(float l, float r)
		{
			if (listener)
			{
				listener->OnCollisionData(m_owner, l, r);
			}
		}
};



RobotController::RobotController()
{
	m_robotStatusProvider = 0;
	m_impl = new RobotControllerImpl(this);
	m_impl->listener = 0;

	for (int i = 0; i < 3; i++){
		m_impl->mvRobot[BASE][i] = new MovAvg(1);
		m_impl->mvRobot[HEAD][i] = new MovAvg(1);
	}

	m_robotThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)timerThreadRobot, this, NULL, NULL);

	_status = ERobotControllerStatus::EStopped;

}

RobotController::~RobotController()
{
	isDone = true;
	Sleep(100);
	TerminateThread(m_robotThread, 0);
	DisconnectRobot();
	delete m_impl;
}

void RobotController::_processData()
{
	if (m_robotStatusProvider)
	{
		RobotStatus st;
		m_robotStatusProvider->GetRobotStatus(st);
	}
}

DWORD RobotController::timerThreadRobot(RobotController *robot, LPVOID pdata){
	int count = 0;
	while (!isDone){
		robot->_ProcessRobot();
		Sleep(1);
		if (!threadStart)
			Sleep(100);
	}
	return 0;
}

void RobotController::_ProcessRobot()
{
	int ret = 0;

	if (_status!=EDisconnecting || _status!=EDisconnected)
		_processData();

	bool ok = false;
	switch (_status)
	{
	case ERobotControllerStatus::EIniting:
		break;
	case ERobotControllerStatus::EConnecting:
		//CONNECT TO ROBOT HERE
		m_impl->m_headController->Connect("COM4");

		if (ok)
			_status = ERobotControllerStatus::EConnected;
		break;
	case ERobotControllerStatus::EConnected:

		//printf("thread h: %d \r", count++);
		head_control(-pan, tilt, roll);

		break;
	case ERobotControllerStatus::EDisconnecting:
		logMessage("Disconnecting Robot");

		//DISCONNECT TO ROBOT HERE
		m_impl->m_headController->Disconnect();

		_status = EDisconnected;
		break;
	case ERobotControllerStatus::EDisconnected:
		break;
	case ERobotControllerStatus::EStopping:
		_status = ERobotControllerStatus::EStopped;
		break;
	case ERobotControllerStatus::EStopped:
		break;
	}


}
void RobotController::InitializeRobot(IRobotStatusProvider* robotStatusProvider)
{
	m_robotStatusProvider = robotStatusProvider;
	_status = EDisconnected;
}
void RobotController::ConnectRobot()
{
	if (GetRobotStatus() !=EDisconnected)
		return;
	_status = EConnecting;
	logMessage("Connecting Robot\n");
}

void RobotController::DisconnectRobot()
{
	if (_status != EConnected)
		return;
	_status = EDisconnecting;
	logMessage("Disconnecting Robot\n");


}

int RobotController::head_control(float pan, float tilt, float roll){

	m_impl->m_headController->SetRotation(math::vector3d(tilt,pan,roll));

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
void QuaternionToEuler(const math::quaternion quaternion, math::vector3df &euler)
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

	float q0 = quaternion.w;
	float q1 = quaternion.y;
	float q2 = quaternion.x;
	float q3 = quaternion.z;

	euler.y = math::toDeg((float)atan2(2 * (q0 * q1 + q2 * q3), 1 - 2 * (q1*q1 + q2*q2)));
	euler.x = math::toDeg((float)asin(2 * (q0 * q2 - q3 * q1)));
	euler.z = math::toDeg((float)atan2(2 * (q0 * q3 + q1 * q2), 1 - 2 * (q2*q2 + q3*q3)));

}

void RobotController::UpdateRobotStatus(const RobotStatus& st)
{
// 	if (!IsConnected())
// 		return;
	if (GetRobotStatus()==EConnected && !st.connected)
	{
		DisconnectRobot();
	}
	else if (GetRobotStatus() != EConnected && st.connected)
	{
		ConnectRobot();
	}

	if (st.connected)
		threadStart = true;
	else
		threadStart = false;

	if (GetRobotStatus() != EConnected)
		return;

	//todo: send the data to control the robot

	int v_scale = 900;
	int r_scale = 1000;

	float v_size;

	robot_vx = m_impl->mvRobot[BASE][0]->getNext(st.speed[0] * v_scale);
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
	//QuaternionToEuler(q, angles);

	tilt = m_impl->mvRobot[HEAD][1]->getNext(-angles.y);
	pan = m_impl->mvRobot[HEAD][0]->getNext(-angles.z);
	roll = m_impl->mvRobot[HEAD][2]->getNext(-angles.x);

	return;

}


void RobotController::SetListener(ITelubeeRobotListener* l)
{
	m_impl->listener = l;
}
ERobotControllerStatus RobotController::GetRobotStatus()
{
	return _status;
}

void RobotController::ShutdownRobot()
{
	if (_status == EStopped || _status == EStopping)
		return;
	_status = EStopping;
}

bool RobotController::GetJointValues(std::vector<float>& values)
{
	return false;
}

std::string RobotController::ExecCommand(const std::string& cmd, const std::string& args)
{
	return "";
}

void RobotController::ParseParameters(const std::map<std::string, std::string>& valueMap)
{

}
