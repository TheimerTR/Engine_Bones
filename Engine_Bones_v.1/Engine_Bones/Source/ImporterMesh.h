#ifndef IMPORTER_MESH_H_
#define IMPORTER_MESH_H_

#include "Globals.h"
#include "Module.h"
#include "Application.h"
#include "AssimpManager.h"
#include "GameObjectManager.h"
#include "ComponentMesh.h"

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
		void ImportMesh(Mesh* M_mesh, const aiMesh* aiMesh);
		uint64 Save(const Mesh* M_mesh, char** fileBuffer);
		void Load(Mesh* M_mesh, const char* fileBuffer);
	}
};


#endif //IMPORTER_MESH_H_
