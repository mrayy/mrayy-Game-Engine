/**
*	NEDO-Obayashi - Data Acquisition Daemon (DAQ)
*	Charith Fernando. (charith@inmojo.com)
*	Nichiha USA, Inc.
*
*	MELSEC Communication Protocol (MC Protocol) Driver Implementation
*	Copyright (c) 2015-2016 Charith Fernando/TachiLab.org.
*
*	2015.11.04.
*
*	This driver implements "MC Protocol" communications as described
*	in Mitsubishi "MELSEC-Q/L MELSEC Communication Protocol Reference
*	Manual", in the following manuals (English & Japanese)...
*	[en] SH(NA)-080008-Q - http://www.meau.com/
*	[jp] SH-08003-V      - http://www.mitsubishielectric.co.jp/
*
*	* Protocol Type 3E (binary) QnA compatible framing
*
**/

#include "StdAfx.h"
#include <stdio.h>
#include <conio.h>
#include "plc_config.h"


using namespace std;
struct in_addr myaddress;



// initialize udp thread for gloveData
MCClient *mc;

mc_buff mcWriteBuf;
mc_buff mcReadBuf;





// ---------------------- Other function definitions -----------------------------------//


bool isnan(double var){
	volatile double d = var;
	return d != d;
}


DWORD_PTR GetNumCPUs(){
	SYSTEM_INFO m_si = { 0, };
	GetSystemInfo(&m_si);
	return (DWORD_PTR)m_si.dwNumberOfProcessors;
}

int getmyip(){
	char ac[80];
	if (gethostname(ac, sizeof(ac)) == SOCKET_ERROR) {
		printf("Error when getting local host name \n");
		return 1;
	}

	struct hostent *phe = gethostbyname(ac);
	if (phe == 0) {
		printf("Bad host lookup.\n");
		return 1;
	}

	for (int i = 0; phe->h_addr_list[i] != 0; ++i) {
		struct in_addr addr;
		memcpy(&addr, phe->h_addr_list[i], sizeof(struct in_addr));
		myaddress = addr;
	}

	return 0;
}




#if 0
// ------------------------------------ Main routine ---------------------------------------- //

void main00(){
	bool bExit = false;

	WSAData wsaData;
	if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0)
		WSACleanup();
	int retval = getmyip();
	printf("Trying to connect to a PLC...\r\n");

	Sleep(100);

	memset(&mcWriteBuf, 0, sizeof(mcWriteBuf)); // initializing test write data
	memset(&mcReadBuf, 0, sizeof(mcReadBuf)); // initializing test read data

	// create connection to Melsec PC via MC Protocol 
	mc = new MCClient(PLC_PORT_TCP_TORSO, MELSEC_PLC);	


	// xyris test data
	mcWriteBuf.xyris.terminal = 1;
	mcWriteBuf.xyris.offline = 0;
	mcWriteBuf.xyris.inOperation = 1;
	mcWriteBuf.xyris.frontAngle = 50;	// max is 255
	mcWriteBuf.xyris.HealthCheckCount = 250;
	mcWriteBuf.xyris.maxTempLeft = 70;
	mcWriteBuf.xyris.battVoltage = 71;
	mcWriteBuf.xyris.battCurrent = 72;
	mcWriteBuf.xyris.HealthCheckCount = 80;

	// torso test data
	mcWriteBuf.torso.J1_rt_angle = 45; 
	mcWriteBuf.torso.J2_rt_angle = 30;
	mcWriteBuf.torso.J3_rt_disp = 80;
	mcWriteBuf.torso.J4_rt_angle = 37;
	mcWriteBuf.torso.J5_rt_angle = 0;
	mcWriteBuf.torso.J6_rt_angle = 67;
	mcWriteBuf.torso.J1_ik_angle = 46;
	mcWriteBuf.torso.J2_ik_angle = 31;
	mcWriteBuf.torso.J3_ik_disp = 81;
	mcWriteBuf.torso.J4_ik_angle = 38;
	mcWriteBuf.torso.J5_ik_angle = 1;
	mcWriteBuf.torso.J6_ik_angle = 68;

	// writing data to PLC
	//mc->batch_write("W", SELECT_XYRIS, 0x00, &mcWriteBuf, 0x20);
	//mc->batch_write("W", SELECT_TORSO, 0xA0, &mcWriteBuf, 0x2D);

	mc->batch_read("W", SELECT_XYRIS, 0x00, &mcReadBuf, 0x20);

	// batch read(device code, head start, readBuffer, read length)
	//mc->batch_read("W", 0x00, &plcReadBuffer, 0x20);

	Sleep(10);



	while(1){
		if(_kbhit()){
			switch(_getch()){
				case 0x1b:
					bExit = true;		
					break;	
 
				default:
					break;
			}

			if(bExit)
				break;
		}

	}


}

#endif