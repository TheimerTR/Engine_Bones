#ifndef MODULE_SCENE
#define MODULE_SCENE

#include "Module.h"
#include "Application.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "ResourceElement.h"

#include <vector>
#include <stdio.h>

using namespace std;

class ModuleScene : public Module
{
public:

	ModuleScene(Application* app, bool start_enabled = true);
	~ModuleScene();

	bool Init() override;
	update_status Update(float dt) override;
	update_status PostUpdate(float dt) override;
	bool CleanUp() override;

	void PushGameObject(GameObject* gameObject);

	void CreateGameCamera(); 
	void AddCameraComponent(GameObject* gm);

public:
	vector<GameObject*> AllGameObjectManagers;
	GameObject* Root = nullptr;
	GameObject* Selected_GameObject = nullptr;

	vector<ResourceElement*> AllResources;

	Timer GameTime;

	bool isTyping;
};

#endif