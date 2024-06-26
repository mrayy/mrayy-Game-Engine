

#include "stdafx.h"
#pragma warning(X:4005)

#include "RobotSerialPort.h"
#include "serial.h"
#include "agent.h"
#include "movingAverage.h"
#include "TelubeeRobotDLL.h"
#include "Point3d.h"
#include "quaternion.h"
#include "IBaseController.h"
//#include "RoombaController.h"
//#include "OmniBaseController.h"
#include "ThreeAxisHead.h"
#include "StringUtil.h"
#include "ILogManager.h"
#include "Tserial_event.h"
#include "serial.h"

float testPosx = 100.00;
float testPosy = 100.00; 

	// used for serial sending
#define RUN		1
#define STOP	0
#define BASE	0
#define HEAD	1

#define ROBOT_CENTER 325

bool debug_print = false;
bool threadStart = false;
bool isDone = false;
bool upCount = true;

using namespace mray;


std::vector<core::string> processSerialOutput(const std::string& ret)
{
	std::vector<core::string> lst= core::StringUtil::Split(ret, "@");
	for (int i = 0; i < lst.size(); ++i)
	{
		lst[i] = core::StringUtil::Trim(lst[i], " \t\n\r");
	}
	return lst;
}

//#define ROOMBA_CONTROLLER

class RobotSerialPortImpl
	{
	public:
#ifdef ROOMBA_CONTROLLER
		mray::RoombaController* m_baseController;
#else 
		mray::IBaseController* m_baseController;
#endif
		typedef ThreeAxisHead TXHeadType;
		TXHeadType* m_headController;

		std::string m_headPort;
		std::string m_basePort;

		//Tserial_event *comHEAD;		// Serial Port
		MovAvg *mvRobot[2][3];		// 1 - base, 2 - head moving avarage 

		ITelubeeRobotListener* listener;
		RobotSerialPort* m_owner;
		RobotSerialPortImpl(RobotSerialPort* o)
		{
			m_owner = o;
#ifdef ROOMBA_CONTROLLER
			m_baseController = new mray::RoombaController;
#else 
			m_baseController = 0;// new mray::OmniBaseController;
#endif
			m_headController = new TXHeadType();
			listener = 0;
		}
		~RobotSerialPortImpl()
		{
			delete m_baseController;
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



RobotSerialPort::RobotSerialPort()
{
	m_robotStatusProvider = 0;
	m_impl = new RobotSerialPortImpl(this);
	m_impl->listener = 0;
	baseConnected = 0;
	load_parameters();

	for (int i = 0; i < 3; i++){
		m_impl->mvRobot[BASE][i] = new MovAvg(1);
		m_impl->mvRobot[HEAD][i] = new MovAvg(1);
	}

	m_robotThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)timerThreadRobot, this, NULL, NULL);

	_status = ERobotControllerStatus::EStopped;
	m_headCounter = 0;
	//ConnectRobot();
	_setupCaps();

}

RobotSerialPort::~RobotSerialPort()
{
	isDone = true;
	Sleep(100);
	TerminateThread(m_robotThread, 0);
	DisconnectRobot();
	delete m_impl;
}

void RobotSerialPort::_processData()
{
	if (m_robotStatusProvider)
	{
		RobotStatus st;
		m_robotStatusProvider->GetRobotStatus(st);
	}
}

DWORD RobotSerialPort::timerThreadRobot(RobotSerialPort *robot, LPVOID pdata){
	int count = 0;
	while (!isDone){
		robot->_ProcessRobot();
		Sleep(1);
		if (!threadStart)
			Sleep(100);
	}
	return 0;
}

