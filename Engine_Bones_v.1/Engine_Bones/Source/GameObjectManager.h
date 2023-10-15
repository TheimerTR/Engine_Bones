#ifndef GAMEOBJECTMANAGER_H_
#define GAMEOBJECTMANAGER_H_

#include "Globals.h"
#include "External/MathGeoLib/include/MathGeoLib.h"
#include <vector>

#include "AssimpManager.h"
#include "TextureManager.h"

using namespace std;

struct GameObjects
{
	math::float3 Transform;
	math::float3 Rotation;

	Mesh* Mesh;

	Texture* Texture;
};

namespace GameObjectManager
{
	vector<GameObjects*> AllGameObjects;

	void CreateGameObject(Mesh* Mesh, Texture* Texture)
	{
		GameObjects* G_GameObject = new GameObjects();

		G_GameObject->Mesh = Mesh;
		G_GameObject->Texture = Texture;

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
