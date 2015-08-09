
#include "stdafx.h"
#include "ServiceLoader.h"

#include "WinFileSystem.h"
#include "Engine.h"
#include "WinOSystem.h"

namespace mray
{

ServiceLoader::ServiceLoader(int argc, _TCHAR* argv[])
{
	_init(argc, argv);
}
ServiceLoader::~ServiceLoader()
{
	_destroy();
}
void ServiceLoader::_init(int argc, _TCHAR* argv[])
{
	new OS::WinFileSystem();
	new Engine(new OS::WinOSystem());

	m_sharedMemory.SetDataSize(sizeof(m_memory));
	m_sharedMemory.SetName("SH_TBee_Service");

	m_sharedMemory.openWrite();
}

void ServiceLoader::_destroy()
{
	m_sharedMemory.Detach();
	delete Engine::getInstancePtr();
}

void ServiceLoader::Run()
{
}

}

