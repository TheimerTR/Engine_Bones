#include "ComponentMesh.h"
#include "GameObjectManager.h"

ComponentMesh::ComponentMesh(GameObjectManager* gameObject) : ComponentManager(gameObject)
{
	C_Mesh = new Mesh();
	Name = "";
}

ComponentMesh::~ComponentMesh()
{
	AssimpManager::Clear_Mesh(C_Mesh);
}

void ComponentMesh::SetMesh(Mesh* T_Mesh)
{
	C_Mesh = T_Mesh;
}

Mesh* ComponentMesh::GetMesh()
{
	return C_Mesh;
}
