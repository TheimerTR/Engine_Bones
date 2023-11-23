#include "ResourceMesh.h"

ResourceMesh::ResourceMesh() : ResourceElement()
{
	this->mesh = new Mesh;
	this->UUID = app->RandomGenerator.Int();
	size = 0;
}

ResourceMesh::ResourceMesh(const char* Rname, const char* Rassetspath, const char* Rlibrarypath, uint32 Ruuid) : ResourceElement(ResourceTypes::R_MESH)
{
	this->name = name;
	this->LibraryPath = LibraryPath;
	this->AssetsPath = AssetsPath;

	if(this->UUID != 0)
	{
		this->UUID == UUID;
	}

	size = 0;
}
