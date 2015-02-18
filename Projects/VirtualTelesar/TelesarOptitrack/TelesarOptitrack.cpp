// TelesarOptitrack.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "OptiTrackClient.h"
#include <conio.h>

#include "mrayNet.h"
#include <winsock2.h>
#include "WinOSystem.h"
#include "TrackController.h"
#include "WinErrorDialog.h"

#include "GloveController.h"
#include "Communicator.h"

using namespace mray;
using namespace math;
using namespace core;

core::string ipaddr;

std::vector<TrackController*> g_controllers;
core::string g_targetNames[]=
{
	"" ,
	"HeadControl",
	"HandControl"

};

bool getIp()
{
	in_addr ipaddress;
	network::INetwork* net=network::createWin32Network();

	char ac[80];
	if (gethostname(ac, sizeof(ac)) == SOCKET_ERROR) {
		printf("Error when getting local host name \n");
		return false;
	}

	struct hostent *phe = gethostbyname(ac);
	if (phe == 0) {
		printf("Bad host lookup.\n");
		return false;
	}

	for (int i = 0; phe->h_addr_list[i] != 0; ++i) {
		struct in_addr addr;
		memcpy(&addr, phe->h_addr_list[i], sizeof(struct in_addr));
		ipaddress = addr;	
	}
	ipaddr=inet_ntoa(ipaddress);
	return true;
}


class OptiListener:public animation::IOptiTrackClientListener
{
public:
	virtual void OnOptiTrackData(animation::OptiTrackClient*client,const animation::OptiTrackRigidBody& body)
	{
//		printf("Opti Data:%s:%s\n",g_targetNames[body.GetID()].c_str(),core::StringConverter::toString(body.GetPosition()).c_str());

		math::vector3d angles;
		body.GetOrintation().toEulerAngles(angles);
		for(int i=0;i<g_controllers.size();++i)
		{
			g_controllers[i]->OnBodyDataReceived(body.GetID(),body.GetPosition(),angles);
		}
	}
}gListener;


enum EUDPCommand
{
	LoadScene1=0x10,
	LoadScene2,
	LoadScene3,
	LoadScene4,
	LoadScene5
};

struct UDPCommandKey
{
	char key;
	EUDPCommand c;
};

UDPCommandKey UDPCommands[]=
{
	{'1',LoadScene1},
	{'2',LoadScene2},
	{'3',LoadScene3},
	{'4',LoadScene4}
};
int UDPCommandKeyCount=sizeof(UDPCommands)/sizeof(UDPCommandKey);

int _tmain(int argc, _TCHAR* argv[])
{

	if(!getIp())
		return -1;

	core::string pcIp=ipaddr;
	int port=1234;
	if(argc<3)
	{
		printf("Usage: %s [IP Address] [port] \n",argv[0]);
	}

	if(argc>=2)
	{
		pcIp=argv[1];
	}
	if(argc>=3)
	{
		port=atoi(argv[2]);
	}

	printf("*************************************\n");
	printf("           --Keys--\n");
	printf("'r': reset position\n");
	printf("'c': recalibrate gloves\n");
	printf("'s': scan for gloves\n");
	printf("'q': quit\n");
	printf("*************************************\n");


	GloveController RGlove("JointRF");
	GloveController LGlove("JointLF");

	new EventMemoryManager();
	new WinErrorDialog();
	Engine* e=new Engine(new OS::WinOSystem());


	Communicator::Instance()->Connec(pcIp,1234,"TelesarV");
	{
		animation::OptiTrackClient* tracker=new animation::OptiTrackClient();
		if(!tracker->Connect(animation::Opti_Unicast,ipaddr,ipaddr,"239.255.42.99"))
		{
			printf("Failed to connect to opti-Track\n");
		}

		RGlove.Open(FD_HAND_RIGHT);
		LGlove.Open(FD_HAND_LEFT);

		TrackController* c=new TrackController();
		c->Init(1,"RHandControl");
		g_controllers.push_back(c);

		c=new TrackController();
		c->Init(2,"LHandControl");
		g_controllers.push_back(c);

		c=new TrackController();
		c->Init(3,"HeadControl");
		g_controllers.push_back(c);

		RGlove.Init();
		LGlove.Init();


		// send scheme after we inited our targets
		Communicator::Instance()->SendScheme();

		tracker->AddListener(&gListener);
		bool done=false;
		while (!done)
		{
			if(_kbhit())
			{
				char c=getch();
				switch(c)
				{
				case 'q':
					printf("Quitting\n");
					done=true;
					break;
				case 'r':
					printf("Reconnecting\n");
					for(int i=0;i<g_controllers.size();++i)
					{
						g_controllers[i]->reset();
					}
					RGlove.Init();
					LGlove.Init();
					Communicator::Instance()->SendScheme();
					break;
				case 'c':
					printf("Recalibrating\n");
					LGlove.Calibrate();
					RGlove.Calibrate();
					break;
				case 's':
					printf("Rescanning Gloves\n");
					RGlove.Rescan();
					LGlove.Rescan();
					break;
				}
				for(int i=0;i<UDPCommandKeyCount;++i)
				{
					if(c==UDPCommands[i].key)
					{
						printf("Sending Custom Command\n");
						Communicator::Instance()->SendCustomCommand((int)UDPCommands[i].c);
					}
				}
			}
			for(int i=0;i<g_controllers.size();++i)
				g_controllers[i]->Send();

			RGlove.Send();
			LGlove.Send();

			Communicator::Instance()->SendValues();
			Sleep(1);
		}
		tracker->Disconnect();
		delete tracker;
	}
	for(int i=0;i<g_controllers.size();++i)
		delete g_controllers[i];
	g_controllers.clear();
	delete e;
	delete Communicator::Instance();

	return 0;
}

