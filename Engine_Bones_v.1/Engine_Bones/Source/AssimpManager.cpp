#include "AssimpManager.h"

#include<stdlib.h>
#include<time.h>
#include<vector>
#include<string>

#include "External\Glew\include\glew.h"
#include "External\SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include "Application.h"
#include "ModuleRenderer3D.h"
#include "FileSystemManager.h"
#include "Globals.h"
#include "GameObjectManager.h"
#include "TextureManager.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentManager.h"
#include "ModuleScene.h"

#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include "External/SDL/include/SDL_opengl.h"
#endif

#include "External/Devil/Include/ilut.h"
#include "External/Devil/Include/ilu.h"
#include "External/Devil/Include/il.h"

#pragma comment (lib, "External/Devil/libx86/DevIL.lib")
#pragma comment (lib, "External/Devil/libx86/ILU.lib")
#pragma comment (lib, "External/Devil/libx86/ILUT.lib")

#pragma comment (lib, "glu32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "External/Glew/libx86/glew32.lib")

void AssimpManager::AssimpLoader(const char* path, const char* pathTexture)
{
	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene != nullptr && scene->HasMeshes())
	{
		string FileName = "";

		FileSystem::StringDivide(path, &FileName, nullptr);

		GameObjectNodeTree(scene, scene->mNumMeshes, 0, scene->mRootNode, app->scene->Root, FileName.c_str(), path, pathTexture );

		aiReleaseImport(scene);
	}
	else
		LOG(LogTypeCase::L_ERROR, "Error loading scene % s", path);
}

void AssimpManager::SimpleAssimpLoader(const char* Path, GameObjectManager* gameObject, const char* texturePath)
{
	const aiScene* scene = aiImportFile(Path, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene != nullptr && scene->HasMeshes())
	{
		string FileName = "";

		FileSystem::StringDivide(Path, &FileName, nullptr);
	
		Mesh* M_mesh = new Mesh();

		M_mesh->Path = Path;

		M_mesh->Name = FileName;

		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			M_mesh->num_vertex = scene->mMeshes[i]->mNumVertices;
			M_mesh->vertex = new float[M_mesh->num_vertex * 3];
			memcpy(M_mesh->vertex, scene->mMeshes[i]->mVertices, sizeof(float) * M_mesh->num_vertex * 3);
			LOG(LogTypeCase::L_CASUAL, "New mesh with %d vertices", M_mesh->num_vertex);

			if (scene->mMeshes[i]->HasFaces())
			{
				M_mesh->num_index = scene->mMeshes[i]->mNumFaces * 3;
				M_mesh->index = new uint[M_mesh->num_index]; // assume each face is a triangle

				/*M_mesh->num_normals_Faces = scene->mMeshes[i]->mNumFaces * 3;
				M_mesh->normals_Faces = new uint[M_mesh->num_normals_Faces];*/

				for (uint d = 0; d < scene->mMeshes[i]->mNumFaces; ++d)
				{
					if (scene->mMeshes[i]->mFaces[d].mNumIndices != 3)
					{
						LOG(LogTypeCase::L_WARNING, "WARNING, geometry face with != 3 indices!");
					}
					else
					{
						memcpy(&M_mesh->index[d * 3], scene->mMeshes[i]->mFaces[d].mIndices, 3 * sizeof(uint));
						//memcpy(&M_mesh->normals_Faces[d * 3], scene->mMeshes[i]->mFaces[d].mIndices, 3 * sizeof(uint));
					}
				}

				LOG(LogTypeCase::L_CASUAL, "With %d indices", M_mesh->num_index);
			}

			if (scene->mMeshes[i]->HasNormals())
			{
				M_mesh->num_normals = scene->mMeshes[i]->mNumVertices;
				M_mesh->normals = new float[M_mesh->num_normals * 3];
				memcpy(M_mesh->normals, scene->mMeshes[i]->mNormals, sizeof(float) * M_mesh->num_normals * 3);
				//memcpy(&M_mesh->index[d * 3], scene->mMeshes[i]->mFaces[d].mIndices, 3 * sizeof(uint));
				/*memcpy(&M_mesh->normals_Faces[d * 3], scene->mMeshes[i]->mFaces[d].mIndices, 3 * sizeof(uint));*/
			}

			uint UV_Index = 0;
			if (scene->mMeshes[i]->HasTextureCoords(UV_Index))
			{
				M_mesh->num_Tex = scene->mMeshes[i]->mNumVertices;
				M_mesh->textures = new math::float2[scene->mMeshes[i]->mNumVertices];

				for (int j = 0; j < M_mesh->num_Tex; j++)
				{
					M_mesh->textures[j].x = scene->mMeshes[i]->mTextureCoords[UV_Index][j].x;
					M_mesh->textures[j].y = scene->mMeshes[i]->mTextureCoords[UV_Index][j].y;
				}
			}
		}

		M_mesh->VBO = 0;
		M_mesh->EBO = 0;
		M_mesh->VN = 0;
		M_mesh->VT = 0;

		glGenBuffers(1, (GLuint*)&(M_mesh->VBO));
		glBindBuffer(GL_ARRAY_BUFFER, M_mesh->VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * M_mesh->num_vertex * 3, M_mesh->vertex, GL_STATIC_DRAW);

		glGenBuffers(1, (GLuint*)&(M_mesh->EBO));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, M_mesh->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * M_mesh->num_index, M_mesh->index, GL_STATIC_DRAW);

		glGenBuffers(1, (GLuint*)&(M_mesh->VN));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, M_mesh->VN);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * M_mesh->num_normals, M_mesh->normals, GL_STATIC_DRAW);

		/*glGenBuffers(1, (GLuint*)&(M_mesh->VNF));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, M_mesh->VNF);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * M_mesh->num_normals_Faces, M_mesh->normals_Faces, GL_STATIC_DRAW);*/

		glGenBuffers(1, (GLuint*)&(M_mesh->VT));
		glBindBuffer(GL_ARRAY_BUFFER, M_mesh->VT);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * M_mesh->num_Tex * 2, M_mesh->textures, GL_STATIC_DRAW);

		AllMeshes.push_back(M_mesh);

		ComponentMesh* C_Mesh = dynamic_cast<ComponentMesh*>(gameObject->AddComponent(ComponentType::MESH));
		C_Mesh->SetMesh(M_mesh);
	}
	else
		LOG(LogTypeCase::L_ERROR, "Error loading scene % s", Path);
}

