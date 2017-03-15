

#include "stdafx.h"
#include "GNSSController.h"
#include "IThreadFunction.h"
#include "IThreadManager.h"


std::string testSting;
std::string testSting2;




namespace mray
{

	float lastTime = 0;

	class GNSSThread :public OS::IThreadFunction
	{
	public:
		GNSSController * owner;

		GNSSThread(GNSSController* o){
			owner = o;
		}
		virtual void execute(OS::IThread*caller, void*arg)
		{
			while (caller->isActive())
			{
				if (owner->IsStarted())
				{
					owner->_Process();
				}else
					Sleep(100);
			}
		}
	};

GNSSController::GNSSController()
{
	m_serial = 0;
//	mc = 0;
	m_thread = 0;
	_dataMutex = 0;
	m_shData = 0;
}
GNSSController::~GNSSController()
{
	Shutdown();
}

bool GNSSController::Init(const core::string& portName/*, core::string plcIP, int plcPort*/)
{
	if (m_serial)
		delete m_serial;

	m_portname = portName;
// 	_plcPort = plcPort;
// 	_plcIP = plcIP;


	//start shared memory
	m_sharedMemory.SetDataSize(sizeof(mc_gnss));
	m_sharedMemory.SetName("SH_Tx_GNSS");
	m_sharedMemory.createWrite();

	m_shData = m_sharedMemory.GetData<mc_gnss>();
	if (!m_shData)
	{
		gLogManager.log("Couldnt open shared memory! Make sure no other GNSS service is running!", ELL_WARNING);
		exit(0);
	}

	unsigned long baud = 0;
	baud = 115200;

	_dataMutex = OS::IThreadManager::getInstance().createMutex();

	m_serial = new serial::Serial(portName, baud, serial::Timeout::simpleTimeout(1000));
	// DTR must be pulled manually in case of CDC / LUFA USB Serial Drivers

	if (m_serial->isOpen())
		gLogManager.log("GNSS SerialPort is Open", ELL_SUCCESS);
	else
	{
		gLogManager.log("Failed to open GNSS SerialPort", ELL_WARNING);
		delete m_serial;
		m_serial = 0;
		return false;
	}


	m_serial->setDTR(true);
	Sleep(10);

	m_thread = OS::IThreadManager::getInstance().createThread(new GNSSThread(this));
	m_thread->start(0);

	return true;
}


void GNSSController::Shutdown()
{
	/*if (mc)
		delete mc;
	mc = 0;*/
	if (m_serial)
		delete m_serial;
	if (_dataMutex)
		delete _dataMutex;

	_dataMutex = 0;
	m_serial = 0;

}

void GNSSController::Start()
{
	if (!IsOpen())
		return;

	_isStarted = true;
}
void GNSSController::Stop()
{
	_isStarted = false;
}

void GNSSController::Render(TBee::ServiceRenderContext* context)
{
	if (!IsOpen())
	{
		context->RenderText("GNSS port is not open!", 0, 0, video::SColor(1, 0, 0, 1));
		return;
	}

	if (!IsStarted())
	{
		context->RenderText("GNSS is waiting for connection.", 0, 0, video::SColor(1, 1, 0, 1));
		return;
	}
	else
		context->RenderText("GNSS has started.", 0, 0, video::SColor(0, 1, 0, 1));

	if (_dataMutex->tryLock())
	{
		memcpy(&_data_copy, &_data, sizeof(_data));
		_dataMutex->unlock();
	}



	char buff[128];
	sprintf(buff, "lk:%d, utc:%6.1f, lat:%4.6f, lon:%5.6f, alt:%5.3f mn:%3.2f, gs:%3.2f\r\n", _data.lock, _data.utc_time, _data.nmea_latitude, _data.nmea_longitude, _data.msl_altitude, _data.course_m, _data.speed_km);
	core::string gnss_msg(buff);

	context->RenderText(gnss_msg, 0, 0, video::SColor(1, 0, 0, 1));

}

void GNSSController::_Process()
{
	std::string result = m_serial->readline();
	const char *cstr = result.c_str();
	
	_dataMutex->lock();

	//if (GPS_Parser((char*)cstr) != 0)
	if (GPS_Parser((char*)cstr) != 0 && _data.utc_time != lastTime)
	{
		lastTime = _data.utc_time;

		//cast the floating point data to int, shift the floating point 5 digits, and write to PLC memory
		m_shData->gps_locked = (unsigned short)(_data.lock);
		m_shData->latitude = (unsigned __int64)(_data.nmea_latitude * 1000000);
		m_shData->longitude = (unsigned __int64)(_data.nmea_longitude * 1000000);
		m_shData->altitude = (signed int)(_data.msl_altitude * 1000);
		m_shData->true_heading = (unsigned short)(_data.course_t * 100);
		m_shData->magnetic_heading = (unsigned short)(_data.course_m * 100);
		m_shData->ground_speed = (unsigned short)(_data.speed_km * 100);
		m_shData->pitch = (unsigned short)(0.00 * 100);
		m_shData->roll = (unsigned short)(0.00 * 100);
	}
	_dataMutex->unlock();


	
}


int GNSSController::GPS_Parser(char *string)
{
#define CHECK(v,f,s)if(f==0)goto fail;else v=s;
	char *field[50]; // used by parse nmea
	int field_count;
	field_count = 0;

	/* NMEA 0183 fields are delimited by commas. The my_token function returns
	pointers to the fields.
	*/
	/* Get the first field pointer */
	field[0] = my_token(string, ',');
	field_count++;

	while (true) {
		/* Contiue retrieving fields until there are no more (NULL) */
		field[field_count] = my_token(NULL, ',');
		if (field[field_count] == NULL)
			break;
		field_count++;
	}
	/* If we got at least ONE field */

	if (field_count) {
	/* Check the first field for the valid NMEA 0183 headers */
		if (field[0] && strcmp(field[0], "$GPGGA") == 0) {
			/* Retrieve the values from the remaining fields */
			CHECK(_data.utc_time, field[1], atof(field[1]));
			CHECK(_data.nmea_latitude, field[2], atof(field[2]));
			CHECK(_data.ns, field[3], *(field[3]));
			CHECK(_data.nmea_longitude, field[4], atof(field[4]));
			CHECK(_data.ew, field[5], *(field[5]));
			CHECK(_data.lock, field[6], atof(field[6]));
			CHECK(_data.satellites, field[7], atof(field[7]));
			CHECK(_data.hdop, field[8], atof(field[8]));
			CHECK(_data.msl_altitude, field[9], atof(field[9]));
			CHECK(_data.msl_units, field[10], *(field[10]));
		}
		if (field[0] && strcmp(field[0], "$GPVTG") == 0) {
			/* Retrieve the data from the remaining fields */
			CHECK(_data.course_t, field[1], atof(field[1]));
			CHECK(_data.course_t_unit, field[2], *(field[2]));
			CHECK(_data.course_m, field[3], atof(field[3]));
			CHECK(_data.course_m_unit, field[4], *(field[4]));
			CHECK(_data.speed_k, field[5], atof(field[5]));
			CHECK(_data.speed_k_unit, field[6], *(field[6]));
			CHECK(_data.speed_km, field[7], atof(field[7]));
			CHECK(_data.speed_km_unit, field[8], *(field[8]));
		}

		if (!field[0])
			return 0;
	}
	return field_count;

fail:
	return 0;
}



char stat_string[128]; // used in my_token
char *current;

char* GNSSController::my_token(char *source, char token)
{
	
	char *start;
	/* The source string is real only for the first call. Subsequent calls
	are made with the source string pointer as NULL
	*/
	if (source != NULL) {
		/* If the string is empty return NULL */
		if (strlen(source) == 0)
			return NULL;
		strcpy(stat_string, source);
		/* Current is our 'current' position within the string */
		current = stat_string;
	}
	start = current;

	if (current == nullptr)
		return NULL;

	while (true) {
		/* If we're at the end of the string, return NULL */
		if ((*current == '\0') && (current == start))
		//if (!current || (*current == '\0') && (current == start))
			return NULL;
		/* If we're at the end now, but weren't when we started, we need
		to return the pointer for the last field before the end of string
		*/
		if (*current == '\0')
			return start;
		/* If we've located our specified token (comma) in the string
		load its location in the copy with an end of string marker
		so that it can be handled correctly by the calling program.
		*/
		if (*current == token) {
			*current = '\0';
			current++;
			return start;
		}
		else {
			current++;
		}
	}
}


}


