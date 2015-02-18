
#include "stdafx.h"
#include "mySqlManager.h"
#include "mySqlConnection.h"




namespace mray
{
namespace db
{
	mySqlManager::mySqlManager():ISQLManager("mySql")
	{
	}
	mySqlManager::~mySqlManager()
	{
		ClearConnections();
	}


	GCPtr<ISQLConnection> mySqlManager::CreateConnection()
	{
		GCPtr<ISQLConnection> c=new mySqlConnection();
		m_connections.push_back(c);
		return c;
	}
	void mySqlManager::RemoveConnection(ISQLConnection* c)
	{
		if(c->IsConnected())
			c->Disconnect();
		std::list<GCPtr<ISQLConnection>>::iterator it=m_connections.begin();
		for (;it!=m_connections.end();++it)
		{
			if((*it).pointer()==c)
			{
				m_connections.erase(it);
				break;;
			}
		}
	}
	void mySqlManager::ClearConnections()
	{
		std::list<GCPtr<ISQLConnection>>::iterator it=m_connections.begin();
		for (;it!=m_connections.end();++it)
		{
			(*it)->Disconnect();
		}
		m_connections.clear();
	}
	const std::list<GCPtr<ISQLConnection>>& mySqlManager::GetConnections()
	{
		return m_connections;
	}

}
}
