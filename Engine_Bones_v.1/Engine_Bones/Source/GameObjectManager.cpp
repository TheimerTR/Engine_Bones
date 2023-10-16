#include "GameObjectManager.h"

#include "ComponentManager.h"
#include "ComponentTransform.h"

GameObjectManager::GameObjectManager()
{

};

GameObjectManager::~GameObjectManager() 
{

};

void GameObjectManager :: AddComponent(ComponentType type)
{

	ComponentTransform* Temp = new ComponentTransform();
	//If existe
	switch (type)
	{
	case ComponentType::TRANSFORM:

		mTransform = Temp;
		mComponents.push_back(mTransform);

		break;
	case ComponentType::MESH:
		break;
	case ComponentType::MATERIAL:
		break;
	default:
		break;
	}
}