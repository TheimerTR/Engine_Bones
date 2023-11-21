#ifndef RESOURCE_H_
#define RESOURCE_H_

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

class Resource
{
public:

	Resource();
	Resource(ResourceTypes Rtype);
	Resource(const char* Rname, const char* Rassetspath, ResourceTypes Rtype, uint32 Ruuid);

	string name;
	uint resourceCounter = 0;
	string AssetsPath = "";
	string LibraryPath = "";

	Resource* R_Manager;

	ResourceTypes type = ResourceTypes::NONE;
	uint32 UUID = 0;

	vector<uint32> ComponentsOrChildrensInModel;

	string getAssetsPath();
	string getLibraryPath();

	uint32 getUUID();
	ResourceTypes getType();
	uint getResourceCounter();
};

#endif // RESOURCE_H_
