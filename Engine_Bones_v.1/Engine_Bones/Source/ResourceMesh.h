#ifndef RESOURCEMESH_H_
#define RESOURCEMESH_H_

#include "AssimpManager.h"
#include "ResourceElement.h"
#include "Application.h"

class ResourceMesh : public ResourceElement
{
public:
	ResourceMesh();
	ResourceMesh(const char* Rname, const char* Rassetspath, const char* Rlibrarypath, uint32 Ruuid);

	Mesh* mesh;

	uint UUID;
	uint size;
};

#endif // RESOURCEMESH_H_
