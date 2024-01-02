#include "ComponentUI.h"
#include "GameObject.h"

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

ComponentUI::ComponentUI(UI_Type type, GameObject* gameObject, uint width, uint heigth, uint PosX, uint PosY) : ComponentManager(gameObject)
{
	gmAtached = gameObject;

	widthPanel = width;
	heigthPanel = heigth;

	this->UUID = app->RandomIntGenerator();

	texture = new Texture();

	float3 transform;

	transform = gameObject->mTransform->gPosition;

	PlaneInScene->vertex[0] = float3(transform.x, transform.y, transform.z);
	PlaneInScene->vertex[1] = float3(transform.x + width, transform.y + heigth, transform.z);
	PlaneInScene->vertex[2] = float3(transform.x + width, transform.y, transform.z);
	PlaneInScene->vertex[3] = float3(transform.x, transform.y, transform.z);

	PlaneInGame->vertex[0] = float3(PosX, PosY + heigth, gameObject->mTransform->mPosition.z);
	PlaneInGame->vertex[1] = float3(PosX + width, PosY + heigth, gameObject->mTransform->mPosition.z);
	PlaneInGame->vertex[2] = float3(PosX + width, PosY, gameObject->mTransform->mPosition.z);
	PlaneInGame->vertex[3] = float3(PosX, PosY, gameObject->mTransform->mPosition.z);
}


ComponentUI::~ComponentUI()
{
	texture = nullptr;
}

void ComponentUI::SetTexture(Texture* T_Texture)
{

}

Texture* ComponentUI::GetTexture()
{
	return texture;
}

void ComponentUI::Enable()
{

}

bool ComponentUI::Update()
{

	return true;
}

void ComponentUI::Disable()
{
	
}

void ComponentUI::ShowInfo(Texture* texture, int i)
{
	
}

void UIPlane::GenerateBuffers()
{
	uint* index = new uint[6];

	for(int i = 0; i < 6; i++)
	{
		index[i] = i;
	}

	//Cube Vertex
	glGenBuffers(1, &buffer[0]);
	glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float3) * 4, vertex, GL_STATIC_DRAW);

	//Cube Vertex definition
	glGenBuffers(1, &buffer[1]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * 6, index, GL_STATIC_DRAW);

	//UVs definition
	glGenBuffers(1, &buffer[2]);
	glBindBuffer(GL_ARRAY_BUFFER, buffer[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float2) * 4, uv, GL_STATIC_DRAW);
}