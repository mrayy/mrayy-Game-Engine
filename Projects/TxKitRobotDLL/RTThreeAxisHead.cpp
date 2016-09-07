

#include "stdafx.h"
#include "RTThreeAxisHead.h"
#include "StringUtil.h"



namespace mray
{


RTThreeAxisHead::RTThreeAxisHead()
{
	connected = false;
	_bufferPos = 0;
	sfp_device.sfp_device_num = 0;
	sfp_device.sfp_handle = 0;
}
RTThreeAxisHead::~RTThreeAxisHead()
{
	_turnServosOff(1);
	_turnServosOff(2);
	_turnServosOff(3);
	_flush();
	Disconnect();
}


bool RTThreeAxisHead::Connect(const core::string& port)
{
	Disconnect();

	int numDevs;
	numDevs = GetFTDIDeviceCount();

	if (numDevs == 0)
	{
		gLogManager.log("Couldnt find any FTDI devices connected!", ELL_WARNING);
		return false;
	}

	printf("Device information: \n");
	char buf[256];
	GetFTDIDeviceInfo(0, buf);
	byte rc = Initialize(0, &sfp_device);

	if (rc != SFP_OK)
	{
		gLogManager.log("Failed to retrive FTDI device details! ",ELL_WARNING);
		return false;
	}

	DWORD status = FT_SetBaudRate(sfp_device.sfp_handle, 1000000);
	//ChangeBaudRate(&sfp_device, 1000000);

	t = 0;

	_turnServosOn(1);
	_turnServosOn(2);
	_turnServosOn(3);
	_turnProjector(false);
	_flush();

	return IsConnected();
}
bool RTThreeAxisHead::IsConnected()
{
	return sfp_device.sfp_handle != 0;
}
void RTThreeAxisHead::Disconnect()
{
	if (!sfp_device.sfp_handle)
		return;
	_sendCommand("#q");

	SetRotation(0);

	ClosePort(&sfp_device);
	sfp_device.sfp_handle = 0;
	sfp_device.sfp_device_num = 0;
	connected = false;
}
int RTThreeAxisHead::calc_checksum(int crc_accum, byte* data_blk_ptr, int data_blk_size) // byte = unsigned char*
{
	int i, j;
	static const ushort crc_table[] {
		0x0000, 0x8005, 0x800F, 0x000A, 0x801B, 0x001E, 0x0014, 0x8011,
			0x8033, 0x0036, 0x003C, 0x8039, 0x0028, 0x802D, 0x8027, 0x0022,
			0x8063, 0x0066, 0x006C, 0x8069, 0x0078, 0x807D, 0x8077, 0x0072,
			0x0050, 0x8055, 0x805F, 0x005A, 0x804B, 0x004E, 0x0044, 0x8041,
			0x80C3, 0x00C6, 0x00CC, 0x80C9, 0x00D8, 0x80DD, 0x80D7, 0x00D2,
			0x00F0, 0x80F5, 0x80FF, 0x00FA, 0x80EB, 0x00EE, 0x00E4, 0x80E1,
			0x00A0, 0x80A5, 0x80AF, 0x00AA, 0x80BB, 0x00BE, 0x00B4, 0x80B1,
			0x8093, 0x0096, 0x009C, 0x8099, 0x0088, 0x808D, 0x8087, 0x0082,
			0x8183, 0x0186, 0x018C, 0x8189, 0x0198, 0x819D, 0x8197, 0x0192,
			0x01B0, 0x81B5, 0x81BF, 0x01BA, 0x81AB, 0x01AE, 0x01A4, 0x81A1,
			0x01E0, 0x81E5, 0x81EF, 0x01EA, 0x81FB, 0x01FE, 0x01F4, 0x81F1,
			0x81D3, 0x01D6, 0x01DC, 0x81D9, 0x01C8, 0x81CD, 0x81C7, 0x01C2,
			0x0140, 0x8145, 0x814F, 0x014A, 0x815B, 0x015E, 0x0154, 0x8151,
			0x8173, 0x0176, 0x017C, 0x8179, 0x0168, 0x816D, 0x8167, 0x0162,
			0x8123, 0x0126, 0x012C, 0x8129, 0x0138, 0x813D, 0x8137, 0x0132,
			0x0110, 0x8115, 0x811F, 0x011A, 0x810B, 0x010E, 0x0104, 0x8101,
			0x8303, 0x0306, 0x030C, 0x8309, 0x0318, 0x831D, 0x8317, 0x0312,
			0x0330, 0x8335, 0x833F, 0x033A, 0x832B, 0x032E, 0x0324, 0x8321,
			0x0360, 0x8365, 0x836F, 0x036A, 0x837B, 0x037E, 0x0374, 0x8371,
			0x8353, 0x0356, 0x035C, 0x8359, 0x0348, 0x834D, 0x8347, 0x0342,
			0x03C0, 0x83C5, 0x83CF, 0x03CA, 0x83DB, 0x03DE, 0x03D4, 0x83D1,
			0x83F3, 0x03F6, 0x03FC, 0x83F9, 0x03E8, 0x83ED, 0x83E7, 0x03E2,
			0x83A3, 0x03A6, 0x03AC, 0x83A9, 0x03B8, 0x83BD, 0x83B7, 0x03B2,
			0x0390, 0x8395, 0x839F, 0x039A, 0x838B, 0x038E, 0x0384, 0x8381,
			0x0280, 0x8285, 0x828F, 0x028A, 0x829B, 0x029E, 0x0294, 0x8291,
			0x82B3, 0x02B6, 0x02BC, 0x82B9, 0x02A8, 0x82AD, 0x82A7, 0x02A2,
			0x82E3, 0x02E6, 0x02EC, 0x82E9, 0x02F8, 0x82FD, 0x82F7, 0x02F2,
			0x02D0, 0x82D5, 0x82DF, 0x02DA, 0x82CB, 0x02CE, 0x02C4, 0x82C1,
			0x8243, 0x0246, 0x024C, 0x8249, 0x0258, 0x825D, 0x8257, 0x0252,
			0x0270, 0x8275, 0x827F, 0x027A, 0x826B, 0x026E, 0x0264, 0x8261,
			0x0220, 0x8225, 0x822F, 0x022A, 0x823B, 0x023E, 0x0234, 0x8231,
			0x8213, 0x0216, 0x021C, 0x8219, 0x0208, 0x820D, 0x8207, 0x0202
	};

	for (j = 0; j < data_blk_size; j++)
	{
		i = (((crc_accum >> 8) ^ (int)data_blk_ptr[j]) & 0xFF);
		crc_accum = ((crc_accum << 8) ^ crc_table[i]);
	}

	return crc_accum;
}

void RTThreeAxisHead::_searchServos()
{
	if (!IsConnected())
		return;
	//read ID data from robot servo
	byte send_packet [14];
	int b;
	int crc;
	//init servo id numbers to show in comboBox2
	int servo_num = 0;

	for (int i = 1; i < 254; i++)
	{
		b = 0;
		send_packet[b++] = (byte)(ROBOTIS_ID & 0xFF); //ID address low
		send_packet[b++] = (byte)((ROBOTIS_ID >> 8) & 0xFF); //ID address high
		send_packet[b++] = 1; //data size (byte) low
		send_packet[b++] = 0; //data size (byte) high
		set_packet(i, INST_READ, send_packet, b);
	}
}
void RTThreeAxisHead::_turnServosOn(int id)
{
	int size = 0;
	byte param [3];
	int current_mode = MODE_POS;

	// set parameters to move servo 
	param[size++] = (byte)(XM_TORQUE & 0xFF);
	param[size++] = (byte)((XM_TORQUE >> 8) & 0xFF);
	param[size++] = TORQUE_ON;

	set_packet(id, INST_WRITE, param, size);
}

void RTThreeAxisHead::_turnServosOff(int id)
{
	int size = 0;
	byte param[3];
	int current_mode = MODE_POS;

	// set parameters to move servo 
	param[size++] = (byte)(XM_TORQUE & 0xFF);
	param[size++] = (byte)((XM_TORQUE >> 8) & 0xFF);
	param[size++] = TORQUE_OFF;

	set_packet(id, INST_WRITE, param, size);
}
void RTThreeAxisHead::_turnProjector(bool on)
{
	if (!IsConnected())
		return;
	byte data = 0;
	byte mask = 0xf0;
	if (on == true)
	{
		//bit cbus 3 off = projector on
		data |= 0x00;
		data |= mask;
	}
	else
	{
		//bit cbus 3 on = projector off
		data |= 0x08;
		data |= mask;
	}
	FT_SetBitMode(sfp_device.sfp_handle, data, 32);
}
void RTThreeAxisHead::_setServoPos(int id, int pos)
{
	long val; //4byte
	int size = 0;
	byte param[6];
	int current_mode = MODE_POS;

	//get value from trackbar to move position
	val = (long)(pos * DEGREE_RESOLUTION);
	//            MessageBox.Show(trackBar1.Value.ToString() + "\n"+ val.ToString());

	// set parameters to move servo 
	param[size++] = (byte)(GOAL_POSITION & 0xFF);
	param[size++] = (byte)((GOAL_POSITION >> 8) & 0xFF);
	param[size++] = (byte)(val & 0x000000FF);
	param[size++] = (byte)((val & 0x0000FF00) >> 8);
	param[size++] = (byte)((val & 0x00FF0000) >> 16);
	param[size++] = (byte)((val & 0xFF000000) >> 24);

	// servo id set to control
	set_packet(id, INST_WRITE, param, size);
}

void RTThreeAxisHead::set_packet(int id, int com, byte* param, int size)
{
	if (!IsConnected())
		return;
	byte send_packet [256];
	int b = 0;
	int crc;

	//            MessageBox.Show(size.ToString());

	send_packet[b++] = 0xFF;
	send_packet[b++] = 0xFF;
	send_packet[b++] = 0xFD;
	send_packet[b++] = 0x00; //reserved

	send_packet[b++] = (byte)(int)(id & 0xFF);//id

	send_packet[b++] = (byte)((size + 3) & 0xFF);//length low add 2 to size for crc 2byte
	send_packet[b++] = (byte)(((size + 3) >> 8) & 0xFF);//length high

	send_packet[b++] = (byte)((com)& 0xFF); //command (R/W)

	for (int i = 0; i < size; i++)
	{
		send_packet[b++] = param[i];
	}


	//checksum
	crc = calc_checksum(0, send_packet, b);
	send_packet[b++] = (byte)(crc & 0xFF);
	send_packet[b++] = (byte)((crc >> 8) & 0xFF);

	DWORD written = 0;
	FT_Write(sfp_device.sfp_handle, &send_packet, b, &written);
	Sleep(1);
	/*
	if (_bufferPos + b >= 256)
		_flush();

	memcpy(_buffer + _bufferPos, send_packet, b);
	_bufferPos += b;*/

}

void RTThreeAxisHead::_flush()
{/*
	comROBOT->sendData((char*)_buffer, _bufferPos);
	_bufferPos = 0;
	Sleep(10);*/

}
void RTThreeAxisHead::_sendCommand(const std::string& cmd)
{
	std::string str = cmd + " \n\r";
	//comROBOT->sendData((char*)str.c_str(), str.length());
}
void RTThreeAxisHead::SetRotation(const math::vector3d& rotation)
{
	if (!IsConnected())
		return;

	t += 0.01f;
	float r = sinf(t)* 60;


	_setServoPos(1, rotation.z + 180);
	_setServoPos(2, rotation.x + 180);
	_setServoPos(3, rotation.y + 180 );
	_flush();
	m_rotation = rotation;
}
math::vector3d RTThreeAxisHead::GetRotation()
{
	return m_rotation;
}

void RTThreeAxisHead::_onSerialData(int size, char *buffer)
{/*
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
		m_rotation.x = core::StringConverter::toFloat(lst[1]);
		m_rotation.y = core::StringConverter::toFloat(lst[2]);
		m_rotation.z = core::StringConverter::toFloat(lst[3]);
	}*/
}

}
