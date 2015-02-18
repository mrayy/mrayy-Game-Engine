

#include "stdafx.h"
#include "CarController.h"
#include "ofSerial.h"
#include "IThreadManager.h"


namespace mray
{

	int ToBytes(float x,float minV,float maxV,int minB,int maxB)
	{
		float v1=(math::clamp(x,minV,maxV)-minV)/(maxV-minV);
		return math::clamp((int)(v1*(float)(maxB-minB)+minB),minB,maxB);
	}

CarController::CarController(const core::string& portName)
{
	m_serialPort=new ofSerial();
	if(!m_serialPort->setup(portName.c_str(),19200))
		gLogManager.log("CarController: Failed to connect to ComPort:"+portName,ELL_WARNING);
	else
	{
	}
	for(int i=0;i<10;++i)
		m_lastT[i]=0;
	m_thread=OS::IThreadManager::getInstance().createThread(this);
	m_mutex=OS::IThreadManager::getInstance().createMutex();

	m_thread->start(0);
}
CarController::~CarController()
{
	m_thread->terminate();
	while(m_thread->isActive())
	{
		OS::IThreadManager::getInstance().sleep(10);
	}

	delete m_mutex;

	core::string cmd;
	cmd="STP000\r\n";
	m_serialPort->writeBytes((uchar*)cmd.c_str(),cmd.length());
	m_serialPort->close();
	delete m_serialPort;
}

void CarController::execute(OS::IThread*caller,void*arg)
{
	while(caller->isActive())
	{
		if(!m_sendList.empty())
		{
			m_mutex->lock();
			core::string cmd=m_sendList.front();
			m_sendList.pop();
			m_mutex->unlock();

			if(m_serialPort->writeBytes((uchar*)cmd.c_str(),cmd.length())==-1)
			{
				gLogManager.log("CarController:Sending command:"+cmd,ELL_WARNING);
			}

		}
		OS::IThreadManager::getInstance().sleep(10);
	}
}

void CarController::SendCommand(const uchar*buff,int index)
{
	float t=gTimer.getSeconds();
	if(t-m_lastT[index]<50)
		return;
	m_lastT[index]=t;
	m_mutex->lock();
	m_sendList.push((char*)buff);
	m_mutex->unlock();
}

void CarController::SetSpeed(const math::vector2d& speed)
{
	uchar buffer[512];
	core::string cmd;
	int v1;
	int v2;

	if(speed.y>0)
	{
		v1=ToBytes(speed.y,0.0f,100.0f,0,255);
		//cmd1="!FWD";
		sprintf((char*)buffer,"!FWD%03d\r\n",v1);
	}else
	{
		v1=ToBytes(-speed.y,0.0f,100.0f,0,255);
		//cmd1="!REV";
		sprintf((char*)buffer,"!REV%03d\r\n",v1);
		//sprintf((char*)buffer,"!REV%03d\r\n",speedVal);
	}

	cmd=(char*)buffer;
	if(speed.y>0)
	{

		//if(abs(speed.x)>10)
		if(speed.x>0)
		{
			v2=ToBytes(speed.x,0.0f,100.0f,0,100);
			//cmd2="!STL";
			sprintf((char*)buffer,"!STL%03d\r\n",v2);
			//sprintf((char*)buffer,"!STL%03d\r\n",speedVal);
		}else
		{
			v2=ToBytes(-speed.x,0.0f,100.0f,0,100);
			//cmd2="!STR";
			sprintf((char*)buffer,"!STR%03d\r\n",v2);
			//sprintf((char*)buffer,"!STR%03d\r\n",speedVal);
		}
		cmd+=(char*)buffer;
	}
	SendCommand((uchar*)cmd.c_str(),0);

}
void CarController::SetLookingDirection(const math::vector3d& dir)
{
	float yaw=-dir.y;
	float pitch=-dir.x;

	int v1,v2;
	uchar buffer[512];

	v1=ToBytes(yaw,-90,90,0,180);
	v2=ToBytes(pitch,-50,30,5,100);

	sprintf((char*)buffer,"!SV1%03d\r\n!SV2%03d\r\n",v1,v2);

	SendCommand(buffer,1);

}

}