void AssimpManager::GameObjectNodeTree(const aiScene* scene, int numMeshes, int pointer, /*aiMesh** M_Array*/ aiNode* actualObj, GameObjectManager* _Parent, string Name, const char* Path, const char* texturePath)
{
	int i = pointer;

	aiVector3D translation, scaling;
	aiQuaternion rotation;
	
	actualObj->mTransformation.Decompose(scaling, rotation, translation);
	float3 pos(translation.x, translation.y, translation.z);
	float3 scale(scaling.x, scaling.y, scaling.z);
	Quat rot(rotation.x, rotation.y, rotation.z, rotation.w);

	GameObjectManager* _ParentObj = new GameObjectManager(Name, _Parent);

	/*AplicateTransform(_ParentObj, pos, scale, rot);*/

	if (actualObj->mNumChildren > 0 && numMeshes > 1)
	{
		for (i; i < numMeshes; i++)
		{
			GameObjectNodeTree(scene, actualObj->mChildren[i]->mNumChildren + i, i, actualObj->mChildren[i], _ParentObj, actualObj->mChildren[i]->mName.C_Str(), Path, texturePath);
		}
	}

	Mesh* M_mesh = new Mesh();

	M_mesh->Path = Path;

	if (i < scene->mNumMeshes)
	{
		M_mesh->num_vertex = scene->mMeshes[i]->mNumVertices;
		M_mesh->vertex = new float[M_mesh->num_vertex * 3];
		memcpy(M_mesh->vertex, scene->mMeshes[i]->mVertices, sizeof(float) * M_mesh->num_vertex * 3);
		LOG(LogTypeCase::L_CASUAL, "New mesh with %d vertices", M_mesh->num_vertex);

		if (scene->mMeshes[i]->HasFaces())
		{
			M_mesh->num_index = scene->mMeshes[i]->mNumFaces * 3;
			M_mesh->index = new uint[M_mesh->num_index]; // assume each face is a triangle

			/*M_mesh->num_normals_Faces = scene->mMeshes[i]->mNumFaces * 3;
			M_mesh->normals_Faces = new uint[M_mesh->num_normals_Faces];*/

			for (uint d = 0; d < scene->mMeshes[i]->mNumFaces; ++d)
			{
				if (scene->mMeshes[i]->mFaces[d].mNumIndices != 3)
				{
					LOG(LogTypeCase::L_WARNING, "WARNING, geometry face with != 3 indices!");
				}
				else
				{
					memcpy(&M_mesh->index[d * 3], scene->mMeshes[i]->mFaces[d].mIndices, 3 * sizeof(uint));
					//memcpy(&M_mesh->normals_Faces[d * 3], scene->mMeshes[i]->mFaces[d].mIndices, 3 * sizeof(uint));
				}
			}

			LOG(LogTypeCase::L_CASUAL, "With %d indices", M_mesh->num_index);
		}

		if (scene->mMeshes[i]->HasNormals())
		{
			M_mesh->normals = new float[scene->mMeshes[i]->mNumVertices * 3];
			memcpy(M_mesh->normals, scene->mMeshes[i]->mNormals, sizeof(float) * scene->mMeshes[i]->mNumVertices * 3);

			M_mesh->center_normal = new float[scene->mMeshes[i]->mNumFaces * 3];
			M_mesh->center = new float[scene->mMeshes[i]->mNumFaces * 3];
			M_mesh->faces = scene->mMeshes[i]->mNumFaces;

			for (uint i = 0; i < M_mesh->num_index; i += 3)
			{
				float3 x0(M_mesh->vertex[(M_mesh->index[i] * 3)], M_mesh->vertex[(M_mesh->index[i] * 3) + 1], M_mesh->vertex[(M_mesh->index[i] * 3) + 2]);
				float3 x1(M_mesh->vertex[(M_mesh->index[i + 1] * 3)], M_mesh->vertex[(M_mesh->index[i + 1] * 3) + 1], M_mesh->vertex[(M_mesh->index[i + 1] * 3) + 2]);
				float3 x2(M_mesh->vertex[(M_mesh->index[i + 2] * 3)], M_mesh->vertex[(M_mesh->index[i + 2] * 3) + 1], M_mesh->vertex[(M_mesh->index[i + 2] * 3) + 2]);

				float3 v0 = x1 - x0;
				float3 v1 = x2 - x1;
				float3 normalized = v0.Cross(v1);

				M_mesh->center[i] = (x0.x + x1.x + x2.x) / 3;
				M_mesh->center[i + 1] = (x0.y + x1.y + x2.y) / 3;
				M_mesh->center[i + 2] = (x0.z + x1.z + x2.z) / 3;

				M_mesh->center_normal[i] = normalized.Normalized().x;
				M_mesh->center_normal[i + 1] = normalized.Normalized().y;
				M_mesh->center_normal[i + 2] = normalized.Normalized().z;
			}
		}

		uint UV_Index = 0;
		if (scene->mMeshes[i]->HasTextureCoords(UV_Index))
		{
			M_mesh->num_Tex = scene->mMeshes[i]->mNumVertices;
			M_mesh->textures = new math::float2[scene->mMeshes[i]->mNumVertices];

			for (int j = 0; j < M_mesh->num_Tex; j++)
			{
				M_mesh->textures[j].x = scene->mMeshes[i]->mTextureCoords[UV_Index][j].x;
				M_mesh->textures[j].y = scene->mMeshes[i]->mTextureCoords[UV_Index][j].y;
			}
		}

		M_mesh->Name = _ParentObj->mName;

		/*FileSystem::StringDivide(Path, &M_mesh->Name, nullptr);*/

		//M_mesh->VAO = 0;
		M_mesh->VBO = 0;
		M_mesh->EBO = 0;
		M_mesh->VN = 0;
		//M_mesh->VNF = 0;
		M_mesh->VT = 0;

		/*glGenVertexArrays(1, &M_mesh->VAO);
		glBindVertexArray(M_mesh->VAO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);*/

		glGenBuffers(1, (GLuint*)&(M_mesh->VBO));
		glBindBuffer(GL_ARRAY_BUFFER, M_mesh->VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * M_mesh->num_vertex * 3, M_mesh->vertex, GL_STATIC_DRAW);

		glGenBuffers(1, (GLuint*)&(M_mesh->EBO));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, M_mesh->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * M_mesh->num_index, M_mesh->index, GL_STATIC_DRAW);

		glGenBuffers(1, (GLuint*)&(M_mesh->VN));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, M_mesh->VN);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * M_mesh->num_vertex * 3, M_mesh->normals, GL_STATIC_DRAW);

		/*glGenBuffers(1, (GLuint*)&(M_mesh->VNF));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, M_mesh->VNF);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * M_mesh->num_normals_Faces, M_mesh->normals_Faces, GL_STATIC_DRAW);*/

		glGenBuffers(1, (GLuint*)&(M_mesh->VT));
		glBindBuffer(GL_ARRAY_BUFFER, M_mesh->VT);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * M_mesh->num_Tex * 2, M_mesh->textures, GL_STATIC_DRAW);

		AllMeshes.push_back(M_mesh);

		//Components here
		ComponentTransform* transform = new ComponentTransform(nullptr);

		ComponentMesh* C_Mesh = dynamic_cast<ComponentMesh*>(_ParentObj->AddComponent(ComponentType::MESH));
		C_Mesh->SetMesh(M_mesh);

		int C = 0;
		for (int m = 0; m < app->scene->AllGameObjectManagers.size(); m++)
		{
			if (C > 0)
			{
				std::string s = std::to_string(C);
				_ParentObj->mName = M_mesh->Name + s;
				int compare = strcmp(app->scene->AllGameObjectManagers.at(m)->mName.c_str(), _ParentObj->mName.c_str());

				if (compare == 0)
				{
					std::string stri = std::to_string(C++);
					_ParentObj->mName = M_mesh->Name + s;
				}
			}

			if (strcmp(app->scene->AllGameObjectManagers.at(m)->mName.c_str(), _ParentObj->mName.c_str()) == 0)
			{
				if (C == 0)
				{
					_ParentObj->mName = M_mesh->Name;
					C++;
				}
			}
		}

		AplicateTransform(_ParentObj, pos, scale, rot);

		if (texturePath != NULL)
		{
			TexturesManager* texturesManager = new TexturesManager();
			ComponentMaterial* C_Texture = dynamic_cast<ComponentMaterial*>(_ParentObj->AddComponent(ComponentType::MATERIAL));
			C_Texture->SetTexture(texturesManager->TexLoader(texturePath));
		}
	}
}

