#pragma once
#include "Module.h"
#include "Globals.h"
#include "Light.h"

#include "External/MathGeoLib/include/Math/float3x3.h"
#include "External/MathGeoLib/include/Math/float4x4.h"
#include "Primitive.h"

#include "ResourceManager.h"
#include "GameObject.h"
#include "TextureManager.h"

#include "ComponentUI.h"

#include <vector>
#include <stdio.h>

#include <iostream>

//todo: REMOVE this before 1st delivery!!

#define MAX_LIGHTS 8

enum class PrimitiveType
{
	NONE, CUBE, CYLINDER, PLANE
};

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(Application* app, bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init();

	bool Start(); 

	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void OnResize(int width, int height);

	void RenderDraw(ComponentMesh* mesh, ComponentTransform* transform, ComponentMaterial* texture = nullptr);
	void RenderUI(GameObject* gm, ComponentUI* UI_Element, bool isGameMode, ComponentMaterial* texture = nullptr);

	void SetCameraEditor(ComponentCamera* camera); 

	void SetCameraGame(ComponentCamera* game); 

	void CheckingActiveGame(); 

	//void UpdateProjection(ComponentCamera* cam);

public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	CPlane Grid;

	bool OpenWindow; 
	bool Wireframe;

	ComponentCamera* ActiveCameraEditor = nullptr; 
	ComponentCamera* cameraGame = nullptr; 
};