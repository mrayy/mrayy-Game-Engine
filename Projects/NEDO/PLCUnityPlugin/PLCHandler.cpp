
#include "stdafx.h"
#include "PLCHandler.h"

#define OFFSETOF(type, field)    ((unsigned long) &(((type *) 0)->field))

unsigned long TORSO_Ptr[] =
{
	OFFSETOF(mc_torso, J1_rt_angle),
	OFFSETOF(mc_torso, J2_rt_angle),
	OFFSETOF(mc_torso, J3_rt_disp),
	OFFSETOF(mc_torso, J4_rt_angle),
	OFFSETOF(mc_torso, J5_rt_angle),
	OFFSETOF(mc_torso, J6_rt_angle),
	OFFSETOF(mc_torso, J1_ik_angle),
	OFFSETOF(mc_torso, J2_ik_angle),
	OFFSETOF(mc_torso, J3_ik_disp),
	OFFSETOF(mc_torso, J4_ik_angle),
	OFFSETOF(mc_torso, J5_ik_angle),
	OFFSETOF(mc_torso, J6_ik_angle),
	OFFSETOF(mc_torso, J1_torque),
	OFFSETOF(mc_torso, J2_torque),
	OFFSETOF(mc_torso, J3_torque),
	OFFSETOF(mc_torso, J4_torque),
	OFFSETOF(mc_torso, J5_torque),
	OFFSETOF(mc_torso, J6_torque),
	OFFSETOF(mc_torso, imuRoll),
	OFFSETOF(mc_torso, imuPitch),
	OFFSETOF(mc_torso, imuYaw),
	OFFSETOF(mc_torso, status),
	OFFSETOF(mc_torso, cameraFps),
	OFFSETOF(mc_torso, colision),
};
unsigned long XYRIS_Ptr[] =
{
	OFFSETOF(mc_xyris, terminal),
	OFFSETOF(mc_xyris, offline),
	OFFSETOF(mc_xyris, inOperation),
	OFFSETOF(mc_xyris, traverse),
	OFFSETOF(mc_xyris, subcraler),
	OFFSETOF(mc_xyris, offroad),
	OFFSETOF(mc_xyris, mainCrwlMtrRight),
	OFFSETOF(mc_xyris, mainCrwlMtrLeft),
	OFFSETOF(mc_xyris, mainCrwlRightRot),
	OFFSETOF(mc_xyris, mainCrwlLeftRot),
	OFFSETOF(mc_xyris, subCrwlPrevAngRight),
	OFFSETOF(mc_xyris, subCrwlPrevAngLeft),
	OFFSETOF(mc_xyris, subCrwlCurrAngRight),
	OFFSETOF(mc_xyris, subCrwlCurrAngLeft),
	OFFSETOF(mc_xyris, frontAngle),
	OFFSETOF(mc_xyris, sideAngle),
	OFFSETOF(mc_xyris, rightTraverse),
	OFFSETOF(mc_xyris, leftTraverse),
	OFFSETOF(mc_xyris, battVoltage),
	OFFSETOF(mc_xyris, battCurrent),
	OFFSETOF(mc_xyris, inForwardDir),
	OFFSETOF(mc_xyris, inTraverseControl),
	OFFSETOF(mc_xyris, maxSpeed),
	OFFSETOF(mc_xyris, traverseSpeed),
	OFFSETOF(mc_xyris, maxTempLeft),
	OFFSETOF(mc_xyris, maxTempRight),
	OFFSETOF(mc_xyris, unused0),
	OFFSETOF(mc_xyris, HealthCheckCount),
};
unsigned long YBM_Ptr[] =
{
	OFFSETOF(mc_ybm, currentDepth),
	OFFSETOF(mc_ybm, currentLoad),
	OFFSETOF(mc_ybm, currentHtCount),
	OFFSETOF(mc_ybm, currentSoil),
	OFFSETOF(mc_ybm, currentTime),
	OFFSETOF(mc_ybm, comments),
	OFFSETOF(mc_ybm, water_pressure),
	OFFSETOF(mc_ybm, penitrationSound),
	OFFSETOF(mc_ybm, Nvalue),
	OFFSETOF(mc_ybm, surveyNo),
	OFFSETOF(mc_ybm, measurementNo),
	OFFSETOF(mc_ybm, unused1),
	OFFSETOF(mc_ybm, remainRecData),
	OFFSETOF(mc_ybm, battVoltage),
	OFFSETOF(mc_ybm, battCurrent),
	OFFSETOF(mc_ybm, rotMotInsCurrent),
	OFFSETOF(mc_ybm, feedMotInsCurrent),
	OFFSETOF(mc_ybm, operatingTime),
	OFFSETOF(mc_ybm, baseInclination_front),
	OFFSETOF(mc_ybm, baseInclination_side),
	OFFSETOF(mc_ybm, rodInclination_front),
	OFFSETOF(mc_ybm, rodInclination_side),
	OFFSETOF(mc_ybm, rodRaised),
	OFFSETOF(mc_ybm, communicationCount),
	OFFSETOF(mc_ybm, feedRise),
	OFFSETOF(mc_ybm, feedModeSwitching),
	OFFSETOF(mc_ybm, feedModeManual),
	OFFSETOF(mc_ybm, FeedModeAuto),
	OFFSETOF(mc_ybm, feedDown),
	OFFSETOF(mc_ybm, inManualMode),
	OFFSETOF(mc_ybm, unused2),
	OFFSETOF(mc_ybm, load25kgDropped),
	OFFSETOF(mc_ybm, load50kgDropped),
	OFFSETOF(mc_ybm, load75kDropped),
	OFFSETOF(mc_ybm, load100kgDropped),
	OFFSETOF(mc_ybm, rotation),
	OFFSETOF(mc_ybm, rotationSwitching),
	OFFSETOF(mc_ybm, rodModeManual),
	OFFSETOF(mc_ybm, rodHalfRotation),
	OFFSETOF(mc_ybm, unused3),
	OFFSETOF(mc_ybm, testContRotation),
	OFFSETOF(mc_ybm, testHalfTurn),
	OFFSETOF(mc_ybm, rodStarted),
	OFFSETOF(mc_ybm, rodStopped),
	OFFSETOF(mc_ybm, inverterON),
	OFFSETOF(mc_ybm, chuckOpen),
	OFFSETOF(mc_ybm, chuckClose),
	OFFSETOF(mc_ybm, clampOpen),
	OFFSETOF(mc_ybm, clampClose),
	OFFSETOF(mc_ybm, graspSortButton),
	OFFSETOF(mc_ybm, rodCuttingOp1),
	OFFSETOF(mc_ybm, rodCuttingOp2),
	OFFSETOF(mc_ybm, rodCuttingOp3),
	OFFSETOF(mc_ybm, rodCuttingOp4),
	OFFSETOF(mc_ybm, emgStop),
	OFFSETOF(mc_ybm, testStarted)
};

