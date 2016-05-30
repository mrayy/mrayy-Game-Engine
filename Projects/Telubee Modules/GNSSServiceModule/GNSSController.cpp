

#include "stdafx.h"
#include "GNSSController.h"
#include "IThreadFunction.h"
#include "IThreadManager.h"

namespace mray
{

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
	mc = 0;
	m_thread = 0;
	_dataMutex = 0;
}
GNSSController::~GNSSController()
{
	Shutdown();
}

bool GNSSController::Init(const core::string& portName, core::string plcIP, int plcPort)
{
	if (m_serial)
		delete m_serial;

	m_portname = portName;
	_plcPort = plcPort;
	_plcIP = plcIP;

	unsigned long baud = 0;
	baud = 9600;

	_dataMutex = OS::IThreadManager::getInstance().createMutex();

	m_serial = new serial::Serial(portName, baud, serial::Timeout::simpleTimeout(1000));
	if (m_serial->isOpen())
		gLogManager.log("GNSS SerialPort is Open", ELL_SUCCESS);
	else
	{
		gLogManager.log("Failed to open GNSS SerialPort", ELL_WARNING);
		delete m_serial;
		m_serial = 0;
		return false;
	}

	printf("Trying to connect to a PLC...\r\n");

	Sleep(100);

	memset(&mcWriteBuf, 0, sizeof(mcWriteBuf)); // initializing test write data

	// create connection to Melsec PC via MC Protocol 
	mc = new MCClient(_plcPort, (char*)_plcIP.c_str());


	Sleep(10);

	m_thread = OS::IThreadManager::getInstance().createThread(new GNSSThread(this));
	m_thread->start(0);

	return true;
}
void GNSSController::Shutdown()
{
	if (mc)
		delete mc;
	if (m_serial)
		delete m_serial;
	if (_dataMutex)
		delete _dataMutex;

	_dataMutex = 0;
	mc = 0;
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

	core::string msg;

	msg = "Long: " + core::StringConverter::toString(_data_copy.nmea_longitude) + ", Lat: " + core::StringConverter::toString(_data_copy.nmea_latitude);
	context->RenderText(msg,0,0,video::SColor(1,1,1,1));

}

void GNSSController::_Process()
{

	std::string result = m_serial->readline();
	const char *cstr = result.c_str();
	char buffer[256];
	float lastTime = 0;

	_dataMutex->lock();
	if (GPS_Parser((char*)cstr) != 0 && _data.utc_time != lastTime)
	{
		Sleep(100);

		lastTime = _data.utc_time;

		//printf("[%f] lat: %f, long: %f\r\n", _data.utc_time, _data.nmea_latitude, _data.nmea_longitude);
		sprintf(buffer, "%f\t%f\t%f\n", _data.utc_time, _data.nmea_latitude, _data.nmea_longitude);

		mcWriteBuf.torso.cameraFPS = _data.nmea_latitude;
		mcWriteBuf.torso.oculusFPS = _data.nmea_longitude;

		_data._mcSuccess = mc->batch_write("W", SELECT_COMMON, 0x034F, &mcWriteBuf, 0x05) != 0;
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
		if (field[0] && strcmp(field[0], "$GNGGA") == 0) {
			/* Retrieve the values from the remaining fields */
			CHECK(_data.utc_time, field[1], atof(field[1]));
			CHECK(_data.nmea_latitude, field[2], atof(field[2]));
			//CHECK(ns, field[3], *(field[3]));
			CHECK(_data.nmea_longitude, field[4], atof(field[4]));
			//CHECK(ew, field[5], *(field[5]));
			//CHECK(lock, field[6], atof(field[6]));
			//CHECK(satellites, field[7], atof(field[7]));
			//CHECK(hdop, field[8], atof(field[8]));
			CHECK(_data.msl_altitude, field[9], atof(field[9]));
			//CHECK(msl_units, field[10], *(field[10]));
		}
		if (field[0] && strcmp(field[0], "$GNRMC") == 0) {
			/* Retrieve the data from the remaining fields */
			CHECK(_data.utc_time, field[1], atof(field[1]));
			CHECK(_data.nmea_latitude, field[3], atof(field[3]));
			//CHECK(ns, field[4], *(field[4]));
			CHECK(_data.nmea_longitude, field[5], atof(field[5]));
			//CHECK(ew, field[6], *(field[6]));

			CHECK(_data.speed_k, field[7], atof(field[7]));
			CHECK(_data.course_d, field[8], atof(field[8]));
			CHECK(_data.date, field[9], atol(field[9]));
		}

		if (!field[0])
			return 0;
	}
	return field_count;

fail:
	return 0;
}


char* GNSSController::my_token(char *source, char token)
{
	char stat_string[128]; // used in my_token
	char *current;
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

	while (true) {
		/* If we're at the end of the string, return NULL */
		if ((*current == '\0') && (current == start))
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