void RobotSerialPort::_ProcessRobot()
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
		if (_config.BaseEnabled && m_impl->m_baseController)
		{
			if (m_impl->m_basePort == "")
				m_impl->m_basePort = _config.robotCOM;
			ret = m_impl->m_baseController->Connect(m_impl->m_basePort);
			if (ret){

				gLogManager.log("Robot Connected", ELL_INFO);
				if (debug_print)
					printf("Robot Connected!\n", ret);
				ok |= true;
			}
			else{

				gLogManager.log("Robot Failed to connected", ELL_WARNING);
				if (debug_print)
				{
					printf("Robot not connected (%ld)\n", ret);
					printf("Robot baud (%ld)\n", ret);
				}
				m_impl->m_baseController->Disconnect();
			}
		}
		if (_config.HeadEnabled)
		{
			if (m_impl->m_headPort == "")
				m_impl->m_headPort = _config.headCOM;
			ret = m_impl->m_headController->Connect(m_impl->m_headPort);
			if (ret){
				gLogManager.log("Head Connected", ELL_INFO);
				if (debug_print)
					printf("Head Connected!\n", ret);
				ok |= true;
			}
			else{

				gLogManager.log("Head Failed to connected", ELL_WARNING);
				if (debug_print)
					printf("Head not connected (%ld)\n", ret);
				m_impl->m_headController->Disconnect();
			}
		}
		m_baseCounter = 0;
		if (ok)
		{
			_status = ERobotControllerStatus::EConnected;
		}
		break;
	case ERobotControllerStatus::EConnected:

		//printf("thread h: %d \r", count++);
		//if (m_headCounter > 5)
		{
			head_control(pan*_config.yAxis, tilt*_config.xAxis, roll*_config.zAxis);
			m_headCounter = 0;
		}
		m_headCounter++;
		if (m_baseCounter > 120)
		{
			//base_control(robot_vx * _config.xSpeed, robot_vy*_config.ySpeed, robot_rot*_config.Rotation, baseConnected ? RUN : STOP);
			m_baseCounter = 0;
		}
		m_baseCounter++;

		break;
	case ERobotControllerStatus::EDisconnecting:
		gLogManager.log("Disconnecting Robot", ELL_INFO);
		if (debug_print)
			printf("Disconnecting Robot\n", ret);
		if (_config.BaseEnabled && m_impl->m_baseController)
		{
			m_impl->m_baseController->DriveStop();
			m_impl->m_baseController->Disconnect();
		}
		if (_config.HeadEnabled)
			m_impl->m_headController->Disconnect();
		_status = EDisconnected;
		break;
	case ERobotControllerStatus::EDisconnected:
		break;
	case ERobotControllerStatus::EStopping:
		if (m_impl->m_baseController)
			m_impl->m_baseController->Disconnect();
		m_impl->m_headController->Disconnect();
		_status = ERobotControllerStatus::EStopped;
		break;
	case ERobotControllerStatus::EStopped:
		break;
	}
	if (threadStart){
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


}

void RobotSerialPort::_setupCaps()
{
	m_caps.hasBattery = true;
	m_caps.hasDistanceSensors = true;
	m_caps.hasBumpSensors = true;
	m_caps.canMove = true;
	m_caps.canRotate = true;
	m_caps.hasParallaxMotion = false;

	m_caps.distanceSensorCount = 7;
	m_caps.bumpSensorCount = 2;
	m_caps.bodyJointsCount = 2 + 3;	//Base: 2 , Head: 3

//	m_caps.enabledMotion.set(false, false, true);
//	m_caps.enabledRotation.set(false, true, false);
//	m_caps.headLimits[0].set(-180, -180, -180);
//	m_caps.headLimits[1].set(180, 180, 180);
}


