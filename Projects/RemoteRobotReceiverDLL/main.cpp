// TelubeeRobotDLL.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "RemoteControllerReceiver.h"
#include <stdio.h>
#include "IRobotController.h"
#include "win32NetInterface.h"
#include "WinOSystem.h"
#include <conio.h>

int main()
{
	mray::OS::IOSystem* os= new mray::OS::WinOSystem();
	mray::GCPtr<mray::Engine> engine= new mray::Engine(os);
	mray::network::createWin32Network();
	RemoteControllerReceiver* controller = new RemoteControllerReceiver();

	while (true)
	{
		controller->Render();
		Sleep(30);
		if (kbhit())
		{
			if (getchar() == 'q')
			{
				break;
			}
		}
	}
	delete controller;

	return 0;
}