

/********************************************************************
	created:	2013/01/27
	created:	27:1:2013   18:18
	filename: 	C:\Development\mrayEngine\Plugins\mySqlDBConnection\mySqlResult.h
	file path:	C:\Development\mrayEngine\Plugins\mySqlDBConnection
	file base:	mySqlResult
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __mySqlResult__
#define __mySqlResult__

#include "ISQLResult.h"

namespace mray
{
namespace db
{

	class mySqlFieldInfo:public ISQLFieldInfo
	{
	protected:
		core::string m_name;
		uint m_length;
	public:
		mySqlFieldInfo(const core::string& name,uint len):m_name(name),m_length(len)
		{

		}
		const core::string& getName(){return m_name;}
		uint Length(){return m_length;}
	};

class mySqlResult:public ISQLResult
{
protected:
	MYSQL_RES* m_res;
	MYSQL_ROW m_row;
	std::vector<mySqlFieldInfo> m_fields;
public:
	mySqlResult(MYSQL_RES* res);
	virtual~mySqlResult();
	MYSQL_RES* GetResult();


	const char* operator[](int index);

	ISQLFieldInfo* GetFieldInfo(int index);

	int Fieldsount();
	int RowsCount();
	bool NextRow();
	bool HasMore();
};

}
}

#endif
