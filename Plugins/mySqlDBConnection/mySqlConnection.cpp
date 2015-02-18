
#include "stdafx.h"
#include "mySqlConnection.h"


#include "ILogManager.h"
#include "mySqlResult.h"
#include "JobPool.h"


namespace mray
{
namespace db
{

class SQLCmdJobOrder:public JobOrder
{
public:
	core::string cmd;
	void* userdata;
	NonBlockingCmdDelegate delg;
	mySqlConnection* caller;
public:
	SQLCmdJobOrder(mySqlConnection* c,const core::string& cmd,void* ud,NonBlockingCmdDelegate d)
	{
		this->cmd=cmd;
		userdata=ud;
		delg=d;
		caller=c;
	}
	virtual bool ExecuteJob()
	{
		bool ret=caller->ExecBlockingCmd(cmd);
		if(delg)
			(*delg)(caller,ret,userdata);
		return true;
	}
};


class SQLCmdRetJobOrder:public JobOrder
{
public:
	core::string cmd;
	void* userdata;
	NonBlockingQueryCmdDelegate delg;
	mySqlConnection* caller;
	bool rollback;
public:
	SQLCmdRetJobOrder(mySqlConnection* c,const core::string& cmd,bool rb,void* ud,NonBlockingQueryCmdDelegate d)
	{
		this->cmd=cmd;
		userdata=ud;
		delg=d;
		rollback=rb;
		caller=c;
	}
	virtual bool ExecuteJob()
	{
		ISQLResult* ret=caller->ExecBlockingCmdWithRet(cmd,rollback);
		if(delg)
			(*delg)(caller,ret,userdata);
		return true;
	}
};

class SQLConnectAsyncJobOrder:public JobOrder
{
public:
	SQLConnectData connectData;
	void* userdata;
	ConnectAsyncDelegate delg;
	mySqlConnection* caller;
public:
	SQLConnectAsyncJobOrder(mySqlConnection* c,const SQLConnectData& cd,void* ud,ConnectAsyncDelegate d)
	{
		connectData=cd;
		userdata=ud;
		delg=d;
		caller=c;
	}
	virtual bool ExecuteJob()
	{
		bool ret=caller->Connect(connectData);
		if(delg)
			(*delg)(caller,ret,userdata);
		return true;
	}
};

class mySqlConnectionImpl
{
protected:
	mySqlConnection* m_owner;
	st_mysql *m_connection;
	core::string m_lastErr;
public:
	mySqlConnectionImpl(mySqlConnection* o)
	{
		m_connection=0;
		m_owner=o;
	}
	virtual~mySqlConnectionImpl()
	{
		Disconnect();
	}
	bool Connect(const SQLConnectData& data);
	void Disconnect();
	void ConnectAsync(const SQLConnectData& data,void* userdata,ConnectAsyncDelegate d);

