#include "stdafx.h"


#include "WinConsole.h"

using namespace std;


namespace mray{
namespace OS{



WinConsole::~WinConsole(){
	maxConsoleRows=500;
	hStdHandle=INVALID_HANDLE_VALUE;
	fp=0;
	hConsoleHandle=0;
	closeConsole();
}

void WinConsole::openConsole(const core::string& title){
	if(hStdHandle==INVALID_HANDLE_VALUE){
		CONSOLE_SCREEN_BUFFER_INFO info;

		AllocConsole();

		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&info);
		info.dwSize.Y=(short)maxConsoleRows;
		SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE),info.dwSize);

		SetConsoleTitle(title.c_str());

		hStdHandle=GetStdHandle(STD_OUTPUT_HANDLE);
		hConsoleHandle=_open_osfhandle((long)hStdHandle,_O_TEXT);
		fp=_fdopen(hConsoleHandle,"w");
		
        *stdout = *fp;
        setvbuf( stdout, NULL, _IONBF, 0 );

        hStdHandle = GetStdHandle(STD_INPUT_HANDLE);
        hConsoleHandle = _open_osfhandle((long)hStdHandle, _O_TEXT);
        fp = _fdopen( hConsoleHandle, "r" );
        *stdin = *fp;
        setvbuf( stdin, NULL, _IONBF, 0 );

        hStdHandle = GetStdHandle(STD_ERROR_HANDLE);
        hConsoleHandle = _open_osfhandle((long)hStdHandle, _O_TEXT);
        fp = _fdopen( hConsoleHandle, "w" );
        *stderr = *fp;
        setvbuf( stderr, NULL, _IONBF, 0 );

		ios::sync_with_stdio();
	}
}

void WinConsole::closeConsole(){
	if(hStdHandle!=INVALID_HANDLE_VALUE){
		FreeConsole();
		fclose(fp);
		hStdHandle=INVALID_HANDLE_VALUE;
	}
}


void WinConsole::setColor(bool r,bool g,bool b){
	WORD attr=(r || g || b) ? FOREGROUND_INTENSITY : 0;

	if (r ) attr |= FOREGROUND_RED;
	if (g ) attr |= FOREGROUND_GREEN;
	if (b ) attr |= FOREGROUND_BLUE;

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), attr); 
}

void WinConsole::print(float r,float g,float b,const mchar*str,...){
	setColor(r,g,b);
	
	mchar text[256];

	va_list ap;
	va_start(ap,str);
	wvsprintf(text,str,ap);
	va_end(ap);

	print(text);
}

void WinConsole::print(const mchar*str){
#ifdef UNICODE
	wprintf_s(str);
#else
	printf_s(str);
#endif
}

void WinConsole::setPosition(int x,int y){
	COORD p;
	p.X=x;
	p.Y=y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),p);
	
}


}
}
