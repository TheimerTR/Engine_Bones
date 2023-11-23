#include "ResourceElement.h"
#include "Application.h"

ResourceElement::ResourceElement()
{
	name = "";
	resourceCounter = 0;
	AssetsPath = "";
	LibraryPath = "";

	type = ResourceTypes::R_NONE;
	UUID = app->RandomGenerator.Int();
}

ResourceElement::ResourceElement(ResourceTypes R_type)
{
	name = "";
	resourceCounter = 0;
	AssetsPath = "";
	LibraryPath = "";

	this->type = R_type;
	UUID = app->RandomGenerator.Int();
}

ResourceElement::ResourceElement(const char* name, const char* AssetsPath, ResourceTypes R_type, uint32 R_UUID)
{
	this->name = name;
	resourceCounter = 0;
	this->AssetsPath = AssetsPath;
	LibraryPath = "";

	this->type = R_type;
	UUID = (R_UUID == 0) ? app->RandomGenerator.Int() : R_UUID;
}

ResourceElement::~ResourceElement()
{
}

string ResourceElement::getAssetsPath()
{
	return this->AssetsPath;
}

string ResourceElement::getLibraryPath()
{
	return this->LibraryPath;
}

uint32 ResourceElement::getUUID()
{
	return this->UUID;
}

ResourceTypes ResourceElement::getType()
{
	return this->type;
}

uint ResourceElement::getResourceCounter()
{
	return this->resourceCounter;
}