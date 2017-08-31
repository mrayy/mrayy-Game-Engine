

#include "stdafx.h"
#include "TxKitHead.h"
#include "StringUtil.h"

#include <windowsx.h>
#include <fstream>


namespace mray
{

	const int ServoCODE[] = {
		0x3,
		0x1,
		0x2
	};

	const TxKitHead::ServoParameters TxKitHead::DEFAULT_PARAMETERS[] = {
		{ 5, 5, 64, 5, 127 }, //tilt
		{ 5, 10, 64, 5, 127 }, //yaw
		{ 5, 5, 64, 5, 127 }  //roll
	};

	const float TxKitHead::DEFAULT_LIMITS[][2] = {
		{ -20, 20 }, //tilt
		{ -90, 90 }, //yaw
		{ -20, 20 }  //roll
	};


	TxKitHead::TxKitHead()
	{
		connected = false;
		m_serial = 0;
		m_EEPROMset = false;
		{
			memcpy(m_limits, DEFAULT_LIMITS, sizeof(DEFAULT_LIMITS));
			memcpy(m_parameters, DEFAULT_PARAMETERS, sizeof(DEFAULT_PARAMETERS));
			//load PID values
			std::ifstream confFile("TxKitSettings.cfg");

			if (confFile.is_open())
			{
				printf("Loading TxKitSettings File\n");
				for (int i = 0; i < 3; ++i)
				{
					confFile >> m_parameters[i].PGain >> m_parameters[i].Deadband >> m_parameters[i].Damping >> m_parameters[i].Response >> m_parameters[i].Speed;

					m_parameters[i].PGain = math::clamp(m_parameters[i].PGain, 0, 10);
					m_parameters[i].Deadband = math::clamp(m_parameters[i].Deadband, 0, 10);
					m_parameters[i].Damping = math::clamp(m_parameters[i].Damping, 0, 127);
					m_parameters[i].Response = math::clamp(m_parameters[i].Response, 0, 10);
					m_parameters[i].Speed = math::clamp(m_parameters[i].Speed, 0, 127);
				}
				for (int i = 0; i < 3; ++i)
				{
					confFile >> m_limits[i][0] >> m_limits[i][1];
				}
				m_paramsLoaded = true;
				confFile.close();
			}
			else
				m_paramsLoaded = false;
		}

	}
	TxKitHead::~TxKitHead()
	{
		Disconnect();
		if (m_serial)
			delete m_serial;
	}


	bool TxKitHead::_writeEEPROM()
	{
		if (m_EEPROMset || !m_paramsLoaded)
			return true;
		for (int index = 0; index < 3; ++index)
		{
			printf("Updating servo[%d] EEPROM\n", ServoCODE[index]);

			uint8_t sCommand[68];
			sCommand[0] = 0xA0 | ServoCODE[index];//Read command
			sCommand[1] = 0x00;//EEPROM access
			uint8_t reply[68];

			int ret = _sendCommand(sCommand, 2 * sizeof(char), reply, 68,250);
			if (ret == 68)
			{
				uint8_t EEPROM[64];
				memcpy(EEPROM, reply + 4, 64);
				/*
				int PGain = (((EEPROM[6] & 0x0f) << 4)) | (EEPROM[7] & 0x0f);
				int Deadband = (((EEPROM[8] & 0x0f) << 4)) | (EEPROM[9] & 0x0f);
				int Damping = (((EEPROM[10] & 0x0f) << 4)) | (EEPROM[11] & 0x0f);
				int Response = (((EEPROM[50] & 0x0f) << 4)) | (EEPROM[51] & 0x0f);
				int Speed = (((EEPROM[4] & 0x0f) << 4)) | (EEPROM[5] & 0x0f);

				gLogManager.log("P.Gain: " + core::StringConverter::toString(PGain),ELL_INFO);
				gLogManager.log("Deadband:" + core::StringConverter::toString(Deadband), ELL_INFO);
				gLogManager.log("Damping: " + core::StringConverter::toString(Damping), ELL_INFO);
				gLogManager.log("Response: " + core::StringConverter::toString(Response), ELL_INFO);
				gLogManager.log("Speed: " + core::StringConverter::toString(Speed), ELL_INFO);
				
				*/
				EEPROM[4] = (m_parameters[index].Speed >> 4) & 0x0f;
				EEPROM[5] = m_parameters[index].Speed & 0x0f;
				 
				EEPROM[6] = (m_parameters[index].PGain >> 4) & 0x0f;
				EEPROM[7] = m_parameters[index].PGain & 0x0f;

				EEPROM[8] = (m_parameters[index].Deadband >> 4) & 0x0f;
				EEPROM[9] = m_parameters[index].Deadband & 0x0f;

				EEPROM[10] = (m_parameters[index].Damping >> 4) & 0x0f;
				EEPROM[11] = m_parameters[index].Damping & 0x0f;

				EEPROM[50] = (m_parameters[index].Response >> 4) & 0x0f;
				EEPROM[51] = m_parameters[index].Response & 0x0f;

				sCommand[0] = 0xC0 | ServoCODE[index];//Write command
				sCommand[1] = 0x00;//EEPROM access

				memcpy(sCommand + 2, EEPROM, 64 * sizeof(uint8_t));
				int ret = _sendCommand(sCommand, 66 * sizeof(char), reply, 68 * sizeof(char), 500);
				if (ret != 68)
				{
					gLogManager.log("TxKitHead::_writeEEPROM() - Failed to write to EEPROM:" + core::StringConverter::toString(index) , ELL_WARNING);
					continue;
				}
				else{
					gLogManager.log("TxKitHead::_writeEEPROM() -EEPROM Updated Successfully:" + core::StringConverter::toString(index), ELL_SUCCESS);
				}
			}
			else {
				gLogManager.log("TxKitHead::_writeEEPROM() -EEPROM Updated Successfully:" + core::StringConverter::toString(index), ELL_WARNING);
				continue;
			}
		}

		uint8_t reply;
		int ret;

		do{
			ret = m_serial->read(&reply, 1);
		} while (ret > 0);

		m_EEPROMset = true;
		return true;
	}
	bool TxKitHead::Connect(const core::string& port)
	{
		Disconnect();
		m_lastValues[0] = m_lastValues[1] = m_lastValues[2] = 0;
		gLogManager.log("Connecting", ELL_INFO);
		m_serial = new serial::Serial(port, 115200, serial::Timeout::simpleTimeout(30), serial::eightbits, serial::parity_even);
		connected = m_serial->isOpen();
		if (!connected)
		{
			//	printf("Failed to connect robot\n");
			delete m_serial;
			m_serial = 0;
		}
		else
		{
			gLogManager.log("Connected", ELL_INFO);
			//comROBOT->setRxSize(15);
			//_sendCommand("#ea");//enable angle logging

			//read EEPROM
			//Note: for some reason the servo IDs are changed when calling write EEPROM function.
			// This function is disabled for the time being
			_writeEEPROM();
		}
		//comROBOT->owner = this;
		return connected;
	}
	bool TxKitHead::IsConnected()
	{
		return connected;
	}
	void TxKitHead::Disconnect()
	{
		if (!m_serial)
			return;

		SetRotation(0);
		_sleep(100);

		uint8_t sCommand[] = { 0, 0, 0 };
		uint8_t reply[6];
		for (int i = 0; i < 3; ++i){

			sCommand[0] = 0x80 | ServoCODE[i];//(PosCtrlCMD << 5)
			sCommand[1] = 0;
			sCommand[2] = 0;
			_sendCommand(sCommand, 3 * sizeof(char), reply, 6);
			//	printf("%d,%d,%d,%d\n", value,(int)sCommand[0], (int)sCommand[1], (int)sCommand[2]);
		}
		m_lastValues[0] = m_lastValues[1] = m_lastValues[2] = 0;
		_sleep(100);
		m_serial->close();
		connected = false;
		delete m_serial;
		m_serial = 0;
	}

