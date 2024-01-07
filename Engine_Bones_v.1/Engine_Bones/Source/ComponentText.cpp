#include "ComponentText.h"
#include "Application.h"
#include "ImporterTexture.h"
#include "TextureManager.h"
#include "CanvasUI.h"
#include "Application.h"
#include "ComponentUI.h"
#include "ModuleFont.h"

#include "External/ImGui/misc/cpp/imgui_stdlib.h"
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

ComponentText::ComponentText(UI_Type type, GameObject* gameObject, uint width, uint heigt, uint PosX, uint PosY, const char* Text) : ComponentUI(type, gameObject, width, heigt, PosX, PosY, Text)
{
	gmAtached = gameObject;
	gmAtached->isText = true;
	actualFonts = ARIAL;

	text = Text;

	font = app->font->actualFont;

	hasBeenModified = false;

	widthPanel = width;
	heigthPanel = heigt;

	positionX = PosX;
	positionY = PosY;

	actualText = Text;
	newText = Text;

	DoText();
}

ComponentText::~ComponentText()
{

}
void ComponentText::ShowInfo(ComponentUI* compUI, string actText, string newText, GameObject* gm, FONTS actFont, uint width, uint heigth, uint _posX, uint _posY)
{
	if(actText != "")
	{
		color[3] = 0;
	}
	else
	{
		color[3] = 255;
	}

	if (ImGui::TreeNode("Text"))
	{
		if (ImGui::InputText("Text", &newText, ImGuiInputTextFlags_None))
		{
			hasBeenModified = true;
		}

		if (hasBeenModified)
		{
			if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
			{
				actText = newText;

				RecreateText(actText, gm, width, heigth, _posX, _posY);

				compUI->actualText = actText;
				compUI->newText = actText;

				hasBeenModified = false;
			}

			if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
			{
				newText = actText;
				hasBeenModified = false;
			}
		}

		if (ImGui::Button("Font"))
		{
			ImGui::OpenPopup("Font");
		}

		ComponentUI* ui = dynamic_cast<ComponentUI*>(gm->GetComponentGameObject(ComponentType::UI));

		switch (actFont)
		{
		case FONTS::ARIAL:
			ImGui::Text("Actual font: Arial");
			break;
		case FONTS::ROBOTO:
			ImGui::Text("Actual font: Roboto");
			break;
		case FONTS::ELIANTO:
			ImGui::Text("Actual font: Elianto");
			break;
		default:
			ImGui::Text("Actual font: None");
			break;
		}

		fonts* namesOfActions = new fonts;

		if (ImGui::BeginPopup("Font"))
		{
			for (int j = 0; j < (FONTS::MAX_DEF); j++)
			{
				if (ImGui::MenuItem((namesOfActions->nameOfFunctions[j] + std::string("##%s") + to_string(j).c_str()).c_str()))
				{
					switch (j)
					{
					case (int)FONTS::ARIAL:
						actFont = ARIAL;
						ui->font = app->font->FontLoader(120, "./Assets/Fonts/Arial.ttf");
						break;
					case (int)FONTS::ROBOTO:
						actFont = ROBOTO;
						ui->font = app->font->FontLoader(120, "./Assets/Fonts/Roboto.ttf");
						break;
					case (int)FONTS::ELIANTO:
						actFont = ELIANTO;
						ui->font = app->font->FontLoader(120, "./Assets/Fonts/Elianto.otf");
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

void ComponentText::DoText()
{
	uint size_of_character = widthPanel / text.length();

	for(int i = 0; i < text.length(); i++)
	{
		uint position_of_character = positionX + size_of_character * i;

		ComponentMesh* mesh = dynamic_cast<ComponentMesh*>(gmAtached->AddComponent(ComponentType::MESH));

		float3 vertex[4];
		float2 uv[4];
		uint buffer[3];
		float3 transform = { (float)position_of_character, (float)positionY, 0 };

		CreatePanel(vertex, transform, size_of_character, heigthPanel);

		mesh->Name = "Letter";
		mesh->C_Mesh->num_vertex = 4;
		mesh->C_Mesh->num_index = 6;
		mesh->C_Mesh->num_Tex = 2;

		uv[0] = float2(0, 1);
		uv[1] = float2(1, 1);
		uv[3] = float2(1, 0);
		uv[2] = float2(0, 0);

		GenerateBuffers(buffer, vertex, uv);

		mesh->C_Mesh->VBO = buffer[0];
		mesh->C_Mesh->EBO = buffer[1];
		mesh->C_Mesh->VT = buffer[2];
	}
}
void ComponentText::RecreateText(string new_Text, GameObject* gm, uint width, uint heigth, uint _posX, uint _posY)
{
	gm->DeleteComponentType(ComponentType::MESH);

	uint size_of_character = width / new_Text.length();

	for (int i = 0; i < new_Text.length(); i++)
	{
		uint position_of_character = _posX + size_of_character * i;

		ComponentMesh* mesh = dynamic_cast<ComponentMesh*>(gm->AddComponent(ComponentType::MESH));

		float3 vertex[4];
		float2 uv[4];
		uint buffer[3];
		float3 transform = { (float)position_of_character, (float)_posY, 0 };

		CreatePanel(vertex, transform, size_of_character, heigth);

		mesh->Name = "Letter";
		mesh->C_Mesh->num_vertex = 4;
		mesh->C_Mesh->num_index = 6;
		mesh->C_Mesh->num_Tex = 2;

		uv[0] = float2(0, 1);
		uv[1] = float2(1, 1);
		uv[3] = float2(1, 0);
		uv[2] = float2(0, 0);

		GenerateBuffers(buffer, vertex, uv);

		mesh->C_Mesh->VBO = buffer[0];
		mesh->C_Mesh->EBO = buffer[1];
		mesh->C_Mesh->VT = buffer[2];
	}
}
