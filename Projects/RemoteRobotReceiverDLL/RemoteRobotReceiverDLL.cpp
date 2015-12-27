// TelubeeRobotDLL.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "RemoteController.h"
#include <stdio.h>
#include "IRobotController.h"
#include "win32NetInterface.h"

int main()
{
	mray::network::createWin32Network();
	RemoteController* controller = new RemoteController();

	while (true)
	{
		if (getchar() == 'q')
		{
			break;
		}
	}
	delete controller;

	return 0;
}