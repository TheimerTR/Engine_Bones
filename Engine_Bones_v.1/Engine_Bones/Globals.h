#pragma once

// Warning disabled ---
#pragma warning( disable : 4577 ) // Warning that exceptions are disabled
#pragma warning( disable : 4530 )

#include <windows.h>
#include <stdio.h>
#include <vector>
#include <string>

enum class LogTypeCase
{
	L_CASUAL, L_WARNING, L_ERROR
};

#define LOG(_type, format, ...) log(__FILE__, __LINE__, _type, format, __VA_ARGS__);

void log(const char file[], int line, LogTypeCase _type, const char* format, ...);

void ClearLogs(std::vector<std::string> l_Logs, ...);

void AddLog(std::vector<std::string> l_Logs, std::string logText);

#define CAP(n) ((n <= 0.0f) ? n=0.0f : (n >= 1.0f) ? n=1.0f : n=n)

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#define PI 3.14159265358979323846264338327950288

#define MAX(a,b) ((a)>(b)) ? (a) : (b)
#define MAX_LOG_FPS 100



typedef unsigned int uint;

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

// Configuration -----------
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define SCREEN_SIZE 1
#define WIN_FULLSCREEN false
#define WIN_RESIZABLE true
#define WIN_BORDERLESS false
#define WIN_FULLSCREEN_DESKTOP false
#define VSYNC true
#define TITLE "Bones Engine :D"