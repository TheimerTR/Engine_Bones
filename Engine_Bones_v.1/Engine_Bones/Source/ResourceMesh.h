#ifndef RESOURCEMESH_H_
#define RESOURCEMESH_H_

#pragma once
#include "AssimpManager.h"
#include "Resource.h"

class Resource;

class ResourceMesh : public Resource
{
public:
	ResourceMesh();
	ResourceMesh(const char* Rname, const char* Rassetspath, const char* Rlibrarypath, uint32 Ruuid);

	Mesh* mesh;

	uint UUID;
	uint size;
};

#endif // RESOURCEMESH_H_