void AssimpManager::ChangeDebugMode(bool type)
{
	if (type)
	{
		// Stream log messages to Debug window
		struct aiLogStream stream;
		stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
		aiAttachLogStream(&stream);
	}
	else
	{
		aiDetachAllLogStreams();
	}
}

void AssimpManager::CleanUp()
{
	for (int i = 0; i < AllMeshes.size(); i++)
	{
		if (AllMeshes[i]->VBO != 0)
		{
			glDeleteBuffers(1, &AllMeshes[i]->VBO);
		}
		/*if (AllMeshes[i]->VAO != 0)
		{
			glDeleteBuffers(1, &AllMeshes[i]->VAO);
		}*/
		if (AllMeshes[i]->EBO != 0)
		{
			glDeleteBuffers(1, &AllMeshes[i]->EBO);
		}
		if (AllMeshes[i]->VN != 0)
		{
			glDeleteBuffers(1, &AllMeshes[i]->VN);
		}
		if (AllMeshes[i]->VT != 0)
		{
			glDeleteBuffers(1, &AllMeshes[i]->VT);
		}
	}
}

template <class T>
void AssimpManager::ClearAssimpVec(std::vector<T>& vector)
{
	if (vector.size() > 0)
	{
		for (int i = 0; i < vector.size() - 1; i++)
		{
			vector.pop_back();
		}
	}

	vector.clear();
}

