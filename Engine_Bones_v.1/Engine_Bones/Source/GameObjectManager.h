#ifndef GAMEOBJECTMANAGER_H_
#define GAMEOBJECTMANAGER_H_

#include "Globals.h"
#include "External/MathGeoLib/include/MathGeoLib.h"
#include <vector>

#include "AssimpManager.h"
#include "TextureManager.h"
#include "ComponentManager.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentTransform.h"
#include "C_Math.h"
#include "Application.h"
#include "ModuleScene.h"

using namespace std;

enum ComponentType; 
class ComponentManager; 
class ComponentTransform; 
class ComponentMaterial;
class ComponentMesh;

class GameObjectManager
{
public:

	GameObjectManager(string name, GameObjectManager* parent, int id = 0);
	~GameObjectManager();

	GameObjectManager* mParent;
	ComponentTransform* mTransform = nullptr;
	vector<ComponentManager*> mComponents;
	string mName;
	bool isActive;
	bool isSelected;

	vector<GameObjectManager*> childrens;

	ComponentManager* AddComponent(ComponentType type);
	vector<ComponentManager*> GetComponentsGameObject(ComponentType type);
	ComponentManager* GetComponentGameObject(ComponentType type);

	bool isActiveGameObject();

	void Enable();
	void Update();
	void Disable();

	/*void Destroy();*/

	void DeleteComponent(ComponentManager* ptr);
	void DeleteChild(GameObjectManager* gameObject);
	void MoveChildIntoParent(int Key);
	int SearchChildPosInVector();

	void ChangeParent(GameObjectManager* gameObject);
};

#endif // GAMEOBJECTMANAGER_H_
