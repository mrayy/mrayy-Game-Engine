

/********************************************************************
	created:	2013/01/27
	created:	27:1:2013   17:17
	filename: 	C:\Development\mrayEngine\Plugins\mySqlDBConnection\mySqlConnection.h
	file path:	C:\Development\mrayEngine\Plugins\mySqlDBConnection
	file base:	mySqlConnection
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __mySqlConnection__
#define __mySqlConnection__

#include "ISQLConnection.h"


namespace mray
{
namespace db
{

	class mySqlConnectionImpl;
class mySqlConnection:public ISQLConnection
{
protected:
	mySqlConnectionImpl* m_impl;
public:
	DECLARE_FIRE_METHOD(OnConnected,(ISQLConnection*c,const SQLConnectData*d),(c,d));
	DECLARE_FIRE_METHOD(OnDisconnected,(ISQLConnection*c),(c));
	DECLARE_FIRE_METHOD(OnCommandExecuted,(ISQLConnection*c,const core::string& cmd),(c,cmd));
	DECLARE_FIRE_METHOD(OnError,(ISQLConnection*c,const core::string& err),(c,err));
public:
	mySqlConnection();
	virtual~mySqlConnection();

	bool Connect(const SQLConnectData& data);
	void ConnectAsync(const SQLConnectData& data,void* userdata,ConnectAsyncDelegate d);

	void Disconnect();

	bool IsConnected();
	void Commit();
	void RollBack();
	bool ExecBlockingCmd(const core::string& cmd);
	ISQLResult* ExecBlockingCmdWithRet(const core::string& cmd,bool rollbackOnFail);

	void ExecNonBlockingCmd(const core::string& cmd,void* userdata,NonBlockingCmdDelegate d);
	void ExecNonBlockingCmdWithRet(const core::string& cmd,bool rollbackOnFail,void* userdata,NonBlockingQueryCmdDelegate d);

	const core::string& GetLastError()const;
};

}
}

#endif
