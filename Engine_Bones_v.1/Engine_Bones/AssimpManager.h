#ifndef ASSMIPMANAGER_H_
#define ASSMIPMANAGER_H_

#include "Globals.h"
#include<vector>

#include "Assimp/include/mesh.h"
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#pragma comment (lib, "Assimp/libx86/assimp.lib")

using namespace std;

struct Mesh
{
	//uint id_index = 0; // index in VRAM
	uint num_index = 0;
	uint* index = nullptr;
	//uint* id_vertex = 0; // unique vertex in VRAM
	uint num_vertex = 0;
	float* vertex = nullptr;

	uint num_normals = 0;
	float* normals = nullptr;

	uint num_Tex = 0;
	float* textures = nullptr;
};

namespace AssimpManager
{
	void AssimpLoader(const char* path);

	void ChangeDebugMode(bool type);

	void CleanUp();

	template <class T>
	void ClearAssimpVec(vector<T>& vector);

	//Mesh* LoadMesh(aiMesh* MeshToLoad);

	vector<Mesh*> Meshes;

	uint VBO;
	uint EBO;
	uint VAO;
	uint VN;
	uint VT;
};

class C_Mesh
{
private:
	Mesh* mesh;
	string path;

public:

	C_Mesh();

	Mesh* GetMesh() const { return mesh; };
	std::string  GetPath() const { return path; };

	void SetMesh(Mesh* mesh);
	void SetPath(string path);
};

extern C_Mesh* CM;

#endif // ASSMIPMANAGER_H_