	int TxKitHead::_sendCommand(const uint8_t* cmd, int len, uint8_t* reply, int rlen, int waitTime)
	{
		m_serial->flushInput();
		m_serial->flushOutput();
		//comROBOT->sendData((char*)cmd, len);
		m_serial->write((const uint8_t*)cmd, len);

		if (waitTime>0)
			_sleep(waitTime);

		int ret = m_serial->read(reply, rlen);

		return ret;
		//if (ret != 6)
		//	gLogManager.log("Failed to read 6 bytes: "+core::StringConverter::toString(ret), ELL_INFO);
		//_sleep(1);
	}

	void TxKitHead::SetRotation(const math::vector3d& rotation)
	{
		float orot[3];
		float rot[3];


		for (int i = 0; i < 3; ++i)
			orot[i] = 0.5f + math::clamp(rotation[i], m_limits[i][0], m_limits[i][1]) / 270.0f; //pitch


		const int MinValue = 3500;
		const int MaxValue = 11500;

		const int PosCtrlCMD = 0x4;//0b100

		uint8_t reply[8];

		float encoders[3] = { 0, 0, 0 };
		uint8_t sCommand[3];
		for (int i = 0; i < 3; ++i){
			rot[i] = MinValue + orot[i] * (MaxValue - MinValue);
			unsigned short value = rot[i];
			if (value == m_lastValues[i])
			{
				//		encoders[i] = m_rotation[i];
				//		continue;
			}
			m_lastValues[i] = value;
			sCommand[0] = (uint8_t)(0x80 | ServoCODE[i]);//(PosCtrlCMD << 5)
			sCommand[1] = (uint8_t)((value >> 7) & 0x7F);
			sCommand[2] = (uint8_t)(value & 0x7F);
			int ret = _sendCommand(sCommand, 3 * sizeof(uint8_t), reply, 6,2);

			if (false && ret == 6)
			{
				ret = ((reply[4] & 0x7F) << 7) | (reply[5] & 0x7F);
			//	printf("%d:%d , %d:%d\n", sCommand[1], sCommand[2], reply[4], reply[5]);

				m_rotation[i] = ((float)(ret - MinValue) / (MaxValue - MinValue));
				m_rotation[i] = (m_rotation[i] - 0.5f)*270.0f;

			}
			else{
				m_rotation[i] = orot[i];
				m_rotation[i] = (m_rotation[i] - 0.5f)*270.0f;
			}


		}

		//printf("Out: %f,%f,%f\n", encoders[0], encoders[1], encoders[2]);
	}
	math::vector3d TxKitHead::GetRotation()
	{
		return math::vector3d(m_rotation[0], m_rotation[1], m_rotation[2]);
	}

	void TxKitHead::_onSerialData(int size, char *buffer)
	{
		return;
		char* ptr = buffer;
		buffer[size - 1] = 0;
		while (*ptr)
		{
			if (*ptr == '@')
			{
				++ptr;
				break;
			}
			++ptr;
		}
		if (!*ptr)
			return;
		std::vector<core::string> lst = core::StringUtil::Split(ptr, " ");
		if (lst.size() == 0)
			return;
		if (lst[0] == "angles ")//angles
		{
		}
	}

}
