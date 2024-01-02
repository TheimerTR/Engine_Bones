#ifndef COMPONENTMANAGER_H_
#define COMPONENTMANAGER_H_

#include "Globals.h"
#include "External/MathGeoLib/include/MathGeoLib.h"
#include <vector>

#include "ResourceManager.h"
#include "TextureManager.h"

class GameObject;

using namespace std;

enum ComponentType 
{
	MESH,
	MATERIAL,
	TRANSFORM,
	SHOWNMATERIAL,
	CAMERA,
	UI,
	CANVAS,
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
	GameObject* Owner;
	long unsigned int UUID;

public:

	ComponentManager(GameObject* gameObject);
	~ComponentManager();

};

#endif // COMPONENTMANAGER_H_