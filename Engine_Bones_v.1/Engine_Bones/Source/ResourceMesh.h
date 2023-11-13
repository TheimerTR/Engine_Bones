#ifndef RESOURCEMESH_H_
#define RESOURCEMESH_H_

#include "Application.h"
#include "Globals.h"
#include "ResourceManager.h"
#include "AssimpManager.h"

using namespace std;

class ResourceMesh : public ResourceManager {
public:
	Mesh* mesh;

	uint UUID;
	uint size;

	ResourceMesh();
	ResourceMesh(const char* Rname, const char* Rassetspath, const char* Rlibrarypath, uint32 Ruuid);
};

#endif // RESOURCEMESH_H_
