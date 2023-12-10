#include "ResourceManager.h"
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
#include "GameObject.h"
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

void ResourceManager::ResourceLoader(const char* path, const char* pathTexture)
{
	if (!app->scene->GameTime.running)
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
			FileSystem::CompatibleString(Div_String);

			size_t pos_File = Div_String.find("Assets");

			if (pos_File != string::npos)
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

			m_Counter = 0;
			GameObjectNodeTree(scene, scene->mNumMeshes, 0, scene->mRootNode, app->scene->Root, FileName.c_str(), path, pathTexture);

			aiReleaseImport(scene);
		}
		else
			LOG(LogTypeCase::L_ERROR, "Error loading scene % s", path);
	}
	else
	{
		LOG(LogTypeCase::L_WARNING, "You are in PLAY mode!");
	}
}

void ResourceManager::SimpleResourceLoader(const char* Path, GameObject* gameObject, const char* texturePath)
{
	const aiScene* scene = aiImportFile(Path, aiProcessPreset_TargetRealtime_MaxQuality);

	m_Counter = 0;

	if (scene != nullptr && scene->HasMeshes())
	{
		Mesh* M_mesh = new Mesh();
		ResourceMesh* R_Mesh = new ResourceMesh();

		R_Mesh->AssetsPath = Path;

		string ModelName = "";

		FileSystem::StringDivide(Path, &ModelName, nullptr);

		string ExistInMeta;
		ExistInMeta = MODELS_FOLDER;
		ExistInMeta.append(ModelName);
		ExistInMeta.append(".meta");

		if (!app->physFSManager->Exists(ExistInMeta.c_str()))
		{
			char* buffer = nullptr;
			uint size = 0;

			Importer::ImporterMesh::ImportMesh(R_Mesh, scene->mMeshes[m_Counter]);
			size = Importer::ImporterMesh::Save(R_Mesh, &buffer);

			app->physFSManager->Save(ExistInMeta.c_str(), buffer, size);

			R_Mesh->name = ModelName;
			R_Mesh->mesh->Name = ModelName;
			R_Mesh->ParentsUUID.push_back(app->editor->actualMesh->UUID);

			M_mesh = R_Mesh->mesh;
			M_mesh->Name = ModelName;
			AllMeshes.push_back(M_mesh);

			app->scene->AllResources.push_back(R_Mesh);

			ComponentMesh* C_Mesh = dynamic_cast<ComponentMesh*>(app->editor->actualMesh->AddComponent(ComponentType::MESH));

			string pathToMeta;
			pathToMeta = MESHES_PATH;
			pathToMeta.append(std::to_string(C_Mesh->UUID));
			pathToMeta.append(".mesh");

			R_Mesh->LibraryPath = pathToMeta;

			if (size > 0)
			{
				app->physFSManager->Save(pathToMeta.c_str(), buffer, size);
			}

			C_Mesh->SetMesh(R_Mesh->mesh);

			app->resource->AllResourcesMap[R_Mesh->getUUID()] = R_Mesh;
			app->resource->AllResourcesMap[R_Mesh->getUUID()]->resourceCounter += 1;

			m_Counter++;

			RELEASE_ARRAY(buffer);
		}
		else
		{
			char* buffer = nullptr;
			uint size = app->physFSManager->Load(ExistInMeta.c_str(), &buffer);
			string path = "";

			if (size > 0)
			{
				ResourceElement* resource = new ResourceElement(ResourceTypes::R_MESH);

				string pathToMeta;
				pathToMeta = MESHES_PATH;
				pathToMeta.append(std::to_string(app->editor->actualMesh->UUID));
				pathToMeta.append(".mesh");

				resource->LibraryPath = pathToMeta;

				if (CheckResourceComponentsExistence(resource))
				{
					ResourceElement* ResourceToGameobject = nullptr;
					ResourceToGameobject = app->resource->LoadResourceElement(resource->LibraryPath.c_str());

					if (ResourceToGameobject != nullptr)
					{
						ResourceToGameobject->UUID = app->editor->actualMesh->UUID;
						ResourceToGameobject->name = ModelName;

						if (ResourceToGameobject->type == ResourceTypes::R_MESH)
						{
							ResourceMesh* R_MeshToComponent = (ResourceMesh*)ResourceToGameobject;

							Importer::ImporterMesh::ImportMesh(R_MeshToComponent, scene->mMeshes[m_Counter]);
							m_Counter++;

							R_MeshToComponent->name = ModelName;
							R_MeshToComponent->mesh->Name = R_MeshToComponent->name;
							R_MeshToComponent->ParentsUUID.push_back(app->editor->actualMesh->UUID);

							ComponentMesh* C_Mesh = dynamic_cast<ComponentMesh*>(app->editor->actualMesh->AddComponent(ComponentType::MESH));
							//R_MeshToComponent->mesh->local_aabb = R_MeshToComponent->local_aabb;
							C_Mesh->SetMesh(R_MeshToComponent->mesh);
						}

						RELEASE_ARRAY(buffer);
					}
				}
				else
				{
					ResourceElement* ResourceToGameobject = nullptr;
					ResourceToGameobject = app->resource->LoadResourceElement(resource->LibraryPath.c_str());

					if (ResourceToGameobject != nullptr)
					{
						ResourceToGameobject->UUID = app->editor->actualMesh->UUID;
						ResourceToGameobject->name = ModelName;

						if (ResourceToGameobject->type == ResourceTypes::R_MESH)
						{
							ResourceMesh* R_MeshToComponent = (ResourceMesh*)ResourceToGameobject;

							Importer::ImporterMesh::ImportMesh(R_MeshToComponent, scene->mMeshes[m_Counter]);
							m_Counter++;

							R_MeshToComponent->name = ModelName;
							R_MeshToComponent->LibraryPath = resource->LibraryPath;
							R_MeshToComponent->mesh->Name = R_MeshToComponent->name;
							R_MeshToComponent->resourceCounter += 1;
							R_MeshToComponent->ParentsUUID.push_back(app->editor->actualMesh->UUID);

							AllMeshes.push_back(R_MeshToComponent->mesh);

							app->scene->AllResources.push_back(ResourceToGameobject);

							ComponentMesh* C_Mesh = dynamic_cast<ComponentMesh*>(app->editor->actualMesh->AddComponent(ComponentType::MESH));
							C_Mesh->SetMesh(R_MeshToComponent->mesh);

							app->resource->AllResourcesMap[R_MeshToComponent->getUUID()] = (ResourceTexture*)R_MeshToComponent;
						}
						
						RELEASE_ARRAY(buffer);
					}
				}
			}
		}
	}
	else
		LOG(LogTypeCase::L_ERROR, "Error loading scene % s", Path);
}

