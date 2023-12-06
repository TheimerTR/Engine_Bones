#include "ModuleResource.h"
#include "ResourceElement.h"
#include "ResourceTexture.h"
#include "FileSystemManager.h"
#include "ImporterMesh.h"
#include "ImporterTexture.h"

ModuleResource::ModuleResource(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleResource::~ModuleResource()
{
}

bool ModuleResource::Init()
{
	return true;
}

bool ModuleResource::CleanUp()
{
	return true;
}

ResourceElement* ModuleResource::CreateResource(const char* name, ResourceTypes type, uint32 UUID, const char* assetsPath)
{
	ResourceElement* R_Temp = nullptr;

	switch(type)
	{
	case ResourceTypes::R_MATERIAL:
			break;
	case ResourceTypes::R_MESH:
		R_Temp = new ResourceMesh(name, assetsPath, MESHES_PATH, UUID);
		break;
	case ResourceTypes::R_MODEL:
		break;
	case ResourceTypes::R_NONE:
		break;
	}

	//AllResourcesMap[R_Temp->UUID] = R_Temp;

	return R_Temp;
}

ResourceElement* ModuleResource::LoadResourceElement(const char* LibraryPath)
{
	char* buffer = nullptr;
	uint size = app->physFSManager->Load(LibraryPath, &buffer);

	ResourceElement* temp_ResourceElement = nullptr;

	if (size > 0)
	{
		string extension = "";
		string Name = "";
		FileSystem::StringDivide(LibraryPath, &Name, &extension);

		if (strcmp(extension.data(), "mesh") == 0)
		{
			ResourceMesh* R_Mesh = new ResourceMesh();
			R_Mesh->name = Name;
			R_Mesh->mesh->Path = LibraryPath;
			Importer::ImporterMesh::Load(R_Mesh, buffer);
			LOG(LogTypeCase::L_CASUAL, "Resource extension .mesh");
			temp_ResourceElement = R_Mesh;
			temp_ResourceElement->type = ResourceTypes::R_MESH;
		}

		if (strcmp(extension.data(), "dds") == 0)
		{
			ResourceTexture* R_Texture = new ResourceTexture();
			R_Texture->name = Name;
			Importer::ImporterTexture::Load(R_Texture->texture, buffer);
			LOG(LogTypeCase::L_CASUAL, "Resource extension .dds");
			temp_ResourceElement = R_Texture;
			temp_ResourceElement->type = ResourceTypes::R_TEXTURE;
		}
	}
	else
	{
		LOG(LogTypeCase::L_ERROR, "The library path does not exist!");
		return nullptr;
	}

	RELEASE_ARRAY(buffer);

	return temp_ResourceElement;
}