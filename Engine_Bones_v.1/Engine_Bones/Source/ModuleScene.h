#ifndef MODULE_SCENE
#define MODULE_SCENE

#include "Module.h"
#include "Application.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "ResourceElement.h"
#include "ComponentUI.h"

#include <vector>
#include <stdio.h>
#include <map>

using namespace std;

class ModuleScene : public Module
{
public:

	ModuleScene(Application* app, bool start_enabled = true);
	~ModuleScene();

	bool Init() override;
	bool Start() override;
	update_status Update(float dt) override;
	update_status PostUpdate(float dt) override;
	bool CleanUp() override;

	void PushGameObject(GameObject* gameObject);

	void CreateGameCamera(); 
	void AddCameraComponent(GameObject* gm);

	void DemoScene();
	void OpenPauseMenu();
	void RestartScene();

public:
	vector<GameObject*> AllGameObjectManagers;
	GameObject* Root = nullptr;
	GameObject* Selected_GameObject = nullptr;

	vector<ResourceElement*> AllResources;
	vector<ComponentUI*> All_UI;
	vector<ComponentUI*> AllInteractuableUI;

	Timer GameTime;

	bool isTyping;
	bool Demo;
	bool isOnScene;
	bool openPauseMenu;
	bool draggable;
	bool restartScene;

	int actual_UI_Object;

	GameObject* pause;
};

#endif