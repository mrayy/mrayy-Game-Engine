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
#define PLC_PORT_TCP_GNSS	49154		// Set this Port as MC Protocol for TORSO TCP Access in GX Works
/* ..  */
#define PLC_PORT_TCP_HMD	53248		// Set this Port as MC Protocol for HMD TCP Access in GX Works
#define PLC_PORT_TCP_DISP	53249		// Set this Port as MC Protocol for Overall Display TCP Access in GX Works
#define PLC_PORT_TCP_GNSSR	53250		// Set this Port as MC Protocol for Unused TCP Access in GX Works
#define PLC_PORT_TCP_UNUSED	53251		// Set this Port as MC Protocol for Unused TCP Access in GX Works
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
#define SELECT_INTERLOCK			4
#define SELECT_GNSS					5
#define SELECT_COMMON				6


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
	unsigned short inTraverse;			// W0003	traverse in operation
	unsigned short subcrawler;			// W0004	Subcrawler in operation
	unsigned short offroad;				// W0005	Off - road in operation(within tilt limits)
	unsigned int mainCrwlRightRPM;		// W0006~7	Main Crawler(Right) RPM
	unsigned int mainCrwlLeftRPM;		// W0008~9	Main Crawler(Left) RPM
	unsigned short mainCrwlRightRotDir; // W000A	Main crawler right rotation direction
	unsigned short mainCrwlLeftRotDir;	// W000B	Main crawler left rotation direction
	signed short subCrwlFrontRight;		// W000C	Subcralwer front (right)
	signed short subCrwlFrontLeft;		// W000D	Subcralwer front (left)
	signed short subCrwlBackRight;		// W000E	Subcralwer back (right)
	signed short subCrwlBackLeft;		// W000F 	Subcralwer back (left)
	signed short pitch;					// W0010	Offroad equipment, Pitch Angle
	signed short roll;					// W0011	Offroad equipment, Roll Angle
	signed short traverseRight;			// W0012	right traverse angle
	signed short traverseLeft;			// W0013 	left traverse angle
	unsigned int battVoltage;			// W0014~15 Offroad equipment battery voltage (*01)
	unsigned int battCurrent;			// W0016~17 Offroad equipment battery current (*01)
	unsigned short forwardControlMode;	// W0018 	Offroad equipment in forward Direction
	unsigned short traverseControlMode;	// W0019 	Sub craweler in traverse control mode
	unsigned short mainCrwlSpeedLimit;	// W001A 	Main Crawler Speed Limit (1~8)
	unsigned short subCrwlSpeedLimit;	// W001B 	Sub crawler Speed Limit (1~8)
	signed short maxTempLeft;			// W001C 	Subcraweler Left Max Temp
	signed short maxTempRight;			// W001D	Subcraweler Right Max Temp
	unsigned short unused0;				// W001E
	unsigned short HealthCheckCount;	// W001F 	Health Check Counter
	//unsigned short unused1;				// W0080
	//unsigned short YBMinOperation;		// W0081 	YBM Device in operation
	//unsigned short YBMinOperationPost;	// W0082	YBM Device in operation posture
} mc_xyris;



typedef struct {		// W00A0 ~ W00BF (32 words)
	unsigned short status;			// 	W00A0 status
	unsigned short userConnected;	// 	W00A1 user connected
	unsigned short robotConnected;	// 	W00A2 robot connected
	unsigned short overCurrent;		// 	W00A3 over current shutdown
	unsigned short cameraFPS;		// 	W00A4 camera FPS (grab rate)
	unsigned short oculusFPS;		// 	W00A5 oculus FPS (rendering rate)
	signed short J1_rt_angle;		// 	W00A6 J1 Realtime Angle (*0.01)
	signed short J2_rt_angle;		// 	W00A7 J2 Realtime Angle(*0.01)
	signed short J3_rt_disp;		// 	W00A8 J3 Realtime Angle(*0.01)
	signed short J4_rt_angle;		// 	W00A9 J4 Realtime Angle(*0.01)
	signed short J5_rt_angle;		// 	W00AA J5 Realtime Angle(*0.01)
	signed short J6_rt_angle;		// 	W00AB J6 Realtime Angle(*0.01)
	signed short J1_ik_angle;		// 	W00AC J1 IK Angle(*0.01)
	signed short J2_ik_angle;		// 	W00AD J2 IK Angle(*0.01)
	signed short J3_ik_disp;		// 	W00AE J3 IK Angle(*0.01)
	signed short J4_ik_angle;		// 	W00AF J4 IK Angle(*0.01)
	signed short J5_ik_angle;		// 	W00B0 J5 IK Angle(*0.01)
	signed short J6_ik_angle;		// 	W00B1 J6 IK Angle(*0.01)
	signed short J1_current;		// 	W00B2 J1 current(*0.01)
	signed short J2_current;		// 	W00B3 J2 current(*0.01)
	signed short J3_current;		// 	W00B4 J3 current(*0.01)
	signed short J4_current;		// 	W00B5 J4 current(*0.01)
	signed short J5_current;		// 	W00B6 J5 current(*0.01)
	signed short J6_current;		// 	W00B7 J6 current(*0.01)
	signed short J1_torque;			// 	W00B8 J1 torque(*0.01)
	signed short J2_torque;			// 	W00B9 J2 torque(*0.01)
	signed short J3_torque;			// 	W00BA J3 torque(*0.01)
	signed short J4_torque;			// 	W00BB J4 torque(*0.01)
	signed short J5_torque;			// 	W00BC J5 torque(*0.01)
	signed short J6_torque;			// 	W00BD J6 torque(*0.01) 
	unsigned short unused;			// 	W00BE unused
	unsigned short collisionYBM;	// 	W00BF In the range of collide with YBM
} mc_torso;


