

#pragma  once

#include "UnityPlugin.h"
#include "PLCHandler.h"


extern "C" EXPORT_API PLCHandler* CreatePLCDriver();
extern "C" EXPORT_API bool PLCDriverConnect(PLCHandler* driver, const char* ip, int port);
extern "C" EXPORT_API bool PLCDriverIsConnected(PLCHandler* driver);
extern "C" EXPORT_API bool PLCDriverDisconnect(PLCHandler* driver);
extern "C" EXPORT_API void PLCDriverDestroy(PLCHandler* driver);
extern "C" EXPORT_API void PLCDriverRead(PLCHandler* driver);
extern "C" EXPORT_API void PLCDriverWrite(PLCHandler* driver);


extern "C" EXPORT_API void PLCSetTorsoUInt(PLCHandler* driver, ETorsoDataField data, unsigned int v);
extern "C" EXPORT_API void PLCSetTorsoUShort(PLCHandler* driver, ETorsoDataField data, unsigned short v);
extern "C" EXPORT_API unsigned int PLCGetTorsoUInt(PLCHandler* driver, ETorsoDataField data);
extern "C" EXPORT_API  int PLCGetTorsoInt(PLCHandler* driver, ETorsoDataField data);
extern "C" EXPORT_API unsigned short PLCGetTorsoUShort(PLCHandler* driver, ETorsoDataField data);

extern "C" EXPORT_API void PLCSetXyrisUInt(PLCHandler* driver, EXyrisDataField data, unsigned int v);
extern "C" EXPORT_API void PLCSetXyrisUShort(PLCHandler* driver, EXyrisDataField data, unsigned short v);
extern "C" EXPORT_API unsigned int PLCGetXyrisUInt(PLCHandler* driver, EXyrisDataField data);
extern "C" EXPORT_API short PLCGetXyrisShort(PLCHandler* driver, EXyrisDataField data);
extern "C" EXPORT_API unsigned short PLCGetXyrisUShort(PLCHandler* driver, EXyrisDataField data);

extern "C" EXPORT_API void PLCSetYbmUInt(PLCHandler* driver, EYbmDataField data, unsigned int v);
extern "C" EXPORT_API void PLCSetYbmUShort(PLCHandler* driver, EYbmDataField data, unsigned short v);
extern "C" EXPORT_API unsigned int PLCGetYbmUInt(PLCHandler* driver, EYbmDataField data);
extern "C" EXPORT_API unsigned short PLCGetYbmUShort(PLCHandler* driver, EYbmDataField data);


extern "C" EXPORT_API unsigned short PLCGetCommonUShort(PLCHandler* driver, ECommonDataField data);
extern "C" EXPORT_API short PLCGetCommonShort(PLCHandler* driver, ECommonDataField data);