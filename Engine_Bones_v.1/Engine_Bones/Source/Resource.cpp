#include "Resource.h"

Resource::Resource()
{
	UUID = app->RandomGenerator.Int();
};

Resource::Resource(ResourceTypes Rtype) : type(Rtype)
{
	UUID = app->RandomGenerator.Int();
};

Resource::Resource(const char* Rname, const char* Rassetspath, ResourceTypes Rtype, uint32 Ruuid) : name(Rname), AssetsPath(Rassetspath), UUID(Ruuid), type(Rtype)
{
	UUID = (Ruuid == 0) ? app->RandomGenerator.Int() : Ruuid;
};

Resource::~Resource()
{
};

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
