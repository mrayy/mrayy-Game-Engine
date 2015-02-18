/*--------------------------------------------------------------------------*/
// TELESAR V, Communication Agent 
//
// Modifiend by Charith@tachilab.org
//
// WIN32 must be defined when compiling for Windows.
// For Visual C++ this is normally already defined.
//
// Copyright (C) 2012, TACHILAB <www.tachilab.org>
//
/*--------------------------------------------------------------------------*/

#include "multicast.h"
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <conio.h>
#include <time.h>
#include <string.h>
#include "shmem.h"
#ifdef WIN32
#include <windows.h>  // for Sleep
#else
#include <unistd.h>  // for usleep
#endif
//#include <console.h>
//#include <colors.h>

#define RADTODEG	57.2957795131
#define	DEGTORAD	0.0174532925199

struct in_addr myaddress;


char robotipAddress[15];
char serveripAddress[15];
int data_sendTo = 0;

clock_t startT, endT;

using namespace std;







bool isnan(double var){
    volatile double d = var;
    return d != d;
}


DWORD_PTR GetNumCPUs(){
	SYSTEM_INFO m_si = {0, }; 
	GetSystemInfo(&m_si);
	return (DWORD_PTR)m_si.dwNumberOfProcessors; 
}


void load_parameters(){

	const int MAX_CHARS_PER_LINE = 512;
	const int MAX_TOKENS_PER_LINE = 20;
	const char const ESCAPE = ';';
	const char* const DELIMITER = "=";

	ifstream confFile("telubee.ini");

	if ( !confFile.is_open() ){
		cout << " FileOpenError(telubee.ini)" << endl;
	}
	else{
		while (!confFile.eof()){
			char buf[MAX_CHARS_PER_LINE];
			confFile.getline(buf, MAX_CHARS_PER_LINE);

			// parse the line into blank-delimited tokens
			int n = 0; // a for-loop index

			if(buf[0] == ESCAPE)
				continue; 

			// array to store memory addresses of the tokens in buf
			const char* token[MAX_TOKENS_PER_LINE] = {}; // initialize to 0

			// parse the line
			token[0] = strtok(buf, DELIMITER); // first token

			if (token[0]){
				for (n = 1; n < MAX_TOKENS_PER_LINE; n++){
					token[n] = strtok(0, DELIMITER); // subsequent tokens
					if (!token[n]) break; // no more tokens
				}

				if(strcmp (token[0], "IP_ADDRESS_ROBOT") == 0)
					strcpy (robotipAddress, token[1]);

				if(strcmp (token[0], "IP_ADDRESS_SERVER") == 0)
					strcpy (serveripAddress, token[1]);

				if(strcmp (token[0], "SEND_TO") == 0)
					data_sendTo = atoi(token[1]);

			}

		}

	}

	confFile.close();

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






// ------------------------------------ Main routine ---------------------------------------- //


int main( int argc, char** argv []){
	bool bExit = false;
	char userdataBuf[280];		// 278
	char userstatusBuf[280];	// 278

	WSAData wsaData;
	if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0)
		WSACleanup();
	int retval = getmyip();

	shmem agent;		// instanciate shmem


	switch (agent.openWrite()){
		case -1:
			//Console::SetTextColor(LIGHTRED);
			std::cout << "error.... shared memory creation failed" << std::endl; 
			break;
		case -2:
			//Console::SetTextColor(LIGHTRED);
			std::cout << "error! mapviewoffile failed" << std::endl;; 
			break;
		default:
			//Console::SetTextColor(LIGHTGREEN);
			std::cout << "shared memory created successfully!" << std::endl; 
			std::cout << "memory map successfully created!" << std::endl; 
	}

	agent.data->user.status.online = true;

	//load_parameters();

start:

	while(true){

		if(_kbhit()){
			switch(_getch()){
				case 0x1b:
					bExit = true;		
					break;	
				case 'p':
					agent.data->user.status.connected = !agent.data->user.status.connected;
					std::cout << "pressed" << std::endl; 
					break;	
				default:
					break;
			}
		}

	}


LoopEnd:
	agent.Detach(); 

}