void ResourceManager::GameObjectNodeTree(const aiScene* scene, int numMeshes, int pointer, /*aiMesh** M_Array*/ aiNode* actualObj, GameObject* _Parent, string Name, const char* Path, const char* texturePath)
{
	int i = pointer;

	aiVector3D translation, scaling;
	aiQuaternion rotation;
	
	actualObj->mTransformation.Decompose(scaling, rotation, translation);
	float3 pos(translation.x, translation.y, translation.z);
	float3 scale(scaling.x, scaling.y, scaling.z);
	Quat rot(rotation.x, rotation.y, rotation.z, rotation.w);

	GameObject* _ParentObj;

	std::string nameChildren = actualObj->mName.C_Str();
	int found = nameChildren.find_first_of("$");

	if (found == std::string::npos)
	{
		_ParentObj = new GameObject(Name, _Parent);
	}
	else
	{
		_ParentObj = _Parent;
	}

	ComponentTransform* transform = new ComponentTransform(nullptr);

	AplicateTransform(_ParentObj, pos, scale, rot);

	/*AplicateTransform(_ParentObj, pos, scale, rot);*/

	if (actualObj->mNumChildren > 0)
	{
		for (i = 0; i < actualObj->mNumChildren; i++)
		{
			GameObjectNodeTree(scene, actualObj->mChildren[i]->mNumChildren, i, actualObj->mChildren[i], _ParentObj, actualObj->mChildren[i]->mName.C_Str(), Path, texturePath);
		}
	}

	string ExistInMeta;
	ExistInMeta = MODELS_FOLDER;
	ExistInMeta.append(_ParentObj->mName);
	ExistInMeta.append(".meta");

	if (actualObj->mNumMeshes > 0)
	{
		Mesh* M_mesh = new Mesh();
		ResourceMesh* R_Mesh = new ResourceMesh();

		R_Mesh->AssetsPath = Path;

		if (!app->physFSManager->Exists(ExistInMeta.c_str()))
		{
			if (i < scene->mNumMeshes)
			{
				char* buffer = nullptr;
				uint size = 0;

				Importer::ImporterMesh::ImportMesh(R_Mesh, scene->mMeshes[m_Counter]);
				size = Importer::ImporterMesh::Save(R_Mesh, &buffer);

				R_Mesh->name = _ParentObj->mName;
				R_Mesh->mesh->Name = _ParentObj->mName;
				R_Mesh->ParentsUUID.push_back(_ParentObj->UUID);

				M_mesh = R_Mesh->mesh;
				M_mesh->Name = _ParentObj->mName;
				AllMeshes.push_back(M_mesh);

				app->scene->AllResources.push_back(R_Mesh);

				ComponentMesh* C_Mesh = dynamic_cast<ComponentMesh*>(_ParentObj->AddComponent(ComponentType::MESH));

				string pathToMeta;
				pathToMeta = MESHES_PATH;
				pathToMeta.append(std::to_string(C_Mesh->UUID));
				pathToMeta.append(".mesh");

				R_Mesh->LibraryPath = pathToMeta;

				if (size > 0)
				{
					app->physFSManager->Save(pathToMeta.c_str(), buffer, size);
				}

				C_Mesh->SetMesh(R_Mesh->mesh);

				app->resource->AllResourcesMap[R_Mesh->getUUID()] = R_Mesh;
				app->resource->AllResourcesMap[R_Mesh->getUUID()]->resourceCounter += 1;

				ResourceTexture* R_Texture = new ResourceTexture();

				buffer = nullptr;
				size = 0;

				if (texturePath != NULL)
				{
					R_Texture->texture->path = texturePath;
					R_Texture->name = texturePath;

					R_Texture->AssetsPath = texturePath;

					string textureName = "";

					FileSystem::StringDivide(texturePath, &textureName, nullptr);

					R_Texture->texture->Name = textureName;
				}

				aiMaterial* mat = nullptr;

				R_Mesh->mesh->num_Materials = scene->mNumMaterials;
				mat = scene->mMaterials[scene->mMeshes[m_Counter]->mMaterialIndex];

				if (mat != nullptr)
				{
					size = Importer::ImporterTexture::ImportTexture(mat, R_Texture, &buffer, true);
				}
				else
				{
					R_Texture->SetColor(Color(0, 0, 0, 0));
				}

				if (R_Texture->texture->path != "")
				{
					TexturesManager* texturesManager = new TexturesManager();
					ComponentMaterial* C_Texture = dynamic_cast<ComponentMaterial*>(_ParentObj->AddComponent(ComponentType::MATERIAL));
					C_Texture->SetTexture(R_Texture->texture);
					C_Texture->colorTexture = R_Texture->colors;

					pathToMeta = TEXTURES_PATH;
					pathToMeta.append(std::to_string(C_Texture->UUID));
					pathToMeta.append(".dds");

					R_Texture->LibraryPath = pathToMeta;

					R_Texture->ParentsUUID.push_back(_ParentObj->UUID);

					if (size > 0)
					{
						app->physFSManager->Save(pathToMeta.c_str(), buffer, size);
					}

					if (R_Texture->name != "Material")
					{
						if (!CheckNotDuplicateFromAssets(R_Texture, _ParentObj->UUID))
						{
							app->resource->AllResourcesMap[R_Texture->getUUID()] = R_Texture;
							app->resource->AllResourcesMap[R_Texture->getUUID()]->resourceCounter += 1;
						}
					}
				}

				m_Counter++;

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
					resource->ComponentsNames.push_back(compInModel.getString("Component_Name"));

					string resourceExtension = "";

					FileSystem::StringDivide(resource->ComponentsInModel[i].c_str(), nullptr, &resourceExtension);

					if (strcmp(resourceExtension.data(), "dds") == 0)
					{
						resource->Resource_red.push_back(compInModel.getNumber("Color_Red"));
						resource->Resource_green.push_back(compInModel.getNumber("Color_Green"));
						resource->Resource_blue.push_back(compInModel.getNumber("Color_Blue"));
						resource->Resource_alpha.push_back(compInModel.getNumber("Color_Alpha"));
					}

					if((resource->ComponentsNames[i] != "Material") && (resource->ComponentsInModel[i] != "") && (!app->physFSManager->Exists(resource->ComponentsInModel[i].c_str())))
					{
						app->physFSManager->Remove(ExistInMeta.c_str());
						
						if (i < scene->mNumMeshes)
						{
							if (m_Counter < scene->mNumMaterials)
							{
								char* buffer = nullptr;
								uint size = 0;

								Importer::ImporterMesh::ImportMesh(R_Mesh, scene->mMeshes[m_Counter]);
								size = Importer::ImporterMesh::Save(R_Mesh, &buffer);

								R_Mesh->name = _ParentObj->mName;
								R_Mesh->mesh->Name = _ParentObj->mName;
								R_Mesh->ParentsUUID.push_back(_ParentObj->UUID);

								M_mesh = R_Mesh->mesh;
								M_mesh->Name = _ParentObj->mName;
								AllMeshes.push_back(M_mesh);

								app->scene->AllResources.push_back(R_Mesh);

								ComponentMesh* C_Mesh = dynamic_cast<ComponentMesh*>(_ParentObj->AddComponent(ComponentType::MESH));

								string pathToMeta;
								pathToMeta = MESHES_PATH;
								pathToMeta.append(std::to_string(C_Mesh->UUID));
								pathToMeta.append(".mesh");

								R_Mesh->LibraryPath = pathToMeta;

								if (size > 0)
								{
									app->physFSManager->Save(pathToMeta.c_str(), buffer, size);
								}

								C_Mesh->SetMesh(R_Mesh->mesh);

								app->resource->AllResourcesMap[R_Mesh->getUUID()] = R_Mesh;
								app->resource->AllResourcesMap[R_Mesh->getUUID()]->resourceCounter += 1;

								ResourceTexture* R_Texture = new ResourceTexture();

								buffer = nullptr;
								size = 0;

								if (texturePath != NULL)
								{
									R_Texture->texture->path = texturePath;
									R_Texture->name = texturePath;

									R_Texture->AssetsPath = texturePath;

									string textureName = "";

									FileSystem::StringDivide(texturePath, &textureName, nullptr);

									R_Texture->texture->Name = textureName;
								}

								R_Mesh->mesh->num_Materials = scene->mNumMaterials;
								aiMaterial* mat = scene->mMaterials[scene->mMeshes[m_Counter]->mMaterialIndex];

								if (mat != nullptr)
								{
									size = Importer::ImporterTexture::ImportTexture(mat, R_Texture, &buffer);
								}
								else
								{
									R_Texture->SetColor(Color(1, 1, 1, 1));
								}

								if (R_Texture->texture->path != "")
								{
									TexturesManager* texturesManager = new TexturesManager();
									ComponentMaterial* C_Texture = dynamic_cast<ComponentMaterial*>(_ParentObj->AddComponent(ComponentType::MATERIAL));
									C_Texture->SetTexture(R_Texture->texture);
									C_Texture->colorTexture = R_Texture->colors;

									pathToMeta = TEXTURES_PATH;
									pathToMeta.append(std::to_string(C_Texture->UUID));
									pathToMeta.append(".dds");

									R_Texture->LibraryPath = pathToMeta;

									R_Texture->ParentsUUID.push_back(_ParentObj->UUID);

									if (size > 0)
									{
										app->physFSManager->Save(pathToMeta.c_str(), buffer, size);
									}

									if (!CheckNotDuplicateFromAssets(R_Texture, _ParentObj->UUID))
									{
										app->resource->AllResourcesMap[R_Texture->getUUID()] = R_Texture;
										app->resource->AllResourcesMap[R_Texture->getUUID()]->resourceCounter += 1;
									}
								}

								m_Counter++;

								RELEASE_ARRAY(buffer);
							}
						}
					}
				}

				_ParentObj->UUID = numberId;

				if (CheckResourceComponentsExistence(resource))
				{
					for (int j = 0; j < resource->ComponentsInModel.size(); j++)
					{
						ResourceElement* ResourceToGameobject = nullptr;
						ResourceToGameobject = app->resource->LoadResourceElement(resource->ComponentsInModel[j].c_str());

						if (strcmp(resource->ComponentsNames[j].data(), "Material") == 0)
						{
							string Name = "";
							FileSystem::StringDivide(resource->ComponentsInModel[j].c_str(), &Name, nullptr);

							ResourceToGameobject = new ResourceTexture();
							ResourceToGameobject->name = Name;
							ResourceToGameobject->type = ResourceTypes::R_TEXTURE;
						}

						if (ResourceToGameobject != nullptr)
						{
							ResourceToGameobject->UUID = numberId;
							ResourceToGameobject->name = name;

							if (ResourceToGameobject->type == ResourceTypes::R_MESH)
							{
								ResourceMesh* R_MeshToComponent = (ResourceMesh*)ResourceToGameobject;

								Importer::ImporterMesh::ImportMesh(R_MeshToComponent, scene->mMeshes[m_Counter]);
								m_Counter++;

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

								R_Texture->texture->path = R_Texture->LibraryPath.c_str();

								R_Texture->name = ResourceToGameobject->name;
								R_Texture->texture->Name = ResourceToGameobject->name;
								R_Texture->colors.r = resource->Resource_red[0];
								R_Texture->colors.g = resource->Resource_green[0];
								R_Texture->colors.b = resource->Resource_blue[0];
								R_Texture->colors.a = resource->Resource_alpha[0];

								R_Texture->LibraryPath = resource->ComponentsInModel[j];
								R_Texture->resourceCounter += 1;


								app->scene->AllResources.push_back(ResourceToGameobject);

								TexturesManager* texturesManager = new TexturesManager();
								ComponentMaterial* C_Texture = dynamic_cast<ComponentMaterial*>(_ParentObj->AddComponent(ComponentType::MATERIAL));
								C_Texture->SetTexture(R_Texture->texture);
								C_Texture->colorTexture = R_Texture->colors;

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

							if (strcmp(resource->ComponentsNames[j].data(), "Material") == 0)
							{
								string Name = "";
								FileSystem::StringDivide(resource->ComponentsInModel[j].c_str(), &Name, nullptr);

								ResourceToGameobject = new ResourceTexture();
								ResourceToGameobject->name = Name;
								ResourceToGameobject->type = ResourceTypes::R_TEXTURE;
							}

							if (ResourceToGameobject != nullptr)
							{
								//ResourceToGameobject->UUID = numberId;
								ResourceToGameobject->name = resource->ComponentsNames[j];

								if (ResourceToGameobject->type == ResourceTypes::R_MESH)
								{
									ResourceMesh* R_MeshToComponent = (ResourceMesh*)ResourceToGameobject;

									Importer::ImporterMesh::ImportMesh(R_MeshToComponent, scene->mMeshes[m_Counter]);
									m_Counter++;

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
									R_Texture->UUID = ResourceToGameobject->UUID;
									R_Texture->LibraryPath = resource->ComponentsInModel[j];

									buffer = nullptr;
									size = 0;

									if (texturePath != NULL)
									{
										size = app->physFSManager->Load(texturePath, &buffer);

										R_Texture->texture->path = texturePath;
										Importer::ImporterTexture::Load(R_Texture->texture, texturePath, size);

										string textureName = "";

										FileSystem::StringDivide(texturePath, &textureName, nullptr);

										R_Texture->name = textureName;
										R_Texture->texture->Name = textureName;
										R_Texture->AssetsPath = texturePath;
									}
									else
									{
										//size = app->physFSManager->Load(R_Texture->LibraryPath.c_str(), &buffer);
										R_Texture->texture->path = R_Texture->LibraryPath.c_str();
										//Importer::ImporterTexture::Load(R_Texture->texture, buffer, size);

										R_Texture->name = ResourceToGameobject->name;
										R_Texture->texture->Name = ResourceToGameobject->name;
										R_Texture->colors.r = resource->Resource_red[0];
										R_Texture->colors.g = resource->Resource_green[0];
										R_Texture->colors.b = resource->Resource_blue[0];
										R_Texture->colors.a = resource->Resource_alpha[0];
										//R_Texture->AssetsPath = texturePath;
									}

									//R_Mesh->mesh->num_Materials = scene->mNumMaterials;
									//aiMaterial* mat = scene->mMaterials[scene->mMeshes[m_Counter]->mMaterialIndex];

									//if ((mat != nullptr) && (m_Counter < R_Mesh->mesh->num_Materials))
									//{
									//	Importer::ImporterTexture::ImportTextureWithMeta(mat, R_Texture, buffer, size);
									//}

									R_Texture->LibraryPath = resource->ComponentsInModel[j];
									R_Texture->resourceCounter += 1;

									app->scene->AllResources.push_back(ResourceToGameobject);

									TexturesManager* texturesManager = new TexturesManager();
									ComponentMaterial* C_Texture = dynamic_cast<ComponentMaterial*>(_ParentObj->AddComponent(ComponentType::MATERIAL));
									C_Texture->SetTexture(R_Texture->texture);
									C_Texture->colorTexture = R_Texture->colors;

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

		M_mesh->Name = _ParentObj->mName; int C = 0;

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
}

void ResourceManager::ChangeDebugMode(bool type)
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

void ResourceManager::CleanUp()
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
void ResourceManager::ClearAssimpVec(std::vector<T>& vector)
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

void  ResourceManager::Clear_Mesh(Mesh* mesh)
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

void ResourceManager::AplicateTransform(GameObject* gameObject, float3 position, float3 scale, Quat rotation)
{
	gameObject->mTransform->SetTransform(gameObject, position, scale, rotation); 
}

void ResourceManager::SetBuffers(Mesh* M_mesh)
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
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * M_mesh->num_normals * 3, M_mesh->normals, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &(GLuint)(M_mesh->VT));
	glBindBuffer(GL_ARRAY_BUFFER, M_mesh->VT);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * M_mesh->num_Tex * 2, M_mesh->textures, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ResourceManager::MetaFileCreator(GameObject* gameObject, const char* path)
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
		string CompName = "";

		if(gameObject->mComponents[i]->Type == ComponentType::MESH)
		{
			ComponentMesh* meshComp = dynamic_cast<ComponentMesh*>(gameObject->mComponents[i]);
			CompName = meshComp->Name;
			jsNode.addString("Component_Name", (string)meshComp->Name);
			name_UUID = "Library/Meshes/";
			string UUID = std::to_string(gameObject->mComponents[i]->UUID);
			name_UUID.append(UUID);
			name_UUID.append(".mesh");
		}

		if (gameObject->mComponents[i]->Type == ComponentType::MATERIAL)
		{
			ComponentMaterial* material = dynamic_cast<ComponentMaterial*>(gameObject->mComponents[i]);
			CompName = material->texture->Name;
			name_UUID = "Library/Textures/";
			string UUID = std::to_string(gameObject->mComponents[i]->UUID);
			name_UUID.append(UUID);
			name_UUID.append(".dds");
			jsNode.addNumber("Color_Red", material->colorTexture.r);
			jsNode.addNumber("Color_Green", material->colorTexture.g);
			jsNode.addNumber("Color_Blue", material->colorTexture.b);
			jsNode.addNumber("Color_Alpha", material->colorTexture.a);
		}

		jsNode.addString("Component_Name", CompName);
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

bool ResourceManager::CheckResourceComponentsExistence(ResourceElement* R_Element)
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

bool ResourceManager::CheckStringComponentsExistence(string Component)
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

bool ResourceManager::CheckNotDuplicateFromAssets(ResourceElement* R_Element, uint32 uuid)
{
	bool Exist = false;

	map<uint32, ResourceElement*>::iterator iteratorExistence = app->resource->AllResourcesMap.begin();
	for (iteratorExistence; iteratorExistence != app->resource->AllResourcesMap.end(); iteratorExistence++)
	{
		string NameIter = iteratorExistence->second->name;
		string NameRes = R_Element->name;

		/*if(NameRes == "Material")
		{
			ResourceTexture* R_textureIterator = (ResourceTexture*)iteratorExistence->second;
			ResourceTexture* R_texture = (ResourceTexture*) R_Element;

			if((R_texture->colors.r == R_textureIterator->colors.r) && (R_texture->colors.g == R_textureIterator->colors.g)
				&& (R_texture->colors.b == R_textureIterator->colors.b) && (R_texture->colors.a == R_textureIterator->colors.a))
			{
				Exist = true;

				if (uuid != -1)
				{
					iteratorExistence->second->ParentsUUID.push_back(uuid);
				}

				iteratorExistence->second->resourceCounter += 1;
			}
		}
		else
		{*/
			if (NameRes == NameIter)
			{
				Exist = true;

				if (uuid != -1)
				{
					iteratorExistence->second->ParentsUUID.push_back(uuid);
				}

				iteratorExistence->second->resourceCounter += 1;
			}
		//}
	}

	return Exist;
}

bool ResourceManager::ImportOnlyTexture(string Path)
{
	if (!app->scene->GameTime.running)
	{
		bool Exist = true;

		ResourceTexture* R_Texture = new ResourceTexture();

		char* buffer = nullptr;
		uint size = 0;

		string T_name = "";
		FileSystem::StringDivide(Path.c_str(), &T_name, nullptr);

		R_Texture->texture->path = Path.c_str();
		R_Texture->texture->Name = T_name;
		R_Texture->name = T_name;
		R_Texture->ParentsUUID.push_back(app->editor->actualMesh->UUID);

		Importer::ImporterTexture::Load(R_Texture->texture, Path.c_str());
		R_Texture->colors.r = 1;
		R_Texture->colors.g = 1;
		R_Texture->colors.b = 1;
		R_Texture->colors.a = 1;

		if (!CheckNotDuplicateFromAssets((ResourceElement*)R_Texture, R_Texture->UUID))
		{
			size = Importer::ImporterTexture::Save(R_Texture, &buffer);
		}

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

		C_Texture->colorTexture.r = 1;
		C_Texture->colorTexture.g = 1;
		C_Texture->colorTexture.b = 1;
		C_Texture->colorTexture.a = 1;

		string pathToMeta = "";

		pathToMeta = TEXTURES_PATH;
		pathToMeta.append(std::to_string(C_Texture->UUID));
		pathToMeta.append(".dds");

		R_Texture->LibraryPath = pathToMeta;

		if (!ResourceManager::CheckResourceComponentsExistence(R_Texture))
		{
			if (!CheckNotDuplicateFromAssets((ResourceElement*)R_Texture, R_Texture->UUID))
			{
				if (size > 0)
				{
					app->physFSManager->Save(pathToMeta.c_str(), buffer, size);
				}

				app->resource->AllResourcesMap[R_Texture->getUUID()] = R_Texture;
				app->resource->AllResourcesMap[R_Texture->getUUID()]->resourceCounter += 1;
			}
		}

		RELEASE_ARRAY(buffer);

		return Exist;
	}
}

Texture* ResourceManager::ChangeTexture(string Path)
{
	if (!app->scene->GameTime.running)
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

		Importer::ImporterTexture::Load(R_Texture->texture, Path.c_str());
		size = Importer::ImporterTexture::Save(R_Texture, &buffer);

		vector<ComponentManager*> objectMeshes = app->editor->actualMesh->GetComponentsGameObject(ComponentType::MATERIAL);

		ComponentMaterial* C_TexturetoShow;

		for (int j = 0; j < objectMeshes.size(); j++)
		{
			C_TexturetoShow = dynamic_cast<ComponentMaterial*>(objectMeshes[j]);

			if (C_TexturetoShow != nullptr)
			{
				if (C_TexturetoShow->texture != nullptr)
				{
					C_TexturetoShow->texture->ShowTextures = false;
				}
			}
		}

		if (!objectMeshes.empty())
		{
			if (C_TexturetoShow != nullptr)
			{
				if (C_TexturetoShow->texture != nullptr)
				{
					C_TexturetoShow->texture->ShowTextures = true;
				}
			}
		}

		ComponentMaterial* C_Texture = dynamic_cast<ComponentMaterial*>(app->editor->actualMesh->GetComponentGameObject(ComponentType::MATERIAL));

		string pathToMeta = "";

		pathToMeta = TEXTURES_PATH;
		pathToMeta.append(std::to_string(C_Texture->UUID));
		pathToMeta.append(".dds");

		R_Texture->LibraryPath = pathToMeta;

		if (!ResourceManager::CheckResourceComponentsExistence(R_Texture))
		{
			if (size > 0)
			{
				app->physFSManager->Save(pathToMeta.c_str(), buffer, size);
			}

			app->resource->AllResourcesMap[R_Texture->getUUID()] = R_Texture;
			app->resource->AllResourcesMap[R_Texture->getUUID()]->resourceCounter += 1;
		}

		RELEASE_ARRAY(buffer);

		return R_Texture->texture;
	}
	else
	{
		LOG(LogTypeCase::L_WARNING, "You are in PLAY mode!");
	}
}

void ResourceManager::MetaExistenceLoader(const char* Path)
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

		GameObject* gm = new GameObject(FileName, app->scene->Root);

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
