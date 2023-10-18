#include "GameObjectManager.h"

#include "ComponentManager.h"
#include "ComponentTransform.h"

GameObjectManager::GameObjectManager(const char* name, GameObjectManager* parent, int id) : mParent(parent), mName(name), mTransform(nullptr), isActive(true)
{
	if(parent != nullptr)
	{
		parent->childrens.push_back(this);
	}

	mTransform = dynamic_cast<ComponentTransform*>(AddComponent(ComponentType::TRANSFORM));
	mComponents.push_back(mTransform);
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
		}
		break;
	case ComponentType::MESH:
		break;
	case ComponentType::MATERIAL:
		break;
	case ComponentType::NONE:
		LOG(LogTypeCase::L_ERROR, "The Component Type was nullptr")
		break;
	default:
		break;
	}

	return Comp;
}