#include "ComponentManager.h"
#include "GameObject.h"
#include "Application.h"

ComponentManager::ComponentManager(GameObject* gameObject) : active(true), Owner(gameObject), Type(ComponentType::NONE)
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

