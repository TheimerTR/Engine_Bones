#ifndef MODULE_RESOURCE
#define MODULE_RESOURCE

#include "Module.h"
#include "Application.h"
#include "Resource.h"
#include "ResourceMesh.h"
#include "GameObjectManager.h"
#include "Resource.h"

#include <vector>
#include <stdio.h>
#include <map>

using namespace std;
class ResourceManager;
class ResourceMesh;

class ModuleResource : public Module
{
public:

	ModuleResource(Application* app, bool start_enabled = true);
	~ModuleResource();

	bool Init() override;
	bool CleanUp() override;

	ResourceManager* CreateResource(const char* name, ResourceTypes type, uint32 UUID, const char* assetsPath);

	ResourceManager* LoadResource(uint32 UUID);
	ResourceManager* GetResource(uint32 UUID);

public:
	map<uint32, ResourceManager*> AllResources;
};

#endif //MODULE_RESOURCE