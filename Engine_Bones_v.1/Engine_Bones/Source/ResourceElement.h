#ifndef RESOURCE_ELEMENT_H
#define RESOURCE_ELEMENT_H

#include "Globals.h"
#include "External/MathGeoLib/include/MathGeoLib.h"
#include "ComponentManager.h"
#include <vector>
#include <map>

using namespace std;
class GameObject;

enum class ResourceTypes
{
	R_MESH,
	R_MODEL,
	R_MATERIAL,
	R_TEXTURE,
	R_NONE
};

class ResourceElement
{
public:
	ResourceElement();
	ResourceElement(ResourceTypes R_type);
	ResourceElement(const char* name, const char* AssetsPath, ResourceTypes R_type, uint32 R_UUID);
	~ResourceElement();

	string getAssetsPath();
	string getLibraryPath();

	/*virtual void Enable() {};
	virtual bool Update();
	virtual void Disable() {};*/

public:
	string name;
	uint resourceCounter;
	string AssetsPath;
	string LibraryPath;

	ResourceTypes type;
	uint32 UUID;
	vector<uint32> ParentsUUID;

	vector<string> MeshesChildrensInModel;
	vector<string> ComponentsInModel;
	vector<string> ComponentsNames;
	vector<float> Resource_red;
	vector<float> Resource_green;
	vector<float> Resource_blue;
	vector<float> Resource_alpha;
	vector<GameObject*> AssociatedGameObjects;

public:
	uint32 getUUID();
	ResourceTypes getType();
	uint getResourceCounter();

};

#endif // COMPONENTMANAGER_H_
