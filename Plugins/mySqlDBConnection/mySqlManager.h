


/********************************************************************
	created:	2013/01/27
	created:	27:1:2013   19:26
	filename: 	C:\Development\mrayEngine\Plugins\mySqlDBConnection\mySqlManager.h
	file path:	C:\Development\mrayEngine\Plugins\mySqlDBConnection
	file base:	mySqlManager
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __mySqlManager__
#define __mySqlManager__



#include "ISQLManager.h"

namespace mray
{
namespace db
{

class mySqlManager:public ISQLManager
{
protected:
	std::list<GCPtr<ISQLConnection>> m_connections;
public:
	mySqlManager();
	virtual~mySqlManager();


	GCPtr<ISQLConnection> CreateConnection();
	void RemoveConnection(ISQLConnection* c);
	void ClearConnections();
	const std::list<GCPtr<ISQLConnection>>& GetConnections();
};

}
}


#endif
