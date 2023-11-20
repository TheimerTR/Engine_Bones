#ifndef RESOURCEMANAGER_H_
#define RESOURCEMANAGER_H_

#include "Application.h"
#include "Globals.h"
#include "JsonManager.h"
#include "AssimpManager.h"

class JsonManager;
using namespace std;

enum class ResourceTypes
{
	MESH,
	MODEL,
	MATERIAL,
	NONE
};

class ResourceManager
{
public:
	string name;
	uint resourceCounter = 0;
	string AssetsPath = "";
	string LibraryPath = "";

	ResourceManager* R_Manager;

	ResourceTypes type = ResourceTypes::NONE;
	uint32 UUID = 0;

	ResourceManager();
	ResourceManager(ResourceTypes Rtype);
	ResourceManager(const char* Rname, const char* Rassetspath, ResourceTypes Rtype, uint32 Ruuid);

	string getAssetsPath();
	string getLibraryPath();

	uint32 getUUID();
	ResourceTypes getType();
	uint getResourceCounter();
};

#endif // RESOURCEMANAGER_H_
