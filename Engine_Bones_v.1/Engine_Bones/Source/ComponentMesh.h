#ifndef COMPONENTMESH_H_
#define COMPONENTMESH_H_

#include "ComponentManager.h"
#include "AssimpManager.h"
#include "External/MathGeoLib/include/Geometry/AABB.h"
#include "External/MathGeoLib/include/Geometry/OBB.h"
#include "ComponentTransform.h"

class GameObjectManager;
class ComponentTransform; 

class ComponentMesh : public ComponentManager
{
public:
	Mesh* C_Mesh;
	const char* Name;
	bool isActive;
	bool isVisible; 

public:

	ComponentMesh(GameObjectManager* gameObject);
	~ComponentMesh();

	void SetMesh(Mesh* T_Mesh);
	Mesh* GetMesh();

	void Enable() override;
	bool Update() override;
	void Disable() override;
	void ShowInfo(Mesh* mesh, int i);

	//AABB GetAABB(); 

	//AABB local_aabb;
	AABB global_aabb;
	OBB  obb;


private:

};


#endif // COMPONENTMESH_H_