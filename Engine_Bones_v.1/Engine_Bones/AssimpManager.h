#pragma once

#ifndef ASSMIPMANAGER_H_
#define ASSMIPMANAGER_H_

#include<stdlib.h>
#include<time.h>
#include<vector>
#include<string>

#include "Application.h"
#include "ModuleRenderer3D.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#pragma comment (lib, "Assimp/libx86/assimp.lib")

namespace AssimpManager
{
	struct Meshes
	{
		uint id_index = 0; // index in VRAM
		uint num_index = 0;
		uint* index = nullptr;
		uint id_vertex = 0; // unique vertex in VRAM
		uint num_vertex = 0;
		float* vertex = nullptr;
	}meshes;

	void AssimpLoader(std::vector<Meshes>& meshes, std::string& path)
	{
	
	}

	void ChangeDebugMode(bool type)
	{
		if(type)
		{
			// Stream log messages to Debug window
			struct aiLogStream stream;
			stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
			aiAttachLogStream(&stream);
		}
		else
		{
			aiDetachAllLogStreams();
		}
	}

	template <class T>
	void ClearAssimpVec(std::vector<T>& vector)
	{
		if (vector.size() > 0)
		{
			for (int i = 0; i < vector.size() - 1; i++)
			{
				vector.pop_back();
			}
		}

		vector.clear();
	}
}

#endif // ASSMIPMANAGER_H_
