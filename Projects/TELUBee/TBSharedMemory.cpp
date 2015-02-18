#include "stdafx.h"
#include "shmem.h"
#include "TBSharedMemory.h"
#include "StringConverter.h"

#include <stdio.h>

namespace mray
{
namespace TBee
{

	class TBSharedMemoryImpl
	{
		TBSharedMemory* m_owner;
		shmem *agent;

	public:

		TBSharedMemoryImpl(TBSharedMemory* o);
		virtual~TBSharedMemoryImpl();

		void Start();
		virtual void Close();
		int GetSendBuffer(char* buff,int len);

	};


TBSharedMemoryImpl::TBSharedMemoryImpl(TBSharedMemory* o)
{
	m_owner=o;
	agent=new shmem();
}
TBSharedMemoryImpl::~TBSharedMemoryImpl()
{
	Close();
	delete agent;
}

void TBSharedMemoryImpl::Start()
{
	/*agent->openRead();*/
	
	switch(agent->openWrite())
	{
	case -1:
		printf("error.... shared memory creation failed\n");
		break;
	case -2:
		printf("error! mapviewoffile failed\n");
		break;
	}
	agent->data->user.status.online=true;
	agent->data->user.status.connected=true;
	agent->data->robot.status.connected=true;

}
void TBSharedMemoryImpl::Close()
{
	agent->data->robot.status.connected=false;
	agent->data->user.status.connected=false;
	agent->Detach();
}

int TBSharedMemoryImpl::GetSendBuffer(char* buff,int len)
{
	int vald = sprintf(buff,"%s, %d, %f, %f, %f, %f, %f, %f, %f, %f, %d, %d, %d, %d, %d"
		,"yamen"
		,agent->data->user.ipaddress.S_un.S_addr
		,agent->data->user.location.latitude
		,agent->data->user.location.longitude
		,agent->data->user.head.x
		,agent->data->user.head.y
		,agent->data->user.head.z
 		,agent->data->user.head.roll
 		,agent->data->user.head.pitch
 		,agent->data->user.head.yaw
		,agent->data->user.status.connected
		,agent->data->user.status.online
		,agent->data->user.status.home
		,agent->data->user.status.initial
		,agent->data->user.status.reset);
	return vald;
}

TBSharedMemory::TBSharedMemory()
{
	m_impl=new TBSharedMemoryImpl(this);
}
TBSharedMemory::~TBSharedMemory()
{
	delete m_impl;
}

void TBSharedMemory::Start()
{
	m_impl->Start();
}
void TBSharedMemory::Close()
{
	m_impl->Close();
}

int TBSharedMemory::GetSendBuffer(char* buff,int len)
{
	return m_impl->GetSendBuffer(buff,len);
}


}
}