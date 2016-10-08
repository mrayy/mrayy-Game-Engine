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


// MELSEC PLC Address List ....
#ifndef _INC_PLCCONFIG_H
#define _INC_PLCCONFIG_H

#include <winsock2.h> // for socker communication
#include <ws2tcpip.h> 
#include <sys/types.h>
#include <winsock2.h>			// link 32bit/64bit ws2_32.lib
#include <process.h>
#include <string>
#include <vector>
#include <stdio.h>
#include <conio.h>


#define MELSEC_PLC	"192.168.1.32"		// for PLC
#define PLC_PORT_TCP_XYRIS	49152		// Set this Port as MC Protocol for Xyris TCP Access in GX Works
#define PLC_PORT_TCP_TORSO	49153		// Set this Port as MC Protocol for TORSO TCP Access in GX Works
/* ..  */
#define PLC_PORT_TCP_HMD	53248		// Set this Port as MC Protocol for HMD TCP Access in GX Works
#define PLC_PORT_TCP_DISP	53249		// Set this Port as MC Protocol for Overall Display TCP Access in GX Works
#define PLC_PORT_TCP_UNUSED	53250		// Set this Port as MC Protocol for Unused TCP Access in GX Works
#define PLC_PORT_UDP		1026		// Set this Port as MC Protocol UDP Access in GX Works


#define MC_3E_REQ_SIG				0x50
#define MC_3E_RSP_SIG				0xD0
#define MC_DEFAULT_NETWORK			0x00
#define MC_DEFAULT_PC				0xFF
#define MC_DEFAULT_MODULE_IO		0x03FF
#define MC_DEFAULT_MODULE_STATION	0x00
#define MC_3E_HEADER_SZ				21	

#define SELECT_XYRIS				1
#define SELECT_TORSO				2
#define SELECT_YBM					3
#define SELECT_ALL					4


// MC Frame Header - Type 3E/Binary - Request
typedef struct {
	char subheader[2];				// subheader value
	unsigned char network_no;		// network/station number
	unsigned char pc_no;			// PC number
	unsigned short request_dest_io;	// request destination module i/o number
	unsigned char request_dest_sta;	// request destination module station number
	unsigned short data_length;		// length of request data section
	unsigned short monitor_timer;	// CPU monitoring timer
	unsigned short command;			// command
} MELSEC_MC_3E_REQ;

// MC Frame Header - Type 3E/Binary - Response
typedef struct {
	char subheader[2];				// subheader value
	unsigned char network_no;		// network/station number
	unsigned char pc_no;			// PC number
	unsigned short request_dest_io;	// request destination module i/o number
	unsigned char request_dest_sta;	// request destination module station number
	unsigned char data_length;		// length of response data
	unsigned short complete_code;	// completion code
} MELSEC_MC_3E_ACK;


// Type 3E/Binary - Abnormal Response structure
typedef struct {
	unsigned char rsp_network;		// responder network/station number
	unsigned char rsp_pc;			// responder PC number
	unsigned short request_dest_io;	// request destination module i/o number
	unsigned char request_dest_sta;	// request destination module station number
	unsigned short command;			// failed command
	unsigned short subcommand;		// failed subcommand
} MELSEC_MC_3E_ABNORMAL;


// Batch Read/Write - Binary [0401/1401]
typedef struct {
	unsigned short subcommand;		// subcommand
	unsigned char head_device[3];			// head device code (3 byte integer)
	unsigned char dev_type;		// device type code
	unsigned short num_points;		// number of device points
} MELSEC_MC_BATCHRW;


typedef struct {
	unsigned short errval;
	char errdesc[128];
} MELSEC_ERRMAP;


typedef struct {
	unsigned char bval;
	char cval[3];
} MELSEC_REGMAP;



typedef struct {		// W0000 ~ W001F (32 words)
	unsigned short terminal;			// W0000	Xyris Terminal Connected
	unsigned short offline;				// W0001	Offroad equipment in Offline control mode
	unsigned short inOperation;			// W0002	Offroad equipment in operation
	unsigned short traverse;			// W0003	traverse in operation
	unsigned short subcraler;			// W0004	Subcrawler in operation
	unsigned short offroad;				// W0005	Off - road in operation(within tilt limits)
	unsigned int mainCrwlMtrRight;		// W0006~7	Main Crawler(Right) Motor In Operation�iLSB, MSB�j
	unsigned int mainCrwlMtrLeft;		// W0008~9	Main Crawler(Left) # of turns(LSB)
	unsigned short mainCrwlRightRot; 	// W000A	Main crawler right rotation
	unsigned short mainCrwlLeftRot;		// W000B	Main crawler left rotation
	short subCrwlPrevAngRight;			// W000C	Subcralwer angle before(right)
	short subCrwlPrevAngLeft;			// W000D	Subcralwer angle before(left)
	short subCrwlCurrAngRight;			// W000E	Subcralwer angle after(right)
	short subCrwlCurrAngLeft;			// W000F 	Subcralwer angle after(left)
	short frontAngle;					// W0010	Offroad equipment, travelling direction angle
	short sideAngle;					// W0011	Offroad equipment, side angle
	short rightTraverse;				// W0012	right angle traverse
	short leftTraverse;					// W0013 	left angle traverse
	unsigned int battVoltage;			// W0014~15 Offroad equipment battery voltage
	unsigned int battCurrent;			// W0016~17 Offroad equipment battery current
	unsigned short inForwardDir;		// W0018 	Offroad equipment in forward Direction
	unsigned short inTraverseControl;	// W0019 	Sub craweler in traverse control mode
	unsigned short maxSpeed;			// W001A 	Active Wheel maximum speed
	unsigned short traverseSpeed;		// W001B 	Sub crawler traverse control speed
	short maxTempLeft;					// W001C 	Subcraweler Left Max Temp
	short maxTempRight;					// W001D	Subcraweler Right Max Temp
	unsigned short unused0;				// W001E
	unsigned short HealthCheckCount;	// W001F 	Health Check Counter
	//unsigned __int8 unused1;				// W0080
	//unsigned __int8 YBMinOperation;		// W0081 	YBM Device in operation
	//unsigned __int8 YBMinOperationPost;	// W0082	YBM Device in operation posture
} mc_xyris;



