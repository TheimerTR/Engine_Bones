#include "ModuleScene.h"

ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	Root = new GameObjectManager("ROOT NODE", nullptr);
	AllGameObjectManagers.push_back(Root);
	Selected_GameObject = Root;
}

ModuleScene::~ModuleScene()
{
}

bool ModuleScene::Init()
{
	return false;
}

update_status ModuleScene::Update(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_DELETE) == KEY_DOWN)
	{
		if ((App->editor->actualMesh != nullptr) && (App->scene->AllGameObjectManagers.size() > 0))
		{
			App->editor->actualMesh->mParent->DeleteChild(App->editor->actualMesh);
		}
	}
	
	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		if (App->editor->isMovingParent)
		{
			App->editor->moveEntityTo = nullptr;
			App->editor->hoveredItem = nullptr;
			App->editor->isMovingParent = false;
		}
	}

	if(AllGameObjectManagers.size() <= 0)
	{
		App->editor->actualMesh = nullptr;
	}

	return UPDATE_CONTINUE;
}

update_status ModuleScene::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

bool ModuleScene::CleanUp()
{
	return true;
}

void ModuleScene::PushGameObject(GameObjectManager* gameObject)
{
	AllGameObjectManagers.push_back(gameObject);
}
