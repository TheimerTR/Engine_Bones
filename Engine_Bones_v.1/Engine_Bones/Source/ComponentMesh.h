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
	bool isActive;

public:

	ComponentMesh(GameObjectManager* gameObject);
	~ComponentMesh();

	void SetMesh(Mesh* T_Mesh);
	Mesh* GetMesh();

	void Enable() override;
	bool Update() override;
	void Disable() override;
	void ShowInfo(Mesh* mesh, int i);

private:

};


#endif // COMPONENTMESH_H_