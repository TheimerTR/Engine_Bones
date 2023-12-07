#include "ModuleScene.h"

ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	Root = new GameObjectManager("ROOT NODE", nullptr);
	Root->UUID = 0;
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
		if (App->editor->isMovingParent && !app->editor->isMovingChild)
		{
			App->editor->moveEntityTo = nullptr;
			App->editor->hoveredItem = nullptr;
			App->editor->isMovingParent = false;
		}

		if (app->editor->isMovingChild && !App->editor->isMovingParent)
		{
			App->editor->actualMesh->mParent->childrens = App->editor->OG_ChildList;

			/*GameObjectManager* tempGameObject = nullptr;

			tempGameObject = App->editor->actualMesh->mParent->childrens.at(App->editor->OG_ChildListPos);
			App->editor->actualMesh->mParent->childrens.at(App->editor->OG_ChildListPos) = App->editor->actualMesh->mParent->childrens.at(App->editor->Actual_ChildPos);
			App->editor->actualMesh->mParent->childrens.at(App->editor->Actual_ChildPos) = tempGameObject;*/

			App->editor->isMovingChild = false;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{
		if (app->editor->isMovingChild && !App->editor->isMovingParent)
		{
			app->editor->isMovingChild = false;
		}
	}

	if (App->editor->actualMesh != nullptr)
	{
		if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN)
		{
			if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
			{
				if (App->editor->actualMesh->mParent != App->scene->AllGameObjectManagers.at(0))
				{
					App->editor->actualMesh->ChangeParent(App->scene->Root);
				}
			}
			else
			{
				if (App->editor->actualMesh != nullptr)
				{
					App->editor->moveEntityTo = App->editor->actualMesh;
					App->editor->isMovingParent = true;
				}
			}
		}

		if (App->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN)
		{
			if (App->editor->actualMesh != nullptr)
			{
				App->editor->OG_ChildListPos = App->editor->actualMesh->SearchChildPosInVector();
				App->editor->OG_ChildList = App->editor->actualMesh->mParent->childrens;

				if (App->editor->OG_ChildListPos != -1)
				{
					App->editor->isMovingChild = true;
				}
				else
				{
					LOG(LogTypeCase::L_ERROR, "Child Was not found in Parent list");
				}
			}
		}
	}

	if(AllGameObjectManagers.size() <= 1)
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

void ModuleScene::CreateGameCamera(                                                                                                                                                                                   )
{
	GameObjectManager* gameObject = new GameObjectManager("camera", Root); 
	ComponentCamera* camera = dynamic_cast<ComponentCamera*>(gameObject->AddComponent(ComponentType::CAMERA));
	App->renderer3D->SetCameraGame(camera); 
}