std::string RobotSerialPort::ScanePorts()
{
	return "";

	char portName[64];
	vector<serial::PortInfo> devices_found = serial::list_ports();

	vector<serial::PortInfo>::iterator iter = devices_found.begin();

	while (iter != devices_found.end())
	{
		serial::PortInfo device = *iter++;

		serial::Serial port;
		if (false)
		{
			Tserial_event s;
			s.connect((char*)device.port.c_str(), _config.head_baudRate, SERIAL_PARITY_ODD, 8, FALSE, TRUE);
			if (s.isconnected())
			{
				std::string cmd = "#q \n\r";
				s.sendData((char*)cmd.c_str(),cmd.length());//try to stop all logging
				Sleep(300);
				s.disconnect();
			}
		}
		//port.Setup(device.port, _config.head_baudRate);
		try
		{
			port.open();
		}
		catch (serial::SerialException* e)
		{
			printf("%s\n", e->what());
			continue;
		}

		if (port.isOpen()){
			port.write("#q \n\r");
			Sleep(300);
			port.read(port.available());
			port.flushInput();
			port.flushOutput();
			Sleep(100);
			if (port.write("#t \n\r") > 0)
			{
				Sleep(500);
				std::string type = port.read(port.available());
				std::vector<core::string> lst = processSerialOutput(type);
				if (lst.size()>0)
				{
					gLogManager.log("Found device [" + device.port + "] : " +lst[0],ELL_INFO);

					if (lst[0] == "RobotBase")
					{
						m_impl->m_basePort = device.port;
					}
					else if (lst[0] == "RobotHead")
					{
						m_impl->m_headPort = device.port;
					}
				}
			}
			port.close();

		}
	}
	return "";
}
void RobotSerialPort::InitializeRobot(IRobotStatusProvider* robotStatusProvider)
{
	Sleep(500);
	ScanePorts();
	m_robotStatusProvider = robotStatusProvider;
	_status = EDisconnected;
}
void RobotSerialPort::ConnectRobot()
{
	if (GetRobotStatus() !=EDisconnected)
		return;
	_status = EConnecting;
	gLogManager.log("Connecting Robot\n", ELL_INFO);
}

void RobotSerialPort::DisconnectRobot()
{
	if (_status != EConnected)
		return;
	_status = EDisconnecting;
	gLogManager.log("Disconnecting Robot\n", ELL_INFO);


}

