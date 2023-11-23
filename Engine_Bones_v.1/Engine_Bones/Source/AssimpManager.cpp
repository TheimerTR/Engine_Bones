#include "AssimpManager.h"
#include "ResourceMesh.h"

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
#include "ImporterMesh.h"
#include "ImporterTexture.h"
#include "ResourceElement.h"

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
		ResourceMesh* R_Mesh = new ResourceMesh();

		M_mesh->Path = Path;

		R_Mesh->name = FileName;

		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			Mesh* M_mesh = new Mesh();

			Importer::ImporterMesh::ImportMesh(R_Mesh, scene->mMeshes[i]);

			char* buffer = nullptr;
			uint size = 0;

			Importer::ImporterMesh::Save(R_Mesh, &buffer);

			M_mesh = R_Mesh->mesh;
			M_mesh->Name = FileName;
			AllMeshes.push_back(M_mesh);

			app->scene->AllResources.push_back(R_Mesh);

			ComponentMesh* C_Mesh = dynamic_cast<ComponentMesh*>(gameObject->AddComponent(ComponentType::MESH));
			C_Mesh->SetMesh(M_mesh);

			string pathToMeta;
			pathToMeta = MESHES_PATH;
			pathToMeta.append(std::to_string(C_Mesh->UUID));
			pathToMeta.append(".mesh");

			if (size > 0)
			{
				app->physFSManager->Save(pathToMeta.c_str(), buffer, size);
			}
			RELEASE_ARRAY(buffer);
		}
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
	ResourceMesh* R_Mesh = new ResourceMesh();

	R_Mesh->AssetsPath = Path;

	string ExistInMeta;
	ExistInMeta = MODELS_FOLDER;
	ExistInMeta.append(_ParentObj->mName);
	ExistInMeta.append(".meta");

	if(!app->physFSManager->Exists(ExistInMeta.c_str()))
	{
		if (i < scene->mNumMeshes)
		{
			Importer::ImporterMesh::ImportMesh(R_Mesh, scene->mMeshes[i]);

			char* buffer = nullptr;
			uint size = 0;
			size = Importer::ImporterMesh::Save(R_Mesh, &buffer);

			R_Mesh->name = _ParentObj->mName;

			M_mesh = R_Mesh->mesh;
			M_mesh->Name = _ParentObj->mName;
			AllMeshes.push_back(M_mesh);

			app->scene->AllResources.push_back(R_Mesh);

			//Components here
			ComponentTransform* transform = new ComponentTransform(nullptr);

			ComponentMesh* C_Mesh = dynamic_cast<ComponentMesh*>(_ParentObj->AddComponent(ComponentType::MESH));
			C_Mesh->SetMesh(M_mesh);

			string pathToMeta;
			pathToMeta = MESHES_PATH;
			pathToMeta.append(std::to_string(C_Mesh->UUID));
			pathToMeta.append(".mesh");

			if (size > 0)
			{
				app->physFSManager->Save(pathToMeta.c_str(), buffer, size);
			}
			RELEASE_ARRAY(buffer);

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
				Texture* texture = new Texture();
				Importer::ImporterTexture::Load(texture, texturePath);

				char* buffer = nullptr;
				uint size = 0;

				size = Importer::ImporterTexture::Save(texture, &buffer);

				TexturesManager* texturesManager = new TexturesManager();
				ComponentMaterial* C_Texture = dynamic_cast<ComponentMaterial*>(_ParentObj->AddComponent(ComponentType::MATERIAL));
				C_Texture->SetTexture(texture);

				string pathToMeta;
				pathToMeta = TEXTURES_PATH;
				pathToMeta.append(std::to_string(C_Texture->UUID));
				pathToMeta.append(".texture");

				if (size > 0)
				{
					app->physFSManager->Save(pathToMeta.c_str(), buffer, size);
				}

				RELEASE_ARRAY(buffer);
			}
		}
	}
	else
	{
		char* buffer = nullptr;
		uint size = app->physFSManager->Load(ExistInMeta.c_str(), &buffer);
		string path = MODELS_FOLDER + _ParentObj->mName;

		if (size > 0)
		{
			JsonManager js(buffer);

			map<uint32, ResourceElement*>::iterator iterator = app->resource->AllResourcesMap.find(js.getNumber("UID"));

			if(iterator != app->resource->AllResourcesMap.end())
			{
				if(app->physFSManager->GetLastModTime(path.c_str()) != js.getNumber("Last_Modification_Date"))
				{
					//Volver a crear GameObject	
				}
			}
			else
			{
				string name = js.getString("Name");
				uint32 UUID = js.getNumber("UID");
				uint32 Parent = js.getNumber("Parent");
				int type = js.getNumber("Type");

				ResourceElement* resource = new ResourceElement(name.c_str(), path.c_str(), (ResourceTypes)type, UUID);
				
				if((ResourceTypes)type == ResourceTypes::R_MODEL)
				{
					JSArray meshesArr = js.getArray("Meshes");

					for(int i = 0; i < meshesArr.GetSize(); i++)
					{
						JsonManager meshInModel = meshesArr.getNode(i);
						resource->MeshesChildrensInModel.push_back(meshInModel.getString("Path"));
					}

					JSArray compArr = js.getArray("Components");

					for (int i = 0; i < compArr.GetSize(); i++)
					{
						JsonManager compInModel = compArr.getNode(i);
						resource->ComponentsInModel.push_back(compInModel.getString("Library_path"));
					}
				}

				for(int j = 0; j < resource->ComponentsInModel.size(); j++)
				{
					ResourceElement* ResourceToGameobject = nullptr;
					ResourceToGameobject = app->resource->LoadResourceElement(resource->ComponentsInModel[j].c_str());
					
					if (ResourceToGameobject != nullptr)
					{
						if (ResourceToGameobject->type == ResourceTypes::R_MESH)
						{
							ResourceMesh* R_MeshToComponent = (ResourceMesh*)ResourceToGameobject;

							Importer::ImporterMesh::ImportMesh(R_MeshToComponent, scene->mMeshes[i]);

							Importer::ImporterMesh::Save(R_MeshToComponent, &buffer);

							R_MeshToComponent->name = _ParentObj->mName;

							AllMeshes.push_back(R_MeshToComponent->mesh);

							app->scene->AllResources.push_back(R_MeshToComponent);

							ComponentMesh* C_Mesh = dynamic_cast<ComponentMesh*>(_ParentObj->AddComponent(ComponentType::MESH));
							C_Mesh->SetMesh(R_MeshToComponent->mesh);
						}
					}
				}
			}
		}

		//Components here
		ComponentTransform* transform = new ComponentTransform(nullptr);

		RELEASE_ARRAY(buffer);

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
			Texture* texture = new Texture();
			Importer::ImporterTexture::Load(texture, texturePath);

			char* buffer = nullptr;

			Importer::ImporterTexture::Save(texture, &buffer);

			TexturesManager* texturesManager = new TexturesManager();
			ComponentMaterial* C_Texture = dynamic_cast<ComponentMaterial*>(_ParentObj->AddComponent(ComponentType::MATERIAL));
			C_Texture->SetTexture(texture);

			RELEASE_ARRAY(buffer);
		}
	}

	if (!app->physFSManager->Exists(ExistInMeta.c_str()))
	{
		GameObjectManager* gameObject;
		MetaFileCreator(_ParentObj);

		for (int f = 0; f < _ParentObj->childrens.size(); f++)
		{
			gameObject = _ParentObj->childrens[f];
			MetaFileCreator(gameObject);
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

void AssimpManager::SetBuffers(Mesh* M_mesh)
{
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
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * M_mesh->num_vertex * 3, M_mesh->normals, GL_STATIC_DRAW);

	glGenBuffers(1, (GLuint*)&(M_mesh->VT));
	glBindBuffer(GL_ARRAY_BUFFER, M_mesh->VT);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * M_mesh->num_Tex * 2, M_mesh->textures, GL_STATIC_DRAW);
}

