

/********************************************************************
	created:	2013/01/29
	created:	29:1:2013   12:08
	filename: 	C:\Development\mrayEngine\Projects\TELUBee\DBConnectionCommands.h
	file path:	C:\Development\mrayEngine\Projects\TELUBee
	file base:	DBConnectionCommands
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __DBConnectionCommands__
#define __DBConnectionCommands__


#include "ISQLConnection.h"

namespace mray
{
namespace TBee
{

class DBConnectionCommands:public db::ISQLConnectionListener
{
protected:
	db::ISQLConnection* m_connection;

	core::string m_userID;
	bool m_connected;

	void OnError(db::ISQLConnection*,const core::string& err);
public:
	DBConnectionCommands();
	virtual~DBConnectionCommands();

	void SetConnection(db::ISQLConnection* c);
	db::ISQLConnection* GetConnection(){return m_connection;}

	bool Connect(const db::SQLConnectData* cd,bool async=false,void* userdata=0,db::ConnectAsyncDelegate d=0);
	bool Login(const core::string& userName,const core::string& pwd,bool async=false,void* userdata=0,db::NonBlockingCmdDelegate d=0);
	db::ISQLResult* LoadRobotsInfo(bool async=false,void* userdata=0,db::NonBlockingQueryCmdDelegate d=0);
	bool Logout(bool async=false,void* userdata=0,db::NonBlockingCmdDelegate d=0);


};

}
}

#endif