int RobotSerialPort::base_control(int velocity_x, int velocity_y, int rotation, int control){

	static int counter = 0;

	if (!_config.BaseEnabled || !m_impl->m_baseController)
		return false;

	if (!m_impl->m_baseController->IsConnected())
		return FALSE;
	if (abs(rotation) < 5)
		rotation = 0;
	if (control == RUN)
		m_impl->m_baseController->Drive(mray::math::vector2di(velocity_x, velocity_y) , rotation );
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



int RobotSerialPort::head_control(float pan, float tilt, float roll){
	if (!_config.HeadEnabled)
		return false;
	//gLogManager.log("Rotation", ELL_INFO);

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

enum RotSeq{ zyx, zyz, zxy, zxz, yxz, yxy, yzx, yzy, xyz, xyx, xzy, xzx };

void twoaxisrot(double r11, double r12, double r21, double r31, double r32, double res[]){
	res[0] = atan2(r11, r12);
	res[1] = acos(r21);
	res[2] = atan2(r31, r32);
}

void threeaxisrot(double r11, double r12, double r21, double r31, double r32, double res[]){
	res[0] = atan2(r31, r32);
	res[1] = asin(r21);
	res[2] = atan2(r11, r12);
}

void quaternion2Euler(const math::quaternion& q, double res[], RotSeq rotSeq)
{
	switch (rotSeq){
	case zyx:
		threeaxisrot(2 * (q.x*q.y + q.w*q.z),
			q.w*q.w + q.x*q.x - q.y*q.y - q.z*q.z,
			-2 * (q.x*q.z - q.w*q.y),
			2 * (q.y*q.z + q.w*q.x),
			q.w*q.w - q.x*q.x - q.y*q.y + q.z*q.z,
			res);
		break;

	case zyz:
		twoaxisrot(2 * (q.y*q.z - q.w*q.x),
			2 * (q.x*q.z + q.w*q.y),
			q.w*q.w - q.x*q.x - q.y*q.y + q.z*q.z,
			2 * (q.y*q.z + q.w*q.x),
			-2 * (q.x*q.z - q.w*q.y),
			res);
		break;

	case zxy:
		threeaxisrot(-2 * (q.x*q.y - q.w*q.z),
			q.w*q.w - q.x*q.x + q.y*q.y - q.z*q.z,
			2 * (q.y*q.z + q.w*q.x),
			-2 * (q.x*q.z - q.w*q.y),
			q.w*q.w - q.x*q.x - q.y*q.y + q.z*q.z,
			res);
		break;

	case zxz:
		twoaxisrot(2 * (q.x*q.z + q.w*q.y),
			-2 * (q.y*q.z - q.w*q.x),
			q.w*q.w - q.x*q.x - q.y*q.y + q.z*q.z,
			2 * (q.x*q.z - q.w*q.y),
			2 * (q.y*q.z + q.w*q.x),
			res);
		break;

	case yxz:
		threeaxisrot(2 * (q.x*q.z + q.w*q.y),
			q.w*q.w - q.x*q.x - q.y*q.y + q.z*q.z,
			-2 * (q.y*q.z - q.w*q.x),
			2 * (q.x*q.y + q.w*q.z),
			q.w*q.w - q.x*q.x + q.y*q.y - q.z*q.z,
			res);
		break;

	case yxy:
		twoaxisrot(2 * (q.x*q.y - q.w*q.z),
			2 * (q.y*q.z + q.w*q.x),
			q.w*q.w - q.x*q.x + q.y*q.y - q.z*q.z,
			2 * (q.x*q.y + q.w*q.z),
			-2 * (q.y*q.z - q.w*q.x),
			res);
		break;

	case yzx:
		threeaxisrot(-2 * (q.x*q.z - q.w*q.y),
			q.w*q.w + q.x*q.x - q.y*q.y - q.z*q.z,
			2 * (q.x*q.y + q.w*q.z),
			-2 * (q.y*q.z - q.w*q.x),
			q.w*q.w - q.x*q.x + q.y*q.y - q.z*q.z,
			res);
		break;

	case yzy:
		twoaxisrot(2 * (q.y*q.z + q.w*q.x),
			-2 * (q.x*q.y - q.w*q.z),
			q.w*q.w - q.x*q.x + q.y*q.y - q.z*q.z,
			2 * (q.y*q.z - q.w*q.x),
			2 * (q.x*q.y + q.w*q.z),
			res);
		break;

	case xyz:
		threeaxisrot(-2 * (q.y*q.z - q.w*q.x),
			q.w*q.w - q.x*q.x - q.y*q.y + q.z*q.z,
			2 * (q.x*q.z + q.w*q.y),
			-2 * (q.x*q.y - q.w*q.z),
			q.w*q.w + q.x*q.x - q.y*q.y - q.z*q.z,
			res);
		break;

	case xyx:
		twoaxisrot(2 * (q.x*q.y + q.w*q.z),
			-2 * (q.x*q.z - q.w*q.y),
			q.w*q.w + q.x*q.x - q.y*q.y - q.z*q.z,
			2 * (q.x*q.y - q.w*q.z),
			2 * (q.x*q.z + q.w*q.y),
			res);
		break;

	case xzy:
		threeaxisrot(2 * (q.y*q.z + q.w*q.x),
			q.w*q.w - q.x*q.x + q.y*q.y - q.z*q.z,
			-2 * (q.x*q.y - q.w*q.z),
			2 * (q.x*q.z + q.w*q.y),
			q.w*q.w + q.x*q.x - q.y*q.y - q.z*q.z,
			res);
		break;

	case xzx:
		twoaxisrot(2 * (q.x*q.z - q.w*q.y),
			2 * (q.x*q.y + q.w*q.z),
			q.w*q.w + q.x*q.x - q.y*q.y - q.z*q.z,
			2 * (q.x*q.z + q.w*q.y),
			-2 * (q.x*q.y - q.w*q.z),
			res);
		break;
	default:
		std::cout << "Unknown rotation sequence" << std::endl;
		break;
	}



	res[0] = math::toDeg(res[0]);
	res[1] = math::toDeg(res[1]);
	res[2] = math::toDeg(res[2]);



}

void RobotSerialPort::UpdateRobotStatus(const RobotStatus& st)
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
	//mray::math::Point3d<double> angles;
	mray::math::quaternion q2(st.headRotation[0], st.headRotation[1], st.headRotation[2], st.headRotation[3]);
	mray::math::quaternion q(q2.w,q2.z,q2.x,q2.y);
	//Matrix rotMat;
	//qtomatrix(rotMat, q);
	//MatrixtoXYZ((double*)rotMat, &angles.x, &angles.y, &angles.z);
	math::vector3d angles;
	//QuaternionToEuler(q, angles); 

	//QuaternionToEulerYZX(q2, angles);
	if (false)
	{
		q.toEulerAngles(angles);
		tilt = m_impl->mvRobot[HEAD][1]->getNext(angles.y);
		pan = m_impl->mvRobot[HEAD][0]->getNext(angles.z);
		roll = m_impl->mvRobot[HEAD][2]->getNext(angles.x);
	}
	else
	{
		double res[3];
		quaternion2Euler(q2, res, RotSeq::yxz);
		//q.toEulerAngles(angles);

		tilt = m_impl->mvRobot[HEAD][1]->getNext(res[1]);//2
		pan = m_impl->mvRobot[HEAD][0]->getNext(res[2]); //0
		roll = m_impl->mvRobot[HEAD][2]->getNext(res[0]);//1
	}

	baseConnected = st.connected;
	return;

}


void RobotSerialPort::SetListener(ITelubeeRobotListener* l)
{
	m_impl->listener = l;
}
ERobotControllerStatus RobotSerialPort::GetRobotStatus()
{
	return _status;
}

void RobotSerialPort::ShutdownRobot()
{
	if (_status == EStopped || _status == EStopping)
		return;
	_status = EStopping;
}

bool RobotSerialPort::GetJointValues(std::vector<float>& values)
{
	values.resize(3*2);
	math::vector3d rot=m_impl->m_headController->GetRotation();
	values[0] = tilt;
	values[1] = -rot.x;
	values[2] = pan;
	values[3] =-rot.y;
	values[4] = roll;
	values[5] = -rot.z;
	return true;
}

std::string RobotSerialPort::ExecCommand(const std::string& cmd, const std::string& args)
{
	if (cmd == CMD_Start)
	{
		if (_config.BaseEnabled && m_impl->m_baseController)
			m_impl->m_baseController->Start();
		return "";
	}
	if (cmd == CMD_Stop)
	{
		if (_config.BaseEnabled && m_impl->m_baseController)
			m_impl->m_baseController->Stop();
		return "";
	}
	 if (cmd == CMD_IsStarted)
	{
		 if (_config.BaseEnabled && m_impl->m_baseController)
			return core::StringConverter::toString(m_impl->m_baseController->IsStarted());
	}
	
	if (cmd == CMD_GetSensorCount)
	{
		if (_config.BaseEnabled && m_impl->m_baseController)
			return core::StringConverter::toString(m_impl->m_baseController->GetSensorCount());

	}
	else if (cmd == CMD_GetSensorValue)
	{
		if (_config.BaseEnabled && m_impl->m_baseController)
			return core::StringConverter::toString(m_impl->m_baseController->GetSensorValue(core::StringConverter::toInt(args)));
	}
	else if (cmd == CMD_GetBatteryLevel)
	{
		if (_config.BaseEnabled && m_impl->m_baseController)
			return core::StringConverter::toString(m_impl->m_baseController->GetBatteryLevel());
	}
	else
		if (m_impl->m_baseController)
		return m_impl->m_baseController->ExecCommand(cmd, args);
	return "";
}

void RobotSerialPort::ParseParameters(const std::map<std::string, std::string>& valueMap)
{

}
