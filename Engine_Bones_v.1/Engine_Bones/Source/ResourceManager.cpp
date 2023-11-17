#include "ResourceManager.h"

ResourceManager::ResourceManager()
{
	this->UUID = app->RandomGenerator.Int();
	R_Manager = this;
}

ResourceManager::ResourceManager(ResourceTypes Rtype) : type(Rtype)
{
	this->UUID = app->RandomGenerator.Int();
	R_Manager = this;
}

ResourceManager::ResourceManager(const char* Rname, uint Rcount, const char* Rassetspath, uint32 Ruuid, ResourceTypes Rtype) : name(Rname), resourceCounter(Rcount), AssetsPath(Rassetspath), UUID(Ruuid), type(Rtype)
{
	this->UUID == 0 ? app->RandomGenerator.Int() : Ruuid;
	R_Manager = this;
}

string ResourceManager::getAssetsPath()
{
	return this->AssetsPath;
}

string ResourceManager::getLibraryPath()
{
	return this->LibraryPath;
}

uint32 ResourceManager::getUUID()
{
	return this->UUID;
}

ResourceTypes ResourceManager::getType()
{
	return this->type;
}

uint ResourceManager::getResourceCounter()
{
	return this->resourceCounter;
}
