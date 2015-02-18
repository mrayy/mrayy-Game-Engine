

/********************************************************************
	created:	2013/01/27
	created:	27:1:2013   17:11
	filename: 	C:\Development\mrayEngine\Engine\mrayEngine\include\ISQLResult.h
	file path:	C:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	ISQLResult
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __ISQLResult__
#define __ISQLResult__


#include "mString.h"

namespace mray
{
namespace db
{

	class ISQLFieldInfo
	{
	public:
		virtual const core::string& getName()=0;
		virtual uint Length()=0;
	};

class ISQLResult
{
protected:
public:
	ISQLResult(){}
	virtual~ISQLResult(){}

	virtual const char* operator[](int index)=0;

	virtual ISQLFieldInfo* GetFieldInfo(int index)=0;
	virtual int Fieldsount()=0;
	virtual int RowsCount()=0;
	virtual bool NextRow()=0;
	virtual bool HasMore()=0;
};

}
}


#endif
