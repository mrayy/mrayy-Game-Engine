
#include "stdafx.h"
#include "PLCHandler.h"

#define OFFSETOF(type, field)    ((unsigned long) &(((type *) 0)->field))

unsigned long TORSO_Ptr[] =
{
	OFFSETOF(mc_torso, status),
	OFFSETOF(mc_torso, userConnected),
	OFFSETOF(mc_torso, robotConnected),
	OFFSETOF(mc_torso, overCurrent),
	OFFSETOF(mc_torso, cameraFPS),
	OFFSETOF(mc_torso, oculusFPS),
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
	OFFSETOF(mc_torso, J1_current),
	OFFSETOF(mc_torso, J2_current),
	OFFSETOF(mc_torso, J3_current),
	OFFSETOF(mc_torso, J4_current),
	OFFSETOF(mc_torso, J5_current),
	OFFSETOF(mc_torso, J6_current),
	OFFSETOF(mc_torso, J1_torque),
	OFFSETOF(mc_torso, J2_torque),
	OFFSETOF(mc_torso, J3_torque),
	OFFSETOF(mc_torso, J4_torque),
	OFFSETOF(mc_torso, J5_torque),
	OFFSETOF(mc_torso, J6_torque),
	OFFSETOF(mc_torso, unused),
	OFFSETOF(mc_torso, collisionYBM),
};
unsigned long XYRIS_Ptr[] =
{	
	// W0000 ~ W001F (32 words)
	OFFSETOF(mc_xyris,terminal),			// W0000	Xyris Terminal Connected
	OFFSETOF(mc_xyris,offline),				// W0001	Offroad equipment in Offline control mode
	OFFSETOF(mc_xyris,inOperation),			// W0002	Offroad equipment in operation
	OFFSETOF(mc_xyris,inTraverse),			// W0003	traverse in operation
	OFFSETOF(mc_xyris,subcrawler),			// W0004	Subcrawler in operation
	OFFSETOF(mc_xyris,offroad),				// W0005	Off - road in operation(within tilt limits)
	OFFSETOF(mc_xyris,mainCrwlRightRPM),		// W0006~7	Main Crawler(Right) RPM
	OFFSETOF(mc_xyris,mainCrwlLeftRPM),		// W0008~9	Main Crawler(Left) RPM
	OFFSETOF(mc_xyris,mainCrwlRightRotDir), // W000A	Main crawler right rotation direction
	OFFSETOF(mc_xyris,mainCrwlLeftRotDir),	// W000B	Main crawler left rotation direction
	OFFSETOF(mc_xyris,subCrwlFrontRight),		// W000C	Subcralwer front (right)
	OFFSETOF(mc_xyris,subCrwlFrontLeft),		// W000D	Subcralwer front (left)
	OFFSETOF(mc_xyris,subCrwlBackRight),		// W000E	Subcralwer back (right)
	OFFSETOF(mc_xyris,subCrwlBackLeft),		// W000F 	Subcralwer back (left)
	OFFSETOF(mc_xyris,pitch),					// W0010	Offroad equipment, Pitch Angle
	OFFSETOF(mc_xyris,roll),					// W0011	Offroad equipment, Roll Angle
	OFFSETOF(mc_xyris,traverseRight),			// W0012	right traverse angle
	OFFSETOF(mc_xyris,traverseLeft),			// W0013 	left traverse angle
	OFFSETOF(mc_xyris,battVoltage),			// W0014~15 Offroad equipment battery voltage (*01)
	OFFSETOF(mc_xyris,battCurrent),			// W0016~17 Offroad equipment battery current (*01)
	OFFSETOF(mc_xyris,forwardControlMode),	// W0018 	Offroad equipment in forward Direction
	OFFSETOF(mc_xyris,traverseControlMode),	// W0019 	Sub craweler in traverse control mode
	OFFSETOF(mc_xyris,mainCrwlSpeedLimit),	// W001A 	Main Crawler Speed Limit (1~8)
	OFFSETOF(mc_xyris,subCrwlSpeedLimit),	// W001B 	Sub crawler Speed Limit (1~8)
	OFFSETOF(mc_xyris,maxTempLeft),			// W001C 	Subcraweler Left Max Temp
	OFFSETOF(mc_xyris,maxTempRight),			// W001D	Subcraweler Right Max Temp
	OFFSETOF(mc_xyris,unused0),				// W001E
	OFFSETOF(mc_xyris, HealthCheckCount)	// W001F 	Health Check Counter
};
unsigned long YBM_Ptr[] =
{// W00E0 ~ W0101 (34 words)
	OFFSETOF(mc_ybm, currentDepth),		// W00E0 �[�x�i���ݒl�j/ Current Depth(*0.1)
	OFFSETOF(mc_ybm, currentLoad),			// W00E1 �׏d�i���ݒl�j/ Current Load
	OFFSETOF(mc_ybm, currentHtCount),		// ����]���P�i���ݒl�j/ Current Half turn count
	OFFSETOF(mc_ybm, currentSoil),			// W00E3 �y���P�i���ݒl�j/ Current Soil 1 
	OFFSETOF(mc_ybm, currentTime),			// W00E4 ���ԂP�i���ݒl�j/ Current Time 1
	OFFSETOF(mc_ybm, comments),			// W00E5 �R�����g�P�i���ݒl�j/ Comments 1
	OFFSETOF(mc_ybm, water_pressure),		// W00E6 �Ԍ������i���ݒl�j/ Current Water pressure(*0.1)
	OFFSETOF(mc_ybm, penitrationSound),	// W00E7 �ѓ����i���ݒl�j/ Penetration sound(*0.1)
	OFFSETOF(mc_ybm, Nvalue),				// W00E8 �m�l�i���ݒl�j/ N value
	OFFSETOF(mc_ybm, surveyNo),			// W00E9 �����n��@No. / Survey No. 
	OFFSETOF(mc_ybm, measurementNo),		// W00EA ����@No. / Measurement No. 
	OFFSETOF(mc_ybm, unused1),				// W00EB Unused 
	OFFSETOF(mc_ybm, remainRecData),		// W00EC �L�^�f�[�^���c�� / Remaining no of recorded data(*0.1)
	OFFSETOF(mc_ybm, battVoltage),			// W00ED �o�b�e���[�d�� / Battery Voltage(*0.1)
	OFFSETOF(mc_ybm, battCurrent),			// W00EE �o�b�e���[�ώZ�d�� / Battery current(*0.1)
	OFFSETOF(mc_ybm, rotMotInsCurrent),	// W00EF ��]���[�^�u���d�� / Rotary Motor instantaneous current(*0.1)
	OFFSETOF(mc_ybm, feedMotInsCurrent),	// W00F0 �t�B�[�h���[�^�u���d�� / Feed Motor instantaneous current(*0.1)
	OFFSETOF(mc_ybm, operatingTime),		// W00F1 �ώZ�^�]���� / Operating time(*0.1)
	OFFSETOF(mc_ybm, baseRoll),			// W00F2�x�[�X�X�Ίp�x�i���E�j/ Base Incliation Angle (Left/Right)(*0.1)
	OFFSETOF(mc_ybm, basePitch),			// W00F3 �x�[�X�X�Ίp�x�i�O��j/ Base Incliation Angle (Front/Back)(*0.1)
	OFFSETOF(mc_ybm, rodPitch),			// W00F4 ���[�_�[�X�Ίp�x�i�O��j/ Rod Incliation Angle (Front/Back)(*0.1)
	OFFSETOF(mc_ybm, rodRoll),				// W00F5 ���[�_�[�X�΁i�n�ʊ�j/ Rod Incliation Ground Reference Angle(*0.1)
	OFFSETOF(mc_ybm, rodRaised),			// W00F6 ���[�_�[�N�|�iذ�ް���̊�j/ Rod Raised or Lowered (w.r.t Leader)
	OFFSETOF(mc_ybm, communicationCount),	// W00F7 �ʐM�p�f�[�^�i�[No // Communication data No.
	OFFSETOF(mc_ybm, feedRise),				// W01000 �t�B�[�h�㏸ / Feed Rise 
	OFFSETOF(mc_ybm, feedModeSwitching),	// W01001 �t�B�[�h���[�h�ؑցi�蓮 / �����j/ Feed Mode Switching (Manual / Auto)
	OFFSETOF(mc_ybm, feedModeManual),		// W01002 �t�B�[�h�蓮���t�B�[�h�㏸ / Feed Manually during feed rise
	OFFSETOF(mc_ybm, FeedModeAuto),			// W01003 �t�B�[�h�������t�B�[�h�㏸ / Feed Auto during feed rise
	OFFSETOF(mc_ybm, feedDown),				// W01004 �t�B�[�h���~ / Feed down
	OFFSETOF(mc_ybm, inManualMode),			// W01005 �蓮�����~�r�b�g / Manual mode
	OFFSETOF(mc_ybm, unused2),				// W01006  
	OFFSETOF(mc_ybm, load25kgDropped),		// W01007 25kg���~ / 25kg Load dropped
	OFFSETOF(mc_ybm, load50kgDropped),		// W01008 50kg���~ / 50kg Load dropped
	OFFSETOF(mc_ybm, load75kDropped),		// W01009 75kg���~ / 75kg Load dropped
	OFFSETOF(mc_ybm,load100kgDropped),		// W0100A 100kg���~ / 100kg Load dropped
	OFFSETOF(mc_ybm,rotation),				// W0100B ��] / Rotation
	OFFSETOF(mc_ybm,rotationSwitching),	// W0100C ��]�ؑցi�A�� / ����]�j/ Rotation switch (continuous / semi rotation)
	OFFSETOF(mc_ybm,rodModeManual),		// W0100D ���b�h�蓮�A����] / Rod Continuous Manual Rotation
	OFFSETOF(mc_ybm,rodHalfRotation),		// W0100E ���b�h����] / Rod half Rotation
	OFFSETOF(mc_ybm,unused3),				// W0100F 
	OFFSETOF(mc_ybm,testContRotation),		// W01010 �������A����] / Continuous Rotation During Test
	OFFSETOF(mc_ybm,testHalfTurn),			// W01011 ����������] / Half Turn During Test
	OFFSETOF(mc_ybm,rodStarted),			// W01012 ���[�_�[�N / Rod Started
	OFFSETOF(mc_ybm,rodStopped),			// W01013 ���[�_�[�| / Rod Stopped
	OFFSETOF(mc_ybm,inverterON),			// W01014 �C���o�[�^ON / Inverrter ON
	OFFSETOF(mc_ybm,chuckOpen),			// W01015 �`���b�N�J / Chuck Open
	OFFSETOF(mc_ybm,chuckClose),			// W01016 �`���b�N�� / Chuck Close
	OFFSETOF(mc_ybm,clampOpen),			// W01017 �N�����v�J / Clamp Open
	OFFSETOF(mc_ybm,clampClose),			// W01018 �N�����v�� / Clamp Close
	OFFSETOF(mc_ybm,graspSortButton),		// W01019 �͂ݑւ��{�^�� / Grasp Sort Button
	OFFSETOF(mc_ybm,rodCuttingOp1),		// W0101A ���b�h�ؒf����P / Rod Cutting Operation 1
	OFFSETOF(mc_ybm,rodCuttingOp2),		// W0101B ���b�h�ؒf����Q / Rod Cutting Operation 2
	OFFSETOF(mc_ybm,rodCuttingOp3),		// W0101C ���b�h�ؒf����R / Rod Cutting Operation 3
	OFFSETOF(mc_ybm,rodCuttingOp4),		// W0101D ���b�h�ؒf����S / Rod Cutting Operation 4
	OFFSETOF(mc_ybm,emgStop),				// W0101E ����~ / Emergency Stop
	OFFSETOF(mc_ybm, testStarted)			// W0101F �����J�n / Test Started
};

