/********************************************************************
	created:	2012/07/29
	created:	29:7:2012   16:31
	filename: 	d:\Development\mrayEngine\Projects\VirtualTelesar\VirtualTelesarApp\VTSharedMemory.h
	file path:	d:\Development\mrayEngine\Projects\VirtualTelesar\VirtualTelesarApp
	file base:	VTSharedMemory
	file ext:	h
	author:		MHD YAMEN SARAIJI
	
	purpose:	
*********************************************************************/
#ifndef ___VTSharedMemory___
#define ___VTSharedMemory___

#include "VTShCompileConfig.h"


#include "ICommunicationLayer.h"

namespace mray
{
namespace VT
{
	class AverageWindow;

	class VTSharedMemoryData;
	class VTSharedMemoryImpl;

class VTSHAREDMEM_DLL VTSharedMemory:public ICommunicationLayer
{
protected:
	VTSharedMemoryImpl* m_impl;
public:
	VTSharedMemory(const core::string& name);
	virtual~VTSharedMemory();

	void Start();
	virtual void Close();

	void Update(float dt);

	float GetTemperature(bool left,int index);
	float GetForce(bool left,int index);

	void GetDebugString(std::vector<core::string>& lst);
	
	const std::vector<core::string>& GetScheme();

	core::string InjectCommand(const core::string& cmd, const core::string& args);
};

}
}

#endif