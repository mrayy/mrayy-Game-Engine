

/********************************************************************
	created:	2013/01/29
	created:	29:1:2013   0:16
	filename: 	C:\Development\mrayEngine\Projects\TELUBee\LoginScreenState.h
	file path:	C:\Development\mrayEngine\Projects\TELUBee
	file base:	LoginScreenState
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __LoginScreenState__
#define __LoginScreenState__

#include "IRenderingState.h"
#include "TBUserInfo.h"


namespace mray
{
	namespace db
	{
		class ISQLConnection;
		class ISQLResult;
		struct SQLConnectData;
	}
	namespace GUI
	{
		class IGUIManager;
		class IGUIPanelElement;
	}
	class GUILoginMenu;
namespace TBee
{
#define ToMap_CODE 0x100
#define ToLocalCamera_CODE 0x101
#define ToRemoteCamera_CODE 0x102
#define ToVideo_CODE 0x103
#define ToFlyingTelubee_CODE 0x104

	class Application;
class LoginScreenState:public IRenderingState,public IDelegateContainer
{
protected:

	enum EState
	{
		EIdle,
		EConnectServer,
		EAuthenticating,
		ERetrievingData,
		EDone,
		EFailedConnect,
		EFailedAuthenticate,
		EFailedRetrieve
	};
	bool m_isConnecting;
	EState m_state;
	db::ISQLConnection* m_connection;

	GUI::IGUIManager* m_guiManager;
	GUI::IGUIPanelElement* m_guiroot;

	GUILoginMenu* m_loginMenu;
	float m_exitTimeOut;
	db::SQLConnectData* m_connectionData;

	


	void OnLoginPressed(IObject* caller,void* args);
	void OnExitPressed(IObject* caller,void* args);
	void OnSeeThroughPressed(IObject* caller,void* args);
	void OnConnectRemotePressed(IObject* caller, void* args);
	void OnConnectFlyingTbeePressed(IObject* caller, void* args);
	void OnConnectLocalPressed(IObject* caller, void* args);
	void OnPlayVideoPressed(IObject* caller, void* args);

	void _OnConnectionCallback(db::ISQLConnection*c,bool res,void* ud);
	void _OnAuthenticatingCallback(db::ISQLConnection*c,db::ISQLResult* res,void* ud);
	void _OnRetrieveCallback(db::ISQLConnection*c,db::ISQLResult* res,void* ud);

	void _OnUserAuthenticated(const TBUserInfo &userInfo){}
public:
	LoginScreenState();
	virtual~LoginScreenState();


	virtual void InitState();

	virtual bool OnEvent(Event* e, const math::rectf& rc);
	virtual void OnEnter(IRenderingState*prev);
	virtual void OnExit();
	virtual video::IRenderTarget* Render(const math::rectf& rc,ETargetEye eye);
	virtual void Update(float dt);

	virtual void LoadFromXML(xml::XMLElement* e);
};

}
}


#endif
