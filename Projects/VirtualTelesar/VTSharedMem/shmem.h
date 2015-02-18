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


#define SHMEM_NAME_PREFIX	"T5MA_MMF_"
#define SHMEM_SIZE			0x00001900
#define SHMEM_HEAD_SIZE		0x00000100



// structure definition for global packet


// Position information
typedef struct shmem_point {
	float pos[3];
	float ori[3];
} shmem_point_t;

typedef struct shmem_coor {
	shmem_point_t eye;
	shmem_point_t shoulder;
	shmem_point_t right_arm;
	shmem_point_t left_arm;
	shmem_point_t T_eye_lhand;
	shmem_point_t T_eye_rhand;
	shmem_point_t T_sh_lhand;
	shmem_point_t T_sh_rhand;
} shmem_coor_t;

typedef struct shmem_target {
	shmem_coor_t user;
	shmem_coor_t kin;
	shmem_coor_t end_effector;
} shmem_target_t;


typedef struct shmem_arms {
	float right[7];
	float left[7];
} shmem_arms_t;

typedef struct shmem_hands {
	float right[16];
	float left[16];
} shmem_hands_t;

typedef struct shmem_glove {
	float right[14];
	float left[14];
	int r_gus;
	int l_gus;
} shmem_glove_t;

typedef struct shmem_force {
	float right[3];
	float left[3];
} shmem_force_t;

typedef struct shmem_temp {
	float right[3];
	float left[3];
} shmem_temp_t;

typedef struct shmem_tactile {
	float right[3];
	float left[3];
} shmem_tactile_t;

typedef struct shmem_current {
	float right[7];
	float left[7];
} shmem_current_t;

typedef struct shmem_torque {
	float right[7];
	float left[7];
} shmem_torque_t;




// joint space / sensor space abd status messages

typedef struct shmem_shoulder {
	float pos[3];
	float ori[9];
} shmem_shoulder_t;

typedef struct shmem_hw_data {
	float body[9];
	bool dviState; 
	bool fsState;  
} shmem_hw_data_t;

typedef struct shmem_joints {
	float kin_body[9];
	float rt_body[9];
	shmem_arms_t kin_arm;
	shmem_arms_t rt_arm;
	shmem_hands_t kin_hand;
	shmem_hands_t rt_hand;
} shmem_joints_t;

typedef struct shmem_sensors {
	shmem_force_t force;
	shmem_temp_t temp;
	shmem_tactile_t tactile;
	shmem_current_t currrent; 
	shmem_torque_t torque; 
} shmem_sensors_t;

typedef struct shmem_status {
	bool telexistence;
	bool connected; 
	bool calibration;  
	bool home; 
	bool initial_pose; 
	bool glove_reset; 
	bool finger_calibrate; 
	bool emergency;
	bool arm_reset;
	bool ik_reset;
	bool force_reset;
	bool thermal;
	bool dviState; 
	bool fsState; 
	bool logStart;
	signed int state; 
} shmem_status_t;


// final TELESAR V data structure

typedef struct shmem_data {
	unsigned long timestamp;
	shmem_target_t target;
	shmem_hw_data_t hardware;
	shmem_shoulder_t shoulder;
	shmem_glove_t glove; 
	shmem_joints_t joints;
	shmem_sensors_t sensors;
	shmem_status_t status; 
} shmem_data_t;



// multicast sync data structure

typedef struct _syncdata {
	shmem_joints_t joints;
	shmem_sensors_t sensors;
	shmem_status_t status; 
} syncdata;



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
