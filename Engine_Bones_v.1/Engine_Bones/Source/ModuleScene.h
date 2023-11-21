#ifndef MODULE_SCENE
#define MODULE_SCENE

#include "Module.h"
#include "Application.h"
#include "AssimpManager.h"
#include "GameObjectManager.h"
#include "Resource.h"

#include <vector>
#include <stdio.h>

using namespace std;
class Resource;

class ModuleScene : public Module
{
public:

	ModuleScene(Application* app, bool start_enabled = true);
	~ModuleScene();

	bool Init() override;
	update_status Update(float dt) override;
	update_status PostUpdate(float dt) override;
	bool CleanUp() override;

	void PushGameObject(GameObjectManager* gameObject);

public:
	vector<GameObjectManager*> AllGameObjectManagers;
	GameObjectManager* Root = nullptr;
	GameObjectManager* Selected_GameObject = nullptr;

	vector<Resource*> AllResources;
};

#endif