PLCHandler::PLCHandler()
{
	m_mc = 0;

	memset(&m_readBuff, 0, sizeof(m_readBuff));
	memset(&m_writeBuff, 0, sizeof(m_writeBuff));
}

PLCHandler::~PLCHandler()
{
	CloseConnection();
}



bool PLCHandler::ConnectToPLC(const std::string& ip, int port)
{
	if (m_mc)
		CloseConnection();
	m_mc = new MCClient(port, ip.c_str());

	if (!m_mc->IsConnected())
	{
		delete m_mc;
		m_mc = 0;
	}

	return m_mc != 0;
}

bool PLCHandler::CloseConnection()
{
	if (m_mc)
	{
		delete m_mc;
		m_mc = 0;
	}
	return true;
}

bool PLCHandler::IsConnected()
{
	return m_mc && m_mc->IsConnected();
}


bool PLCHandler::WriteData()
{
	if (!m_mc)
		return false;

	int cnt = 0;
	cnt  = m_mc->batch_write("W", SELECT_XYRIS, 0x00, &m_writeBuff, 0x20);
	cnt += m_mc->batch_write("W", SELECT_TORSO, 0xA0, &m_writeBuff, 0x2D);
	cnt += m_mc->batch_write("W", SELECT_XYRIS, 0x00, &m_writeBuff, 0x20);
	return cnt > 0;
}

