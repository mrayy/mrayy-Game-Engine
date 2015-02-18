
#include "stdafx.h"
#include "mySqlResult.h"

namespace mray
{
namespace db
{


mySqlResult::mySqlResult(MYSQL_RES* res)
{
	m_res=res;
	m_row=0;
	if(m_res)
	{
		MYSQL_FIELD* fields=mysql_fetch_fields(m_res);
		int cnt=Fieldsount();
		for (int i=0;i<cnt;++i)
		{
			mySqlFieldInfo fi(fields[i].name,fields[i].length);
			m_fields.push_back(fi);
		}
	}
	
}
mySqlResult::~mySqlResult()
{
	if(m_res)
		mysql_free_result(m_res);
}
MYSQL_RES* mySqlResult::GetResult(){return m_res;}

const char* mySqlResult::operator[](int index)
{
	if(!m_res || !m_row || index>=mysql_num_fields(m_res))
		return 0;
	return m_row[index];
}

ISQLFieldInfo* mySqlResult::GetFieldInfo(int index)
{
	if(index>=m_fields.size())
		return 0;
	return &m_fields[index];
}
int mySqlResult::Fieldsount()
{
	if(!m_res)
		return 0;
	return mysql_num_fields(m_res);
}

int mySqlResult::RowsCount()
{
	if(!m_res)
		return 0;
	return mysql_num_rows(m_res);
}

bool mySqlResult::NextRow()
{
	if(!m_res)
		return false;
	return (m_row=mysql_fetch_row(m_res))!=0;
}

bool mySqlResult::HasMore()
{
	if(!m_res)
		return false;
	return !mysql_eof(m_res);
}

}
}
