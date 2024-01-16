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
	isRescaleUI = false;
	isFading = false;

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

	if(isFading)
	{
		if(FadeToBlack(dt))
		{
			PassScene();
			isFading = false;
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
				AllInteractuableUI[j]->isSelected = false;
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
				mat->colorTexture.r = 0;
				mat->colorTexture.g = 1;
				mat->colorTexture.b = 0;
				mat->colorTexture.a = 1;
				AllInteractuableUI[actual_UI_Object]->isSelected = true;
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

	ComponentUI* comp_UI = new ComponentUI(UI_Type::DEF, App->scene->Root, 0, 0, 0, 0, nullptr);
	ComponentTransform* transform;
	ComponentMaterial* mat = nullptr;

	comp_UI->CreateGameObjectUI(App->scene->Root, UI_Type::CANV, app->editor->GameWindowSize.x, app->editor->GameWindowSize.y, 0, 0, nullptr, nullptr);

	CanvasUI* canv_UI = new CanvasUI(App->editor->Canvas, app->editor->GameWindowSize.x, app->editor->GameWindowSize.y, 0, 0);

	comp_UI->CreateGameObjectUI(App->scene->Root, UI_Type::TEXT, 25, 25, 0, 0, nullptr, "START", 1, nullptr, (canv_UI->widthPanel / 2) - (canv_UI->widthPanel / 8), (canv_UI->heigthPanel / 2) + (canv_UI->heigthPanel / 9), 25, 25);
	transform = dynamic_cast<ComponentTransform*>(App->scene->AllGameObjectManagers[App->scene->AllGameObjectManagers.size() - 1]->GetComponentGameObject(ComponentType::TRANSFORM));
	transform->mPosition = { (float)((canv_UI->widthPanel / 2) - (canv_UI->widthPanel / 8)), (float)((canv_UI->heigthPanel / 2) + (canv_UI->heigthPanel / 9)), 0 };
	
	mat = dynamic_cast<ComponentMaterial*>(App->scene->AllGameObjectManagers[App->scene->AllGameObjectManagers.size() - 1]->GetComponentGameObject(ComponentType::MATERIAL));

	if (mat != nullptr)
	{
		mat->colorTexture.a = 1;
	}

	comp_UI->CreateGameObjectUI(App->scene->Root, UI_Type::INPUT_TEXT, 20, 20, 0, 0, "Assets/Textures/Input_Text_Empty.png", "", 1, nullptr, (canv_UI->widthPanel / 2) - (canv_UI->widthPanel / 10), (canv_UI->heigthPanel / 2) + (canv_UI->heigthPanel / 3.6), 160, 80);
	transform = dynamic_cast<ComponentTransform*>(App->scene->AllGameObjectManagers[App->scene->AllGameObjectManagers.size() - 1]->GetComponentGameObject(ComponentType::TRANSFORM));
	transform->mPosition = { (float)((canv_UI->widthPanel / 2) - (canv_UI->widthPanel / 10)), (float)((canv_UI->heigthPanel / 2) + (canv_UI->heigthPanel / 3.6)), 0 };

	mat = dynamic_cast<ComponentMaterial*>(App->scene->AllGameObjectManagers[App->scene->AllGameObjectManagers.size() - 1]->GetComponentGameObject(ComponentType::MATERIAL));

	if (mat != nullptr)
	{
		mat->colorTexture.a = 1;
	}

	comp_UI->CreateGameObjectUI(App->scene->Root, UI_Type::BUTTON, 160, 80, 0, 0, "Assets/Textures/Button3.png", nullptr, 0, nullptr, (canv_UI->widthPanel / 2) - (canv_UI->widthPanel / 6), (canv_UI->heigthPanel / 2) + (canv_UI->heigthPanel / 20), 160, 80);
	transform = dynamic_cast<ComponentTransform*>(App->scene->AllGameObjectManagers[App->scene->AllGameObjectManagers.size() - 1]->GetComponentGameObject(ComponentType::TRANSFORM));
	transform->mPosition = { (float)((canv_UI->widthPanel / 2) - (canv_UI->widthPanel / 6)), (float)((canv_UI->heigthPanel / 2) + (canv_UI->heigthPanel / 20)), 0 };

	mat = dynamic_cast<ComponentMaterial*>(App->scene->AllGameObjectManagers[App->scene->AllGameObjectManagers.size() - 1]->GetComponentGameObject(ComponentType::MATERIAL));

	if (mat != nullptr)
	{
		mat->colorTexture.a = 1;
	}

	comp_UI->CreateGameObjectUI(App->scene->Root, UI_Type::IMAGE, 160, 40, 0, 0, "Assets/Textures/Unchecked-checkbox.png", nullptr, 0, 0, 160, 40);
	transform = dynamic_cast<ComponentTransform*>(App->scene->AllGameObjectManagers[App->scene->AllGameObjectManagers.size() - 1]->GetComponentGameObject(ComponentType::TRANSFORM));
	transform->mPosition = { (float)((canv_UI->widthPanel / 2) - (canv_UI->widthPanel / 6)), (float)((canv_UI->heigthPanel / 2) + (canv_UI->heigthPanel / 4)), 0 };
	
	mat = dynamic_cast<ComponentMaterial*>(App->scene->AllGameObjectManagers[App->scene->AllGameObjectManagers.size() - 1]->GetComponentGameObject(ComponentType::MATERIAL));

	if (mat != nullptr)
	{
		mat->colorTexture.a = 1;
	}

	comp_UI->CreateGameObjectUI(App->scene->Root, UI_Type::IMAGE, (uint)canv_UI->widthPanel, (uint)canv_UI->heigthPanel, 0, 0, "Assets/Textures/TheStreetHouse.png", nullptr, 1, nullptr, 0, 0, (uint)canv_UI->widthPanel, (uint)canv_UI->heigthPanel);
	
	mat = dynamic_cast<ComponentMaterial*>(App->scene->AllGameObjectManagers[App->scene->AllGameObjectManagers.size() - 1]->GetComponentGameObject(ComponentType::MATERIAL));

	if (mat != nullptr)
	{
		mat->colorTexture.a = 1;
	}
}

void ModuleScene::OpenPauseMenu()
{
	pause->isActive = openPauseMenu;


	for (int i = 0; i < pause->childrens.size(); i++)
	{
		pause->childrens[i]->isActive = openPauseMenu;

		if(pause->childrens[i]->childrens.size() > 0)
		{
			for(int j = 0; j < pause->childrens[i]->childrens.size(); j++)
			{
				pause->childrens[i]->childrens[j]->isActive = openPauseMenu;

				ComponentMaterial* mat = dynamic_cast<ComponentMaterial*>(pause->childrens[i]->childrens[j]->GetComponentGameObject(ComponentType::MATERIAL));

				mat->colorTexture.a = 100;
			}
		}

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
	app->scene->AllInteractuableUI.clear();
	app->scene->All_UI.clear();

	DemoScene();
	pause = new GameObject("Pause", App->editor->Canvas);
}

bool ModuleScene::FadeToBlack(float dt)
{
	bool ret = false;

	for(int i = 0; i < app->editor->Canvas->childrens.size(); i++)
	{
		ComponentMaterial* mat = dynamic_cast<ComponentMaterial*>(app->editor->Canvas->childrens[i]->GetComponentGameObject(ComponentType::MATERIAL));
		
		if (mat != nullptr)
		{
			if (mat->colorTexture.a <= 0.0005)
			{
				app->editor->Canvas->childrens[i]->isActive = false;
				ret = true;
			}
			else
			{
				mat->colorTexture.a -= 0.5 * dt;
			}
		}
	}

	return ret;
}

void ModuleScene::PassScene()
{
	LOG(LogTypeCase::L_CASUAL, "Pass Scene!");

	ResourceManager::ResourceLoader("Assets/ModelsFbx/Street.fbx");
	app->scene->Selected_GameObject = app->scene->AllGameObjectManagers.at(app->scene->AllGameObjectManagers.size() - 1);
	app->editor->actualMesh = app->scene->AllGameObjectManagers.at(app->scene->AllGameObjectManagers.size() - 1);
	app->editor->actualMesh->isSelected = true;

	app->editor->actualResource = app->resource->AllResourcesMap.begin()->second;

	vector<ComponentUI*> objectsUI;

	for (int i = 0; i < app->scene->AllGameObjectManagers.size(); i++)
	{
		ComponentUI* objectUI = dynamic_cast<ComponentUI*>(app->scene->AllGameObjectManagers[i]->GetComponentGameObject(ComponentType::UI));

		if (objectUI != nullptr && objectUI->ui_Type != CANV)
		{
			app->scene->AllGameObjectManagers[i]->isActive = false;
			//objectsUI.push_back(objectUI);
		}
	}

	app->scene->AllInteractuableUI.clear();
	app->scene->All_UI.clear();

	/*for (int i = 0; i < objectsUI.size(); i++)
	{
		objectsUI[i]->Owner->mParent->DeleteChild(objectsUI[i]->Owner);
	}*/

	CreatePauseMenu();

	ComponentTransform* transform = dynamic_cast<ComponentTransform*>(app->editor->Canvas->GetComponentGameObject(ComponentType::TRANSFORM));
	ComponentTransform* transformPivot = nullptr;

	ComponentUI* comp_UI = new ComponentUI(UI_Type::DEF, app->scene->Root, 0, 0, 0, 0, nullptr);

	comp_UI->CreateGameObjectUI(app->scene->Root, UI_Type::IMAGE, 40, 40, 0, 0, "Assets/Textures/Crosshair.png", nullptr, 1, nullptr, ((transform->mScale.x / 2) - 30), ((transform->mScale.y / 2) - 20), 40, 40);
	app->scene->AllGameObjectManagers[app->scene->AllGameObjectManagers.size() - 1]->mName = "Crosshair";
	transformPivot = dynamic_cast<ComponentTransform*>(app->scene->AllGameObjectManagers[app->scene->AllGameObjectManagers.size() - 1]->GetComponentGameObject(ComponentType::TRANSFORM));
	transformPivot->mPosition = { (float)((transform->mScale.x / 2) - 30), (float)((transform->mScale.y / 2) - 20), 0 };

	ComponentMaterial* mat = dynamic_cast<ComponentMaterial*>(app->scene->AllGameObjectManagers[app->scene->AllGameObjectManagers.size() - 1]->GetComponentGameObject(ComponentType::MATERIAL));
	mat->colorTexture.r = 255;
	mat->colorTexture.g = 0;
	mat->colorTexture.b = 0;

	app->scene->isOnScene = true;
}

void ModuleScene::CreatePauseMenu()
{
	pause->isActive = true;
	ComponentTransform* transform;

	ComponentUI* comp_UI = new ComponentUI(UI_Type::DEF, App->scene->Root, 0, 0, 0, 0, nullptr);

	comp_UI = comp_UI->CreateGameObjectUI(app->scene->Root, TEXT, app->editor->GameWindowSize.x / 10, app->editor->GameWindowSize.y / 8, 0, 0, nullptr, "PAUSE", 0, nullptr, (app->editor->GameWindowSize.x / 2) - (app->editor->GameWindowSize.x / 4), (app->editor->GameWindowSize.y / 2) - (app->editor->GameWindowSize.y / 2.5), app->editor->GameWindowSize.x / 10, app->editor->GameWindowSize.y / 8);
	transform = dynamic_cast<ComponentTransform*>(app->scene->AllGameObjectManagers[app->scene->AllGameObjectManagers.size() - 1]->GetComponentGameObject(ComponentType::TRANSFORM));
	transform->mPosition = { (float)((app->editor->GameWindowSize.x / 2) - (app->editor->GameWindowSize.x / 4)), (float)((app->editor->GameWindowSize.y / 2) - (app->editor->GameWindowSize.y / 2.5)), 0 };
	transform->UpdateTransformation();
	comp_UI->isChildOfText = true;

	GameObject* gm1 = app->scene->AllGameObjectManagers[app->scene->AllGameObjectManagers.size() - 1];

	comp_UI = comp_UI->CreateGameObjectUI(app->scene->Root, CHECKER, app->editor->GameWindowSize.x / 8, app->editor->GameWindowSize.x / 8, 0, 0, "Assets/Textures/Ckeck-checkedbox.png", nullptr, 0, "Assets/Textures/Unchecked-checkbox.png", (app->editor->GameWindowSize.x / 2) - (app->editor->GameWindowSize.x / 4.3), (app->editor->GameWindowSize.y / 2) - (app->editor->GameWindowSize.y / 5), app->editor->GameWindowSize.x / 8, app->editor->GameWindowSize.x / 8);
	transform = dynamic_cast<ComponentTransform*>(app->scene->AllGameObjectManagers[app->scene->AllGameObjectManagers.size() - 1]->GetComponentGameObject(ComponentType::TRANSFORM));
	transform->mPosition = { (float)((app->editor->GameWindowSize.x / 2) - (app->editor->GameWindowSize.x / 4.3)), (float)((app->editor->GameWindowSize.y / 2) - (app->editor->GameWindowSize.y / 5)), 0 };
	transform->UpdateTransformation();
	comp_UI->isChildOfText = true;

	GameObject* gm2 = app->scene->AllGameObjectManagers[app->scene->AllGameObjectManagers.size() - 1];

	comp_UI = comp_UI->CreateGameObjectUI(app->scene->Root, TEXT, app->editor->GameWindowSize.x / 20, app->editor->GameWindowSize.y / 10, 0, 0, nullptr, "VSYNC", 0, nullptr, (app->editor->GameWindowSize.x / 2) - (app->editor->GameWindowSize.x / 10), (app->editor->GameWindowSize.y / 2) - (app->editor->GameWindowSize.y / 5.5), app->editor->GameWindowSize.x / 20, app->editor->GameWindowSize.y / 10);
	transform = dynamic_cast<ComponentTransform*>(app->scene->AllGameObjectManagers[app->scene->AllGameObjectManagers.size() - 1]->GetComponentGameObject(ComponentType::TRANSFORM));
	transform->mPosition = { (float)((app->editor->GameWindowSize.x / 2) - (app->editor->GameWindowSize.x / 10)), (float)((app->editor->GameWindowSize.y / 2) - (app->editor->GameWindowSize.y / 5.5)), 0 };
	transform->UpdateTransformation();
	comp_UI->isChildOfText = true;

	GameObject* gm3 = app->scene->AllGameObjectManagers[app->scene->AllGameObjectManagers.size() - 1];

	comp_UI = comp_UI->CreateGameObjectUI(app->scene->Root, CHECKER, app->editor->GameWindowSize.x / 8, app->editor->GameWindowSize.x / 8, 0, 0, "Assets/Textures/Ckeck-checkedbox.png", nullptr, 1, "Assets/Textures/Unchecked-checkbox.png", (app->editor->GameWindowSize.x / 2) - (app->editor->GameWindowSize.x / 4.3), (app->editor->GameWindowSize.y / 2), app->editor->GameWindowSize.x / 8, app->editor->GameWindowSize.x / 8);
	transform = dynamic_cast<ComponentTransform*>(app->scene->AllGameObjectManagers[app->scene->AllGameObjectManagers.size() - 1]->GetComponentGameObject(ComponentType::TRANSFORM));
	transform->mPosition = { (float)((app->editor->GameWindowSize.x / 2) - (app->editor->GameWindowSize.x / 4.3)), (float)((app->editor->GameWindowSize.y / 2)), 0 };
	transform->UpdateTransformation();
	comp_UI->isChildOfText = true;

	GameObject* gm4 = app->scene->AllGameObjectManagers[app->scene->AllGameObjectManagers.size() - 1];

	comp_UI = comp_UI->CreateGameObjectUI(app->scene->Root, TEXT, app->editor->GameWindowSize.x / 20, app->editor->GameWindowSize.y / 10, 0, 0, nullptr, "DRAGABLE", 1, nullptr, (app->editor->GameWindowSize.x / 2) - (app->editor->GameWindowSize.x / 10), (app->editor->GameWindowSize.y / 1.95), app->editor->GameWindowSize.x / 20, app->editor->GameWindowSize.y / 10);
	transform = dynamic_cast<ComponentTransform*>(app->scene->AllGameObjectManagers[app->scene->AllGameObjectManagers.size() - 1]->GetComponentGameObject(ComponentType::TRANSFORM));
	transform->mPosition = { (float)((app->editor->GameWindowSize.x / 2) - (app->editor->GameWindowSize.x / 10)), (float)((app->editor->GameWindowSize.y / 1.95)), 0 };
	transform->UpdateTransformation();
	comp_UI->isChildOfText = true;

	GameObject* gm5 = app->scene->AllGameObjectManagers[app->scene->AllGameObjectManagers.size() - 1];

	comp_UI = comp_UI->CreateGameObjectUI(app->scene->Root, IMAGE, app->editor->GameWindowSize.x - (app->editor->GameWindowSize.x / 4), app->editor->GameWindowSize.y - (app->editor->GameWindowSize.y / 20), 0, 0, "Assets/Textures/BackgroundPause.jpg", nullptr, 1, nullptr, (app->editor->GameWindowSize.x / 2) - (app->editor->GameWindowSize.x / 2.6), (app->editor->GameWindowSize.y / 20), app->editor->GameWindowSize.x - (app->editor->GameWindowSize.x / 4), app->editor->GameWindowSize.y - (app->editor->GameWindowSize.y / 20));
	transform = dynamic_cast<ComponentTransform*>(app->scene->AllGameObjectManagers[app->scene->AllGameObjectManagers.size() - 1]->GetComponentGameObject(ComponentType::TRANSFORM));
	transform->mPosition = { (float)((app->editor->GameWindowSize.x / 2) - (app->editor->GameWindowSize.x / 2.6)), (float)((app->editor->GameWindowSize.y / 20)), 0 };
	transform->UpdateTransformation();
	comp_UI->isChildOfText = true;

	GameObject* gm6 = app->scene->AllGameObjectManagers[app->scene->AllGameObjectManagers.size() - 1];

	app->scene->AllGameObjectManagers[app->scene->AllGameObjectManagers.size() - 1]->ChangeParent(app->scene->pause);

	gm1->ChangeParent(gm6);
	gm1->isActive = false;
	gm2->ChangeParent(gm6);
	gm2->isActive = false;
	gm3->ChangeParent(gm6);
	gm3->isActive = false;
	gm4->ChangeParent(gm6);
	gm4->isActive = false;
	gm5->ChangeParent(gm6);
	gm5->isActive = false;

	ComponentMaterial* mat = dynamic_cast<ComponentMaterial*>(app->scene->AllGameObjectManagers[app->scene->AllGameObjectManagers.size() - 1]->GetComponentGameObject(ComponentType::MATERIAL));

	mat->colorTexture.a = 0.5;

	app->scene->pause->isActive = false;

	for (int i = 0; i < app->scene->pause->childrens.size(); i++)
	{
		app->scene->pause->childrens[i]->isActive = false;
	}
}