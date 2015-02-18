

#include "stdafx.h"
#include "MapObject.h"
#include "MapCamera.h"
#include "GUIWebPageControl.h"
#include "BrowserWindow.h"
#include "TBAppGlobals.h"
#include "Application.h"
#include "RenderWindow.h"





namespace mray
{
namespace TBee
{
	
const int S_MinZoom=2;
const int S_MaxZoom=15;



class MapPageListener:public web::IBrowserWindowListener
{
	MapObject* m_owner;
public:
	MapPageListener(MapObject* o)
	{
		m_owner=o;
	}
	virtual void OnAddressChanged(web::BrowserWindow* wnd,const core::string& addr)
	{
		printf("OnAddressChanged\n");
	}
	virtual void OnStartLoading(web::BrowserWindow* wnd,const core::string& addr)
	{
		printf("OnStartLoading\n");
	}
	virtual void OnLoad(web::BrowserWindow* wnd)
	{
		printf("OnLoad\n");
	}
	virtual void OnCrashed(web::BrowserWindow* wnd)
	{
		printf("OnCrashed\n");
	}
	virtual void OnCrashWorker(web::BrowserWindow* wnd)
	{
		printf("OnCrashWorker\n");
	}
	virtual void OnCrashPlugin(web::BrowserWindow* wnd,const core::stringw& plugin)
	{
		printf("OnCrashPlugin\n");
	}
	virtual void OnNavigationRequested(web::BrowserWindow* wnd,const core::string& url,const core::string& referrer,bool newWnd)
	{
		printf("OnNavigationRequested\n");
	}
	virtual void OnLoadingStateChanged(web::BrowserWindow* wnd,bool isLoading)
	{
		printf("OnLoadingStateChanged\n");
	}
	virtual void OnJavascriptCallback(web::BrowserWindow* wnd,const core::string& url,const core::stringw& function,const std::vector<core::stringw>& args)
	{
		wprintf(L"OnJavascriptCallback: %s\n",function.c_str());
		m_owner->HandleCallback(function,args);
	}
};

const core::stringw MarkerClicked_Callback(L"MarkerClicked");
const core::stringw MarkerHover_Callback(L"MarkerHover");
const core::stringw MarkerOut_Callback(L"MarkerOut");

MapObject::MapObject():m_camera(0),m_mapWebpage(0),m_lastDist(0),m_lastAngle(0),m_zoomLevel(S_MinZoom),m_panPos(20,-10)
{
	m_mapListener=new MapPageListener(this);
}
MapObject::~MapObject()
{
	delete m_mapListener;
}

void MapObject::Init(MapCamera*cam,GUI::GUIWebPageControl* map)
{
	m_camera=cam;
	m_mapWebpage=map;

	SetZoom(m_zoomLevel);
	SetPos(m_panPos.x,m_panPos.y);
	m_mapWebpage->GetBrowserWindow()->BindFunction(MarkerClicked_Callback);
	m_mapWebpage->GetBrowserWindow()->BindFunction(MarkerHover_Callback);
	m_mapWebpage->GetBrowserWindow()->BindFunction(MarkerOut_Callback);
	m_mapWebpage->GetBrowserWindow()->AddListener(m_mapListener);
}

void MapObject::AddMarker(float x,float y,int id)
{

	core::string str="AddMarker("+core::StringConverter::toString(x)+","+core::StringConverter::toString(y)+","+core::StringConverter::toString(id)+")";
	GetMapControl()->GetBrowserWindow()->ExecuteScript(core::string_to_wchar(str));
}

void MapObject::DropMarker()
{
	core::string str="DropMarker()";
	GetMapControl()->GetBrowserWindow()->ExecuteScript(core::string_to_wchar(str));
}

void MapObject::ClearMarkers()
{
}

void MapObject::ShowMarkers()
{
}

void MapObject::HideMarkers()
{
}

bool MapObject::SelectRobot(Event*e)
{
	MouseEvent evt;
	video::RenderWindow* rw= e->GetOwnerRenderWindow();
	evt.SetOwnerRenderWindow(rw);
	evt.pos.x=rw->GetSize().x/2;
	evt.pos.y=rw->GetSize().y/2;
	evt.vpSize=rw->GetSize();
	evt.event=MET_MOVED;
	m_mapWebpage->OnEvent(&evt);
	evt.event=MET_LEFTDOWN;
	m_mapWebpage->OnEvent(&evt);
	evt.event=MET_LEFTUP;
	m_mapWebpage->OnEvent(&evt);
	return true;
}

void MapObject::MouseMoved(Event*e)
{
	MouseEvent evt;
	video::RenderWindow* rw;
	if(e)
		rw=e->GetOwnerRenderWindow();
	else
	{
		rw = TBAppGlobals::Instance()->App->GetRenderWindow();
	}
	evt.SetOwnerRenderWindow(rw);
	evt.pos.x=rw->GetSize().x/2;
	evt.pos.y=rw->GetSize().y/2;
	evt.vpSize=rw->GetSize();
	evt.event=MET_MOVED;
	m_mapWebpage->OnEvent(&evt);
}

void MapObject::Reset()
{
	SetZoom(S_MinZoom);
	SetPos(20,-10);
	SetTilt(0);
	GetCamera()->SetYaw(0);
}
void MapObject::SetEffect(video::ParsedShaderPP* eff)
{
	if(!eff)
		return;
	m_blurParam=eff->GetParam("RadialBlur");
}
void MapObject::ChangeDeltaPos(float dx,float dy)
{
	float z=1-(m_zoomLevel-S_MinZoom)/(float)(S_MaxZoom+1-(S_MinZoom));
	z=z*z*z;
	dx*=z;
	dy*=z;
	SetPos(m_panPos.x+dx,m_panPos.y+dy);
}
void MapObject::SetPos(float x,float y)
{
	m_panPos.x=math::clamp<float>(x,-85,85);
	m_panPos.y=y;
	core::string str="panTo("+core::StringConverter::toString(m_panPos.x)+","+core::StringConverter::toString(m_panPos.y)+")";
	GetMapControl()->GetBrowserWindow()->ExecuteScript(core::string_to_wchar(str));
}
void MapObject::SetTilt(float tilt)
{
	GetCamera()->SetTilt(tilt);
}


void MapObject::ZoomIn()
{
	m_zoomLevel++;
	if(m_zoomLevel>S_MaxZoom)
		m_zoomLevel=S_MaxZoom;
	core::string str="SetZoomLevel("+core::StringConverter::toString(m_zoomLevel)+")";
	GetMapControl()->GetBrowserWindow()->ExecuteScript(core::string_to_wchar(str));
}

void MapObject::ZoomOut()
{
	m_zoomLevel--;
	if(m_zoomLevel<S_MinZoom)
		m_zoomLevel=S_MinZoom;
	core::string str="SetZoomLevel("+core::StringConverter::toString(m_zoomLevel)+")";
	GetMapControl()->GetBrowserWindow()->ExecuteScript(core::string_to_wchar(str));
}

void MapObject::SetZoom(int lvl)
{
	m_zoomLevel=math::clamp<int>(lvl,S_MinZoom,S_MaxZoom);
	core::string str="SetZoomLevel("+core::StringConverter::toString(m_zoomLevel)+")";
	GetMapControl()->GetBrowserWindow()->ExecuteScript(core::string_to_wchar(str));
}

int MapObject::GetZoom()
{
	return m_zoomLevel;
}

void MapObject::ZoomCamera(float delta)
{
	float d=GetCamera()->getDist();
	float dd=(d-m_lastDist) ;
	m_lastDist=d;

	if(GetBlurParam())
	{
		GetBlurParam()->SetValue(dd*200);
	}

	//m_angles.y+=dy;
	GetCamera()->AddDist(delta);
}
void MapObject::SetZoomCamera(float zoom)
{
	GetCamera()->SetDistance(zoom);
}
void MapObject::SetBlur(float amount)
{
	if(GetBlurParam())
	{
		GetBlurParam()->SetValue(amount);
	}

}

void MapObject::HandleCallback(const core::stringw& function,const std::vector<core::stringw>& args)
{
	if(function==MarkerClicked_Callback)
	{
		float x,y;
		int id;
		x =core::StringConverter::toFloat(core::wchar_to_string(args[0].c_str()));
		y =core::StringConverter::toFloat(core::wchar_to_string(args[1].c_str()));
		id=core::StringConverter::toInt(core::wchar_to_string(args[2].c_str()));
		FIRE_LISTENR_METHOD(OnMarkerlClicked,(this,x,y,id));
	}else if(function==MarkerHover_Callback)
	{
		float x,y;
		int id;
		x =core::StringConverter::toFloat(core::wchar_to_string(args[0].c_str()));
		y =core::StringConverter::toFloat(core::wchar_to_string(args[1].c_str()));
		id=core::StringConverter::toInt(core::wchar_to_string(args[2].c_str()));
		FIRE_LISTENR_METHOD(OnMarkerlHovered,(this,x,y,id));
	}else if(function==MarkerOut_Callback)
	{
		float x,y;
		int id;
		x =core::StringConverter::toFloat(core::wchar_to_string(args[0].c_str()));
		y =core::StringConverter::toFloat(core::wchar_to_string(args[1].c_str()));
		id=core::StringConverter::toInt(core::wchar_to_string(args[2].c_str()));
		FIRE_LISTENR_METHOD(OnMarkerlOut,(this,x,y,id));
	}
}

}
}