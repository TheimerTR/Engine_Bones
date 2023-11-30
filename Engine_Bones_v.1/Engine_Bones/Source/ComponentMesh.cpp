#include "ComponentMesh.h"
#include "GameObjectManager.h"

#include "External/Glew/include/glew.h"
#include "External/ImGui/imgui.h"
#include "External/ImGui/backends/imgui_impl_sdl2.h"
#include "External/ImGui/backends/imgui_impl_opengl3.h"
#include <stdio.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include "External/SDL/include/SDL_opengl.h"
#endif

#include "FileSystemManager.h"
#include "ComponentTransform.h"

ComponentMesh::ComponentMesh(GameObjectManager* gameObject) : ComponentManager(gameObject)
{
	C_Mesh = new Mesh();
	Name = gameObject->mName.c_str();
	isActive = true;
	this->UUID = app->RandomIntGenerator();
}

ComponentMesh::~ComponentMesh()
{
	AssimpManager::Clear_Mesh(C_Mesh);
	Name = "";
}

void ComponentMesh::SetMesh(Mesh* T_Mesh)
{
	C_Mesh = T_Mesh;
	
	C_Mesh->local_aabb.SetNegativeInfinity(); 
	C_Mesh->local_aabb.Enclose((float3*)C_Mesh->vertex, C_Mesh->num_vertex); 

	obb = C_Mesh->local_aabb;
	obb.Transform(Owner->mTransform->GetGlobalMatrix()); 
	
	global_aabb.SetNegativeInfinity();
	global_aabb.Enclose(obb); 

	//ComponentTransform* transformation = dynamic_cast<ComponentTransform*>(app->editor->actualMesh->GetComponentGameObject(ComponentType::TRANSFORM));
	//transformation->transform = true;

	Owner->mTransform->transform = true; 

}

Mesh* ComponentMesh::GetMesh()
{
	return C_Mesh;
}

void ComponentMesh::Enable()
{
	this->isActive = true;
}

bool ComponentMesh::Update()
{
	//float3 points[8];
	//global_aabb.GetCornerPoints(points);
	//app->renderer3D->DrawBox(points, float3(245.f, 16.f, 16.f));

	//float3 points2[8];
	//obb.GetCornerPoints(points2);
	//app->renderer3D->DrawBox(points2, float3(0.0f, 152.f, 70.f));

	glBegin(GL_LINES); 
	glLineWidth(3.0f); 
	glColor4f(0.0f, 152.f, 70.f, 1.0f); 

	for (uint i = 0; i < 12; i++)
	{
		glVertex3d(obb.Edge(i).a.x, obb.Edge(i).a.y, obb.Edge(i).a.z); 
		glVertex3d(obb.Edge(i).b.x, obb.Edge(i).b.y, obb.Edge(i).b.z); 
	}

	glColor4f(245.f, 16.f, 16.f, 1.0f);

	for (uint i = 0; i < 12; i++)
	{
		glVertex3d(global_aabb.Edge(i).a.x, global_aabb.Edge(i).a.y, global_aabb.Edge(i).a.z);
		glVertex3d(global_aabb.Edge(i).b.x, global_aabb.Edge(i).b.y, global_aabb.Edge(i).b.z);
	}

	glEnd(); 
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f); 

	//Owner->mTransform->UpdateBox(); 

	return true;
}

void ComponentMesh::Disable()
{
	this->isActive = false;
}

void ComponentMesh::ShowInfo(Mesh* mesh, int i)
{
	ImGui::Checkbox((std::string("Active##%s") + to_string(i).c_str()).c_str(), &this->isActive);
	ImGui::Text("%s", mesh->Name.c_str());

	if (this->isActive)
	{	
		ImGui::Text("Path: %s", mesh->Path.c_str());
		ImGui::Text("Mesh UUID: %d", this->UUID);
		ImGui::Text("Number of index: %d", mesh->num_index);
		ImGui::Text("Number of vertex: %d", mesh->num_vertex);
		ImGui::Checkbox((std::string("Show Normals##%s") + to_string(i).c_str()).c_str(), &mesh->ShowNormals);

		if (ImGui::Button((std::string("Delete Mesh##%s") + to_string(i).c_str()).c_str()))
		{
			map<uint32, ResourceElement*>::iterator iterator = app->resource->AllResourcesMap.begin();

			for (iterator; iterator != app->resource->AllResourcesMap.end(); iterator++)
			{
				for (int h = 0; h < iterator->second->ParentsUUID.size(); h++)
				{
					if (Owner->UUID == iterator->second->ParentsUUID[h])
					{
						if (iterator->second->type == ResourceTypes::R_MESH)
						{
							iterator->second->resourceCounter -= 1;
						}
					}
				}
			}

			Owner->DeleteComponent(this);
		}
	}
}

//AABB ComponentMesh::GetAABB() {
//
//	return local_aabb; 
//}
