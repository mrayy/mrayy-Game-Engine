
#include "stdafx.h"
#include "UnityExports.h"


extern "C" EXPORT_API PLCHandler* CreatePLCDriver()
{
	return new PLCHandler();
}

extern "C" EXPORT_API void PLCDriverDestroy(PLCHandler* driver)
{
	delete driver;
}
extern "C" EXPORT_API bool PLCDriverConnect(PLCHandler* driver, const char* ip, int port)
{
	if (driver)
	{
		return driver->ConnectToPLC(ip, port);
	}
	return false;
}

extern "C" EXPORT_API bool PLCDriverIsConnected(PLCHandler* driver)
{
	if (driver)
	{
		return driver->IsConnected();
	}
	return false;

}

extern "C" EXPORT_API bool PLCDriverDisconnect(PLCHandler* driver)
{
	if (driver)
	{
		return driver->CloseConnection();
	}
	return false;
}

extern "C" EXPORT_API void PLCDriverRead(PLCHandler* driver)
{
	if (driver)
	{
		 driver->ReadData();
	}

}
extern "C" EXPORT_API void PLCDriverWrite(PLCHandler* driver)
{
	if (driver)
	{
		driver->WriteData();
	}

}


extern "C" EXPORT_API void PLCSetTorsoUInt(PLCHandler* driver, ETorsoDataField data, unsigned int v)
{
	if (driver)
	{
		 driver->SetTorsoDataUInt(data,v);
	}
}
extern "C" EXPORT_API void PLCSetTorsoUShort(PLCHandler* driver, ETorsoDataField data, unsigned short v)
{
	if (driver)
	{
		driver->SetTorsoDataUShort(data, v);
	}
}

extern "C" EXPORT_API unsigned int PLCGetTorsoUInt(PLCHandler* driver, ETorsoDataField data)
{
	if (driver)
	{
		return driver->GetTorsoUInt(data);
	}
	return 0;
}
extern "C" EXPORT_API  int PLCGetTorsoInt(PLCHandler* driver, ETorsoDataField data)
{
	if (driver)
	{
		return driver->GetTorsoInt(data);
	}
	return 0;
}
extern "C" EXPORT_API unsigned short PLCGetTorsoUShort(PLCHandler* driver, ETorsoDataField data)
{
	if (driver)
	{
		return driver->GetTorsoUShort(data);
	}
	return 0;
}



extern "C" EXPORT_API void PLCSetXyrisUInt(PLCHandler* driver, EXyrisDataField data, unsigned int v)
{
	if (driver)
	{
		driver->SetXyrisDataUInt(data, v);
	}
}
extern "C" EXPORT_API void PLCSetXyrisUShort(PLCHandler* driver, EXyrisDataField data, unsigned short v)
{
	if (driver)
	{
		driver->SetXyrisDataUShort(data, v);
	}
}

extern "C" EXPORT_API unsigned int PLCGetXyrisUInt(PLCHandler* driver, EXyrisDataField data)
{
	if (driver)
	{
		return driver->GetXyrisUInt(data);
	}
	return 0;
}

extern "C" EXPORT_API  short PLCGetXyrisShort(PLCHandler* driver, EXyrisDataField data)
{
	if (driver)
	{
		return driver->GetXyrisShort(data);
	}
	return 0;
}
extern "C" EXPORT_API unsigned short PLCGetXyrisUShort(PLCHandler* driver, EXyrisDataField data)
{
	if (driver)
	{
		return driver->GetXyrisUShort(data);
	}
	return 0;
}