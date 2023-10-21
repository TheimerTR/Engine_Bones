#include "ComponentManager.h"
#include "GameObjectManager.h"

ComponentManager::ComponentManager(GameObjectManager* gameObject) : active(true), Owner(gameObject), Type(ComponentType::NONE)
{
};

ComponentManager::~ComponentManager() 
{
	//switch(this->Type)
	//{
	//case ComponentType::MESH:
	//	delete ComponentMesh(this);
	//	break;
	//}
}

bool ComponentManager::Update()
{
	return true; 
}

