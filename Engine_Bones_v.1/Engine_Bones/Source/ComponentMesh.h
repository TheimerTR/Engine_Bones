#ifndef COMPONENTMESH_H_
#define COMPONENTMESH_H_

#include "ComponentManager.h"
#include "AssimpManager.h"

class GameObjectManager;

class ComponentMesh : public ComponentManager
{
public:
	Mesh* C_Mesh;
	const char* Name;

public:

	ComponentMesh(GameObjectManager* gameObject);
	~ComponentMesh();

	void SetMesh(Mesh* T_Mesh);
	Mesh* GetMesh();

private:

};


#endif // COMPONENTMESH_H_