	bool IsConnected();
	void Commit();
	void RollBack();
	bool ExecBlockingCmd(const core::string& cmd);
	ISQLResult* ExecBlockingCmdWithRet(const core::string& cmd,bool rollbackOnFail);
	void ExecNonBlockingCmd(const core::string& cmd,void* userdata,NonBlockingCmdDelegate d);
	void ExecNonBlockingCmdWithRet(const core::string& cmd,bool rollbackOnFail,void* userdata,NonBlockingQueryCmdDelegate d);
	const core::string& GetLastError()const
	{
		return m_lastErr;
	}
};

mySqlConnection::mySqlConnection()
{
	m_impl=new mySqlConnectionImpl(this);
}
mySqlConnection::~mySqlConnection()
{
	delete m_impl;
}


bool mySqlConnection::Connect(const SQLConnectData& data)
{
	return m_impl->Connect(data);
}

void mySqlConnection::ConnectAsync(const SQLConnectData& data,void* userdata,ConnectAsyncDelegate d)
{
	return m_impl->ConnectAsync(data,userdata,d);
}
void mySqlConnection::Disconnect()
{
	m_impl->Disconnect();
}

bool mySqlConnection::IsConnected()
{
	return m_impl->IsConnected();
}
void mySqlConnection::Commit()
{
	m_impl->Commit();
}
void mySqlConnection::RollBack()
{
	m_impl->RollBack();
}
bool mySqlConnection::ExecBlockingCmd(const core::string& cmd)
{
	return m_impl->ExecBlockingCmd(cmd);
}
ISQLResult* mySqlConnection::ExecBlockingCmdWithRet(const core::string& cmd,bool rollbackOnFail)
{
	return m_impl->ExecBlockingCmdWithRet(cmd,rollbackOnFail);
}

void mySqlConnection::ExecNonBlockingCmd(const core::string& cmd,void* userdata,NonBlockingCmdDelegate d)
{
	m_impl->ExecNonBlockingCmd(cmd,userdata,d);
}
void mySqlConnection::ExecNonBlockingCmdWithRet(const core::string& cmd,bool rollbackOnFail,void* userdata,NonBlockingQueryCmdDelegate d)
{
	m_impl->ExecNonBlockingCmdWithRet(cmd,rollbackOnFail,userdata,d);
}

const core::string& mySqlConnection::GetLastError()const
{
	return m_impl->GetLastError();
}


//////////////////////////////////////////////////////////////////////////

bool mySqlConnectionImpl::Connect(const SQLConnectData& data)
{
	if(m_connection)
	{
		Disconnect();
	}
	ulong flags=
		(data.clientflag&ClientCompress ? CLIENT_COMPRESS:0) |
		(data.clientflag&ClientFoundRows ? CLIENT_FOUND_ROWS:0) |
		(data.clientflag&ClientIgnoreSpace ? CLIENT_IGNORE_SPACE:0) |
		(data.clientflag&ClientInteractive ? CLIENT_INTERACTIVE:0) |
		(data.clientflag&ClientMultiStatements ? CLIENT_MULTI_STATEMENTS:0) |
		(data.clientflag&ClientSSL ? CLIENT_SSL:0) ;
	m_connection=mysql_init(0);
	bool ret=mysql_real_connect(m_connection,data.host.c_str(),data.user.c_str(),data.passwd.c_str(),data.db.c_str(),data.port,0,flags)!=0;
	if(!ret)
		return false;
	mysql_set_character_set(m_connection, "utf8");
	m_owner->__FIRE_OnConnected(m_owner,&data);
	return ret;
}

void mySqlConnectionImpl::ConnectAsync(const SQLConnectData& data,void* userdata,ConnectAsyncDelegate d)
{
	JobPool::getInstance().AddRequest(new SQLConnectAsyncJobOrder(m_owner,data,userdata,d),0);

}


void mySqlConnectionImpl::Disconnect()
{
	if(m_connection)
	{
		mysql_close(m_connection);
		m_connection=0;
		m_owner->__FIRE_OnDisconnected(m_owner);
	}
}

bool mySqlConnectionImpl::IsConnected()
{
	return m_connection!=0;
}
void mySqlConnectionImpl::Commit()
{
	if(IsConnected())
		mysql_commit(m_connection);
}
void mySqlConnectionImpl::RollBack()
{
	if(IsConnected())
		mysql_rollback(m_connection);
}
bool mySqlConnectionImpl::ExecBlockingCmd(const core::string& cmd)
{
	int res;
	res=mysql_query(m_connection,cmd.c_str());
	if(res!=0)
	{
		m_lastErr=mysql_error(m_connection);
		m_owner->__FIRE_OnError(m_owner,m_lastErr);
		gLogManager.log("mySQL:ExecBlockingCmd() Failed to execute command:\""+cmd+"\" with error :" +m_lastErr,ELL_WARNING);
		return false;
	}
	m_owner->__FIRE_OnCommandExecuted(m_owner,cmd);
	return true;
}
ISQLResult* mySqlConnectionImpl::ExecBlockingCmdWithRet(const core::string& cmd,bool rollbackOnFail)
{
	if(!ExecBlockingCmd(cmd))
		return false;
	mySqlResult* res=new mySqlResult(mysql_store_result(m_connection));
	return res;
}

void mySqlConnectionImpl::ExecNonBlockingCmd(const core::string& cmd,void* userdata,NonBlockingCmdDelegate d)
{
	JobPool::getInstance().AddRequest(new SQLCmdJobOrder(m_owner,cmd,userdata,d),0);
}
void mySqlConnectionImpl::ExecNonBlockingCmdWithRet(const core::string& cmd,bool rollbackOnFail,void* userdata,NonBlockingQueryCmdDelegate d)
{
	JobPool::getInstance().AddRequest(new SQLCmdRetJobOrder(m_owner,cmd,rollbackOnFail,userdata,d),0);
}

}
}

