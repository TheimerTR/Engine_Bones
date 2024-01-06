#ifndef GAMEOBJECT_H_
#define GAMEOBJECT_H_

#include "Globals.h"
#include "External/MathGeoLib/include/MathGeoLib.h"
#include <vector>

#include "ResourceManager.h"
#include "TextureManager.h"
#include "ComponentManager.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentTransform.h"
#include "ComponentCamera.h"
#include "C_Math.h"
#include "Application.h"
#include "ModuleScene.h"
#include "ComponentUI.h"

using namespace std;

enum ComponentType; 
class ComponentManager; 
class ComponentTransform; 
class ComponentMaterial;
class ComponentMesh;
class ComponentCamera; 

class GameObject
{
public:

	GameObject(string name, GameObject* parent, int id = 0);
	~GameObject();

	GameObject* mParent;
	ComponentTransform* mTransform = nullptr;
	vector<ComponentManager*> mComponents;
	string mName;
	bool isActive;
	bool isSelected;
	bool isText;

	unsigned long UUID;

	vector<GameObject*> childrens;

	ComponentManager* AddComponent(ComponentType type, UI_Type ui_type = UI_Type::DEF, uint width = 10, uint heigth = 10, uint posX = 10, uint posY = 10, const char* imagePath = nullptr);
	vector<ComponentManager*> GetComponentsGameObject(ComponentType type);
	ComponentManager* GetComponentGameObject(ComponentType type);

	bool isActiveGameObject();

	void Enable();
	void Update();
	void Disable();

	/*void Destroy();*/

	void DeleteComponent(ComponentManager* ptr);
	void DeleteChild(GameObject* gameObject);
	void MoveChildIntoParent(int Key);
	int SearchChildPosInVector();

	void CreateEmptyObject(GameObject* gameObject);

	void ChangeParent(GameObject* gameObject);
};

#endif // GAMEOBJECT_H_
