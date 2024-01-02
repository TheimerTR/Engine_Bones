#include "CanvasUI.h"
#include "GameObject.h"
#include "ComponentTransform.h"

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

CanvasUI::CanvasUI(GameObject* gameObject, uint width, uint heigth, uint PosX, uint PosY) : ComponentManager(gameObject)
{
	gmAtached = gameObject;

	widthPanel = width;
	heigthPanel = heigth;

	this->UUID = app->RandomIntGenerator();
}


CanvasUI::~CanvasUI()
{

}

void CanvasUI::Enable()
{

}

bool CanvasUI::Update()
{
	return true;
}

void CanvasUI::Disable()
{

}

void CanvasUI::Draw()
{
	ComponentTransform* comp_transform = dynamic_cast<ComponentTransform*>(gmAtached->GetComponentGameObject(ComponentType::TRANSFORM));

	glBegin(GL_LINE_LOOP);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	float3 pos = comp_transform->mPosition;

	glVertex3f(pos.x, pos.y, pos.z);
	glVertex3f(pos.x, (pos.y + heigthPanel), pos.z);
	glVertex3f((pos.x + widthPanel), (pos.y + heigthPanel), pos.z);
	glVertex3f((pos.x + widthPanel), pos.y, pos.z);

	glEnd();
}