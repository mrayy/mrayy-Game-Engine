// Console.h
// コンソール出力用ユーティリティ関数

#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <conio.h>

static HANDLE STDOUT = GetStdHandle(STD_OUTPUT_HANDLE);

namespace Console
{
    void    locate(int x, int y);
    void    clear();
    void    clear(int line);
	void	setColor(bool r, bool g, bool b);
};

void    Console::locate(int x, int y)
{
    COORD coord = {x, y};
    SetConsoleCursorPosition(STDOUT, coord);
}

void    Console::clear()
{
    COORD coordScreen = {0, 0};
    DWORD written;
    CONSOLE_SCREEN_BUFFER_INFO  csbi;

    GetConsoleScreenBufferInfo(STDOUT, &csbi);
    FillConsoleOutputCharacter(STDOUT, (TCHAR)' ', csbi.dwSize.X * csbi.dwSize.Y, coordScreen, &written);
    GetConsoleScreenBufferInfo(STDOUT, &csbi);
    FillConsoleOutputAttribute(STDOUT, csbi.wAttributes, csbi.dwSize.X * csbi.dwSize.Y, coordScreen, &written);
}

void    Console::clear(int line)
{
    COORD coordScreen = {0, line};
    DWORD written;
    CONSOLE_SCREEN_BUFFER_INFO  csbi;

    GetConsoleScreenBufferInfo(STDOUT, &csbi);
    FillConsoleOutputCharacter(STDOUT, (TCHAR)' ', csbi.dwSize.X, coordScreen, &written);
    GetConsoleScreenBufferInfo(STDOUT, &csbi);
    FillConsoleOutputAttribute(STDOUT, csbi.wAttributes, csbi.dwSize.X, coordScreen, &written);
}

void Console::setColor(bool r, bool g, bool b)
{

	WORD attr = (r || g || b) ? FOREGROUND_INTENSITY : 0;

	if (r) attr |= FOREGROUND_RED;
	if (g) attr |= FOREGROUND_GREEN;
	if (b) attr |= FOREGROUND_BLUE;

	SetConsoleTextAttribute(STDOUT, attr);
}