typedef struct {		// W00A0 ~ W00CC (45 words)
	int J1_rt_angle;		// 	W00A0~A1 J1 Realtime Angle
	int J2_rt_angle;		// 	W00A2~A3 J2 Realtime Angle
	int J3_rt_disp;		// 	W00A4~A5 J3 Realtime Angle
	int J4_rt_angle;		// 	W00A6~A7 J4 Realtime Angle
	int J5_rt_angle;		// 	W00A8~A9 J5 Realtime Angle
	int J6_rt_angle;		// 	W00AA~AB J6 Realtime Angle
	int J1_ik_angle;			// 	W00AC~AD J1 IK Angle
	int J2_ik_angle;			// 	W00AE~AF J2 IK Angle
	int J3_ik_disp;			// 	W00B0~B1 J3 IK Angle
	int J4_ik_angle;			// 	W00B2~B3 J4 IK Angle
	int J5_ik_angle;			// 	W00B4~B5 J5 IK Angle
	int J6_ik_angle;			// 	W00B6~B7 J6 IK Angle
	int J1_torque;				// 	W00B8~B9 J1 torque
	int J2_torque;				// 	W00BA~BB J2 torque
	int J3_torque;				// 	W00BC~BD J3 torque
	int J4_torque;				// 	W00BE~BF J4 torque
	int J5_torque;				// 	W00C0~C1 J5 torque
	int J6_torque;				// 	W00C2~C3 J6 torque 
	int imuRoll;				// 	W00C4~C5 roll
	int imuPitch;				// 	W00C6~C7 pitch
	int imuYaw;				// 	W00C8~C9 yaw
	unsigned short status;				// 	W00CA status
	unsigned short cameraFps;			// 	W00CB fps
	unsigned short colision;			// 	W00CC In the range of collide with YBM
} mc_torso;


