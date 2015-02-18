
#include "stdafx.h"
#include "OFSerialPort.h"
#include "ofSerial.h"

#include <setupapi.h>
// 	#include <regstr.h>

#pragma comment (lib,"setupapi.lib")

namespace mray
{
namespace OS
{

OFSerialPort::OFSerialPort()
{
	m_sp=new ofSerial();
	m_open=false;
	m_stream=new SerialPortStream(this);
}
OFSerialPort::~OFSerialPort()
{
	delete m_sp;
	delete m_stream;
}


bool OFSerialPort::OpenByName(const core::string& port,int baudRate)
{
	if(m_open)
		Close();
	m_open=m_sp->setup(port.c_str(),baudRate);
	return m_open;
}
bool OFSerialPort::OpenByID(int ID,int baudRate)
{
	if(m_open)
		Close();
	m_open=m_sp->setup(ID,baudRate);
	return m_open;
}
void OFSerialPort::Close()
{
	m_open=false;
	m_sp->close();

}

int OFSerialPort::Write(const void* data,size_t size)
{
	return m_sp->writeBytes((const unsigned char*)data,size);
}
int OFSerialPort::Read(void* data,size_t size)
{
	return m_sp->readBytes((unsigned char*)data,size);
}

int OFSerialPort::AvailableData()
{
	return m_sp->available();
}
bool OFSerialPort::IsOpen()
{
	return m_open;
}
void OFSerialPort::Flush(bool flushIn,bool flushOut)
{
	m_sp->flush(flushIn,flushOut);
}

IStream* OFSerialPort::GetStream()
{
	return m_stream;
}



class OFServiceProvider
{
	char 		** portNamesShort;//[MAX_SERIAL_PORTS];
	char 		** portNamesFriendly; ///[MAX_SERIAL_PORTS];
	int nPorts;
	bool bPortsEnumerated;
	void enumerateWin32Ports();
	std::string				deviceType;
	std::vector <ofSerialDeviceInfo> devices;
	bool bHaveEnumeratedDevices;

	static bool isDeviceArduino( ofSerialDeviceInfo & A ){
		//TODO - this should be ofStringInString
		return ( strstr(A.getDeviceName().c_str(), "usbserial") != NULL );
	}

public:
	OFServiceProvider()
	{
		nPorts=0;
		bPortsEnumerated=false;
	}
	std::vector<SerialPortInfo> EnumAvaliablePorts();
}s_serviceProvider;

void OFServiceProvider::enumerateWin32Ports()
{

	// thanks joerg for fixes...

	if (bPortsEnumerated == true) return;

	HDEVINFO hDevInfo = NULL;
	SP_DEVINFO_DATA DeviceInterfaceData;
	int i = 0;
	DWORD dataType, actualSize = 0;
	unsigned char dataBuf[MAX_PATH + 1];

	// Reset Port List
	nPorts = 0;
	// Search device set
	hDevInfo = SetupDiGetClassDevs((struct _GUID *)&GUID_SERENUM_BUS_ENUMERATOR,0,0,DIGCF_PRESENT);
	if ( hDevInfo ){
		while (TRUE){
			ZeroMemory(&DeviceInterfaceData, sizeof(DeviceInterfaceData));
			DeviceInterfaceData.cbSize = sizeof(DeviceInterfaceData);
			if (!SetupDiEnumDeviceInfo(hDevInfo, i, &DeviceInterfaceData)){
				// SetupDiEnumDeviceInfo failed
				break;
			}

			if (SetupDiGetDeviceRegistryPropertyA(hDevInfo,
				&DeviceInterfaceData,
				SPDRP_FRIENDLYNAME,
				&dataType,
				dataBuf,
				sizeof(dataBuf),
				&actualSize)){

					sprintf(portNamesFriendly[nPorts], "%s", dataBuf);
					portNamesShort[nPorts][0] = 0;

					// turn blahblahblah(COM4) into COM4

					char *   begin    = NULL;
					char *   end    = NULL;
					begin          = strstr((char *)dataBuf, "COM");


					if (begin)
					{
						end          = strstr(begin, ")");
						if (end)
						{
							*end = 0;   // get rid of the )...
							strcpy(portNamesShort[nPorts], begin);
						}
						if (nPorts++ > MAX_SERIAL_PORTS)
							break;
					}
			}
			i++;
		}
	}
	SetupDiDestroyDeviceInfoList(hDevInfo);

	bPortsEnumerated = false;
}

std::vector<SerialPortInfo> OFServiceProvider::EnumAvaliablePorts()
{
	std::vector<SerialPortInfo> retList;

	deviceType = "serial";
	devices.clear();

	std::vector <std::string> prefixMatch;

	//---------------------------------------------
#ifdef TARGET_WIN32
	//---------------------------------------------
	enumerateWin32Ports();
	//ofLogNotice() << "ofSerial: listing devices (" << nPorts << " total)";
	for (int i = 0; i < nPorts; i++){
		//NOTE: we give the short port name for both as that is what the user should pass and the short name is more friendly
		ofSerialDeviceInfo ifo;
		ifo.devicePath=portNamesShort[i];
		ifo.deviceID=i;
		devices.push_back(ifo);
	}
	//---------------------------------------------
#endif
	//---------------------------------------------

	//here we sort the device to have the aruino ones first. 
	partition(devices.begin(), devices.end(), isDeviceArduino);
	//we are reordering the device ids. too!
	for(int k = 0; k < (int)devices.size(); k++){
		devices[k].deviceID = k;
		SerialPortInfo ifo;
		ifo.ID=devices[k].getDeviceID();
		ifo.Name=devices[k].getDeviceName().c_str();
		retList.push_back(ifo);
	}

	bHaveEnumeratedDevices = true;
	return retList;
}


}
}

