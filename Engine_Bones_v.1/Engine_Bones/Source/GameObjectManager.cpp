#include "GameObjectManager.h"

#include "Application.h"
#include "ComponentManager.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ModuleScene.h"

GameObjectManager::GameObjectManager(string name, GameObjectManager* parent, int id) : mParent(parent), mName(name), mTransform(nullptr), isActive(true)
{
	if(parent != nullptr)
	{
		parent->childrens.push_back(this);
	}

	mTransform = dynamic_cast<ComponentTransform*>(AddComponent(ComponentType::TRANSFORM));

	if (app != nullptr)
	{
		app->scene->AllGameObjectManagers.push_back(this);
		app->scene->Selected_GameObject = this;
		app->editor->actualMesh = this;
	}
};

GameObjectManager::~GameObjectManager() 
{
	for(uint i = 0; i < mComponents.size(); i++)
	{
		RELEASE(mComponents[i]); 
	}

	mComponents.clear(); 

	for (uint i = 0; i < childrens.size(); i++)
	{
		RELEASE(childrens[i]);
	}

	childrens.clear(); 
};

ComponentManager* GameObjectManager::AddComponent(ComponentType type)
{
	ComponentManager* Comp = nullptr;

	//If existe
	switch (type)
	{
	case ComponentType::TRANSFORM:
		if (mTransform == nullptr)
		{
			Comp = new ComponentTransform(this);
			Comp->Type = ComponentType::TRANSFORM;
		}
		break;
	case ComponentType::MESH:
		Comp = new ComponentMesh(this);
		Comp->Type = ComponentType::MESH;
		break;
	case ComponentType::MATERIAL:
		Comp = new ComponentMaterial(this);
		Comp->Type = ComponentType::MATERIAL;
		break;
	case ComponentType::NONE:
		LOG(LogTypeCase::L_ERROR, "The Component Type was nullptr");
		Comp->Type = ComponentType::NONE;
		break;
	default:
		break;
	}

	if (Comp != nullptr)
	{
		mComponents.push_back(Comp);
	}

	return Comp;
}

vector<ComponentManager*> GameObjectManager::GetComponentsGameObject(ComponentType type)
{
	std::vector<ComponentManager*> VecComponentsFound;

	for (auto it = mComponents.begin(); it != mComponents.end(); ++it)
	{
		if ((*it)->Type == type) 
		{
			VecComponentsFound.push_back((*it));
		}
	}

	return VecComponentsFound;
}

ComponentManager* GameObjectManager::GetComponentGameObject(ComponentType type)
{
	std::vector<ComponentManager*> objectMaterials;

	if (type != ComponentType::SHOWNMATERIAL)
	{
		objectMaterials = this->GetComponentsGameObject(type);
	}
	else
	{
		objectMaterials = this->GetComponentsGameObject(ComponentType::MATERIAL);
	}

	for (int i = 0; i < objectMaterials.size(); i++)
	{
		ComponentMaterial* object = (ComponentMaterial*)objectMaterials.at(i);
		
		switch (type)
		{
		case ComponentType::MESH:
			if (object->Type == type)
			{
				return (object);
			}
			break;
		case ComponentType::MATERIAL:
			if (object->Type)
			{
				return (object);
			}
			break;
		case ComponentType::SHOWNMATERIAL:
			if (object->Type == ComponentType::MATERIAL && object->texture->ShowTextures)
			{
				return (object);
			}
			break;
		case ComponentType::TRANSFORM:
			if (object->Type == type)
			{
				return (object);
			}
			break;
		default:
			break;
		}
	}

	return nullptr;
}

bool GameObjectManager::isActiveGameObject()
{
	return isActive; 
}

void GameObjectManager::Enable()
{
	isActive = true; 

	if(mParent != nullptr)
	{
		mParent->Enable(); 
	}
}

void GameObjectManager::Disable()
{
	isActive = false;
}

void GameObjectManager::Update() 
{
	GameObjectManager* gameObjects = nullptr; 

	for (uint i = 0; i < app->scene->AllGameObjectManagers.size(); i++)
	{
		gameObjects = app->scene->AllGameObjectManagers[i]; 

		if (gameObjects->isActiveGameObject()) {

			for (uint i = 0; i < gameObjects->mComponents.size(); i++)
			{
				gameObjects->mComponents[i]->Update(); 
			}
		}
	}
}

void GameObjectManager::DeleteComponent(ComponentManager* ptr)
{
	if (ptr != nullptr)
	{
		mComponents.erase(find(mComponents.begin(), mComponents.end(), ptr)); 
		RELEASE(ptr); 
	}
}

void GameObjectManager::DeleteChild(GameObjectManager* gameObject)
{
	if (gameObject != nullptr)
	{
		childrens.erase(find(childrens.begin(), childrens.end(), gameObject));
		RELEASE(gameObject);
	}  
}

void GameObjectManager::ChangeParent(GameObjectManager* gameObject)
{
	if (this->mParent != gameObject)
	{
		mParent->DeleteChild(this);
		mParent = gameObject;
		mParent->childrens.push_back(this);
	}
}
