#include "CheckerUI.h"
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

CheckerUI::CheckerUI(UI_Type type, GameObject* gameObject, uint width, uint heigt, uint PosX, uint PosY, const char* imagePathActive, const char* imagePathDisabled) : ComponentUI(type, gameObject, width, heigt, PosX, PosY, imagePathActive)
{
	gmAtached = gameObject;
	actualFunction = _functions::_DEF;
	positionX = PosX;
	positionY = PosY;
	widthPanel = width;
	heigthPanel = heigt;

	if (textureActive != nullptr)
	{
		textureActive = new Texture();
		Importer::ImporterTexture::Load(textureActive, imagePathActive);
	}
	if (textureDisabled != nullptr)
	{
		textureDisabled = new Texture();
		Importer::ImporterTexture::Load(textureDisabled, imagePathDisabled);
	}
}

CheckerUI::~CheckerUI()
{

}

bool CheckerUI::OnClicked()
{
	if (!isDragabble && !isSelected)
	{
		color = float4(0.2, 0.2, 0.2, 0.2);
		isSelected = true;
	}

	return isSelected;
}

bool CheckerUI::OnHover()
{
	if (!isDragabble && !isSelected)
	{
		color = float4(0.5, 0.5, 0.5, 0.5);
	}

	return true;
}

bool CheckerUI::OnClick(ComponentUI* UI_Element)
{
	if (app->renderer3D->cameraGame != nullptr)
	{
		switch (UI_Element->actualChecker)
		{
		case _functions::_VSYNC:
			isSelected = !isSelected;
			app->editor->Vsync = isSelected;
			break;
		case _functions::_DRAG:
			isSelected = !isSelected;
			app->scene->draggable = isSelected;
			break;
		default:
			break;
		}
	}

	ComponentMaterial* mat = dynamic_cast<ComponentMaterial*>(UI_Element->gmAtached->GetComponentGameObject(ComponentType::MATERIAL));

	if(isSelected)
	{
		if (UI_Element->active != nullptr)
		{
			mat->SetTexture(UI_Element->active);
		}
	}
	else
	{
		if (UI_Element->disabled != nullptr)
		{
			mat->SetTexture(UI_Element->disabled);
		}
	}

	return isSelected;
}

void CheckerUI::ShowInfo(ComponentUI* UI_Element)
{
	if (ImGui::TreeNode("Checker"))
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

		switch (UI_Element->actualChecker)
		{
		case _functions::_VSYNC:
			ImGui::Text("Actual action: Vsync");
			break;
		case _functions::_DRAG:
			ImGui::Text("Actual action: Drag");
			break;
		case _functions::_DEF:
			ImGui::Text("Actual action: None");
			break;
		default:
			ImGui::Text("Actual action: None");
			break;
		}

		values* namesOfValues = new values;

		if (ImGui::BeginPopup("Action"))
		{
			for (int j = 0; j < (_functions::_MAX); j++)
			{
				if (ImGui::MenuItem((namesOfValues->nameOfFunctions[j] + std::string("##%s") + to_string(j).c_str()).c_str()))
				{
					switch (j)
					{
					case (int)_functions::_VSYNC:
						UI_Element->actualChecker = _VSYNC;
						break;
					case (int)_functions::_DRAG:
						UI_Element->actualChecker = _DRAG;
						break;
					case (int)_functions::_DEF:
						UI_Element->actualChecker = _DEF;
						break;
					default:
						break;
					}
				}
			}

			ImGui::EndPopup();
		}

		ImGui::Checkbox("State", &isSelected);

		ImGui::TreePop();
	}
}