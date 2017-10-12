#include "stdafx.h"


#include "Engine.h"

#include "AsyncLoadManager.h"

#include "XMLExpatParser.h"
#include "common.h"
#include "CSettingsFile.h"
#include "SkeletonResourceManager.h"
#include "ShaderSemanticTable.h"
//#include "StreamLogger.h"
#include "MaterialResourceManager.h"
#include "FileResourceManager.h"
#include "ScriptResourceManager.h"
#include "MeshAnimatorManager.h"

#include "IMonitorDeviceManager.h"//just for linking


#include "MeshFileCreatorManager.h"

#include "LogManager.h"
#include "GUIOverlayManager.h"
#include "EnumManager.h"
#include "DefaultEnum.h"
#include "SceneHelper.h"
#include "DefaultObjects.h"

#include "SkyBoxManager.h"

#include "TextDecorateNodeFactory.h"
#include "DeviceCapabilites.h"

#include "LogSubSystemManager.h"
#include "FunctionProfileManager.h"

#include "BenchmarkItem.h"


#include "VideoResourceManager.h"
#include "TextureResourceManager.h"
#include "SoundResourceManager.h"
#include "MeshResourceManager.h"
#include "FontResourceManager.h"
#include "ResourceGroup.h"
#include "ShaderResourceManager.h"
#include "AnimationResourceManager.h"
#include "MaterialResourceManager.h"
#include "ImageSetResourceManager.h"
#include "ScheduleManager.h"
#include "MeshGenerator.h"
#include "TraceManager.h"
#include "ArchiveManager.h"
#include "IOSRegistry.h"
#include "ShaderSemanticTable.h"
#include "GUIThemeManager.h"
#include "IDllManager.h"
#include "ILogManager.h"
#include "PluginManager.h"
#include "SoundManagerFactory.h"
#include "IErrorDialog.h"
#include "ITimer.h"
#include "FPSCalc.h"
#include "SParticleSystemFactory.h"
#include "SMeshManipulator.h"
#include "EventQueue.h"
#include "CPUInfo.h"
#include "JobPool.h"
#include "DelegateManager.h"
#include "IDeviceFactory.h"
#include "IFileSystem.h"
#include "GUIElementFactory.h"
#include "TextureSourceFactory.h"
#include "VideoLoggerSystem.h"

#include "InternetCacheManager.h"


namespace mray
{


	class EngineImpl
	{
	public:

		JobPool* jobPool;

		GCPtr<OS::IOSystem> ioSystem;
		GCPtr<video::IVideoDevice> device;
		OS::ArchiveManager* archiveManager;

		scene::MeshGenerator* meshGenerator;
		//	GCPtr<GUI::GUIThemeManager> skinManager;
		GUI::GUIThemeManager* themeManager;

		ILogManager* loggerManager;
		PluginManager* pluginManager;

		sound::SoundManagerFactory* soundManagerFactory;
		/*
		TextureResourceManager* textureResManager;
		SoundResourceManager* soundResManager;
		MaterialResourceManager* mtrlResManager;
		MeshResourceManager* meshResManager;
		FontResourceManager* fontResManager;
		ImageSetResourceManager* imagesetResManager;
		ShaderResourceManager* shaderResManager;
		AnimationResourceManager* animResManager;*/
		ResourceGroup* resourceGroup;

		DelegateManager* delegateManager;

		scene::SMeshManipulator* meshManipulator;
		scene::SParticleSystemFactory* particleSystemFactory;

		xml::IXMLParser* xmlParser;

		EventQueue* eventQueue;

		TraceManager* traceManager;

		math::MTrigTable* mathTrigTable;


		video::DeviceFactory* deviceFactory;

		video::ShaderSemanticTable* shaderSemTable;

//		CPUInfo* cpuInfo;

		ScheduleManager* scheduleManager;

		BenchmarkItem *rootBenchmark;

		OS::ITimer* timer;
		core::FPSCalc* fpsCounter;

	public:

		EngineImpl(GCPtr<OS::IOSystem> system)
		{
			rootBenchmark = new BenchmarkItem(mT("Engine"));

			new FunctionProfileManager();
			new DefaultObjects();

			DefaultObjects::registerDefaultObjects();

			new EnumManager();
			DefaultEnum::registerDefaultEnum();


			ioSystem = system;

			traceManager = new TraceManager();

			traceFunction(eEngine);


			delegateManager = new DelegateManager();

			scheduleManager = new ScheduleManager();

			mathTrigTable = new math::MTrigTable();


			loggerManager = new LogManager();

			new LogSubSystemManager();

		//	new LoggerSystemVideo();

//			cpuInfo = new CPUInfo();

			eventQueue = new EventQueue();

			jobPool = new JobPool();

			particleSystemFactory = new scene::SParticleSystemFactory();

			meshManipulator = new scene::SMeshManipulator();


			// 	GCPtr<StreamLogger> logger= new StreamLogger(true);
			// 	loggerManager->addLogDevice(logger);
			// 	logger->setStream(gFileSystem.createTextFileWriter(mT("log.txt")));

			resourceGroup = new ResourceGroup();

			shaderSemTable = new video::ShaderSemanticTable();

			new scene::MeshFileCreatorManager();


			new ScriptResourceManager();

			new scene::SceneHelper();

			new scene::SkyBoxManager();

			new AsyncLoadManager();

			pluginManager = new PluginManager();

			new FileResourceManager();
			new AnimationResourceManager();
			new SkeletonResourceManager();
			new MaterialResourceManager();
			new SoundResourceManager();
			new MeshResourceManager();
			new TextureResourceManager(0);
			new FontResourceManager(0);
			new ShaderResourceManager(0);
			new ImageSetResourceManager();
			new VideoResourceManager();

			new video::TextureSourceFactory();

			soundManagerFactory = new sound::SoundManagerFactory();

			meshGenerator = new scene::MeshGenerator();


			xmlParser = new xml::XMLExpatParser();


			archiveManager = new OS::ArchiveManager();

			fpsCounter = new core::FPSCalc();

			timer = system->createTimer();
			timer->initTimer();
			fpsCounter->resetTime(timer->getSeconds());


			//	skinManager=new GUI::GUIThemeManager();
			themeManager = new GUI::GUIThemeManager();


			new GUI::GUIOverlayManager();
			new GUI::GUIElementFactory();



			deviceFactory = new video::DeviceFactory();

			new scene::MeshAnimatorManager();

			new GUI::TextDecorateNodeFactory();


			new network::InternetCacheManager();

		}
		~EngineImpl(){
			traceFunction(Engine);

			delete network::InternetCacheManager::getInstancePtr();

			delete pluginManager;

			delete mathTrigTable;

			delete deviceFactory;

			delete themeManager;

			delete GUI::TextDecorateNodeFactory::getInstancePtr();
			delete scene::MeshAnimatorManager::getInstancePtr();
			delete GUI::GUIOverlayManager::getInstancePtr();
			delete GUI::GUIElementFactory::getInstancePtr();

			delete AsyncLoadManager::getInstancePtr();

			delete scene::SkyBoxManager::getInstancePtr();


			delete archiveManager;
			SoundResourceManager::getInstance().removeAll();
			ScriptResourceManager::getInstance().removeAll();
			FileResourceManager::getInstance().removeAll();
			FontResourceManager::getInstance().removeAll();
			ShaderResourceManager::getInstance().removeAll();
			MaterialResourceManager::getInstance().removeAll();
			MeshResourceManager::getInstance().removeAll();
			AnimationResourceManager::getInstance().removeAll();
			ImageSetResourceManager::getInstance().removeAll();
			TextureResourceManager::getInstance().removeAll();
			VideoResourceManager::getInstance().removeAll();

			delete video::TextureSourceFactory::getInstancePtr();

			delete resourceGroup;
			delete soundManagerFactory;

			delete xmlParser;

			delete particleSystemFactory;
			delete meshManipulator;

			delete shaderSemTable;

			delete scheduleManager;

			delete meshGenerator;

			device = 0;
			OS::IDllManager::getInstance().ClearLibraries();
			delete jobPool;

			loggerManager->log(mT("Engine shutdown"), ELL_SUCCESS);
			loggerManager->close();

			delete scene::SceneHelper::getInstancePtr();
			delete scene::MeshFileCreatorManager::getInstancePtr();

			delete LogSubSystemManager::getInstancePtr();

			delete loggerManager;

			delete delegateManager;

//			delete cpuInfo;
			delete eventQueue;
			delete fpsCounter;
			delete timer;

			forceExitFunction();
			delete traceManager;

			DefaultEnum::removeDefaultEnum();
			delete EnumManager::getInstancePtr();
			delete DefaultObjects::getInstancePtr();

			delete FunctionProfileManager::getInstancePtr();

			delete rootBenchmark;
			ioSystem = 0;
		}


		void updateBenchMark(){
			traceFunction(Engine);
			static double lastTime = 0;
			double t = timer->getSeconds();
			lastTime = t;
		}

		void updateEngine(){
			traceFunction(Engine);
			float currtime = timer->getSeconds();
			fpsCounter->regFrame(currtime);
			updateBenchMark();
			video::ShaderSemanticTable::getInstance().setTime(timer->getSeconds());
			video::ShaderSemanticTable::getInstance().setDTime(fpsCounter->dt());
			scheduleManager->update(fpsCounter->dt());
		}