unsigned long COMMON_Ptr[] =
{
	// W0000 ~ W001F (32 words)
	OFFSETOF(mc_common, batteryCurrent),
	OFFSETOF(mc_common, battertVoltage),
	OFFSETOF(mc_common, unused),
	OFFSETOF(mc_common, rssi_base),
	OFFSETOF(mc_common, rssi_robot),

};
unsigned long INTERLOCK_Ptr[] =
{
	// W0000 ~ W001F (32 words)
	OFFSETOF(mc_interlock, torso_ybm_collison),
	OFFSETOF(mc_interlock, ybm_rod),
	OFFSETOF(mc_interlock, ybm_origin),
	OFFSETOF(mc_interlock, xyris_moving),
	OFFSETOF(mc_interlock, xyris_traversing),
	OFFSETOF(mc_interlock, lowBattery),

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
	cnt += m_mc->batch_read("W", SELECT_COMMON, 0x034F, &m_readBuff, 0x05);
	cnt += m_mc->batch_read("W", SELECT_INTERLOCK, 0x0360, &m_readBuff, 0x06); 
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



unsigned short PLCHandler::GetCommonFieldUShort(ECommonDataField data)
{
	return *(unsigned short*)((char*)&m_readBuff.common + COMMON_Ptr[data]);
}
short PLCHandler::GetCommonFieldShort(ECommonDataField data)
{
	return *(short*)((char*)&m_readBuff.common + COMMON_Ptr[data]);
}