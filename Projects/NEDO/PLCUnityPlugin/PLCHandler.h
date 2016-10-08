

#ifndef __PLCHANDLER__
#define __PLCHANDLER__

#include "plc_config.h"


enum ETorsoDataField
{
	J1RT,
	J2RT,
	J3RT,
	J4RT,
	J5RT,
	J6RT,
	J1IK,
	J2IK,
	J3IK,
	J4IK,
	J5IK,
	J6IK,
	J1TQ,
	J2TQ,
	J3TQ,
	J4TQ,
	J5TQ,
	J6TQ,
	IMURoll,
	IMUPitch,
	IMUYaw,
	Status,
	CameraFPS,
	Collision
};


enum EXyrisDataField
{
	terminal,
	offline,
	inOperation,
	traverse,
	subcraler,
	offroad,
	mainCrwlMtrRight,
	mainCrwlMtrLeft,
	mainCrwlRightRot,
	mainCrwlLeftRot,
	subCrwlPrevAngRight,
	subCrwlPrevAngLeft,
	subCrwlCurrAngRight,
	subCrwlCurrAngLeft,
	frontAngle,
	sideAngle,
	rightTraverse,
	leftTraverse,
	battVoltage,
	battCurrent,
	inForwardDir,
	inTraverseControl,
	maxSpeed,
	traverseSpeed,
	maxTempLeft,
	maxTempRight,
	unused0,
	HealthCheckCount,
};
enum EYbmDataField
{
	currentDepth,
	currentLoad,
	currentHtCount,
	currentSoil,
	currentTime,
	comments,
	water_pressure,
	penitrationSound,
	Nvalue,
	surveyNo,
	measurementNo,
	unused1,
	remainRecData,
	YBMbattVoltage,
	YBMbattCurrent,
	rotMotInsCurrent,
	feedMotInsCurrent,
	operatingTime,
	baseInclination_front,
	baseInclination_side,
	rodInclination_front,
	rodInclination_side,
	rodRaised,
	communicationCount,
	feedRise,
	feedModeSwitching,
	feedModeManual,
	FeedModeAuto,
	feedDown,
	inManualMode,
	unused2,
	load25kgDropped,
	load50kgDropped,
	load75kDropped,
	load100kgDropped,
	rotation,
	rotationSwitching,
	rodModeManual,
	rodHalfRotation,
	unused3,
	testContRotation,
	testHalfTurn,
	rodStarted,
	rodStopped,
	inverterON,
	chuckOpen,
	chuckClose,
	clampOpen,
	clampClose,
	graspSortButton,
	rodCuttingOp1,
	rodCuttingOp2,
	rodCuttingOp3,
	rodCuttingOp4,
	emgStop,
	testStarted
};

 enum ECommonDataField {
	batteryCurrent,
	battertVoltage,
	commonUnused,
	rssi_base,
	rssi_robot
};

class PLCHandler
{
protected:
	MCClient *m_mc;
	mc_buff m_writeBuff;
	mc_buff m_readBuff;
public:
	PLCHandler();
	virtual ~PLCHandler();


	bool ConnectToPLC(const std::string& ip, int port);
	bool CloseConnection();
	bool IsConnected();

	bool WriteData();
	bool ReadData();

	//TORSO Code
	void SetTorsoDataUInt(ETorsoDataField data, unsigned int value);
	void SetTorsoDataUShort(ETorsoDataField data, unsigned short value);
	unsigned int GetTorsoUInt(ETorsoDataField data);
	int GetTorsoInt(ETorsoDataField data);
	unsigned short GetTorsoUShort(ETorsoDataField data);

	//xyris Code
	void SetXyrisDataUInt(EXyrisDataField data, unsigned int value);
	void SetXyrisDataUShort(EXyrisDataField data, unsigned short value);
	unsigned int GetXyrisUInt(EXyrisDataField data);
	unsigned short GetXyrisUShort(EXyrisDataField data);
	short GetXyrisShort(EXyrisDataField data);

	//ybm Code
	void SetYbmDataUInt(EYbmDataField data, unsigned int value);
	void SetYbmDataUShort(EYbmDataField data, unsigned short value);
	void SetYbmDataUChar(EYbmDataField data, unsigned char value);
	unsigned int GetYbmUInt(EYbmDataField data);
	unsigned short GetYbmUShort(EYbmDataField data);
	unsigned char GetYbmUChar(EYbmDataField data);

	unsigned short GetCommonFieldUShort(ECommonDataField data);
	short GetCommonFieldShort(ECommonDataField data);
};



#endif
