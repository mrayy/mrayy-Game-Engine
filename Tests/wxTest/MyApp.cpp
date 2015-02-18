

#include "stdafx.h"
#include "MyApp.h"
#include "MyFrame.h"

#include "Engine.h"
#include "WinOSystem.h"


#include "ImageSetResourceManager.h"
#include "FontResourceManager.h"
#include "GUIThemeManager.h"
#include "JobPool.h"
#include "OptionContainer.h"


#include "wxBasedMainForm.h"
#include "EventMemoryManager.h"

using namespace mray;


MyApp::MyApp()
{
}

bool MyApp::OnInit()
{

	 new EventMemoryManager();
	m_engine=new Engine(new OS::WinOSystem());

	GCPtr<StreamLogger> logger=new StreamLogger(true);
	logger->setStream(gFileSystem.createTextFileWriter( gFileSystem.getAppPath()+mT(".log")));
	gLogManager.addLogDevice(logger);


	m_engine->loadPlugins("Plugins.stg");
	m_engine->loadResourceFile("dataPath.stg");


	core::string deviceType=mT("OpenGL");

	//Create Null window just to register threads to it
	OptionContainer opt;
	opt.AddOption(SOptionElement ("Visible","false"));
	m_device=Engine::getInstance().createDevice(mT("OpenGL"));
	video::RenderWindow* w= m_device->CreateRenderWindow("NullWindow",1,0,opt,0);
	JobPool::getInstance().SetupDefaultPools();


	{
		gImageSetResourceManager.loadImageSet(mT("VistaCG.imageset"));
		GCPtr<OS::IStream> themeStream=gFileSystem.createBinaryFileReader(mT("VistaCG.xml"));
		GUI::GUIThemeManager::getInstance().loadTheme(themeStream);
		GUI::GUIThemeManager::getInstance().setActiveTheme(mT("VistaCG"));
		//load font
		GCPtr<GUI::IFont>font=gFontResourceManager.getOrCreate(mT("solo5_font.xml"));
		gFontResourceManager.setDefaultFont(font);
	}

	wxBasedMainForm* f=new wxBasedMainForm(0);
	SetTopWindow(f);

	f->Show();
	//f->UpdateEngine();

	//m_window->DestroyWindow();

	return true;
}

int MyApp::OnExit()
{
	int res=wxApp::OnExit();

	delete m_engine;
	return res;
}

IMPLEMENT_APP(MyApp);