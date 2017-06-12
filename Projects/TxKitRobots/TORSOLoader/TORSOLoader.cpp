// TORSOLoader.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "TorsoRobotDLL.h"
#include "IRobotController.h"
#include "vectors.h"
#include <stdlib.h>
#include <thread>

#include <windows.h>
#include <iostream>

#include <cctype>
IRobotController* robot;
RobotStatus status;

bool quit = false;


float normq(Quaternion q)
{
	float len = q[0] * q[0]
		+ q[1] * q[1]
		+ q[2] * q[2]
		+ q[3] * q[3];
	if (len)
	{
		len = sqrt(len);
		q[0] /= len;
		q[1] /= len;
		q[2] /= len;
		q[3] /= len;
	}
	return len;
}
void eulertoq(float pitch, float yaw, float roll, Quaternion& q)
{

	float num = (roll * 0.5f)*DEGREE_TO_RAD;
	float num2 = (float)sin((double)num);
	float num3 = (float)cos((double)num);
	float num4 = (pitch * 0.5f)*DEGREE_TO_RAD;
	float num5 = (float)sin((double)num4);
	float num6 = (float)cos((double)num4);
	float num7 = (yaw * 0.5f)*DEGREE_TO_RAD;
	float num8 = (float)sin((double)num7);
	float num9 = (float)cos((double)num7);
	q[0] = num9 * num6 * num3 + num8 * num5 * num2;
	q[1] = num9 * num5 * num3 + num8 * num6 * num2;
	q[2] = num8 * num6 * num3 - num9 * num5 * num2;
	q[3] = num9 * num6 * num2 - num8 * num5 * num3;
	normq(q);
}

void PrintStatus()
{
	printf("-----------------   Status    -------------------------------\n");
	printf("Robot status: %s\n", robot->IsConnected() ? "Connected" : "Disconnected");
	printf("-------------------------------------------------------------\n");
}

void PrintCommand()
{
	printf("-----------------   Commands List    -----------------------\n");
	printf("Commands List:\n");
	if (robot->IsConnected())
	{
		printf("D- Disconnect\n");

		printf("P- Drive position [x,y,z]\n");
		printf("R- Drive rotation [tilt,yaw,roll]\n");
	}
	else
		printf("C- Connect\n");

	printf("Q- Quit\n");
}

void ProcessCommand(char c)
{
	c = tolower(c);
	switch (c)
	{
	case 'c':
		robot->ConnectRobot();
		status.connected = true;
		break;
	case 'd':
		robot->DisconnectRobot();
		status.connected = false;
		break;
	case 'p':
	{
		if (!robot->IsConnected())
			break;
		float x, y, z;
		printf("Enter Position Values (x,y,z):");
		scanf_s("%f,%f,%f", &x, &y, &z);
		status.headPos[0] = x;
		status.headPos[1] = y;
		status.headPos[2] = z;
	}
	break;
	case 'r':
	{
		if (!robot->IsConnected())
			break;
		float x, y, z;
		printf("Enter Rotation Values (tilt,yaw,roll):");
		scanf_s("%f,%f,%f", &x, &y, &z);

		eulertoq(x, y, z, status.headRotation);
	}
		break;
	case 'q':
		quit = true;
		break;
	default:
		break;
	}
}

void ProcessThread(void* arg)
{
	while (!quit)
	{
		if (robot->IsConnected())
		{
			robot->UpdateRobotStatus(status);
		}
		Sleep(10);
	}
}
int _tmain(int argc, _TCHAR* argv[])
{

	mray::DLL_RobotInit();
	robot = mray::DLL_GetRobotController();

	std::thread process(ProcessThread,(void*) NULL);;
	process.detach();//start robot process

	do 
	{
		PrintStatus();
		PrintCommand();

		printf("Enter Command:");
		
		char c;
		
		do 
		{
			c = getchar();
		} while (c=='\n');
		system("cls");

		ProcessCommand(c);
		printf("\n\n");

	} while (!quit);
	robot->DisconnectRobot();
	mray::DLL_RobotDestroy();
	return 0;
}

