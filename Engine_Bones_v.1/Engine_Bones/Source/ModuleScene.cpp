#include "ModuleScene.h"
#include "CanvasUI.h"
#include "ButtonUI.h"
#include "CheckerUI.h"

ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	Root = new GameObject("ROOT NODE", nullptr);
	Root->UUID = 0;
	AllGameObjectManagers.push_back(Root);
	Selected_GameObject = Root;

	isTyping = false;
	Demo = true;
	isOnScene = false;
	openPauseMenu = false;
	draggable = false;
	restartScene = false;

	actual_UI_Object = 0;

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
			pause = new GameObject("Pause", App->editor->Canvas);
			Demo = false;
		}
	}

	ComponentUI* objectUI = nullptr;

	for (int i = 1; i < App->scene->AllGameObjectManagers.size(); i++)
	{
		if (App->scene->AllGameObjectManagers[i]->isActive)
		{
			objectUI = dynamic_cast<ComponentUI*>(App->scene->AllGameObjectManagers[i]->GetComponentGameObject(ComponentType::UI));

			if (objectUI != nullptr)
			{
				bool isComponent = false;

				for (int j = 0; j < All_UI.size(); j++)
				{
					if (All_UI[j] == objectUI)
					{
						isComponent = true;
					}
				}

				if (!isComponent)
				{
					All_UI.push_back(objectUI);

					if(objectUI->ui_Type == CHECKER || objectUI->ui_Type == BUTTON)
					{
						AllInteractuableUI.push_back(objectUI);
					}
				}
			}
		}
	}

	if(app->input->GetKey(SDL_SCANCODE_TAB) == KEY_DOWN)
	{
		if (AllInteractuableUI.size() > 0 && GameTime.running && !GameTime.paused)
		{
			for (int j = 0; j < AllInteractuableUI.size(); j++)
			{
				ComponentMaterial* mat = dynamic_cast<ComponentMaterial*>(AllInteractuableUI[j]->gmAtached->GetComponentGameObject(ComponentType::MATERIAL));
				mat->colorTexture.r = 1;
				mat->colorTexture.g = 1;
				mat->colorTexture.b = 1;
				mat->colorTexture.a = 1;
			}

			if (GameTime.running && !GameTime.paused)
			{
				if (actual_UI_Object < AllInteractuableUI.size() - 1)
				{
					actual_UI_Object++;
				}
				else
				{
					actual_UI_Object = 0;
				}

				ComponentMaterial* mat = dynamic_cast<ComponentMaterial*>(AllInteractuableUI[actual_UI_Object]->gmAtached->GetComponentGameObject(ComponentType::MATERIAL));
				mat->colorTexture.r = 1;
				mat->colorTexture.g = 0.7;
				mat->colorTexture.b = 0;
				mat->colorTexture.a = 1;
			}
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
	
	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN && app->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_DOWN)
	{
		App->scene->GameTime.running = false;
	}

	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		openPauseMenu = !openPauseMenu;

		if (App->scene->GameTime.running && isOnScene && !app->editor->isMovingChild && !App->editor->isMovingParent)
		{
			OpenPauseMenu();
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

		if (GameTime.running && !GameTime.paused)
		{
			if (AllInteractuableUI.size() > 0)
			{
				switch (AllInteractuableUI[actual_UI_Object]->ui_Type)
				{
				case BUTTON:
				{
					ButtonUI* button = (ButtonUI*)AllInteractuableUI[actual_UI_Object];
					button->OnClick(&AllInteractuableUI[actual_UI_Object]->actualButtonAction);
				}
				break;
				case CHECKER:
				{
					CheckerUI* checker = (CheckerUI*)this;
					checker->OnClick(AllInteractuableUI[actual_UI_Object]);
				}
				break;
				default:
					break;
				}
			}
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
	if(restartScene)
	{
		RestartScene();

		restartScene = false;
	}

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

void ModuleScene::CreateGameCamera()
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
	CreateGameCamera();

	CanvasUI* canv_UI = new CanvasUI(Root, app->editor->GameWindowSize.x, app->editor->GameWindowSize.y, 0, 0);

	ComponentUI* canv_Comp_UI = new ComponentUI(UI_Type::DEF, App->scene->Root, app->editor->GameWindowSize.x, app->editor->GameWindowSize.y, 0, 0, nullptr);

	canv_Comp_UI = canv_Comp_UI->CreateGameObjectUI(App->scene->Root, UI_Type::CANV, app->editor->GameWindowSize.x, app->editor->GameWindowSize.y, 0, 0, nullptr, nullptr);
	
	ComponentUI* start_Text = new ComponentUI(UI_Type::DEF, App->editor->Canvas, 80, 40, (canv_UI->widthPanel / 2) - (canv_UI->widthPanel / 9), (canv_UI->heigthPanel / 2) + (canv_UI->heigthPanel / 12), nullptr);
	
	start_Text = start_Text->CreateGameObjectUI(App->editor->Canvas, UI_Type::TEXT, 80, 40, (canv_UI->widthPanel / 2) - (canv_UI->widthPanel / 9), (canv_UI->heigthPanel / 2) + (canv_UI->heigthPanel / 12), nullptr, "START");
	
	ComponentUI* start_Button = new ComponentUI(UI_Type::DEF, App->editor->Canvas, 140, 70, (canv_UI->widthPanel / 2) - (canv_UI->widthPanel / 4.9), (canv_UI->heigthPanel / 2) + (canv_UI->heigthPanel / 20), "Assets/Textures/Button3.png");

	start_Button = start_Button->CreateGameObjectUI(App->editor->Canvas, UI_Type::BUTTON, 140, 70, (canv_UI->widthPanel / 2) - (canv_UI->widthPanel / 4.9), (canv_UI->heigthPanel / 2) + (canv_UI->heigthPanel / 20), "Assets/Textures/Button3.png", nullptr, 0);

	ComponentUI* background_Img = new ComponentUI(UI_Type::DEF, App->editor->Canvas, (uint)canv_UI->widthPanel, (uint)canv_UI->heigthPanel, 0, 0, "Assets/Textures/TheStreetHouse.png");

	background_Img = background_Img->CreateGameObjectUI(App->editor->Canvas, UI_Type::IMAGE, (uint)canv_UI->widthPanel, (uint)canv_UI->heigthPanel, 0, 0, "Assets/Textures/TheStreetHouse.png", nullptr);
}

void ModuleScene::OpenPauseMenu()
{
	pause->isActive = openPauseMenu;


	for (int i = 0; i < pause->childrens.size(); i++)
	{
		pause->childrens[i]->isActive = openPauseMenu;

		ComponentMaterial* mat = dynamic_cast<ComponentMaterial*>(pause->childrens[i]->GetComponentGameObject(ComponentType::MATERIAL));

		mat->colorTexture.a = 100;
	}
}

void ModuleScene::RestartScene()
{
	for(int i = 1; i < AllGameObjectManagers.size(); i++)
	{
		if (AllGameObjectManagers.size() > 1)
		{
			AllGameObjectManagers[1]->mParent->DeleteChild(AllGameObjectManagers[1]);
		}
		else
		{
			continue;
		}
	}

	App->editor->actualMesh = nullptr;

	DemoScene();
	pause = new GameObject("Pause", App->editor->Canvas);
}
