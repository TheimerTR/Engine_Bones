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
	L_CASUAL, L_WARNING, L_ERROR, L_ALL
};

#define LOG(_type, format, ...) log(__FILE__, __LINE__, _type, format, __VA_ARGS__);

void log(const char file[], int line, LogTypeCase _type, const char* format, ...);

void ClearLogs(std::vector<std::string>& l_Logs);

void AddLog(std::string logText, LogTypeCase _type);

template <class T>
void VectorLogsPushBack(std::vector<T>& vector, int MaxValue, T& Log);

#define CAP(n) ((n <= 0.0f) ? n=0.0f : (n >= 1.0f) ? n=1.0f : n=n)

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#define PI 3.14159265358979323846264338327950288

#define MAX(a,b) ((a)>(b)) ? (a) : (b)
#define MAX_LOG_FPS 100
#define MAX_FPS 100



typedef unsigned int uint;
typedef unsigned __int64 uint64;
typedef unsigned __int32 uint32;

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
#define VSYNC false
#define TITLE "Bones Engine :D"

#define CHECKERS_HEIGHT 64
#define CHECKERS_WIDTH 64

#define LIBRARY_PATH "Library/"
#define MESHES_PATH "Library/Meshes/"
#define TEXTURES_PATH "Library/Textures/"
#define SCENES_PATH "Library/Scenes/"
#define MODELS_PATH "Library/Models/"

#define ASSETS_FOLDER "Assets/"
#define MODELS_FOLDER "Assets/ModelsFbx/"
#define TEXTURES_FOLDER "Assets/Textures/"
#define SCENES_FOLDER "Assets/Scenes/"

#define MESH_EXTENSION ".mesh"
#define SCENE_EXTENSION	".scene"
#define TEXTURE_EXTENSION ".tex"

// Deletes a buffer
#define RELEASE( x )\
    {\
       if( x != nullptr )\
       {\
         delete x;\
	     x = nullptr;\
       }\
    }

// Deletes an array of buffers
#define RELEASE_ARRAY( x )\
	{\
       if( x != nullptr )\
       {\
           delete[] x;\
	       x = nullptr;\
	   }\
	}

#define RELEASE_ARRAY_LIST( x )\
	{\
		for (auto it_list = x.begin(); it_list != x.end();)\
		{\
			RELEASE(*it_list);\
			it_list = x.erase(it_list);\
		}\
		x.clear();\
	}