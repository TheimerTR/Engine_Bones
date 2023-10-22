#ifndef ASSMIPMANAGER_H_
#define ASSMIPMANAGER_H_

#include "Globals.h"
#include "External/MathGeoLib/include/MathGeoLib.h"
#include<vector>

#include "External/Assimp/include/mesh.h"
#include "External/Assimp/include/cimport.h"
#include "External/Assimp/include/scene.h"
#include "External/Assimp/include/postprocess.h"
#pragma comment (lib, "External/Assimp/libx86/assimp.lib")

class GameObjectManager;
class TextureManager;

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
	
	uint num_normals_Faces = 0;
	uint* normals_Faces = nullptr;

	uint num_Tex = 0;
	math::float2* textures = nullptr;

	string Path;
	//bool isSelected;
	bool ShowNormals = false;

	uint VAO;
	uint VBO;
	uint EBO;
	uint VN;
	uint VNF;
	uint VT;
};

namespace AssimpManager
{
	void AssimpLoader(const char* path, const char* pathTexture = nullptr);

	void ChangeDebugMode(bool type);

	void CleanUp();

	template <class T>
	void ClearAssimpVec(vector<T>& vector);

	void GameObjectNodeTree(const aiScene* scene, int numMeshes, int pointer, /*aiMesh** M_Array*/ aiNode* actualObj, GameObjectManager* _Parent, string Name, const char* Path, const char* texturePath);

	void Clear_Mesh(Mesh* mesh);

	void AplicateTransform(GameObjectManager* gameObject, float3 position, float3 scale, Quat rotation); 

	//Mesh* LoadMesh(aiMesh* MeshToLoad);

	vector<Mesh*> AllMeshes;
};

#endif // ASSMIPMANAGER_H_
