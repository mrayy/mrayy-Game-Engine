
#ifndef __GNSSCONTROLLER__
#define __GNSSCONTROLLER__

#include "ServiceContext.h"
#include "serial/serial.h"
#include "plc_config.h"
#include "IThread.h"

namespace mray
{
class GNSSController
{
protected:
	core::string m_portname;
	serial::Serial* m_serial;
	MCClient *mc;
	mc_buff mcWriteBuf;
	GCPtr<OS::IThread> m_thread;
	core::string _plcIP;
	int _plcPort;

	bool _isStarted;

	struct GNSSData
	{
		//Internal Data

		bool _mcSuccess;

		// calculated values
		float dec_longitude;
		float dec_latitude;
		float altitude_ft;


		// GGA - Global Positioning System Fixed Data
		float nmea_longitude;
		float nmea_latitude;
		float utc_time;
		char ns, ew;
		int lock;
		int satelites;
		int satellites;
		float hdop;
		float msl_altitude;
		char msl_units;


		// RMC - Recommended Minimmum Specific GNS Data
		char rmc_status;
		float speed_k;
		float course_d;
		int date;

		// GLL
		char gll_status;

		// VTG - Course over ground, ground speed
		float course_t; // ground speed true
		char course_t_unit;
		float course_m; // magnetic
		char course_m_unit;
		char speed_k_unit;
		float speed_km; // speek km/hr
		char speed_km_unit;
	};

	GNSSData _data;
	GNSSData _data_copy;
	OS::IMutex* _dataMutex;
	 char *my_token(char *source, char token);
	 int GPS_Parser(char *string);
public:
	GNSSController();
	~GNSSController();

	bool Init(const core::string& portName,core::string plcIP,int plcPort);
	void Shutdown();
	bool IsOpen(){ return m_serial != 0 && mc!=0; }
	bool IsStarted(){ return _isStarted; }

	void Start();
	void Stop();

	void Render(TBee::ServiceRenderContext* context);

	void _Process();
};

}


#endif
