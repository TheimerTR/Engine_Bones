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

	widthPanel = width;
	heigthPanel = heigt;

	isPressed = false;
}

ButtonUI::~ButtonUI()
{
}

bool ButtonUI::OnClicked()
{

	return true;
}

bool ButtonUI::OnIdle(ComponentUI* UI_Element)
{
	if (!UI_Element->isSelected && !app->scene->isFading)
	{
		color = float4(1, 1, 1, 1);

		/*	float col[4] = { 0.5, 0.5, 0.5, 0.5 };*/
		ComponentMaterial* mat = dynamic_cast<ComponentMaterial*>(UI_Element->gmAtached->GetComponentGameObject(ComponentType::MATERIAL));
		mat->colorTexture.r = 1;
		mat->colorTexture.g = 1;
		mat->colorTexture.b = 1;
		mat->colorTexture.a = 1;
	}

	return false;
}

bool ButtonUI::OnHover(ComponentUI* UI_Element)
{
	if(!app->scene->isFading)
	{
		color = float4(0.5, 0.5, 0.5, 0.5);

		/*	float col[4] = { 0.5, 0.5, 0.5, 0.5 };*/
		ComponentMaterial* mat = dynamic_cast<ComponentMaterial*>(UI_Element->gmAtached->GetComponentGameObject(ComponentType::MATERIAL));
		mat->colorTexture.r = 0; 
		mat->colorTexture.g = 1; 
		mat->colorTexture.b = 0; 
		mat->colorTexture.a = 1; 
	}

	return true;
}

bool ButtonUI::OnClick(int* action)
{
	switch ((functions)*action)
	{
		case functions::PASS_SCENE:
			app->scene->isFading = true;
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
		ImGui::Checkbox("Active", &gmAtached->isActive);

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
			color[3] = 50;
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
