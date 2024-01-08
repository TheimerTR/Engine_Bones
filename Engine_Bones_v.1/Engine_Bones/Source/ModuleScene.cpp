#include "ModuleScene.h"
#include "CanvasUI.h"

ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	Root = new GameObject("ROOT NODE", nullptr);
	Root->UUID = 0;
	AllGameObjectManagers.push_back(Root);
	Selected_GameObject = Root;

	isTyping = false;
	Demo = true;

	GameTime.Restart();
	GameTime.Stop();
}

ModuleScene::~ModuleScene()
{
}

bool ModuleScene::Init()
{
	return false;
}

bool ModuleScene::Start()
{
	/*App->font->FontLoader(120, "./Assets/Fonts/Roboto.ttf");
	App->font->FontLoader(120, "./Assets/Fonts/Elianto.otf");*/
	App->font->actualFont = App->font->FontLoader(120, "./Assets/Fonts/Arial.ttf");

	return true;
}

update_status ModuleScene::Update(float dt)
{
	if (Demo)
	{
		if (app->editor->GameWindowSize.x > 50 && app->editor->GameWindowSize.y > 50)
		{
			DemoScene();
			Demo = false;
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_DELETE) == KEY_DOWN)
	{
		if (!app->scene->GameTime.running && !isTyping)
		{
			if ((App->editor->actualMesh != nullptr) && (App->scene->AllGameObjectManagers.size() > 0))
			{
				App->editor->actualMesh->mParent->DeleteChild(App->editor->actualMesh);
			}
		}
		else
		{
			LOG(LogTypeCase::L_WARNING, "You are in PLAY mode!");
		}
	}
	
	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		App->scene->GameTime.running = false;

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
			if (!app->scene->GameTime.running && !isTyping)
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
			else
			{
				LOG(LogTypeCase::L_WARNING, "You are in PLAY mode!");
			}
		}

		if (App->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN)
		{
			if (!app->scene->GameTime.running && !isTyping)
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
			else
			{
				LOG(LogTypeCase::L_WARNING, "You are in PLAY mode!");
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

void ModuleScene::PushGameObject(GameObject* gameObject)
{
	AllGameObjectManagers.push_back(gameObject);
}

void ModuleScene::CreateGameCamera(                                                                                                                                                                                   )
{
	GameObject* gameObject = new GameObject("camera", Root);
	ComponentCamera* camera = dynamic_cast<ComponentCamera*>(gameObject->AddComponent(ComponentType::CAMERA));
	App->renderer3D->SetCameraGame(camera); 
}

void ModuleScene::AddCameraComponent(GameObject* gm)
{
	ComponentCamera* camera = dynamic_cast<ComponentCamera*>(gm->AddComponent(ComponentType::CAMERA));
	App->renderer3D->SetCameraGame(camera);
}

void ModuleScene::DemoScene()
{
	CanvasUI* canv_UI = new CanvasUI(Root, app->editor->GameWindowSize.x, app->editor->GameWindowSize.y, 0, 0);

	ComponentUI* canv_Comp_UI = new ComponentUI(UI_Type::DEF, App->scene->Root, app->editor->GameWindowSize.x, app->editor->GameWindowSize.y, 0, 0, nullptr);

	canv_Comp_UI = canv_Comp_UI->CreateGameObjectUI(UI_Type::CANV, app->editor->GameWindowSize.x, app->editor->GameWindowSize.y, 0, 0, nullptr, nullptr);
	
	ComponentUI* start_Text = new ComponentUI(UI_Type::DEF, App->scene->Root, 80, 40, (uint)app->editor->GameWindowSize.x / 2 - ((uint)app->editor->GameWindowSize.x / 12), ((uint)app->editor->GameWindowSize.y / 1.68), nullptr);
	
	start_Text = start_Text->CreateGameObjectUI(UI_Type::TEXT, 80, 40, (uint)app->editor->GameWindowSize.x / 2 - ((uint)app->editor->GameWindowSize.x / 12), ((uint)app->editor->GameWindowSize.y / 1.68), nullptr, "START");
	
	ComponentUI* start_Button = new ComponentUI(UI_Type::DEF, App->scene->Root, 140, 70, (uint)app->editor->GameWindowSize.x / 2 - ((uint)app->editor->GameWindowSize.x / 6.5), ((uint)app->editor->GameWindowSize.y / 1.8), "Assets/Textures/Button2.png");

	start_Button = start_Button->CreateGameObjectUI(UI_Type::BUTTON, 140, 70, (uint)app->editor->GameWindowSize.x / 2 - ((uint)app->editor->GameWindowSize.x / 6.5), ((uint)app->editor->GameWindowSize.y / 1.8), "Assets/Textures/Button2.png", nullptr, 0);

	ComponentUI* background_Img = new ComponentUI(UI_Type::DEF, App->scene->Root, (uint)canv_UI->widthPanel, (uint)canv_UI->heigthPanel, 0, 0, "Assets/Textures/TheStreetHouse.png");

	background_Img = background_Img->CreateGameObjectUI(UI_Type::IMAGE, (uint)canv_UI->widthPanel, (uint)canv_UI->heigthPanel, 0, 0, "Assets/Textures/TheStreetHouse.png", nullptr);
}
