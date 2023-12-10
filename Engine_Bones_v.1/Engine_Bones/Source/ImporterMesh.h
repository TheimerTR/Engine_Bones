#ifndef IMPORTER_MESH_H_
#define IMPORTER_MESH_H_

#include "Globals.h"
#include "Module.h"
#include "Application.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "ComponentMesh.h"
#include "ResourceMesh.h"

#include <vector>
#include <stdio.h>

using namespace std;

struct aiScene;
struct aiNode;
struct aiMesh;
struct Mesh;

namespace Importer
{
	namespace ImporterMesh
	{
		void ImportMesh(ResourceMesh* M_mesh, const aiMesh* aiMesh);
		uint64 Save(const ResourceMesh* M_mesh, char** fileBuffer);
		void Load(ResourceMesh* M_mesh, const char* fileBuffer);
	}
};


#endif //IMPORTER_MESH_H_