typedef struct {		// W00E0 ~ W0101 (34 words)
	unsigned short currentDepth;		// W00E0 �[�x�i���ݒl�j/ Current Depth(*0.1)
	unsigned short currentLoad;			// W00E1 �׏d�i���ݒl�j/ Current Load
	unsigned short currentHtCount;		// ����]���P�i���ݒl�j/ Current Half turn count
	unsigned short currentSoil;			// W00E3 �y���P�i���ݒl�j/ Current Soil 1 
	unsigned short currentTime;			// W00E4 ���ԂP�i���ݒl�j/ Current Time 1
	unsigned short comments;			// W00E5 �R�����g�P�i���ݒl�j/ Comments 1
	unsigned short water_pressure;		// W00E6 �Ԍ������i���ݒl�j/ Current Water pressure(*0.1)
	unsigned short penitrationSound;	// W00E7 �ѓ����i���ݒl�j/ Penetration sound(*0.1)
	unsigned short Nvalue;				// W00E8 �m�l�i���ݒl�j/ N value
	unsigned short surveyNo;			// W00E9 �����n��@No. / Survey No. 
	unsigned short measurementNo;		// W00EA ����@No. / Measurement No. 
	unsigned short unused1;				// W00EB Unused 
	unsigned short remainRecData;		// W00EC �L�^�f�[�^���c�� / Remaining no of recorded data(*0.1)
	unsigned short battVoltage;			// W00ED �o�b�e���[�d�� / Battery Voltage(*0.1)
	unsigned short battCurrent;			// W00EE �o�b�e���[�ώZ�d�� / Battery current(*0.1)
	unsigned short rotMotInsCurrent;	// W00EF ��]���[�^�u���d�� / Rotary Motor instantaneous current(*0.1)
	unsigned short feedMotInsCurrent;	// W00F0 �t�B�[�h���[�^�u���d�� / Feed Motor instantaneous current(*0.1)
	unsigned short operatingTime;		// W00F1 �ώZ�^�]���� / Operating time(*0.1)
	unsigned short baseRoll;			// W00F2�x�[�X�X�Ίp�x�i���E�j/ Base Incliation Angle (Left/Right)(*0.1)
	unsigned short basePitch;			// W00F3 �x�[�X�X�Ίp�x�i�O��j/ Base Incliation Angle (Front/Back)(*0.1)
	unsigned short rodPitch;			// W00F4 ���[�_�[�X�Ίp�x�i�O��j/ Rod Incliation Angle (Front/Back)(*0.1)
	unsigned short rodRoll;				// W00F5 ���[�_�[�X�΁i�n�ʊ�j/ Rod Incliation Ground Reference Angle(*0.1)
	unsigned short rodRaised;			// W00F6 ���[�_�[�N�|�iذ�ް���̊�j/ Rod Raised or Lowered (w.r.t Leader)
	unsigned short communicationCount;	// W00F7 �ʐM�p�f�[�^�i�[No // Communication data No.
	unsigned char feedRise;				// W01000 �t�B�[�h�㏸ / Feed Rise 
	unsigned char feedModeSwitching;	// W01001 �t�B�[�h���[�h�ؑցi�蓮 / �����j/ Feed Mode Switching (Manual / Auto)
	unsigned char feedModeManual;		// W01002 �t�B�[�h�蓮���t�B�[�h�㏸ / Feed Manually during feed rise
	unsigned char FeedModeAuto;			// W01003 �t�B�[�h�������t�B�[�h�㏸ / Feed Auto during feed rise
	unsigned char feedDown;				// W01004 �t�B�[�h���~ / Feed down
	unsigned char inManualMode;			// W01005 �蓮�����~�r�b�g / Manual mode
	unsigned char unused2;				// W01006  
	unsigned char load25kgDropped;		// W01007 25kg���~ / 25kg Load dropped
	unsigned char load50kgDropped;		// W01008 50kg���~ / 50kg Load dropped
	unsigned char load75kDropped;		// W01009 75kg���~ / 75kg Load dropped
	unsigned char load100kgDropped;		// W0100A 100kg���~ / 100kg Load dropped
	unsigned char rotation;				// W0100B ��] / Rotation
	unsigned char rotationSwitching;	// W0100C ��]�ؑցi�A�� / ����]�j/ Rotation switch (continuous / semi rotation)
	unsigned char rodModeManual;		// W0100D ���b�h�蓮�A����] / Rod Continuous Manual Rotation
	unsigned char rodHalfRotation;		// W0100E ���b�h����] / Rod half Rotation
	unsigned char unused3;				// W0100F 
	unsigned char testContRotation;		// W01010 �������A����] / Continuous Rotation During Test
	unsigned char testHalfTurn;			// W01011 ����������] / Half Turn During Test
	unsigned char rodStarted;			// W01012 ���[�_�[�N / Rod Started
	unsigned char rodStopped;			// W01013 ���[�_�[�| / Rod Stopped
	unsigned char inverterON;			// W01014 �C���o�[�^ON / Inverrter ON
	unsigned char chuckOpen;			// W01015 �`���b�N�J / Chuck Open
	unsigned char chuckClose;			// W01016 �`���b�N�� / Chuck Close
	unsigned char clampOpen;			// W01017 �N�����v�J / Clamp Open
	unsigned char clampClose;			// W01018 �N�����v�� / Clamp Close
	unsigned char graspSortButton;		// W01019 �͂ݑւ��{�^�� / Grasp Sort Button
	unsigned char rodCuttingOp1;		// W0101A ���b�h�ؒf����P / Rod Cutting Operation 1
	unsigned char rodCuttingOp2;		// W0101B ���b�h�ؒf����Q / Rod Cutting Operation 2
	unsigned char rodCuttingOp3;		// W0101C ���b�h�ؒf����R / Rod Cutting Operation 3
	unsigned char rodCuttingOp4;		// W0101D ���b�h�ؒf����S / Rod Cutting Operation 4
	unsigned char emgStop;				// W0101E ����~ / Emergency Stop
	unsigned char testStarted;			// W0101F �����J�n / Test Started

} mc_ybm;


