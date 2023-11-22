#ifndef RESOURCE_H_
#define RESOURCE_H_

#include "Application.h"
#include "Globals.h"

enum ResourceTypes
{
	R_MESH,
	R_MODEL,
	R_MATERIAL,
	R_NONE
};

class Resource
{
public:
	Resource();
	Resource(ResourceTypes Rtype);
	Resource(const char* Rname, const char* Rassetspath, ResourceTypes Rtype, uint32 Ruuid);
	~Resource();

	std::string name;
	uint resourceCounter;
	std::string AssetsPath;
	std::string LibraryPath;

	ResourceTypes type;
	uint32 UUID;

	std::vector<uint32> ComponentsOrChildrensInModel;

	std::string getAssetsPath();
	std::string getLibraryPath();

	uint32 getUUID();
	ResourceTypes getType();
	uint getResourceCounter();
};

#endif // RESOURCE_H_
