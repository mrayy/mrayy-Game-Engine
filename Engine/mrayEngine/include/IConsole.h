
/********************************************************************
	created:	2009/01/02
	created:	2:1:2009   16:50
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\IConsole.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	IConsole
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___IConsole___
#define ___IConsole___

#include "mString.h"
#include "ISingleton.h"

namespace mray{
namespace OS{

class IConsole:public ISingleton<IConsole>
{
public:
	IConsole(){}
	virtual~IConsole(){}

	virtual void openConsole(const core::string& title)=0;
	virtual void closeConsole()=0;

	virtual void setColor(bool r,bool g,bool b)=0;
	virtual void setPosition(int x,int y)=0;

	virtual void print(const mchar*str)=0;
	virtual void print(float r,float g,float b,const mchar*str,...)=0;
};


};
}


#endif //___IConsole___
