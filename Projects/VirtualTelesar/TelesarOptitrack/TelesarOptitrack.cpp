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
	printf("Local IP Address:%s\n", ipaddr.c_str());
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

class OptiTrackDataWriter :public animation::IOptiTrackClientListener
{
	core::string m_fileName;
	OS::IStreamPtr m_file;
	OS::StreamWriter m_writer;
	bool m_isActive;
public:
	OptiTrackDataWriter()
	{
		m_isActive = false;
	}
	~OptiTrackDataWriter()
	{
		Stop();
	}
	void Start(core::string file)
	{
		m_fileName = file;
		m_file = gFileSystem.openFile(m_fileName, OS::TXT_WRITE);
		m_writer.setStream(m_file);
		std::stringstream str;
		str << "Name" << "\t" << "Position X" << "\t" << "Position Y" << "\t" << "Position Z" << "\t"
			<< "Angles X" << "\t" << "Angles Y" << "\t" << "Angles Z" << "\n";
		m_writer.writeString(str.str());
		m_isActive = true;
	}
	void Stop()
	{
		m_isActive=false;
		m_file->close();
	}
	void Pause()
	{
		printf("Writing data Paused\n");
		m_isActive = false;
	}
	void Resume()
	{
		printf("Resume writing data\n");
		m_isActive = true;
	}

	bool IsActive(){ return m_isActive; }
	virtual void OnOptiTrackData(animation::OptiTrackClient*client, const animation::OptiTrackRigidBody& body)
	{
		//		printf("Opti Data:%s:%s\n",g_targetNames[body.GetID()].c_str(),core::StringConverter::toString(body.GetPosition()).c_str());

		if (!m_isActive)
			return;

		math::vector3d angles;
		body.GetOrintation().toEulerAngles(angles);

		std::stringstream str;

		str << body.GetName() << "\t" << body.GetPosition().x << "\t" << body.GetPosition().y << "\t" << body.GetPosition().z << "\t"
			<< angles.x << "\t" << angles.y << "\t" << angles.z << "\n";
		m_writer.writeString(str.str());
	}
};

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
	core::string serverIp = ipaddr;
	int port=1234;
	if(argc<4)
	{
		printf("Usage: %s [OptiTrack IP] [IP Address] [port] \n",argv[0]);
	}
	if (argc >= 2)
	{
		serverIp = argv[1];
	}
	if(argc>=3)
	{
		pcIp=argv[2];
	}
	if(argc>=4)
	{
		port=atoi(argv[3]);
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

	OptiTrackDataWriter* writer = new OptiTrackDataWriter();
	writer->Start("WorkingSpace.txt");
	writer->Pause();

	Communicator::Instance()->Connec(pcIp,1234,"TelesarV");
	{
		animation::OptiTrackClient* tracker=new animation::OptiTrackClient();
		if(!tracker->Connect(animation::Opti_Multicast,serverIp,ipaddr,"239.255.38.99"))
		{
			printf("Failed to connect to opti-Track\n");
		}
		else{
			printf("Connected to OptiTrack Server!\n");
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

		printf("Starting Listening.\n");

		// send scheme after we inited our targets
		Communicator::Instance()->SendScheme();

		tracker->AddListener(&gListener);
		tracker->AddListener(writer);
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
				case '0':
					if (writer->IsActive())
						writer->Pause();
					else 
						writer->Resume();
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
	writer->Stop();
	delete writer;
	delete e;
	delete Communicator::Instance();

	return 0;
}

