#ifndef ASSMIPMANAGER_H_
#define ASSMIPMANAGER_H_

#include "Globals.h"
#include<vector>

#include "External/Assimp/include/mesh.h"
#include "External/Assimp/include/cimport.h"
#include "External/Assimp/include/scene.h"
#include "External/Assimp/include/postprocess.h"
#pragma comment (lib, "External/Assimp/libx86/assimp.lib")

using namespace std;

struct Mesh
{
	string Name = "";

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

	uint VAO;
	uint VBO;
	uint EBO;
	uint VN;
	uint VT;
};

namespace AssimpManager
{
	void AssimpLoader(const char* path);

	void ChangeDebugMode(bool type);

	void CleanUp();

	template <class T>
	void ClearAssimpVec(vector<T>& vector);

	void Clear_Mesh(Mesh* mesh);

	//Mesh* LoadMesh(aiMesh* MeshToLoad);

	vector<Mesh*> Meshes;
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