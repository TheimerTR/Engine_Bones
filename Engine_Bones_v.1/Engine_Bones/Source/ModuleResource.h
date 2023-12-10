#ifndef MODULE_RESOURCE
#define MODULE_RESOURCE

#include "Module.h"
#include "Application.h"
#include "ResourceMesh.h"
#include "GameObject.h"
#include "ResourceElement.h"

#include <vector>
#include <stdio.h>
#include <map>

using namespace std;

class ModuleResource : public Module
{
public:

	ModuleResource(Application* app, bool start_enabled = true);
	~ModuleResource();

	bool Init() override;
	bool CleanUp() override;

	ResourceElement* CreateResource(const char* name, ResourceTypes type, uint32 UUID, const char* assetsPath);

	ResourceElement* LoadResourceElement(const char* LibraryPath);

	//ResourceElement* LoadResource(uint32 UUID);
	//ResourceElement* GetResource(uint32 UUID);

public:
	map<uint32, ResourceElement*> AllResourcesMap;
};

#endif //MODULE_RESOURCE