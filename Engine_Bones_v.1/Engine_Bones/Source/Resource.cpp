#include "Resource.h"

Resource::Resource()
{
	this->UUID = app->RandomGenerator.Int();
	R_Manager = this;
}

Resource::Resource(ResourceTypes Rtype) : type(Rtype)
{
	this->UUID = app->RandomGenerator.Int();
	R_Manager = this;
}

Resource::Resource(const char* Rname, const char* Rassetspath, ResourceTypes Rtype, uint32 Ruuid) : name(Rname), AssetsPath(Rassetspath), UUID(Ruuid), type(Rtype)
{
	this->UUID == 0 ? app->RandomGenerator.Int() : Ruuid;
	R_Manager = this;
}

string Resource::getAssetsPath()
{
	return this->AssetsPath;
}

string Resource::getLibraryPath()
{
	return this->LibraryPath;
}

uint32 Resource::getUUID()
{
	return this->UUID;
}

ResourceTypes Resource::getType()
{
	return this->type;
}

uint Resource::getResourceCounter()
{
	return this->resourceCounter;
}
