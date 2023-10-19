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

struct GameObjects
{
	ComponentTransform* Transform;

	Mesh* Mesh;

	Texture* Texture;
};

class GameObjectManager
{
public:

	GameObjectManager(const char* name, GameObjectManager* parent, int id = 0);
	~GameObjectManager();

	//vector<GameObjects*> AllGameObjects;

	GameObjectManager* mParent;
	ComponentTransform* mTransform = nullptr;
	vector<ComponentManager*> mComponents;
	const char* mName;
	bool isActive;
	bool isSelected;

	vector<GameObjectManager*> childrens;

	ComponentManager* AddComponent(ComponentType type);
	vector<ComponentManager*> GetComponentsGameObject(ComponentType type);
	ComponentManager* GetComponentGameObject(ComponentType type);

	void ActivateGameObject();

	void Enable();
	void Update();
	void Disable();

	void Destroy();

	void DeleteComponent(ComponentManager* ptr);
	void DeleteChild(GameObjectManager* gameObject);

	void ChangeParent(GameObjectManager* gameObject);

	/*void CreateGameObject(Mesh* Mesh, Texture* Texture, ComponentTransform* Transform)
	{
		GameObjects* G_GameObject = new GameObjects();

		G_GameObject->Mesh = Mesh;
		G_GameObject->Texture = Texture;
		G_GameObject->Transform = Transform; 

		AllGameObjects.push_back(G_GameObject);
	}*/

	void Clear_GameObject(GameObjectManager* Obj)
	{
		//std::vector<ComponentManager*> objectMeshes = GetComponentsGameObject(ComponentType::MESH);

		//for (int i = 0; i < objectMeshes.size(); i++)
		//{
		//	ComponentMesh* objectMesh = dynamic_cast<ComponentMesh*>(objectMeshes.at(i));
		//	ComponentMaterial* objectTexture = dynamic_cast<ComponentMaterial*>(app->scene->AllGameObjectManagers[i]->GetComponentGameObject(ComponentType::MATERIAL));

		//	Texture* texture = objectTexture->GetTexture();
		//	Mesh* mesh = objectMesh->C_Mesh;

		//	AssimpManager::Clear_Mesh(mesh);
		//	//Needs clear Texture

		//	for (auto it = app->scene->AllGameObjectManagers.begin(); it != app->scene->AllGameObjectManagers.end();)
		//	{
		//		if (*it == Obj)
		//		{
		//			it = app->scene->AllGameObjectManagers.erase(it);
		//		}
		//		else {
		//			++it;
		//		}
		//	}
		//	
		//	Obj->mParent = nullptr;
		//	Obj->mTransform = nullptr;
		//	C_Math::ClearVec(Obj->mComponents);
		//}
	}
};

#endif // GAMEOBJECTMANAGER_H_