typedef struct {		// W034F ~ W0353 (5 words)
	unsigned short batteryCurrent;		// W034F 
	unsigned short battertVoltage;		// W0350 
	unsigned short unused;				// W0351
	signed short rssi_base;				// W0352
	signed short rssi_robot;			// W0353
}mc_common;


typedef struct {		// W0360 ~ W0365 (6 words)
	unsigned short torso_ybm_collison;	// W0360 torsoInOperation
	unsigned short ybm_rod;				// W0361 YBM Rod In Operation
	unsigned short ybm_origin;			// W0362 YBM Origin Error
	unsigned short xyris_moving;		// W0363 Xyris in Operation
	unsigned short xyris_traversing;	// W0364 Xyris in Traverse Mode (Within Safety Limits)
	unsigned short lowBattery;		    // W0365 Low Battery
}mc_interlock;


typedef struct {		// W00C0 ~ W00C3 (4 words)
	unsigned int latitude;			// W00C0~C1 Offroad equipment battery voltage (*01)
	unsigned int longitude;			// W00C2~C3 Offroad equipment battery current (*01)
}mc_gnss;



typedef struct {
	mc_xyris xyris; 
	mc_torso torso;
	mc_ybm ybm; 
	mc_common common;
	mc_interlock interlock;
	mc_gnss gnss; 
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
	MCClient(int portnum, char *ipaddr);
	~MCClient();
	char* errmsg(unsigned short errnum);
	unsigned char devcode(char devs[3]);

	void* set_header_3e(MELSEC_MC_3E_REQ* ahdr);
	int batch_read(char* devname, char destination, int head_dev, mc_buff *outbuf, unsigned short length);
	int batch_write(char* devname, char destination, int head_dev, mc_buff* inbuf, unsigned short length);

};




#endif