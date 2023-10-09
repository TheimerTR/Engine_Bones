#pragma once

#ifndef FILESYSTEMMANAGER_H_
#define FILESYSTEMMANAGER_H_

#include "Globals.h"
#include<vector>

using namespace std;

enum class FileType
{
	MODEL_3D,
	TEXTURE,
	DEFAULT
};

namespace FileSystem
{
	void ReadFyleType(const char* Path);

	void StringDivide(const char* OG_Path, string* file, string* extension);

	void CompatibleString(string& ConvertPath);

	FileType& ExtensionType(string ConvertPath);
};

#endif // FILESYSTEMMANAGER_H_