#include "ModuleResource.h"
#include "ResourceElement.h"
#include "FileSystemManager.h"
#include "ImporterMesh.h"

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

	AllResourcesMap[R_Temp->UUID] = R_Temp;

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
		FileSystem::StringDivide(LibraryPath, nullptr, &extension);

		if (strcmp(extension.data(), "mesh") == 0)
		{
			ResourceMesh* R_Mesh = new ResourceMesh();
			Importer::ImporterMesh::Load(R_Mesh, buffer);
			LOG(LogTypeCase::L_CASUAL, "Resource extension .mesh");
			temp_ResourceElement = R_Mesh;
			temp_ResourceElement->type = ResourceTypes::R_MESH;
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