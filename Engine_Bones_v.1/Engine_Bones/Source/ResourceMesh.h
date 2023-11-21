#ifndef RESOURCEMESH_H_
#define RESOURCEMESH_H_

#include "Application.h"
#include "Globals.h"
#include "Resource.h"
#include "AssimpManager.h"

using namespace std;
class Resource;

class ResourceMesh : public Resource
{
public:
	Mesh* mesh;

	uint UUID;
	uint size;

	ResourceMesh();
	ResourceMesh(const char* Rname, const char* Rassetspath, const char* Rlibrarypath, uint32 Ruuid);
};

#endif // RESOURCEMESH_H_
