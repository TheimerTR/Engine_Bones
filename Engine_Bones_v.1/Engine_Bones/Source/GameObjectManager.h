#ifndef GAMEOBJECTMANAGER_H_
#define GAMEOBJECTMANAGER_H_

#include "Globals.h"
#include "External/MathGeoLib/include/MathGeoLib.h"
#include <vector>

#include "AssimpManager.h"
#include "TextureManager.h"

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

	vector<GameObjects*> AllGameObjects;

	GameObjectManager* mParent;
	ComponentTransform* mTransform;
	vector<ComponentManager*> mComponents;
	const char* mName;
	bool isActive;

	vector<GameObjectManager*> childrens;

	ComponentManager* AddComponent(ComponentType type);

	void ActivateGameObject();

	void Enable();
	void Update();
	void Disable();

	void Destroy();

	void DeleteComponent(ComponentManager* ptr);
	void DeleteChild(GameObjectManager* gameObject);

	void ChangeParent(GameObjectManager* gameObject);

	void CreateGameObject(Mesh* Mesh, Texture* Texture, ComponentTransform* Transform)
	{
		GameObjects* G_GameObject = new GameObjects();

		G_GameObject->Mesh = Mesh;
		G_GameObject->Texture = Texture;
		G_GameObject->Transform = Transform; 

		AllGameObjects.push_back(G_GameObject);
	}

	void Clear_GameObject(GameObjects* Obj)
	{
		AssimpManager::Clear_Mesh(Obj->Mesh);
		//Needs clear Texture

		for (auto it = AllGameObjects.begin(); it != AllGameObjects.end();) 
		{
			if (*it == Obj)
			{
				it = AllGameObjects.erase(it);
			}
			else {
				++it;
			}
		}
	}
};

#endif // GAMEOBJECTMANAGER_H_
