#ifndef COMPONENTMANAGER_H_
#define COMPONENTMANAGER_H_

#include "Globals.h"
#include "External/MathGeoLib/include/MathGeoLib.h"
#include <vector>

#include "AssimpManager.h"
#include "TextureManager.h"

class GameObjectManager;

using namespace std;

enum ComponentType 
{
	TRANSFORM,
	MESH,
	MATERIAL,
	SHOWNMATERIAL,
	CAMERA,
	NONE,
};

class ComponentManager
{
public:

	/*virtual void RenderInspector() {};*/

	virtual void Enable() {};
	virtual bool Update();
	virtual void Disable() {};

public:
	bool active;
	ComponentType Type;
	GameObjectManager* Owner;
	long unsigned int UUID;

public:

	ComponentManager(GameObjectManager* gameObject);
	~ComponentManager();

};

#endif // COMPONENTMANAGER_H_