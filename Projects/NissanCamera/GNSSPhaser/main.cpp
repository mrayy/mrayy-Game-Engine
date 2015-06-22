


#include "./include/ubloxM8/ublox_m8.h"
#include <iostream>
#include <cstdio>
#include <windows.h>
#include <conio.h>

//boost libraries
// http://sourceforge.net/projects/boost/files/boost-binaries/1.58.0/boost_1_58_0-msvc-12.0-32.exe/download

ublox_m8::UbloxM8 ublox;

using std::string;
using std::exception;
using std::cout;
using std::cerr;
using std::endl;
using std::vector;

std::ofstream logFile;

inline void NavPosLlhCallback(ublox_m8::NavPosLLH nav_position, double time_stamp);
vector<serial::PortInfo> devices_found;

void enumerate_ports()
{

	vector<serial::PortInfo>::iterator iter = devices_found.begin();

	int i = 0;
	while (iter != devices_found.end())
	{
		serial::PortInfo device = *iter++;

		printf("[%d] - (%s, %s, %s)\n", i,device.port.c_str(), device.description.c_str(),
			device.hardware_id.c_str());

		++i;
	}
}

int main(int argc, char **argv)
{
	devices_found = serial::list_ports();
	enumerate_ports();

	int index = 0;
	printf("Select port number:");
	scanf_s("%d", &index);

	string port(devices_found[index].port);
	if (!logFile.is_open())
	{
		printf("Failed to open log file for writing\n");
		return 0;
	}

	unsigned long baud = 0;
	baud=115200;
	
	if (!ublox.Connect(port, baud))
	{
		printf("Failed to connect to device, Please check port name.\n");
		enumerate_ports();
		return 0;
	}

	logFile.open("DataLog.txt");
	ublox.set_nav_pos_llh_callback(NavPosLlhCallback);


	while (true)
	{
		if (kbhit())
		{
			if (getch() == 'q')
				break;
			Sleep(100);
		}
	}

	logFile.close();
	ublox.Disconnect();

}


//////////////////////////// Callbacks


inline void NavPosLlhCallback(ublox_m8::NavPosLLH nav_position, double time_stamp)
{
	std:: cout << "NAV-POSLLH: " << endl <<
	"  GPS milliseconds: " << nav_position.iTOW << std::endl <<
	"  Latitude: " << nav_position.latitude_scaled << std::endl <<
	"  Longitude: " << nav_position.longitude_scaled << std::endl <<
	"  Height: " << nav_position.height << std::endl << std::endl;

	logFile << time_stamp << "\t" << nav_position.latitude_scaled << "\t" << nav_position.longitude_scaled
		<< "\t" << nav_position.height << "\n";
}
