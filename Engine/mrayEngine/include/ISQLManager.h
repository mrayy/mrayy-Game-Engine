

/********************************************************************
	created:	2013/01/27
	created:	27:1:2013   19:24
	filename: 	C:\Development\mrayEngine\Engine\mrayEngine\include\ISQLManager.h
	file path:	C:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	ISQLManager
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __ISQLManager__
#define __ISQLManager__


#include "mString.h"
#include "GCPtr.h"
#include "ISQLConnection.h"

namespace mray
{
namespace db
{
class ISQLManager
{
protected:
	core::string m_connectionType;
public:
	ISQLManager(const core::string& type):m_connectionType(type)
	{}
	virtual~ISQLManager(){}

	const core::string& GetSQLConnectionType(){return m_connectionType;}

	virtual GCPtr<ISQLConnection> CreateConnection()=0;

	virtual void RemoveConnection(ISQLConnection* c)=0;
	virtual void ClearConnections()=0;
	virtual const std::list<GCPtr<ISQLConnection>>& GetConnections()=0;
};

}
}


#endif
