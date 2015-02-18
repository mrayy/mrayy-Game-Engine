#ifndef _INC_T5_SHMEM_H
#define _INC_T5_SHMEM_H

/*--------------------------------------------------------------------------*/
// Shared Memory map definition shared mem access class
//
// Written by Charith@tachilab.org
//
// WIN32 must be defined when compiling for Windows.
// For Visual C++ this is normally already defined.
//
// Copyright (C) 2012, TACHILAB <www.tachilab.org>
//
/*--------------------------------------------------------------------------*/

#include <winsock2.h>		// for struct in_addr
#include <string>


#define SHMEM_NAME_PREFIX	"TELEUB_MMF_"
#define SHMEM_SIZE			0x00001900
#define SHMEM_HEAD_SIZE		0x00000100


// custom typedefs
typedef struct shmem_status {
	bool connected;
	bool online;  
	bool home; 
	bool initial;
	bool reset;
} shmem_status_t;


typedef struct head {
	float x;
	float y;
	float z;
	float roll;
	float pitch;
	float yaw;
} head_t;


typedef struct joystick {
	float wheel_l;
	bool button_A;
	bool button_B;
	bool button_C;
	bool button_D;
} joystick_t;


typedef struct location {
	float latitude;
	float longitude;
} location_t;


typedef struct shmem_user {
	std::string user_id; 
//	char tmp[12];
	in_addr ipaddress;
	location_t location;
	head_t head;
	joystick_t joystick;
	shmem_status_t status;
} shmem_user_t;


typedef struct shmem_robot {
	std::string robot_id; 
	in_addr ipaddress;
	location_t location;
	head_t head;
	joystick_t joystick;
	shmem_status_t status;
} shmem_robot_t;



// final TELUBee data structure

typedef struct shmem_data {
	unsigned long timestamp;
	shmem_user_t user;
	shmem_robot_t robot; 
} shmem_data_t;




class shmem {

public:
	bool Attach(void);
	bool Detach(void);
	bool attached(void); 
	int createRead(void); 
	int createWrite(void); 
	int openRead(void); 
	int openWrite(void); 
	shmem_data_t* data;
private: 
	void* hMap;
	void* pMap;
}; 


#endif
