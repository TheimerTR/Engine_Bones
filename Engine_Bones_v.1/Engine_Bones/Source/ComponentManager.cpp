#include "ComponentManager.h"
#include "GameObjectManager.h"

ComponentManager::ComponentManager()
{
	active = true;
	Type = ComponentType::NONE;
};

ComponentManager::~ComponentManager() 
{


}

