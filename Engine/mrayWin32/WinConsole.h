

#ifndef ___WinConsole___
#define ___WinConsole___


#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <io.h>

#include "CompileConfig.h"
#include <mTypes.h>
#include <IConsole.h>


namespace mray{
namespace OS{

class MRAY_Win32_DLL WinConsole:public IConsole
{
protected:
	short maxConsoleRows;

	FILE*fp;
	HANDLE hStdHandle;
	int hConsoleHandle;
public:
	virtual~WinConsole();

	void openConsole(const core::string& title);
	void closeConsole();
	
	void setColor(bool r,bool g,bool b);
	void setPosition(int x,int y);

	void print(const mchar*str);
	void print(float r,float g,float b,const mchar*str,...);
};


};//OS
};//mray
#endif