bool PLCHandler::ReadData()
{
	if (!m_mc)
		return false;

	int cnt = 0;
	cnt += m_mc->batch_read("W", SELECT_XYRIS, 0x00, &m_readBuff, 0x20);
	cnt += m_mc->batch_read("W", SELECT_TORSO, 0xA0, &m_readBuff, 0x2D);
	cnt += m_mc->batch_read("W", SELECT_YBM, 0xE0, &m_readBuff, 0x22);
	return cnt > 0;
}


//////////////////////////////////////////////////////////////////////////

void PLCHandler::SetTorsoDataUInt(ETorsoDataField data, unsigned int value)
{
	*(unsigned int*)((char*)&m_writeBuff.torso + TORSO_Ptr[data]) = value;
}
void PLCHandler::SetTorsoDataUShort(ETorsoDataField data, unsigned short value)
{
	*(unsigned short*)((char*)&m_writeBuff.torso + TORSO_Ptr[data]) = value;
}
unsigned int PLCHandler::GetTorsoUInt(ETorsoDataField data)
{
	return *(unsigned int*)((char*)&m_readBuff.torso + TORSO_Ptr[data]);
}
 int PLCHandler::GetTorsoInt(ETorsoDataField data)
{
	return *( int*)((char*)&m_readBuff.torso + TORSO_Ptr[data]);
}
unsigned short PLCHandler::GetTorsoUShort(ETorsoDataField data)
{
	return *(unsigned short*)((char*)&m_readBuff.torso + TORSO_Ptr[data]);

}
//////////////////////////////////////////////////////////////////////////

void PLCHandler::SetXyrisDataUInt(EXyrisDataField data, unsigned int value)
{
	*(unsigned int*)((char*)&m_writeBuff.xyris + XYRIS_Ptr[data]) = value;
}
void PLCHandler::SetXyrisDataUShort(EXyrisDataField data, unsigned short value)
{
	*(unsigned short*)((char*)&m_writeBuff.xyris + XYRIS_Ptr[data]) = value;
}
unsigned int PLCHandler::GetXyrisUInt(EXyrisDataField data)
{
	return *(unsigned int*)((char*)&m_readBuff.xyris + XYRIS_Ptr[data]);
}
unsigned short PLCHandler::GetXyrisUShort(EXyrisDataField data)
{
	return *(unsigned short*)((char*)&m_readBuff.xyris + XYRIS_Ptr[data]);

}
short PLCHandler::GetXyrisShort(EXyrisDataField data)
{
	return *( short*)((char*)&m_readBuff.xyris + XYRIS_Ptr[data]);

}
//////////////////////////////////////////////////////////////////////////

void PLCHandler::SetYbmDataUInt(EYbmDataField data, unsigned int value)
{
	*(unsigned int*)((char*)&m_writeBuff.ybm + YBM_Ptr[data]) = value;
}
void PLCHandler::SetYbmDataUShort(EYbmDataField data, unsigned short value)
{
	*(unsigned short*)((char*)&m_writeBuff.ybm + YBM_Ptr[data]) = value;
}
void PLCHandler::SetYbmDataUChar(EYbmDataField data, unsigned char value)
{
	*(unsigned char*)((char*)&m_writeBuff.ybm + YBM_Ptr[data]) = value;
}

unsigned int PLCHandler::GetYbmUInt(EYbmDataField data)
{
	return *(unsigned int*)((char*)&m_readBuff.ybm + YBM_Ptr[data]);
}
unsigned short PLCHandler::GetYbmUShort(EYbmDataField data)
{
	return *(unsigned short*)((char*)&m_readBuff.ybm + YBM_Ptr[data]);

}
unsigned char PLCHandler::GetYbmUChar(EYbmDataField data)
{
	return *(unsigned char*)((char*)&m_readBuff.ybm + YBM_Ptr[data]);

}