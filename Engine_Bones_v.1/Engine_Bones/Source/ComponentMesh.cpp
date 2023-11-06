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

ComponentMesh::ComponentMesh(GameObjectManager* gameObject) : ComponentManager(gameObject)
{
	C_Mesh = new Mesh();
	Name = "";
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
			Owner->mParent->DeleteComponent(this);
		}
	}
}
