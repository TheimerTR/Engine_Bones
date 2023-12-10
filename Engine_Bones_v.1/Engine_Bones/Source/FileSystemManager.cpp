#include "FileSystemManager.h"

#include<stdlib.h>
#include<time.h>
#include<vector>
#include<string>

#include "Globals.h"
#include "Application.h"
#include "AssimpManager.h"
#include "ModuleEditor.h"
#include "TextureManager.h"
#include "ComponentManager.h"
#include "ComponentMaterial.h"
#include "ResourceElement.h"
#include "ResourceTexture.h"
#include "ImporterTexture.h"

void FileSystem::ReadFyleType(const char* Path)
{
	string file;
	string extension;
	string pathNomralized;

	if (Path != nullptr)
	{
		StringDivide(Path, &file, &extension);

		FileType Type = FileType::DEFAULT;

		Type = ExtensionType(extension);

		if (!app->scene->GameTime.running)
		{
			switch (Type)
			{
			case FileType::MODEL_3D:
				file = "Assets/ModelsFbx/" + file;
				//pathNomralized = app->physFSManager->NormalizePath(Path);
				AssimpManager::AssimpLoader(Path);
				LOG(LogTypeCase::L_CASUAL, "Model Type: MODEL 3D");
				break;
			case FileType::TEXTURE:
				file = "Assets/Textures/" + file;
				if (app->editor->actualMesh->isSelected)
				{
					AssimpManager::ImportOnlyTexture(Path);

					/*TexturesManager* texturesManager = new TexturesManager();
					std::vector<ComponentManager*> objectMaterials = (app->editor->actualMesh->GetComponentsGameObject(ComponentType::MATERIAL));

					if (objectMaterials.size() != 0)
					{
						ComponentMaterial* objectTexture = dynamic_cast<ComponentMaterial*>(app->editor->actualMesh->AddComponent(ComponentType::MATERIAL));
						objectTexture->SetTexture(texturesManager->TexLoader(Path));
					}
					else
					{
						ComponentMaterial* objectTexture = dynamic_cast<ComponentMaterial*>(app->editor->actualMesh->AddComponent(ComponentType::MATERIAL));
						objectTexture->SetTexture(texturesManager->TexLoader(Path));
					}*/

					//ComponentMaterial* objectTexture = dynamic_cast<ComponentMaterial*>(app->editor->actualMesh->GetComponentGameObject(ComponentType::MATERIAL));
					//objectTexture->GetTexture()->TextureID = texturesManager->TexLoader(Path)->TextureID;
				}
				LOG(LogTypeCase::L_CASUAL, "Model Type: TEXTURE");
				break;
			case FileType::DEFAULT:
				LOG(LogTypeCase::L_WARNING, "Model Type: DEFAULT");
				break;
			default:
				break;
			}
		}
		else
		{
			LOG(LogTypeCase::L_ERROR, "The file was not imported: %s", Path);
		}
	}
	else
	{
		LOG(LogTypeCase::L_WARNING, "You are in PLAY mode!");
	}
}

void FileSystem::StringDivide(const char* OG_Path, string* file, string* extension)
{
	string Div_String(OG_Path);
	CompatibleString(Div_String);

	size_t pos_File = Div_String.find_last_of("\\/");
	size_t pos_Extension = Div_String.find_last_of(".");

	//if (pos_File < Div_String.length())
	//{
	//	OG_Path = Div_String.substr(0, pos_File + 1).c_str();
	//}

	if (file != nullptr)
	{
		if (pos_File < Div_String.length())
		{
			*file = Div_String.substr(pos_File + 1);
		}
		else
		{
			*file = Div_String;
		}
	}
	
	if (extension != nullptr)
	{
		if (pos_Extension < Div_String.length())
		{
			*extension = Div_String.substr(pos_Extension + 1);
		}
		else
		{
			extension->clear();
		}
	}
}

void FileSystem::CompatibleString(string& ConvertPath)
{
	for(int i = 0; i < ConvertPath.size(); ++i)
	{
		if(ConvertPath[i] == '\\')
		{
			ConvertPath[i] = '/';
		}
	}
}

FileType FileSystem::ExtensionType(string Extension)
{
	FileType _type = FileType::DEFAULT;

	if(strcmp(Extension.data(), "fbx") == 0)
	{
		_type = FileType::MODEL_3D;
		LOG(LogTypeCase::L_CASUAL, "Model Extension .fbx");
	}
	else if(strcmp(Extension.data(), "FBX") == 0)
	{
		_type = FileType::MODEL_3D;
		LOG(LogTypeCase::L_CASUAL, "Model Extension .FBX");
	}
	else if(strcmp(Extension.data(), "obj") == 0)
	{
		_type = FileType::MODEL_3D;
		LOG(LogTypeCase::L_CASUAL, "Model Extension .obj");
	}
	else if(strcmp(Extension.data(), "png"))
	{
		_type = FileType::TEXTURE;
		LOG(LogTypeCase::L_CASUAL, "Model Extension .png");
	}
	else if(strcmp(Extension.data(), "jpg"))
	{
		_type = FileType::TEXTURE;
		LOG(LogTypeCase::L_CASUAL, "Model Extension .jpg");
	}
	else if(strcmp(Extension.data(), "dds"))
	{
		_type = FileType::TEXTURE;
		LOG(LogTypeCase::L_CASUAL, "Model Extension .dds");
	}
	else if(strcmp(Extension.data(), "tga"))
	{
		_type = FileType::TEXTURE;
		LOG(LogTypeCase::L_CASUAL, "Model Extension .tga");
	}

	return _type;
}