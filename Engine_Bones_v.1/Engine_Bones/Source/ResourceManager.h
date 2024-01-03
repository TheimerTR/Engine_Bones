#ifndef ASSMIPMANAGER_H_
#define ASSMIPMANAGER_H_

#include "Globals.h"
#include "JsonManager.h"
#include "External/MathGeoLib/include/MathGeoLib.h"
#include "ResourceElement.h"
#include<vector>
#include<map>

#include "External/Assimp/include/mesh.h"
#include "External/Assimp/include/cimport.h"
#include "External/Assimp/include/scene.h"
#include "External/Assimp/include/postprocess.h"
#pragma comment (lib, "External/Assimp/libx86/assimp.lib")

class GameObject;
class ResourceElement;
class TextureManager;
class JsonManager;
class JSArray;
struct Texture;

using namespace std;

struct Mesh
{
	Mesh() {
		local_aabb.SetNegativeInfinity();
	}
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

	uint num_Materials = 0;

	string Path;
	//bool isSelected;
	bool ShowNormals = false;

	float* center_normal = nullptr;
	float* center = nullptr;
	int faces = 0;

	//uint VAO;
	uint VBO;
	uint EBO;
	uint VN;
	//uint VNF;
	uint VT;

	//Abb boxes
	AABB local_aabb; 
};

namespace ResourceManager
{
	void ResourceLoader(const char* path, const char* pathTexture = nullptr);

	void SimpleResourceLoader(const char* Path, GameObject* gameObject, const char* texturePath = nullptr);

	void MetaExistenceLoader(const char* Path);

	void ChangeDebugMode(bool type);

	void CleanUp();

	template <class T>
	void ClearAssimpVec(vector<T>& vector);

	void GameObjectNodeTree(const aiScene* scene, int numMeshes, int pointer, /*aiMesh** M_Array*/ aiNode* actualObj, GameObject* _Parent, string Name, const char* Path, const char* texturePath);

	void Clear_Mesh(Mesh* mesh);

	void AplicateTransform(GameObject* gameObject, float3 position, float3 scale, Quat rotation);

	void SetBuffers(Mesh* M_mesh);

	void MetaFileCreator(GameObject* gameObject, const char* path);

	bool CheckNotDuplicateFromAssets(ResourceElement* R_Element, uint32 uuid);
	bool CheckResourceComponentsExistence(ResourceElement* R_Element);
	bool CheckStringComponentsExistence(string Component);

	bool ImportOnlyTexture(string Path);
	Texture* ChangeTexture(string Path);

	//Mesh* LoadMesh(aiMesh* MeshToLoad);

	vector<Mesh*> AllMeshes;

	int m_Counter;
};

#endif // ASSMIPMANAGER_H_