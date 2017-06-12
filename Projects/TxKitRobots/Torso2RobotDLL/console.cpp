
#include "stdafx.h"
#include "console.h"

void Console::locate(int x, int y)
{
	COORD coord = { x, y };
	SetConsoleCursorPosition(STDOUT, coord);
}

void Console::clear()
{
	COORD coordScreen = { 0, 0 };
	DWORD written;
	CONSOLE_SCREEN_BUFFER_INFO  csbi;

	GetConsoleScreenBufferInfo(STDOUT, &csbi);
	FillConsoleOutputCharacter(STDOUT, (TCHAR)' ', csbi.dwSize.X * csbi.dwSize.Y, coordScreen, &written);
	GetConsoleScreenBufferInfo(STDOUT, &csbi);
	FillConsoleOutputAttribute(STDOUT, csbi.wAttributes, csbi.dwSize.X * csbi.dwSize.Y, coordScreen, &written);
}


void Console::clear(int line)
{
	COORD coordScreen = { 0, line };
	DWORD written;
	CONSOLE_SCREEN_BUFFER_INFO  csbi;

	GetConsoleScreenBufferInfo(STDOUT, &csbi);
	FillConsoleOutputCharacter(STDOUT, (TCHAR)' ', csbi.dwSize.X, coordScreen, &written);
	GetConsoleScreenBufferInfo(STDOUT, &csbi);
	FillConsoleOutputAttribute(STDOUT, csbi.wAttributes, csbi.dwSize.X, coordScreen, &written);
}