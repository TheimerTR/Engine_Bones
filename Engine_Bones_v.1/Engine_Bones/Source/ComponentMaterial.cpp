#include "ComponentMaterial.h"
#include "GameObjectManager.h"

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

#include "FileSystemManager.h"

ComponentMaterial::ComponentMaterial(GameObjectManager* gameObject) : ComponentManager(gameObject)
{
	texture = new Texture();
	CM_TextureID = NULL;
	this->UUID = app->RandomIntGenerator();
}

ComponentMaterial::~ComponentMaterial()
{
	if (texture->TextureID != 0)
		glDeleteTextures(1, &texture->TextureID);

	texture->imageFormat = 0;
	texture->imageHeight = 0;
	texture->imageType = 0;
	texture->imageWidth = 0;
	texture->Name = "";
	texture->path = "";
	texture->ShowTextures = false;
	texture->TextureID = 0;

	texture = nullptr;
}

void ComponentMaterial::SetTexture(Texture* T_Texture)
{
	texture = T_Texture;
	if (texture != nullptr)
	{
		CM_TextureID = T_Texture->TextureID;
	}
	else
	{
		CM_TextureID = 0;
	}
}

Texture* ComponentMaterial::GetTexture()
{
	return texture;
}

void ComponentMaterial::Enable()
{
	active = true;
}

bool ComponentMaterial::Update()
{

	return true;
}

void ComponentMaterial::Disable()
{
	active = false;
}

void ComponentMaterial::ShowInfo(Texture* texture, int i)
{
	if (active)
	{
		if (texture != nullptr)
		{
			FileSystem::StringDivide(texture->path, &texture->Name, nullptr);

			if (ImGui::TreeNode("%s##2", texture->Name.c_str()))
			{
				ImGui::Image((void*)texture->TextureID, ImVec2(texture->imageWidth, texture->imageHeight));
				ImGui::Text("Texture path: %s", texture->path);
				ImGui::Text("Texture ID: %d", texture->TextureID);
				ImGui::Text("Texture Size: Width:%d x Height:%d", texture->imageWidth, texture->imageHeight);
				ImGui::Checkbox((std::string("Show Texture##%s") + to_string(i).c_str()).c_str(), &texture->ShowTextures);

				if (ImGui::Button((std::string("Select Texture##%s") + to_string(i).c_str()).c_str()))
				{
					ImGui::OpenPopup((std::string("SelectTexture##%s") + to_string(i).c_str()).c_str());
				}

				if (ImGui::BeginPopup((std::string("SelectTexture##%s") + to_string(i).c_str()).c_str()))
				{
					TexturesManager* texturesManager = new TexturesManager();

					if (ImGui::MenuItem((std::string("House Texture##%s") + to_string(i).c_str()).c_str()))
					{
						if (texture->path != "Assets/Textures/Baker_house.dds")
						{
							this->SetTexture(texturesManager->TexLoader("Assets/Textures/Baker_house.dds"));
						}
					}

					if (ImGui::MenuItem((std::string("Grass Texture##%s") + to_string(i).c_str()).c_str()))
					{
						if (texture->path != "Assets/Textures/Grass.dds")
						{
							this->SetTexture(texturesManager->TexLoader("Assets/Textures/Grass.dds"));
						}
					}

					ImGui::EndPopup();
				}

				if (ImGui::Button((std::string("Delete Texture##%s") + to_string(i).c_str()).c_str()))
				{
					Owner->DeleteComponent(this);
				}

				ImGui::TreePop();
			}
		}
		else
		{
			if (ImGui::TreeNode((std::string("Empty Texture##%s") + to_string(i).c_str()).c_str()))
			{
				if (ImGui::Button((std::string("Select Texture##%s") + to_string(i).c_str()).c_str()))
				{
					ImGui::OpenPopup((std::string("SelectTexture2##%s") + to_string(i).c_str()).c_str());
				}

				if (ImGui::BeginPopup((std::string("SelectTexture2##%s") + to_string(i).c_str()).c_str()))
				{
					TexturesManager* texturesManager = new TexturesManager();

					if (ImGui::MenuItem((std::string("House Texture##%s") + to_string(i).c_str()).c_str()))
					{
						this->SetTexture(texturesManager->TexLoader("Assets/Textures/Baker_house.dds"));
					}

					if (ImGui::MenuItem((std::string("Grass Texture##%s") + to_string(i).c_str()).c_str()))
					{
						this->SetTexture(texturesManager->TexLoader("Assets/Textures/Grass.dds"));
					}

					ImGui::EndPopup();
				}

				ImGui::TreePop();
			}
		}
	}
}