typedef struct {		// W00E0 ~ W0101 (34 words)
	unsigned short currentDepth;		// W00E0 �[�x�i���ݒl�j/ Current Depth
	unsigned short currentLoad;			// W00E1 �׏d�i���ݒl�j/ Current Load
	unsigned short currentHtCount;		// ����]���P�i���ݒl�j/ Current Half turn count
	unsigned short currentSoil;			// W00E3 �y���P�i���ݒl�j/ Current Soil 1 
	unsigned short currentTime;			// W00E4 ���ԂP�i���ݒl�j/ Current Time 1
	unsigned short comments;			// W00E5 �R�����g�P�i���ݒl�j/ Comments 1
	unsigned short water_pressure;		// W00E6 �Ԍ������i���ݒl�j/ Current Water pressure
	unsigned short penitrationSound;	// W00E7 �ѓ����i���ݒl�j/ Penetration sound
	unsigned short Nvalue;				// W00E8 �m�l�i���ݒl�j/ N value
	unsigned short surveyNo;			// W00E9 �����n��@No. / Survey No. 
	unsigned short measurementNo;		// W00EA ����@No. / Measurement No. 
	unsigned short unused1;				// W00EB Unused 
	unsigned short remainRecData;		// W00EC �L�^�f�[�^���c�� / Remaining no of recorded data
	unsigned short battVoltage;			// W00ED �o�b�e���[�d�� / Battery Voltage
	unsigned short battCurrent;			// W00EE �o�b�e���[�ώZ�d�� / Battery current
	unsigned short rotMotInsCurrent;	// W00EF ��]���[�^�u���d�� / Rotary Motor instantaneous current
	unsigned short feedMotInsCurrent;	// W00F0 �t�B�[�h���[�^�u���d�� / Feed Motor instantaneous current
	unsigned short operatingTime;		// W00F1 �ώZ�^�]���� / Operating time
	unsigned short baseInclination_front;// W00F2�x�[�X�X�Ίp�x�i���E�j/ Base Incliation Angle (Left/Right)
	unsigned short baseInclination_side;// W00F3 �x�[�X�X�Ίp�x�i�O��j/ Base Incliation Angle (Front/Back)
	unsigned short rodInclination_front;// W00F4 ���[�_�[�X�Ίp�x�i�O��j/ Rod Incliation Angle (Front/Back)
	unsigned short rodInclination_side;	// W00F5 ���[�_�[�X�΁i�n�ʊ�j/ Rod Incliation Ground Reference Angle
	unsigned short rodRaised;			// W00F6 ���[�_�[�N�|�iذ�ް���̊�j/ Rod Raised or Lowered (w.r.t Leader)
	unsigned short communicationCount;	// W00F7 �ʐM�p�f�[�^�i�[No // Communication data No.
	unsigned char  feedRise;				// W01000 �t�B�[�h�㏸ / Feed Rise 
	unsigned char  feedModeSwitching;	// W01001 �t�B�[�h���[�h�ؑցi�蓮 / �����j/ Feed Mode Switching (Manual / Auto)
	unsigned char  feedModeManual;		// W01002 �t�B�[�h�蓮���t�B�[�h�㏸ / Feed Manually during feed rise
	unsigned char  FeedModeAuto;			// W01003 �t�B�[�h�������t�B�[�h�㏸ / Feed Auto during feed rise
	unsigned char  feedDown;				// W01004 �t�B�[�h���~ / Feed down
	unsigned char  inManualMode;			// W01005 �蓮�����~�r�b�g / Manual mode
	unsigned char  unused2;				// W01006  
	unsigned char  load25kgDropped;		// W01007 25kg���~ / 25kg Load dropped
	unsigned char  load50kgDropped;		// W01008 50kg���~ / 50kg Load dropped
	unsigned char  load75kDropped;		// W01009 75kg���~ / 75kg Load dropped
	unsigned char  load100kgDropped;		// W0100A 100kg���~ / 100kg Load dropped
	unsigned char  rotation;				// W0100B ��] / Rotation
	unsigned char  rotationSwitching;	// W0100C ��]�ؑցi�A�� / ����]�j/ Rotation switch (continuous / semi rotation)
	unsigned char  rodModeManual;		// W0100D ���b�h�蓮�A����] / Rod Continuous Manual Rotation
	unsigned char  rodHalfRotation;		// W0100E ���b�h����] / Rod half Rotation
	unsigned char  unused3;				// W0100F 
	unsigned char  testContRotation;		// W01010 �������A����] / Continuous Rotation During Test
	unsigned char  testHalfTurn;			// W01011 ����������] / Half Turn During Test
	unsigned char  rodStarted;			// W01012 ���[�_�[�N / Rod Started
	unsigned char  rodStopped;			// W01013 ���[�_�[�| / Rod Stopped
	unsigned char  inverterON;			// W01014 �C���o�[�^ON / Inverrter ON
	unsigned char  chuckOpen;			// W01015 �`���b�N�J / Chuck Open
	unsigned char  chuckClose;			// W01016 �`���b�N�� / Chuck Close
	unsigned char  clampOpen;			// W01017 �N�����v�J / Clamp Open
	unsigned char  clampClose;			// W01018 �N�����v�� / Clamp Close
	unsigned char  graspSortButton;		// W01019 �͂ݑւ��{�^�� / Grasp Sort Button
	unsigned char  rodCuttingOp1;		// W0101A ���b�h�ؒf����P / Rod Cutting Operation 1
	unsigned char  rodCuttingOp2;		// W0101B ���b�h�ؒf����Q / Rod Cutting Operation 2
	unsigned char  rodCuttingOp3;		// W0101C ���b�h�ؒf����R / Rod Cutting Operation 3
	unsigned char  rodCuttingOp4;		// W0101D ���b�h�ؒf����S / Rod Cutting Operation 4
	unsigned char  emgStop;				// W0101E ����~ / Emergency Stop
	unsigned char  testStarted;			// W0101F �����J�n / Test Started
 
} mc_ybm;



typedef struct {
	mc_xyris xyris; 
	mc_torso torso;
	mc_ybm ybm; 
}mc_buff;


extern MELSEC_REGMAP regmap[];
extern MELSEC_ERRMAP errmap[];




// ---------------------- PLC MCProtocol Driver definition -----------------------------------//

class MCClient{
private:
	WSADATA wsaData;
	struct sockaddr_in server;
	SOCKET sock;
public:
	MCClient(int portnum, const char *ipaddr);
	~MCClient();
	char* errmsg(unsigned short errnum);
	unsigned char devcode(char devs[3]);
	bool IsConnected(){ return sock != 0; }

	void* set_header_3e(MELSEC_MC_3E_REQ* ahdr);
	int batch_read(char* devname, char destination, int head_dev, mc_buff *outbuf, unsigned short length);
	int batch_write(char* devname, char destination, int head_dev, mc_buff* inbuf, unsigned short length);

};




#endif