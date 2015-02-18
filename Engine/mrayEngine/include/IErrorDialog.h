/********************************************************************
	created:	2009/02/27
	created:	27:2:2009   23:49
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\IErrorDialog.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	IErrorDialog
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___IErrorDialog___
#define ___IErrorDialog___

#include "ISingleton.h"
#include "mString.h"

namespace mray{

class MRAY_DLL IErrorDialog:public ISingleton<IErrorDialog>
{
public:

	IErrorDialog(){};
	virtual ~IErrorDialog(){};

	virtual bool run(const core::string&msg)=0;
};

}


#endif //___IErrorDialog___