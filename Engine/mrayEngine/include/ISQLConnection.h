

/********************************************************************
	created:	2013/01/27
	created:	27:1:2013   17:04
	filename: 	C:\Development\mrayEngine\Engine\mrayEngine\include\ISQLConnection.h
	file path:	C:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	ISQLConnection
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __ISQLConnection__
#define __ISQLConnection__

#include "mString.h"
#include "common.h"
#include "IDelegate.h"
#include "ListenerContainer.h"


namespace mray
{
namespace db
{

	enum SQLConnectionFlags
	{
		ClientCompress=BIT(0), //Use compression protocol
		ClientFoundRows=BIT(1), //Return the number of found (matched) rows, not the number of changed rows
		ClientIgnoreSpace=BIT(2), //Permit spaces after function names. Makes all functions names reserved words.
		ClientInteractive=BIT(3),
		ClientMultiStatements=BIT(4), //Tell the server that the client may send multiple statements in a single string (separated by .. If this flag is not set, multiple-statement execution is disabled. See the note following this table for more information about this flag.
		ClientSSL=BIT(5)	//Use SSL (encrypted protocol). Do not set this option within an application program; it is set internally in the client library
	};
	class ISQLResult;
	class ISQLConnection;

	typedef IDelegate3<void,ISQLConnection*,bool,void*>* NonBlockingCmdDelegate;
	typedef IDelegate3<void,ISQLConnection*,bool,void*>* ConnectAsyncDelegate;
	typedef IDelegate3<void,ISQLConnection*,ISQLResult* ,void*>* NonBlockingQueryCmdDelegate;

	struct SQLConnectData
	{
	public:
		SQLConnectData():port(0),clientflag(0)
		{
		}

		core::string host;
		core::string user;
		core::string passwd;
		core::string db;
		unsigned int port;
		unsigned long clientflag;
	};

	class ISQLConnectionListener
	{
	public:
		virtual void OnConnected(ISQLConnection*,const SQLConnectData*){};
		virtual void OnDisconnected(ISQLConnection*){};
		virtual void OnCommandExecuted(ISQLConnection*,const core::string& cmd){};
		virtual void OnError(ISQLConnection*,const core::string& err){};
	};
class ISQLConnection:public ListenerContainer<ISQLConnectionListener*>
{
protected:
public:
	ISQLConnection(){}
	virtual~ISQLConnection(){}

	virtual bool Connect(const SQLConnectData& data)=0;

	bool Connect(const core::string& host,
		const core::string& user,
		const core::string& passwd,
		const core::string& db,
		unsigned int port,
		unsigned long clientflag)
	{
		SQLConnectData data;
		data.host=host;
		data.user=user;
		data.passwd=passwd;
		data.db=db;
		data.port=port;
		data.clientflag=clientflag;
		return Connect(data);
	}

	virtual void ConnectAsync(const SQLConnectData& data,void* userdata,ConnectAsyncDelegate d)=0;

	virtual void Disconnect()=0;

	virtual bool IsConnected()=0;
	virtual void Commit()=0;
	virtual void RollBack()=0;
	virtual bool ExecBlockingCmd(const core::string& cmd)=0;
	virtual ISQLResult* ExecBlockingCmdWithRet(const core::string& cmd,bool rollbackOnFail)=0;

	virtual void ExecNonBlockingCmd(const core::string& cmd,void* userdata,NonBlockingCmdDelegate d)=0;
	virtual void ExecNonBlockingCmdWithRet(const core::string& cmd,bool rollbackOnFail,void* userdata,NonBlockingQueryCmdDelegate d)=0;

	virtual const core::string& GetLastError()const=0;
};

}
}

#endif
