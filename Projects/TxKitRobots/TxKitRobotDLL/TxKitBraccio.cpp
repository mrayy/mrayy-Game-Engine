

#include "stdafx.h"
#include "TxKitBraccio.h"
#include "StringUtil.h"

#include <windowsx.h>
#include <fstream>


namespace mray
{

	TxKitBraccio::TxKitBraccio()
	{
		connected = false;
		m_serial = 0;

		_joints[0] = 90;
		_joints[1] = 45;
		_joints[2] = 180;
		_joints[3] = 180;
		_joints[4] = 90;
		_joints[5] = 10;

	}
	TxKitBraccio::~TxKitBraccio()
	{
		Disconnect();
		if (m_serial)
			delete m_serial;
	}

	bool TxKitBraccio::Connect(const core::string& p)
	{
		Disconnect();
		core::string port = p;

		gLogManager.log("Connecting to: " + port, ELL_INFO);
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
			//_writeEEPROM();
		}
		//comROBOT->owner = this;
		return connected;
	}
	bool TxKitBraccio::IsConnected()
	{
		return connected;
	}
	void TxKitBraccio::Disconnect()
	{
		if (!m_serial)
			return;

		SetRotation(0);
		SetPosition(0);
		_sleep(200);

		m_serial->close();
		connected = false;
		delete m_serial;
		m_serial = 0;
	}

	void TxKitBraccio::_sendCommand()
	{
		m_serial->flushInput();
		m_serial->flushOutput();

		byte data[7];
		data[0] = 0xF7;
		data[1] = (byte)(_joints[0]);
		data[2] = (byte)(_joints[1]);
		data[3] = (byte)(_joints[2]);
		data[4] = (byte)(_joints[3]);
		data[5] = (byte)(_joints[4]);
		data[6] = (byte)(_joints[5]);
		m_serial->write(data, 7);
	}

	void TxKitBraccio::SetRotation(const math::vector3d& rotation)
	{
		_rotation = rotation;
	}

	void TxKitBraccio::SetPosition(const math::vector3d& pos)
	{
		_position = pos;
		_calcIK(_position.x, _position.y + 0.224f, _position.z+0.04f);
	}

	void TxKitBraccio::UpdateThreaded()
	{
		_sendCommand();
	}


	void TxKitBraccio::_calcIK(float x,float y,float z)
	{

		// Base angle
		float bas_angle_r = atan2f(x, z);
		float bas_angle_d = math::toDeg(bas_angle_r ) + 90.0f;

		float wrt_y = y - BASE_HGT; // Wrist relative height to shoulder
		float s_w = x * x + z * z + wrt_y * wrt_y; // Shoulder to wrist distance square
		float s_w_sqrt = sqrtf(s_w);

		// Elbow angle: knowing 3 edges of the triangle, get the angle
		float elb_angle_r = acosf((hum_sq + uln_sq - s_w) / (2.0f * HUMERUS * ULNA));
		float elb_angle_d = 270.0f - math::toDeg( elb_angle_r );

		// Shoulder angle = a1 + a2
		float a1 = atan2f(wrt_y, sqrtf(x * x + z * z));
		float a2 = acosf((hum_sq + s_w - uln_sq) / (2.0f * HUMERUS * s_w_sqrt));
		float shl_angle_r = a1 + a2;
		float shl_angle_d = 180.0f - math::toDeg(shl_angle_r );


		// Update angles
		if (bas_angle_d >= 0.0f && bas_angle_d <= 180.0f)
			_joints[0] = bas_angle_d;
		if (shl_angle_d >= 15.0f && shl_angle_d <= 165.0f)
			_joints[1] = shl_angle_d;
		if (elb_angle_d >= 0.0f && elb_angle_d <= 180.0f)
			_joints[2] = elb_angle_d;
	}

}
