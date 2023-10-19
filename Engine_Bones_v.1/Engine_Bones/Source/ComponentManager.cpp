#include "ComponentManager.h"
#include "GameObjectManager.h"

ComponentManager::ComponentManager(GameObjectManager* gameObject) : active(true), Owner(gameObject), Type(ComponentType::NONE)
{
};

ComponentManager::~ComponentManager() 
{


}

bool ComponentManager::Update()
{
	return true; 
}

