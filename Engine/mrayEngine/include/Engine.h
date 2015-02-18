
#ifndef ___Engine___
#define ___Engine___



#include "GCPtr.h"
#include "IOSystem.h"
#include "IVideoDevice.h"


namespace mray
{
	namespace core
	{
		class FPSCalc;
	}
	namespace OS
	{
		class ITimer;
	}
	class BenchmarkItem;
	class EngineImpl;

class MRAY_DLL Engine:public ISingleton<Engine>
{
protected:

	EngineImpl* m_impl;

	void updateBenchMark();
public:

	Engine(GCPtr<OS::IOSystem> system);
	virtual~Engine();



	void updateEngine();


	//GCPtr<video::IVideoDevice> createDevice(video::EDeviceType deviceType,uint window,math::Point2di size,int bits,
	//	bool vsync=false,int multiSample=0,bool stencilBuffer=false);

	GCPtr<video::IVideoDevice> createDevice(const core::string& type);



	void loadPlugins(const core::string &pluginsConfigFile);
	void loadResourceFile(const core::string &resFile);

	GCPtr<video::IVideoDevice> getDevice();
	OS::ITimer* getTimer();
	core::FPSCalc* getFPS();
	BenchmarkItem* getRootBenchmarking();

};

#define gEngine Engine::getInstance() 

}







#endif


