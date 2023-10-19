#include "GameObjectManager.h"

#include "Application.h"
#include "ComponentManager.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ModuleScene.h"

GameObjectManager::GameObjectManager(const char* name, GameObjectManager* parent, int id) : mParent(parent), mName(name), mTransform(nullptr), isActive(true)
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
	}
};

GameObjectManager::~GameObjectManager() 
{

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
	for (auto it = mComponents.begin(); it != mComponents.end(); ++it)
	{
		if ((*it)->Type == type)
		{
			return (*it);
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
