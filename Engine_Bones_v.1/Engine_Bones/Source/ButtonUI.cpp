#include "ButtonUI.h"
#include "Application.h"
#include "ImporterTexture.h"
#include "TextureManager.h"

#include "External/Glew/include/glew.h"
#include "External/ImGui/imgui.h"
#include "External/ImGui/backends/imgui_impl_sdl2.h"
#include "External/ImGui/backends/imgui_impl_opengl3.h"
#include <stdio.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include "External/SDL/include/SDL_opengl.h"
#endif

ButtonUI::ButtonUI(UI_Type type, GameObject* gameObject, uint width, uint heigt, uint PosX, uint PosY, const char* imagePath) : ComponentUI(type, gameObject, width, heigt, PosX, PosY, imagePath)
{
	gmAtached = gameObject;

	positionX = PosX;
	positionY = PosY;
}

ButtonUI::~ButtonUI()
{

}

bool ButtonUI::OnClicked()
{
	if(!isDragabble && !isPressed)
	{
		color = float4(0.2, 0.2, 0.2, 0.2);
		isPressed = true;
	}

	return isPressed;
}

bool ButtonUI::OnHover()
{
	if(!isDragabble && !isPressed)
	{
		color = float4(0.5, 0.5, 0.5, 0.5);
	}

	return true;
}

bool ButtonUI::OnClick(int* action)
{
	switch ((functions)*action)
	{
		case functions::PASS_SCENE:
			PassScene();
			break;
		default:
			break;
	}

	return isPressed;
}

void ButtonUI::ShowInfo(int* action)
{
	if (ImGui::TreeNode("Button"))
	{
		if (texture != nullptr)
		{
			ImGui::Image((void*)texture->TextureID, ImVec2(texture->imageWidth, texture->imageHeight));
			ImGui::Text("Texture path: %s", texture->path);
			ImGui::Text("Texture ID: %d", texture->TextureID);
			ImGui::Text("Texture Size: Width:%d x Height:%d", texture->imageWidth, texture->imageHeight);
			ImGui::Checkbox("Show Texture", &texture->ShowTextures);

			color[3] = 0;
		}
		else
		{
			color[3] = 255;
		}

		float col[4] = { color[0], color[1], color[2], color[3] };

		ImGui::ColorEdit4("Material Color", col);

		color[0] = col[0];
		color[1] = col[1];
		color[2] = col[2];
		color[3] = col[3];

		if (ImGui::Button("Action"))
		{
			ImGui::OpenPopup("Action");
		}

		switch ((functions)*action)
		{
		case functions::PASS_SCENE:
			ImGui::Text("Actual action: Pass Scene");
			break;
		case functions::DEFAULT:
			ImGui::Text("Actual action: None");
			break;
		default:
			ImGui::Text("Actual action: None");
			break;
		}

		actions* namesOfActions = new actions;

		if (ImGui::BeginPopup("Action"))
		{
			for (int j = 0; j < (functions::MAX); j++)
			{
				if (ImGui::MenuItem((namesOfActions->nameOfFunctions[j] + std::string("##%s") + to_string(j).c_str()).c_str()))
				{
					switch (j)
					{
					case (int)functions::PASS_SCENE:
						actualFunction = PASS_SCENE;
						*action = 0;
						break;
					case (int)functions::DEFAULT:
						actualFunction = DEFAULT;
						*action = 1;
						break;
					default:
						break;
					}
				}
			}

			ImGui::EndPopup();
		}

		ImGui::TreePop();
	}
}

void ButtonUI::PassScene()
{
	LOG(LogTypeCase::L_CASUAL, "Pass Scene!");

	ResourceManager::ResourceLoader("Assets/ModelsFbx/Street.fbx");
	app->scene->Selected_GameObject = app->scene->AllGameObjectManagers.at(app->scene->AllGameObjectManagers.size() - 1);
	app->editor->actualMesh = app->scene->AllGameObjectManagers.at(app->scene->AllGameObjectManagers.size() - 1);
	app->editor->actualMesh->isSelected = true;

	app->editor->actualResource = app->resource->AllResourcesMap.begin()->second;

	//Active no active para render!!!!
	//Si acitve es false, entonces no renderizar y el mouse no puede interactuar!
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

	/*for (int i = 0; i < objectsUI.size(); i++)
	{
		objectsUI[i]->Owner->mParent->DeleteChild(objectsUI[i]->Owner);
	}*/

	ComponentUI* crosshair = new ComponentUI(UI_Type::IMAGE, app->scene->Root, 40, 40, (uint)app->editor->GameWindowSize.x / 2 - ((uint)app->editor->GameWindowSize.x / 16), ((uint)app->editor->GameWindowSize.y / 2), "Assets/Textures/Crosshair.png");

	crosshair = crosshair->CreateGameObjectUI(UI_Type::IMAGE, 40, 40, (uint)app->editor->GameWindowSize.x / 2 - ((uint)app->editor->GameWindowSize.x / 16), ((uint)app->editor->GameWindowSize.y / 2), "Assets/Textures/Crosshair.png");
	app->scene->AllGameObjectManagers[app->scene->AllGameObjectManagers.size() - 1]->mName = "Crosshair";
	ComponentMaterial* mat = dynamic_cast<ComponentMaterial*>(app->scene->AllGameObjectManagers[app->scene->AllGameObjectManagers.size() - 1]->GetComponentGameObject(ComponentType::MATERIAL));
	mat->colorTexture.r = 255;
	mat->colorTexture.g = 0;
	mat->colorTexture.b = 0;
}