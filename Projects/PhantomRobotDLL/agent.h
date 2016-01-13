
/*--------------------------------------------------------------------------*/
// Agent Routines
//
// Written by charith@tachilab.org
//
// WIN32 must be defined when compiling for Windows.
// For Visual C++ this is normally already defined.
//
// Copyright (C) 2012, TACHILAB <www.tachilab.org>
//
/*--------------------------------------------------------------------------*/


#ifndef _INC_AGENT_H
#define _INC_AGENT_H


#ifdef WIN32
#include <windows.h>  // for Sleep
#else
#include <unistd.h>  // for usleep
#endif

#include <time.h>
#include <Mmsystem.h>
#include <fstream>
#include <iostream>
#include <conio.h>


#define RADTODEG	57.2957795131
#define	DEGTORAD	0.0174532925199

char headCOM[10], robotCOM[10];
int robot_baudRate = 115200, head_baudRate = 115200;
int xAxis = 1, yAxis = 1, zAxis = 1;

clock_t startT, endT;

using namespace std;


void wait(long milliseconds){
	register long endwait;
	endwait = clock() + milliseconds * CLOCKS_PER_SEC * 0.001;
	while (clock() < endwait){} 
}



DWORD_PTR GetNumCPUs(){
	SYSTEM_INFO m_si = {0, }; 
	GetSystemInfo(&m_si);
	return (DWORD_PTR)m_si.dwNumberOfProcessors; 
}



#endif 


void load_parameters(){

	const int MAX_CHARS_PER_LINE = 512;
	const int MAX_TOKENS_PER_LINE = 20;
	const char const ESCAPE = ';';
	const char* const DELIMITER = "=";

	char buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);

	
	int index = -1;
	int len = strlen(buffer);
	for (int i = len - 1; i >= 0; --i)
	{
		if (buffer[i] == '\\')
		{
			index = i;
			break;
		}
	}
	std::string path="";
	if (index != -1)
	{
		buffer[index + 1] = 0;
		path = buffer;

	}
	path += "robotconf.ini";
	printf("Path: %s\n", path.c_str());

	ifstream confFile(path);

	xAxis = 1;
	yAxis = 1;
	zAxis = 1;

	if ( !confFile.is_open() ){
		cout << " FileOpenError(robotconf.ini)" << endl;
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
				
 				if(strcmp (token[0], "ROBOT_COM_PORT") == 0)
 					strcpy (robotCOM, token[1]);
				
				 if(strcmp (token[0], "ROBOT_BAUD") == 0)
					robot_baudRate = atoi(token[1]);

				else if(strcmp (token[0], "HEAD_COM_PORT") == 0)
					strcpy (headCOM, token[1]);

				else if(strcmp (token[0], "HEAD_BAUD") == 0)
					head_baudRate = atoi(token[1]);/**/
				
				else if (strcmp(token[0], "X") == 0)
					xAxis = atoi(token[1]);
				else if (strcmp(token[0], "Y") == 0)
					yAxis = atoi(token[1]);
				else if (strcmp(token[0], "Z") == 0)
					zAxis = atoi(token[1]);

			}

			// process (print) the tokens
			//for (int i = 0; i < n; i++) // n = #of tokens
			//	cout << "Token[" << i << "] = " << token[i] << endl;
			//cout << endl;

		}

	}

	confFile.close();

}