

#include "stdafx.h"

#include "LoginScreenState.h"
#include "TBAppGlobals.h"
#include "ISQLManager.h"
#include "TBAppGlobals.h"
#include "IVideoDevice.h"
#include "Application.h"
#include "ISQLResult.h"
#include "TBUserInfo.h"
#include "GUILoginMenu.h"
#include "GUIOverlay.h"
#include "GUIManager.h"
#include "GUIThemeManager.h"
#include "IGUIPanel.h"
#include "StringUtil.h"
#include "GUILoadingElement.h"
#include "TBRobotInfo.h"
#include "RobotInfoManager.h"
#include "FontResourceManager.h"


namespace mray
{
namespace TBee
{


LoginScreenState::LoginScreenState()
:IRenderingState("Login")
{
	m_connection=TBAppGlobals::Instance()->sqlManager->CreateConnection();
	m_loginMenu=new GUILoginMenu();
	m_isConnecting=false;

	m_connectionData=new db::SQLConnectData();

	m_connectionData->host="127.0.0.1";
	m_connectionData->port=3306;
	m_connectionData->db="TELUBee";
	m_connectionData->user="Telubee";
	m_connectionData->passwd="TelubeeRemote";
	m_connectionData->clientflag=0;

	IDelegateContainer::AddDelegate(CreateObjectDelegate(LoginScreenState,this,OnLoginPressed));
	IDelegateContainer::AddDelegate(CreateObjectDelegate(LoginScreenState,this,OnExitPressed));
	IDelegateContainer::AddDelegate(CreateObjectDelegate(LoginScreenState, this, OnSeeThroughPressed));
	IDelegateContainer::AddDelegate(CreateObjectDelegate(LoginScreenState, this, OnConnectLocalPressed));
	IDelegateContainer::AddDelegate(CreateObjectDelegate(LoginScreenState, this, OnConnectRemotePressed));
	IDelegateContainer::AddDelegate(CreateObjectDelegate(LoginScreenState, this, OnConnectFlyingTbeePressed));
	IDelegateContainer::AddDelegate(CreateObjectDelegate(LoginScreenState, this, OnPlayVideoPressed));
}
LoginScreenState::~LoginScreenState()
{
	TBAppGlobals::Instance()->sqlManager->RemoveConnection(m_connection);
	delete m_connectionData;
}

void LoginScreenState::OnConnectRemotePressed(IObject* caller, void* args)
{
	m_state = EDone;
	m_loginMenu->MessageLbl->SetText(core::string("Done!!"));
	m_exitTimeOut = 0;
	m_exitCode = ToRemoteCamera_CODE;

	return;
}
void LoginScreenState::OnConnectLocalPressed(IObject* caller, void* args)
{
	m_state = EDone;
	m_loginMenu->MessageLbl->SetText(core::string("Done!!"));
	m_exitTimeOut = 0;
	m_exitCode = ToLocalCamera_CODE;

	return; 
}

void LoginScreenState::OnConnectFlyingTbeePressed(IObject* caller, void* args)
{
	m_state = EDone;
	m_loginMenu->MessageLbl->SetText(core::string("Done!!"));
	m_exitTimeOut = 0;
	m_exitCode = ToFlyingTelubee_CODE;

	return;
}

void LoginScreenState::OnPlayVideoPressed(IObject* caller, void* args)
{
	m_state = EDone;
	m_loginMenu->MessageLbl->SetText(core::string("Done!!"));
	m_exitTimeOut = 0;
	m_exitCode = ToVideo_CODE;

	return;
}

void LoginScreenState::OnLoginPressed(IObject* caller,void* args)
{
	if(m_isConnecting)
	{
		m_loginMenu->WarningLbl->SetText(core::string("Please Wait"));
		return;
	}

/*

	m_state=EDone;
	m_loginMenu->MessageLbl->SetText(core::string("Done!!"));
	m_exitTimeOut=0;

	return;*/
	m_state=EConnectServer;
	m_loginMenu->MessageLbl->SetText(core::string("Connecting to server.."));
	//Connect to server
	

	db::ConnectAsyncDelegate delg=new ClassDelegate3<LoginScreenState,void,db::ISQLConnection*,bool ,void* >("",this,&LoginScreenState::_OnConnectionCallback);

	m_connection->ConnectAsync(*m_connectionData,0,delg);
}
void LoginScreenState::OnExitPressed(IObject* caller,void* args)
{
	TBAppGlobals::Instance()->App->terminate();
}
void LoginScreenState::OnSeeThroughPressed(IObject* caller,void* args)
{
	m_exitCode=ToLocalCamera_CODE;
}
void LoginScreenState::InitState()
{
	IRenderingState::InitState();

	m_guiManager=new GUI::GUIManager(Engine::getInstance().getDevice());
	m_guiManager->SetActiveTheme(GUI::GUIThemeManager::getInstance().getActiveTheme());

	m_guiroot=(GUI::IGUIPanelElement*)new GUI::IGUIPanelElement(core::string(""),m_guiManager);
	m_guiroot->SetDocking(GUI::EED_Fill);
	m_guiManager->SetRootElement(m_guiroot);

	GUI::GUILoadingElement* elem=new GUI::GUILoadingElement(m_guiManager);
	elem->SetPosition(math::vector2d(40,40));
	elem->SetSize(200);
	m_guiroot->AddElement(elem);

	GUI::GUIOverlay* o= GUI::GUIOverlayManager::getInstance().LoadOverlay("TBee\\GUILoginMenu.gui");
	if(o)
	{
		o->CreateElements(m_guiManager,m_guiroot,this,m_loginMenu);
	}
}

bool LoginScreenState::OnEvent(Event* e, const math::rectf& rc)
{
	if(m_guiManager && m_state!=EDone)
	{
		return m_guiManager->OnEvent(e,&rc);
	}
	return false;
}

void LoginScreenState::_OnConnectionCallback(db::ISQLConnection*c,bool res,void* ud)
{
	if(res==false)
	{
		m_isConnecting=false;
		m_state=EFailedConnect;
		m_loginMenu->MessageLbl->SetText(core::string("Connection Failed!"));
	}
	else
	{
		m_loginMenu->MessageLbl->SetText(core::string("Authenticating"));
		m_state=EAuthenticating;
		db::NonBlockingQueryCmdDelegate delg=new ClassDelegate3<LoginScreenState,void,db::ISQLConnection*,db::ISQLResult* ,void* >("",this,&LoginScreenState::_OnAuthenticatingCallback);

		core::string userName=m_loginMenu->UserTxt->GetText().GetAsString();
		userName=core::StringUtil::Trim(userName);
		userName.make_upper();

		core::string cmd="select ID,Name,Email from TELUBee.UsersTable where Name='"+userName+"' and Password='"+m_loginMenu->PasswordTxt->GetText().GetAsString()+"'";
		m_connection->ExecNonBlockingCmdWithRet(cmd,0,0,delg);
	}

}
void LoginScreenState::_OnAuthenticatingCallback(db::ISQLConnection*c,db::ISQLResult* res,void* ud)
{
	if(res==0 || res->RowsCount()==0)
	{
		m_isConnecting=false;
		m_state=EFailedAuthenticate;
		m_loginMenu->MessageLbl->SetText(core::string("Authenticating Failed!"));
	}
	else
	{
		m_loginMenu->MessageLbl->SetText(core::string("Retrieving Robot Data.."));
		m_state=ERetrievingData;
		res->NextRow();
		TBUserInfo info;
		info.ID= core::StringConverter::toInt((*res)[0]);
		info.name= (*res)[1];
		info.email=( (*res)[2]==0? "":(*res)[2]);
		_OnUserAuthenticated(info);
		TBAppGlobals::Instance()->userName=info.name;

		db::NonBlockingQueryCmdDelegate delg=new ClassDelegate3<LoginScreenState,void,db::ISQLConnection*,db::ISQLResult* ,void* >("",this,&LoginScreenState::_OnRetrieveCallback);
		core::string cmd="Select r.ID ID, r.Name Name,r.IP IP,r.Lat Lat,r.Lng Lng, s.Connected Connected,s.Avaliable Avaliable "\
			"from TELUBee.RobotsInfo r, TELUBee.RobotStatus s "\
			"Where r.ID=s.RobotID";
		m_connection->ExecNonBlockingCmdWithRet(cmd,0,0,delg);
	}

}

void LoginScreenState::_OnRetrieveCallback(db::ISQLConnection*c,db::ISQLResult* res,void* ud)
{
	if(!res)
	{
		m_isConnecting=false;
		m_state=EFailedRetrieve;
		m_loginMenu->MessageLbl->SetText(core::string("Couldn't retrieve robot data.."));
	}else
	{
		TBAppGlobals::Instance()->robotInfoManager->ClearRobots();
		for(int i=0;i<res->RowsCount();++i)
		{
			res->NextRow();
			TBRobotInfo roboIfo;
			core::string ID=  (*res)[0];
			core::string Name=  (*res)[1];
			core::string IP=  (*res)[2];
			core::string Lat=  (*res)[3];
			core::string Lng=  (*res)[4];
			core::string Connected=  (*res)[5];
			core::string Avaliable=  (*res)[6];

			roboIfo.ID=core::StringConverter::toInt(ID);
			roboIfo.name=Name;
			roboIfo.IP=IP;
			roboIfo.lat=core::StringConverter::toFloat(Lat);
			roboIfo.lng=core::StringConverter::toFloat(Lng);
			roboIfo.Connected=core::StringConverter::toBool(Connected);
			roboIfo.Avaliable=core::StringConverter::toBool(Avaliable);

			TBAppGlobals::Instance()->robotInfoManager->AddRobotInfo(roboIfo);
		}
		//record users time stamp
		m_state=EDone;
		m_loginMenu->MessageLbl->SetText(core::string("Done!!"));
		m_exitTimeOut=0;
	}
}

void LoginScreenState::OnEnter(IRenderingState*prev)
{
	m_state=EIdle;
	m_isConnecting=false;
//	m_loginMenu->UserTxt->SetText(core::string(""));
//	m_loginMenu->PasswordTxt->SetText(core::string(""));

}
void LoginScreenState::OnExit()
{
	IRenderingState::OnExit();
}
video::IRenderTarget* LoginScreenState::Render(const math::rectf& rc,ETargetEye eye)
{
	IRenderingState::Render(rc,eye);


	video::IVideoDevice* dev=Engine::getInstance().getDevice();
	dev->setRenderTarget(m_renderTarget[GetEyeIndex(eye)]);
	dev->set2DMode();
	TBAppGlobals::Instance()->App->getDevice()->draw2DRectangle(rc,video::DefaultColors::Black);

	m_guiManager->DrawAll(&rc);// m_renderTarget[GetEyeIndex(eye)]);
	if(m_state==EDone)
	{
		dev->draw2DRectangle(rc,video::SColor(0,0,0,m_exitTimeOut));
	}

	GUI::GUIBatchRenderer renderer;
	renderer.SetDevice(Engine::getInstance().getDevice());
	GCPtr<GUI::IFont> font=gFontResourceManager.getDefaultFont();
	if(font){
		renderer.Prepare();

		float yoffset=50;


		GUI::FontAttributes attr;
		attr.fontColor.Set(1,1,1,1);
		attr.fontAligment=GUI::EFA_MiddleLeft;
		attr.fontSize=14;
		attr.hasShadow=true;
		attr.shadowColor.Set(0,0,0,1);
		attr.shadowOffset=math::vector2d(2);
		attr.spacing=2;
		attr.wrap=0;
		attr.RightToLeft=0;
		core::string msg=mT("Version= ");
		msg+=TBAppGlobals::Instance()->GetVersion();
		font->print(math::rectf(rc.BRPoint.x-300,rc.BRPoint.y-100+yoffset,10,10),&attr,0,msg,&renderer);
		yoffset+=attr.fontSize;
		msg=mT("Build= ");
		msg+=TBAppGlobals::Instance()->GetBuild();
		font->print(math::rectf(rc.BRPoint.x-300,rc.BRPoint.y-100+yoffset,10,10),&attr,0,msg,&renderer);
		yoffset+=attr.fontSize;

	}
	renderer.Flush();
	dev->setRenderTarget(0);
	return m_renderTarget[GetEyeIndex(eye)];
}

void LoginScreenState::Update(float dt)
{
	if(m_state==EDone)
	{
		m_exitTimeOut+=dt;
		if(m_exitTimeOut>=1)
		{
			m_exitTimeOut=1;
			m_exitCode=ToMap_CODE;
		}
	}
	m_guiManager->Update(dt);
}

void LoginScreenState::LoadFromXML(xml::XMLElement* e)
{
	xml::XMLAttribute* attr;
	attr=e->getAttribute("Host");
	if(attr)
		m_connectionData->host=attr->value;
	attr=e->getAttribute("User");
	if(attr)
		m_connectionData->user=attr->value;
	attr=e->getAttribute("Password");
	if(attr)
		m_connectionData->passwd=attr->value;
	attr=e->getAttribute("Port");
	if(attr)
		m_connectionData->port=core::StringConverter::toInt(attr->value);
}

}
}
