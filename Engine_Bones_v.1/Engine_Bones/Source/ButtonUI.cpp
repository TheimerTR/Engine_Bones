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

bool ButtonUI::OnClick()
{
	switch (actualFunction)
	{
		case functions::PASS_SCENE:
			PassScene();
			break;
		default:
			break;
	}

	return isPressed;
}

void ButtonUI::ShowInfo()
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

		switch (actualFunction)
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
			for (int j = 0; j < (functions::MAX - 1); j++)
			{
				if (ImGui::MenuItem((namesOfActions->nameOfFunctions[j] + std::string("##%s") + to_string(j).c_str()).c_str()))
				{
					switch (j)
					{
					case (int)functions::PASS_SCENE:
						actualFunction = PASS_SCENE;
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
}