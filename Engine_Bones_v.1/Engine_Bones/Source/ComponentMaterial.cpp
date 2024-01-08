#include "ComponentMaterial.h"
#include "GameObject.h"

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

ComponentMaterial::ComponentMaterial(GameObject* gameObject) : ComponentManager(gameObject)
{
	texture = new Texture();
	CM_TextureID = NULL;
	Material = true;
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
	//texture->Name = "";
	//texture->path = "";
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
			string new_Path = "";
			new_Path.append(TEXTURES_FOLDER);
			new_Path.append(texture->Name);

			texture->path = new_Path.c_str();

			if (texture->path != nullptr)
			{
				FileSystem::StringDivide(texture->path, &texture->Name, nullptr);
			}

			if(texture->TextureID == 0)
			{
				texture->Name = "Texture";
			}

			if (ImGui::TreeNode("%s##2", texture->Name.c_str()))
			{
				ImGui::Image((void*)texture->TextureID, ImVec2(texture->imageWidth, texture->imageHeight));
				ImGui::Text("Texture path: %s", texture->path);
				ImGui::Text("Texture ID: %d", texture->TextureID);
				ImGui::Text("Texture Size: Width:%d x Height:%d", texture->imageWidth, texture->imageHeight);
				ImGui::Checkbox((std::string("Show Texture##%s") + to_string(i).c_str()).c_str(), &texture->ShowTextures);

				float col[4] = { colorTexture.r, colorTexture.g, colorTexture.b, colorTexture.a };

				ImGui::ColorEdit4("Material Color", col);

				colorTexture.r = col[0];
				colorTexture.g = col[1];
				colorTexture.b = col[2];
				colorTexture.a = col[3];

				if (ImGui::Button((std::string("Select Texture##%s") + to_string(i).c_str()).c_str()))
				{
					ImGui::OpenPopup((std::string("SelectTexture##%s") + to_string(i).c_str()).c_str());
				}

				if (ImGui::BeginPopup((std::string("SelectTexture##%s") + to_string(i).c_str()).c_str()))
				{
					TexturesManager* texturesManager = new TexturesManager();

					vector<string> fileListCheck;
					vector<string> dirListCheck;
					app->physFSManager->DiscoverFiles("Assets/Textures/", fileListCheck, dirListCheck);

					string TexName = "";

					for (int j = 0; j < fileListCheck.size(); j++)
					{
						if (ImGui::MenuItem((fileListCheck[j] + std::string("##%s") + to_string(j).c_str()).c_str()))
						{
							string pathTex = "Assets/Textures/";

							pathTex.append(fileListCheck[j]);

							if (texture->path != pathTex.c_str())
							{
								texture = ResourceManager::ChangeTexture(pathTex);
								this->SetTexture(texture);
							}
						}

						//if (ImGui::MenuItem((std::string("House Texture##%s") + to_string(i).c_str()).c_str()))
						//{
						//	if (texture->path != "Assets/Textures/Baker_house.dds")
						//	{
						//		AssimpManager::ImportOnlyTexture("Assets/Textures/Baker_house.dds");
						//		//this->SetTexture(texturesManager->TexLoader("Assets/Textures/Baker_house.dds"));
						//	}
						//}

						//if (ImGui::MenuItem((std::string("Grass Texture##%s") + to_string(i).c_str()).c_str()))
						//{
						//	if (texture->path != "Assets/Textures/Grass.dds")
						//	{
						//		AssimpManager::ImportOnlyTexture("Assets/Textures/Grass.dds");
						//		//this->SetTexture(texturesManager->TexLoader("Assets/Textures/Grass.dds"));
						//	}
						//}
					}

					ImGui::EndPopup();
				}

				if (ImGui::Button((std::string("Delete Texture##%s") + to_string(i).c_str()).c_str()))
				{
					if (!app->scene->GameTime.running)
					{
						map<uint32, ResourceElement*>::iterator iterator = app->resource->AllResourcesMap.begin();

						for (iterator; iterator != app->resource->AllResourcesMap.end(); iterator++)
						{
							for (int h = 0; h < iterator->second->ParentsUUID.size(); h++)
							{
								if (Owner->UUID == iterator->second->ParentsUUID[h])
								{
									if (iterator->second->type == ResourceTypes::R_TEXTURE)
									{
										iterator->second->resourceCounter -= 1;
									}
								}
							}
						}

						Owner->DeleteComponent(this);
					}
					else
					{
						LOG(LogTypeCase::L_WARNING, "You are in PLAY mode!");
					}
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

					vector<string> fileListCheck;
					vector<string> dirListCheck;
					app->physFSManager->DiscoverFiles("Assets/Textures/", fileListCheck, dirListCheck);

					string TexName = "";

					for (int j = 0; j < fileListCheck.size(); j++)
					{
						if (ImGui::MenuItem((fileListCheck[j] + std::string("##%s") + to_string(j).c_str()).c_str()))
						{
							string pathTex = "Assets/Textures/";

							pathTex.append(fileListCheck[j]);

							texture = ResourceManager::ChangeTexture(pathTex);
							this->SetTexture(texture);

							colorTexture.r = 1;
							colorTexture.g = 1;
							colorTexture.b = 1;
							colorTexture.a = 1;
						}
					}

					/*if (ImGui::MenuItem((std::string("House Texture##%s") + to_string(i).c_str()).c_str()))
					{
						this->SetTexture(texturesManager->TexLoader("Assets/Textures/Baker_house.dds"));
					}

					if (ImGui::MenuItem((std::string("Grass Texture##%s") + to_string(i).c_str()).c_str()))
					{
						this->SetTexture(texturesManager->TexLoader("Assets/Textures/Grass.dds"));
					}*/

					ImGui::EndPopup();
				}

				ImGui::TreePop();
			}
		}
	}
}