void AssimpManager::MetaFileCreator(GameObjectManager* gameObject)
{
	//string nameToMeta;
	//string extensionToMeta;
	string pathToAssetsMeta;
	string pathToLibraryMeta;
	JsonManager json;

	char* buffer;
	uint size;

	pathToAssetsMeta = MODELS_FOLDER;
	pathToAssetsMeta.append(gameObject->mName.c_str());
	pathToAssetsMeta.append(".meta");
	
	pathToLibraryMeta = MODELS_PATH;
	pathToLibraryMeta.append(gameObject->mName.c_str());
	pathToLibraryMeta.append(".model");

	//app->physFSManager->SplitFilePath(MODELS_FOLDER, nullptr, &nameToMeta, &extensionToMeta);
	//gameObject->mName = nameToMeta;

	json.addString("Name", gameObject->mName.c_str());
	json.addNumber("Parent", gameObject->mParent->UUID);
	//json.addString("Extension", extensionToMeta.c_str());
	json.addNumber("UID", gameObject->UUID);
	json.addString("Library_Path", MODELS_PATH);
	json.addNumber("Type", 1);

	uint64 modDate = app->physFSManager->GetLastModTime(MODELS_FOLDER);
	json.addNumber("Last_Modification_Date", modDate);

	JSArray ChildrenstoMeta = json.addArray("Meshes");
	for (int i = 0; i < gameObject->childrens.size(); i++)
	{
		JsonManager& jsNode = ChildrenstoMeta.addNode();
		string name_UUID = "Library/Models/";
		string UUID = std::to_string(gameObject->childrens[i]->UUID);
		name_UUID.append(UUID);
		name_UUID.append(".model");
		jsNode.addString("Path", name_UUID.c_str());
	}

	JSArray ComponentstoMeta = json.addArray("Components");
	for(int i = 0; i < gameObject->mComponents.size(); i++)
	{
		JsonManager& jsNode = ComponentstoMeta.addNode();
		string name_UUID = "";

		if(gameObject->mComponents[i]->Type == ComponentType::MESH)
		{
			name_UUID = "Library/Meshes/";
			string UUID = std::to_string(gameObject->mComponents[i]->UUID);
			name_UUID.append(UUID);
			name_UUID.append(".mesh");
		}

		jsNode.addNumber("Type", gameObject->mComponents[i]->Type);
		jsNode.addNumber("UID", gameObject->mComponents[i]->UUID);
		jsNode.addNumber("Parent", gameObject->mComponents[i]->Owner->UUID);
		jsNode.addString("Library_path", name_UUID);
	}

	size = json.toSerialize(&buffer);

	if(size > 0)
	{
		app->physFSManager->Save(pathToAssetsMeta.c_str(), buffer, size);
		app->physFSManager->Save(pathToLibraryMeta.c_str(), buffer, size);
	}

	RELEASE_ARRAY(buffer);
}
