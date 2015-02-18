
#include "stdafx.h"
#include "DBConnectionCommands.h"

namespace mray
{
namespace TBee
{

DBConnectionCommands::DBConnectionCommands()
{
	m_connection=0;
	m_userID="";
}

DBConnectionCommands::~DBConnectionCommands()
{

}
void DBConnectionCommands::SetConnection(db::ISQLConnection* c)
{
	if(m_connection)
		m_connection->RemoveListener(this);
	m_connection=c;
	if(m_connection)
		m_connection->AddListener(this);
}


void DBConnectionCommands::OnError(db::ISQLConnection*,const core::string& err)
{
	gLogManager.log("DBConnection- Error occurred:"+err,ELL_WARNING);
}
bool DBConnectionCommands::Connect(const db::SQLConnectData* cd,bool async,void* userdata,db::ConnectAsyncDelegate d)
{
	if(async)
	{
		m_connection->ConnectAsync(*cd,userdata,d);
	}else
	{
		return m_connection->Connect(*cd);
	}
	return true;
}

bool DBConnectionCommands::Login(const core::string& userName,const core::string& pwd,bool async,void* userdata,db::NonBlockingCmdDelegate d)
{
	if(async)
	{
		core::string cmd="select * from TELUBee.UsersTable u where u.Name=\'"+userName+"\' and Password=\'"+pwd+"\'";
		m_connection->ExecNonBlockingCmd(cmd,userdata,d);
	}else
	{
	}
	return true;
}

db::ISQLResult* DBConnectionCommands::LoadRobotsInfo(bool async,void* userdata,db::NonBlockingQueryCmdDelegate d)
{
	return 0;
}

bool DBConnectionCommands::Logout(bool async,void* userdata,db::NonBlockingCmdDelegate d)
{
	return 0;
}



}
}