		/*
		GCPtr<video::IVideoDevice> Engine::createDevice(video::EDeviceType deviceType,uint window,math::Point2di size,
		int bits,bool vsync,int multiSample,bool stencilBuffer)
		{

		traceFunction(Engine);
		device=deviceFactory->createDevice(deviceType,window,size,bits,vsync,multiSample,stencilBuffer);
		if(!device){
		gLogManager.log(mT("Device Type Unkown!"),ELL_WARNING);
		return 0;
		}

		textureResManager->setDevice(device);
		fontResManager->setDevice(device);
		shaderResManager->setDevice(device);

		if(device->getCapabilities()->isFeatureSupported(video::EDF_VertexProgram))
		{
		scene::MeshAnimatorManager::getInstance().SetGPUBasedAnimators(true);
		}else
		scene::MeshAnimatorManager::getInstance().SetGPUBasedAnimators(false);
		return device;

		}*/
		GCPtr<video::IVideoDevice> createDevice(const core::string& type)
		{
			traceFunction(Engine);
			if (device)
				gLogManager.log(mT("Device already created"), ELL_ERROR);

			device = deviceFactory->createDevice(type);
			if (!device){
				gLogManager.log(mT("Device Type Unkown!"), ELL_WARNING);
				return 0;
			}

			TextureResourceManager::getInstance().setDevice(device);
			FontResourceManager::getInstance().setDevice(device);
			ShaderResourceManager::getInstance().setDevice(device);


			if (device->getCapabilities()->isFeatureSupported(video::EDF_VertexProgram))
			{
				scene::MeshAnimatorManager::getInstance().SetGPUBasedAnimators(true);
			}
			else
				scene::MeshAnimatorManager::getInstance().SetGPUBasedAnimators(false);

			gLogManager.log(mT("Device Created: ") + type, ELL_SUCCESS);
			return device;

		}

		void loadPlugins(const core::string &pluginsConfigFile){
			traceFunction(Engine);
			gLogManager.startSection(mT("Loading Plugins"));
			OS::IStreamPtr stream = gFileSystem.createTextFileReader(pluginsConfigFile.c_str());
			GCPtr<script::CSettingsFile> resources = new script::CSettingsFile(stream);
			script::SConfigTag*tag = resources->getTag(mT("Plugins"));
			if (tag){
				script::SConfigTag::AttributesIT it = tag->m_attrSet.begin();
				for (; it != tag->m_attrSet.end(); ++it){
					PluginManager::getInstance().addPlugin(it->second);
				}
			}
			gLogManager.endSection(1);
		}

		void loadResourceFile(const core::string &resFile){

			traceFunction(Application);
			OS::IStreamPtr stream = gFileSystem.createTextFileReader(resFile);
			GCPtr<script::CSettingsFile> resources = new script::CSettingsFile(stream);
			script::SConfigTag*tag = resources->getTag(mT("SearchPath"));
			if (tag){
				script::SConfigTag::AttributesIT it = tag->m_attrSet.begin();
				for (; it != tag->m_attrSet.end(); ++it){
					gFileSystem.addPath(it->second);
				}
			}
			tag = resources->getTag(mT("Archive"));
			if (tag){
				script::SConfigTag::AttributesIT it = tag->m_attrSet.begin();
				for (; it != tag->m_attrSet.end(); ++it){
					OS::ArchiveManager::getInstance().addArchive(it->second);
				}
			}
			tag = resources->getTag(mT("Materials"));
			if (tag){
				script::SConfigTag::AttributesIT it = tag->m_attrSet.begin();
				for (; it != tag->m_attrSet.end(); ++it){
					gMaterialResourceManager.parseMaterialScript(gFileSystem.createBinaryFileReader(it->second));
				}
			}
		}
	};

	Engine::Engine(GCPtr<OS::IOSystem> system)
	{

		m_impl = new EngineImpl(system);
	}
	Engine::~Engine()
	{
		delete m_impl;
	}

void Engine::updateBenchMark(){
	m_impl->updateBenchMark();
}

void Engine::updateEngine(){
	m_impl->updateEngine();
}

/*
GCPtr<video::IVideoDevice> Engine::createDevice(video::EDeviceType deviceType,uint window,math::Point2di size,
										   int bits,bool vsync,int multiSample,bool stencilBuffer)
{

	traceFunction(Engine);
	device=deviceFactory->createDevice(deviceType,window,size,bits,vsync,multiSample,stencilBuffer);
	if(!device){
		gLogManager.log(mT("Device Type Unkown!"),ELL_WARNING);
		return 0;
	}

	textureResManager->setDevice(device);
	fontResManager->setDevice(device);
	shaderResManager->setDevice(device);

	if(device->getCapabilities()->isFeatureSupported(video::EDF_VertexProgram))
	{
		scene::MeshAnimatorManager::getInstance().SetGPUBasedAnimators(true);
	}else
		scene::MeshAnimatorManager::getInstance().SetGPUBasedAnimators(false);
	return device;

}*/
GCPtr<video::IVideoDevice> Engine::createDevice(const core::string& type)
{
	return m_impl->createDevice(type);

}

void Engine::loadPlugins(const core::string &pluginsConfigFile){
	m_impl->loadPlugins(pluginsConfigFile);
}

void Engine::loadResourceFile(const core::string &resFile){
	m_impl->loadResourceFile(resFile);

}
GCPtr<video::IVideoDevice> Engine::getDevice(){ return m_impl-> device; }

OS::ITimer* Engine::getTimer(){ return m_impl->timer; }
core::FPSCalc* Engine::getFPS(){ return m_impl->fpsCounter; }
BenchmarkItem* Engine::getRootBenchmarking(){ return m_impl->rootBenchmark; }


/*
void Engine::debugBreak(){
#ifdef _DEBUG
	_asm{
		int 3
	}
#endif
}*/

}

