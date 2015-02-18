// TestDLL.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "GCCollector.h"
#include "WinOSystem.h"
#include "PluginManager.h"
#include "IDeviceFactory.h"
#include "JobPool.h"

using namespace mray;
using namespace core;
using namespace math;

class TestThread:public OS::IThreadFunction
{
public:
	virtual void setup()
	{
	}
	virtual void execute(OS::IThread*caller,void*arg)
	{
	}
};
int _tmain(int argc, _TCHAR* argv[])
{
#if defined(DEBUG) | defined(_DEBUG)
	//	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );//_crtBreakAlloc
	long temp=-1;
	_crtBreakAlloc=temp;
#endif

	GCPtr<video::ITexture> tex=0;

	core::string resFileName=mT("plugins.stg");
	GCPtr<Engine> engine=new Engine(new OS::WinOSystem());
	PluginManager::getInstance().addPluginFromDLL(OS::IDllManager::getInstance().getLibrary(mT("mrayGL_d.dll")));
	PluginManager::getInstance().addPluginFromDLL(OS::IDllManager::getInstance().getLibrary(mT("mrayParticles_d.dll")));
	GCPtr<video::IVideoDevice> dev=engine->createDevice("OpenGL");
	GCPtr<video::RenderWindow> rw= dev->CreateRenderWindow(mT(""),500,0,OptionContainer(),0);


	JobPool::getInstance().SetupDefaultPools();
	tex=0;
	rw=0;
	dev=0;
	engine=0;

	GCCollector::shutdown();
	return 0;
}

