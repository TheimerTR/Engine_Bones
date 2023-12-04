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

		string dstFolder;
		dstFolder = MODELS_FOLDER;

		string newPath = "";

		string nPath = app->physFSManager->NormalizePath(path);
		string Div_String(nPath);
		FileSystem:: CompatibleString(Div_String);

		size_t pos_File = Div_String.find("Assets");

		if(pos_File > 1000)
		{
			if (!app->physFSManager->Exists(path))
			{
				app->physFSManager->DuplicateFile(path, dstFolder.c_str(), newPath);
			}
		}
		else
		{
			nPath = Div_String.substr(pos_File);

			if (!app->physFSManager->Exists(nPath.c_str()))
			{
				app->physFSManager->DuplicateFile(path, dstFolder.c_str(), newPath);
			}
		}

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

			size = Importer::ImporterMesh::Save(R_Mesh, &buffer);

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

	GameObjectManager* _ParentObj;

	std::string nameChildren = actualObj->mName.C_Str();
	int found = nameChildren.find_first_of("$");

	if (found == std::string::npos)
	{
		_ParentObj = new GameObjectManager(Name, _Parent);
	}
	else
	{
		_ParentObj = _Parent;
	}

	/*AplicateTransform(_ParentObj, pos, scale, rot);*/

	if (actualObj->mNumChildren > 0)
	{
		for (i = 0; i < actualObj->mNumChildren; i++)
		{
			GameObjectNodeTree(scene, actualObj->mChildren[i]->mNumChildren, i, actualObj->mChildren[i], _ParentObj, actualObj->mChildren[i]->mName.C_Str(), Path, texturePath);
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
			char* buffer = nullptr;
			uint size = 0;

			Importer::ImporterMesh::ImportMesh(R_Mesh, scene->mMeshes[i]);
			size = Importer::ImporterMesh::Save(R_Mesh, &buffer);

			R_Mesh->name = _ParentObj->mName;
			R_Mesh->ParentsUUID.push_back(_ParentObj->UUID);

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

			R_Mesh->LibraryPath = pathToMeta;

			if (size > 0)
			{
				app->physFSManager->Save(pathToMeta.c_str(), buffer, size);
			}

			Importer::ImporterMesh::Load(R_Mesh, buffer);
			Importer::ImporterMesh::ImportMesh(R_Mesh, scene->mMeshes[i]);

			app->resource->AllResourcesMap[R_Mesh->getUUID()] = R_Mesh;
			app->resource->AllResourcesMap[R_Mesh->getUUID()]->resourceCounter += 1;

			if (texturePath != NULL)
			{
				ResourceTexture* R_Texture = new ResourceTexture();

				buffer = nullptr;
				size = 0;

				R_Texture->texture->path = texturePath;
				R_Texture->name = texturePath;

				Importer::ImporterTexture::ImportTexture(R_Texture, buffer, size);
				Importer::ImporterTexture::Load(R_Texture->texture, texturePath);

				size = Importer::ImporterTexture::Save(&buffer);

				TexturesManager* texturesManager = new TexturesManager();
				ComponentMaterial* C_Texture = dynamic_cast<ComponentMaterial*>(_ParentObj->AddComponent(ComponentType::MATERIAL));
				C_Texture->SetTexture(R_Texture->texture);

				pathToMeta = TEXTURES_PATH;
				pathToMeta.append(std::to_string(C_Texture->UUID));
				pathToMeta.append(".texture");

				R_Texture->LibraryPath = pathToMeta;
				R_Texture->AssetsPath = texturePath;

				string textureName = "";

				FileSystem::StringDivide(texturePath, &textureName, nullptr);

				R_Texture->texture->Name = textureName;
				R_Texture->ParentsUUID.push_back(_ParentObj->UUID);

				if (size > 0)
				{
					app->physFSManager->Save(pathToMeta.c_str(), buffer, size);
				}

				if(!CheckNotDuplicateFromAssets(R_Texture, _ParentObj->UUID))
				{
					app->resource->AllResourcesMap[R_Texture->getUUID()] = R_Texture;
					app->resource->AllResourcesMap[R_Texture->getUUID()]->resourceCounter += 1;
				}
			}

			RELEASE_ARRAY(buffer);
		}
	}
	else
	{
		char* buffer = nullptr;
		uint size = app->physFSManager->Load(ExistInMeta.c_str(), &buffer);
		string path = "";

		if (size > 0)
		{
			JsonManager js(buffer);

			int type = js.getNumber("Type");
			uint32 Parent = js.getNumber("Parent");

			if ((ResourceTypes)type == ResourceTypes::R_MESH)
			{
				path = MODELS_FOLDER + _ParentObj->mName;
			}
			if ((ResourceTypes)type == ResourceTypes::R_TEXTURE)
			{
				path = TEXTURES_FOLDER + _ParentObj->mName;
			}

			map<uint32, ResourceElement*>::iterator iterator = app->resource->AllResourcesMap.find(js.getNumber("UID"));

			uint32 numberId = js.getNumber("UID");
			string name = js.getString("Name");

			ResourceElement* resource = new ResourceElement(name.c_str(), path.c_str(), (ResourceTypes)type, numberId);
			resource->resourceCounter += 1;

			JSArray meshesArr = js.getArray("Meshes");

			for (int i = 0; i < meshesArr.GetSize(); i++)
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

			_ParentObj->UUID = numberId;

			if(CheckResourceComponentsExistence(resource))
			{
				for (int j = 0; j < resource->ComponentsInModel.size(); j++)
				{
					ResourceElement* ResourceToGameobject = nullptr;
					ResourceToGameobject = app->resource->LoadResourceElement(resource->ComponentsInModel[j].c_str());

					if (ResourceToGameobject != nullptr)
					{
						ResourceToGameobject->UUID = numberId;
						ResourceToGameobject->name = name;

						if (ResourceToGameobject->type == ResourceTypes::R_MESH)
						{
							ResourceMesh* R_MeshToComponent = (ResourceMesh*)ResourceToGameobject;

							Importer::ImporterMesh::ImportMesh(R_MeshToComponent, scene->mMeshes[i]);

							R_MeshToComponent->name = _ParentObj->mName;
							R_MeshToComponent->mesh->Name = R_MeshToComponent->name;
							R_MeshToComponent->ParentsUUID.push_back(_ParentObj->UUID);

							ComponentMesh* C_Mesh = dynamic_cast<ComponentMesh*>(_ParentObj->AddComponent(ComponentType::MESH));
							//R_MeshToComponent->mesh->local_aabb = R_MeshToComponent->local_aabb;
							C_Mesh->SetMesh(R_MeshToComponent->mesh);
						}
						if (ResourceToGameobject->type == ResourceTypes::R_TEXTURE)
						{
							ResourceTexture* R_Texture = (ResourceTexture*)ResourceToGameobject;

							buffer = nullptr;
							size = 0;

							R_Texture->texture->path = texturePath;
							R_Texture->ParentsUUID.push_back(_ParentObj->UUID);

							Importer::ImporterTexture::ImportTexture(R_Texture, buffer, size);
							Importer::ImporterTexture::Load(R_Texture->texture, texturePath);

							TexturesManager* texturesManager = new TexturesManager();
							ComponentMaterial* C_Texture = dynamic_cast<ComponentMaterial*>(_ParentObj->AddComponent(ComponentType::MATERIAL));
							C_Texture->SetTexture(R_Texture->texture);

							RELEASE_ARRAY(buffer);
						}
					}
				}
			}
			else
			{
				if (iterator != app->resource->AllResourcesMap.end())
				{
					if (app->physFSManager->GetLastModTime(path.c_str()) != js.getNumber("Last_Modification_Date"))
					{
						//Volver a crear GameObject	
					}
				}
				else
				{
					for (int j = 0; j < resource->ComponentsInModel.size(); j++)
					{
						ResourceElement* ResourceToGameobject = nullptr;
						ResourceToGameobject = app->resource->LoadResourceElement(resource->ComponentsInModel[j].c_str());

						if (ResourceToGameobject != nullptr)
						{
							ResourceToGameobject->UUID = numberId;
							ResourceToGameobject->name = resource->name;

							if (ResourceToGameobject->type == ResourceTypes::R_MESH)
							{
								ResourceMesh* R_MeshToComponent = (ResourceMesh*)ResourceToGameobject;

								Importer::ImporterMesh::ImportMesh(R_MeshToComponent, scene->mMeshes[i]);

								R_MeshToComponent->name = _ParentObj->mName;
								R_MeshToComponent->LibraryPath = resource->ComponentsInModel[j];
								R_MeshToComponent->mesh->Name = R_MeshToComponent->name;
								R_MeshToComponent->resourceCounter += 1;
								R_MeshToComponent->ParentsUUID.push_back(_ParentObj->UUID);

								AllMeshes.push_back(R_MeshToComponent->mesh);

								app->scene->AllResources.push_back(ResourceToGameobject);

								ComponentMesh* C_Mesh = dynamic_cast<ComponentMesh*>(_ParentObj->AddComponent(ComponentType::MESH));
								C_Mesh->SetMesh(R_MeshToComponent->mesh);

								app->resource->AllResourcesMap[R_MeshToComponent->getUUID()] = (ResourceTexture*)R_MeshToComponent;
							}

							if (ResourceToGameobject->type == ResourceTypes::R_TEXTURE)
							{
								ResourceTexture* R_Texture = (ResourceTexture*)ResourceToGameobject;

								buffer = nullptr;
								size = 0;

								R_Texture->texture->path = texturePath;
								R_Texture->LibraryPath = resource->ComponentsInModel[j];
								R_Texture->resourceCounter += 1;

								Importer::ImporterTexture::ImportTexture(R_Texture, buffer, size);
								Importer::ImporterTexture::Load(R_Texture->texture, texturePath);

								app->scene->AllResources.push_back(ResourceToGameobject);

								TexturesManager* texturesManager = new TexturesManager();
								ComponentMaterial* C_Texture = dynamic_cast<ComponentMaterial*>(_ParentObj->AddComponent(ComponentType::MATERIAL));
								C_Texture->SetTexture(R_Texture->texture);

								string textureName = "";

								FileSystem::StringDivide(texturePath, &textureName, nullptr);

								R_Texture->name = textureName;
								R_Texture->texture->Name = textureName;
								R_Texture->AssetsPath = texturePath;

								//ResourceToGameobject->name = textureName;

								if (!CheckNotDuplicateFromAssets(R_Texture, _ParentObj->UUID))
								{
									app->resource->AllResourcesMap[R_Texture->UUID] = (ResourceTexture*)R_Texture;
									R_Texture->ParentsUUID.push_back(_ParentObj->UUID);
								}

								RELEASE_ARRAY(buffer);
							}
						}
					}

					resource->AssociatedGameObjects.push_back(_ParentObj);
				}
			}
		}
	}

	M_mesh->Name = _ParentObj->mName;

	//Components here
	ComponentTransform* transform = new ComponentTransform(nullptr);

	AplicateTransform(_ParentObj, pos, scale, rot);

	if (!app->physFSManager->Exists(ExistInMeta.c_str()))
	{
		/*GameObjectManager* gameObject;
		MetaFileCreator(_ParentObj, Path);*/

		if(_ParentObj->childrens.empty())
		{
			//gameObject = _ParentObj->childrens[f];
			MetaFileCreator(_ParentObj, Path);
		}
	}

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

	glGenBuffers(1, &(GLuint)(M_mesh->VBO));
	glBindBuffer(GL_ARRAY_BUFFER, M_mesh->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * M_mesh->num_vertex * 3, M_mesh->vertex, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &(GLuint)(M_mesh->EBO));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, M_mesh->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * M_mesh->num_index, M_mesh->index, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &(GLuint)(M_mesh->VN));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, M_mesh->VN);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * M_mesh->num_index * 3, M_mesh->normals, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &(GLuint)(M_mesh->VT));
	glBindBuffer(GL_ARRAY_BUFFER, M_mesh->VT);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * M_mesh->num_Tex * 2, M_mesh->textures, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void AssimpManager::MetaFileCreator(GameObjectManager* gameObject, const char* path)
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
	json.addString("Assets_Path", path);
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

		if (gameObject->mComponents[i]->Type == ComponentType::MATERIAL)
		{
			name_UUID = "Library/Textures/";
			string UUID = std::to_string(gameObject->mComponents[i]->UUID);
			name_UUID.append(UUID);
			name_UUID.append(".texture");
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

bool AssimpManager::CheckResourceComponentsExistence(ResourceElement* R_Element)
{
	bool Exist = false;

	map<uint32, ResourceElement*>::iterator iteratorExistence = app->resource->AllResourcesMap.begin();
	for (iteratorExistence; iteratorExistence != app->resource->AllResourcesMap.end(); iteratorExistence++)
	{
		for (int h = 0; h < R_Element->ComponentsInModel.size(); h++)
		{
			if (R_Element->ComponentsInModel[h] == iteratorExistence->second->LibraryPath)
			{
				Exist = true;
				iteratorExistence->second->resourceCounter += 1;
			}
		}
	}

	return Exist;
}

bool AssimpManager::CheckStringComponentsExistence(string Component)
{
	bool Exist = false;

	map<uint32, ResourceElement*>::iterator iteratorExistence = app->resource->AllResourcesMap.begin();
	for (iteratorExistence; iteratorExistence != app->resource->AllResourcesMap.end(); iteratorExistence++)
	{
		if (Component == iteratorExistence->second->AssetsPath)
		{
			Exist = true;
			iteratorExistence->second->resourceCounter += 1;
		}
	}

	return Exist;
}

bool AssimpManager::CheckNotDuplicateFromAssets(ResourceElement* R_Element, uint32 uuid)
{
	bool Exist = false;

	map<uint32, ResourceElement*>::iterator iteratorExistence = app->resource->AllResourcesMap.begin();
	for (iteratorExistence; iteratorExistence != app->resource->AllResourcesMap.end(); iteratorExistence++)
	{
		if (R_Element->AssetsPath == iteratorExistence->second->AssetsPath)
		{
			Exist = true;

			if (uuid != -1)
			{
				iteratorExistence->second->ParentsUUID.push_back(uuid);
			}

			iteratorExistence->second->resourceCounter += 1;
		}
	}

	return Exist;
}

bool AssimpManager::ImportOnlyTexture(string Path)
{
	bool Exist = true;

	ResourceTexture* R_Texture = new ResourceTexture();

	char* buffer = nullptr;
	uint size = 0;

	string T_name = "";
	FileSystem::StringDivide(Path.c_str(), &T_name, nullptr);

	R_Texture->texture->path = Path.c_str();
	R_Texture->texture->Name = T_name;
	R_Texture->ParentsUUID.push_back(app->editor->actualMesh->UUID);

	Importer::ImporterTexture::ImportTexture(R_Texture, buffer, size);
	Importer::ImporterTexture::Load(R_Texture->texture, Path.c_str());

	vector<ComponentManager*> objectMeshes = app->editor->actualMesh->GetComponentsGameObject(ComponentType::MATERIAL);

	for (int j = 0; j < objectMeshes.size(); j++)
	{
		ComponentMaterial* C_Texture;
		C_Texture = dynamic_cast<ComponentMaterial*>(objectMeshes[j]);
		C_Texture->texture->ShowTextures = false;
	}

	TexturesManager* texturesManager = new TexturesManager();
	ComponentMaterial* C_Texture = dynamic_cast<ComponentMaterial*>(app->editor->actualMesh->AddComponent(ComponentType::MATERIAL));
	C_Texture->SetTexture(R_Texture->texture);

	if (!AssimpManager::CheckNotDuplicateFromAssets(R_Texture, app->editor->actualMesh->UUID))
	{
		app->resource->AllResourcesMap[R_Texture->getUUID()] = R_Texture;
		app->resource->AllResourcesMap[R_Texture->getUUID()]->resourceCounter += 1;
	}

	RELEASE_ARRAY(buffer);

	return Exist;
}

void AssimpManager::MetaExistenceLoader(const char* Path)
{
	char* buffer = nullptr;
	uint size = app->physFSManager->Load(Path, &buffer);
	string path = Path;

	if (size > 0)
	{
		JsonManager js(buffer);

		int type = js.getNumber("Type");
		string pathToAssets = js.getString("Assets_Path");
		uint32 Parent = js.getNumber("Parent");
		uint32 numberId = js.getNumber("UID");
		string name = js.getString("Name");

		const aiScene* scene = aiImportFile(pathToAssets.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);

		string FileName = "";

		FileSystem::StringDivide(pathToAssets.c_str(), &FileName, nullptr);

		GameObjectManager* gm = new GameObjectManager(FileName, app->scene->Root);

		aiVector3D translation, scaling;
		aiQuaternion rotation;

		scene->mRootNode->mTransformation.Decompose(scaling, rotation, translation);
		float3 pos(translation.x, translation.y, translation.z);
		float3 scale(scaling.x, scaling.y, scaling.z);
		Quat rot(rotation.x, rotation.y, rotation.z, rotation.w);

		gm->UUID = numberId;

		//map<uint32, ResourceElement*>::iterator iterator = app->resource->AllResourcesMap.find(js.getNumber("UID"));

		//ResourceElement* resource = iterator->second;
		ResourceElement* resource = new ResourceElement(FileName.c_str(), pathToAssets.c_str(), ResourceTypes::R_MODEL, numberId);
		resource->resourceCounter += 1;

		JSArray meshesArr = js.getArray("Meshes");

		for (int i = 0; i < meshesArr.GetSize(); i++)
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

		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			for (int h = 0; h < resource->ComponentsInModel.size(); h++)
			{
				if (CheckStringComponentsExistence(resource->ComponentsInModel[h]))
				{
					for (int j = 0; j < resource->ComponentsInModel.size(); j++)
					{
						ResourceElement* ResourceToGameobject = nullptr;
						ResourceToGameobject = app->resource->LoadResourceElement(resource->ComponentsInModel[j].c_str());

						if (ResourceToGameobject != nullptr)
						{
							ResourceToGameobject->UUID = numberId;
							ResourceToGameobject->name = name;

							if (ResourceToGameobject->type == ResourceTypes::R_MESH)
							{
								ResourceMesh* R_MeshToComponent = (ResourceMesh*)ResourceToGameobject;

								Importer::ImporterMesh::ImportMesh(R_MeshToComponent, scene->mMeshes[i]);

								R_MeshToComponent->name = gm->mName;
								R_MeshToComponent->mesh->Name = R_MeshToComponent->name;
								R_MeshToComponent->ParentsUUID.push_back(gm->UUID);

								ComponentMesh* C_Mesh = dynamic_cast<ComponentMesh*>(gm->AddComponent(ComponentType::MESH));
								//R_MeshToComponent->mesh->local_aabb = R_MeshToComponent->local_aabb;
								C_Mesh->SetMesh(R_MeshToComponent->mesh);
							}
							/*if (ResourceToGameobject->type == ResourceTypes::R_TEXTURE)
							{
								ResourceTexture* R_Texture = (ResourceTexture*)ResourceToGameobject;

								buffer = nullptr;
								size = 0;

								R_Texture->texture->path = texturePath;
								R_Texture->ParentsUUID.push_back(_ParentObj->UUID);

								Importer::ImporterTexture::ImportTexture(R_Texture, buffer, size);
								Importer::ImporterTexture::Load(R_Texture->texture, texturePath);

								TexturesManager* texturesManager = new TexturesManager();
								ComponentMaterial* C_Texture = dynamic_cast<ComponentMaterial*>(_ParentObj->AddComponent(ComponentType::MATERIAL));
								C_Texture->SetTexture(R_Texture->texture);

								RELEASE_ARRAY(buffer);
							}*/
						}
					}
				}
				else
				{
					for (int j = 0; j < resource->ComponentsInModel.size(); j++)
					{
						ResourceElement* ResourceToGameobject = nullptr;
						ResourceToGameobject = app->resource->LoadResourceElement(resource->ComponentsInModel[j].c_str());

						if (ResourceToGameobject != nullptr)
						{
							ResourceToGameobject->UUID = numberId;
							ResourceToGameobject->name = resource->name;

							if (ResourceToGameobject->type == ResourceTypes::R_MESH)
							{
								ResourceMesh* R_MeshToComponent = (ResourceMesh*)ResourceToGameobject;

								Importer::ImporterMesh::ImportMesh(R_MeshToComponent, scene->mMeshes[i]);

								R_MeshToComponent->name = gm->mName;
								R_MeshToComponent->LibraryPath = resource->ComponentsInModel[j];
								R_MeshToComponent->mesh->Name = R_MeshToComponent->name;
								R_MeshToComponent->resourceCounter += 1;
								R_MeshToComponent->ParentsUUID.push_back(gm->UUID);

								AllMeshes.push_back(R_MeshToComponent->mesh);

								app->scene->AllResources.push_back(ResourceToGameobject);

								ComponentMesh* C_Mesh = dynamic_cast<ComponentMesh*>(gm->AddComponent(ComponentType::MESH));
								C_Mesh->SetMesh(R_MeshToComponent->mesh);

								app->resource->AllResourcesMap[R_MeshToComponent->getUUID()] = (ResourceTexture*)R_MeshToComponent;
							}

							//if (ResourceToGameobject->type == ResourceTypes::R_TEXTURE)
							//{
							//	ResourceTexture* R_Texture = (ResourceTexture*)ResourceToGameobject;

							//	buffer = nullptr;
							//	size = 0;

							//	R_Texture->texture->path = texturePath;
							//	R_Texture->LibraryPath = resource->ComponentsInModel[j];
							//	R_Texture->resourceCounter += 1;

							//	Importer::ImporterTexture::ImportTexture(R_Texture, buffer, size);
							//	Importer::ImporterTexture::Load(R_Texture->texture, texturePath);

							//	app->scene->AllResources.push_back(ResourceToGameobject);

							//	TexturesManager* texturesManager = new TexturesManager();
							//	ComponentMaterial* C_Texture = dynamic_cast<ComponentMaterial*>(_ParentObj->AddComponent(ComponentType::MATERIAL));
							//	C_Texture->SetTexture(R_Texture->texture);

							//	string textureName = "";

							//	FileSystem::StringDivide(texturePath, &textureName, nullptr);

							//	R_Texture->name = textureName;
							//	R_Texture->texture->Name = textureName;
							//	R_Texture->AssetsPath = texturePath;

							//	//ResourceToGameobject->name = textureName;

							//	if (!CheckNotDuplicateFromAssets(R_Texture, _ParentObj->UUID))
							//	{
							//		app->resource->AllResourcesMap[R_Texture->UUID] = (ResourceTexture*)R_Texture;
							//		R_Texture->ParentsUUID.push_back(_ParentObj->UUID);
							//	}

							//	RELEASE_ARRAY(buffer);
							//}
						}
					}

						resource->AssociatedGameObjects.push_back(gm);
					}
				}
			}

		Mesh* M_mesh = new Mesh();

		M_mesh->Name = gm->mName;

		//Components here
		ComponentTransform* transform = new ComponentTransform(nullptr);

		AplicateTransform(gm, pos, scale, rot);

		int C = 0;
		for (int m = 0; m < app->scene->AllGameObjectManagers.size(); m++)
		{
			if (C > 0)
			{
				std::string s = std::to_string(C);
				gm->mName = M_mesh->Name + s;
				int compare = strcmp(app->scene->AllGameObjectManagers.at(m)->mName.c_str(), gm->mName.c_str());

				if (compare == 0)
				{
					std::string stri = std::to_string(C++);
					gm->mName = M_mesh->Name + s;
				}
			}

			if (strcmp(app->scene->AllGameObjectManagers.at(m)->mName.c_str(), gm->mName.c_str()) == 0)
			{
				if (C == 0)
				{
					gm->mName = M_mesh->Name;
					C++;
				}
			}
		}
	}
}
