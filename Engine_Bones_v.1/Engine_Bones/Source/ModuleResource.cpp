#include "ModuleResource.h"

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

Resource* ModuleResource::CreateResource(const char* name, ResourceTypes type, uint32 UUID, const char* assetsPath)
{
	Resource* R_Temp = nullptr;

	switch(type)
	{
	case ResourceTypes::R_MATERIAL:
			break;
	case ResourceTypes::R_MESH:
		//R_Temp = new ResourceMesh(name, assetsPath, MESHES_PATH, UUID);
		break;
	case ResourceTypes::R_MODEL:
		break;
	case ResourceTypes::R_NONE:
		break;
	}

	AllResources[R_Temp->UUID] = R_Temp;

	return R_Temp;
}