void  AssimpManager::Clear_Mesh(Mesh* mesh)
{
	if (mesh->VBO != 0)
	{
		glDeleteBuffers(1, &mesh->VBO);
	}
	/*if (mesh->VAO != 0)
	{
		glDeleteBuffers(1, &mesh->VAO);
	}*/
	if (mesh->EBO != 0)
	{
		glDeleteBuffers(1, &mesh->EBO);
	}
	if (mesh->VN != 0)
	{
		glDeleteBuffers(1, &mesh->VN);
	}
	if (mesh->VT != 0)
	{
		glDeleteBuffers(1, &mesh->VT);
	}

	mesh->Name = "";
	mesh->Path = "";

	mesh->num_index = 0;
	mesh->index = nullptr;
	mesh->num_vertex = 0;
	mesh->vertex = nullptr;

	mesh->num_normals = 0;
	mesh->normals = nullptr;

	mesh->num_Tex = 0;
	mesh->textures = nullptr;

	for (auto it = AllMeshes.begin(); it != AllMeshes.end();) 
	{
		if (*it == mesh) 
		{
			it = AllMeshes.erase(it);
		}
		else {
			++it;
		}
	}
}

void AssimpManager::AplicateTransform(GameObjectManager* gameObject, float3 position, float3 scale, Quat rotation) 
{
	gameObject->mTransform->SetTransform(gameObject, position, scale, rotation); 
}