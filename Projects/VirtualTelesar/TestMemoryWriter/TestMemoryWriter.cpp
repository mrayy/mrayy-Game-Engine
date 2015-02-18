// TestMemoryWriter.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "shmem.h"
#include <windows.h>
#include <conio.h>
#include <iostream>


shmem*m_agent=0;

float mult=1;

char leftPosMap[]=
{
	'1','2','3','4','5','6','7'
};
char leftNegMap[]=
{
	'q','w','e','r','t','y','u'
};
char rightPosMap[]=
{
	'!','@','#','$','%','^','&'
};
char rightNegMap[]=
{
	'Q','W','E','R','T','Y','U'
};
char BodyPosMap[]=
{
	's','d','f','g','h','j','k','l','p'
};
char BodyNegMap[]=
{
	'S','D','F','G','H','J','K','L','P'
};
int findIndex(char *arr,char c,int len)
{
	for(int i=0;i<len;++i)
		if(arr[i]==c)
			return i;
	return -1;
}
void ProcessKey(char c,float dt)
{
	float da=0.01*mult*dt/3.1415f;//400*3.1415f*dt/180.0f;
	int index;
	float v=0;
	if(c=='+')
	{
		mult*=2;
		v=mult;
	}
	else if(c=='-')
	{
		mult*=0.5f;
		v=mult;
	}
	if((index=findIndex(leftPosMap,c,7))!=-1)
	{
		v=(m_agent->data->joints.rt_arm.left[index]+=da);
		m_agent->data->joints.kin_arm.left[index]=v;
	}else if((index=findIndex(leftNegMap,c,7))!=-1)
	{
		v=(m_agent->data->joints.rt_arm.left[index]-=da);
		m_agent->data->joints.kin_arm.left[index]=v;
	}else if((index=findIndex(rightPosMap,c,7))!=-1)
	{
		v=(m_agent->data->joints.rt_arm.right[index]+=da);
		m_agent->data->joints.kin_arm.right[index]=v;
	}else if((index=findIndex(rightNegMap,c,7))!=-1)
	{
		v=(m_agent->data->joints.rt_arm.right[index]-=da);
		m_agent->data->joints.kin_arm.right[index]=v;
	}else if((index=findIndex(BodyPosMap,c,9))!=-1)
	{
		v=(m_agent->data->joints.rt_body[index]+=da);
		m_agent->data->joints.kin_body[index]=v;
	}else if((index=findIndex(BodyNegMap,c,9))!=-1)
	{
		v=(m_agent->data->joints.rt_body[index]-=da);
		m_agent->data->joints.kin_body[index]=v;
	}
	printf("%f\n",v);
}

int main(int argc, char *argv[])
{
	m_agent=new shmem();
	m_agent->createWrite();
	m_agent->openWrite();
	m_agent->data->status.calibration=true;
	m_agent->data->joints.kin_body[0]=m_agent->data->joints.rt_body[0]=0;//-60*3.1415/180.0;
	m_agent->data->joints.kin_body[1]=m_agent->data->joints.rt_body[1]=-60*3.1415/180.0;
	m_agent->data->joints.kin_body[3]=m_agent->data->joints.rt_body[3]=60*3.1415/180.0;


	while (true) {
		/* do some work */
		if(kbhit())
		{
			ProcessKey(getch(),0.01);
		}
		Sleep(10);
	}
	m_agent->Detach();
	delete m_agent;
}

