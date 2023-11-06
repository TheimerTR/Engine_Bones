#include "ComponentManager.h"
#include "GameObjectManager.h"
#include "Application.h"

ComponentManager::ComponentManager(GameObjectManager* gameObject) : active(true), Owner(gameObject), Type(ComponentType::NONE)
{

};

ComponentManager::~ComponentManager() 
{
	//active = false;
	/*Type = NONE;
	Owner = nullptr;*/
}

bool ComponentManager::Update()
{
	return true; 
}

