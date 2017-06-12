// Console.h
// コンソール出力用ユーティリティ関数

#pragma once


#ifndef __consoleH__
#define __consoleH__


#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <conio.h>

static HANDLE STDOUT = GetStdHandle(STD_OUTPUT_HANDLE);

namespace Console
{
    void    locate(int x, int y);
    void    clear();
    void    clear(int line);
};

#endif