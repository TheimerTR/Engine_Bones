#pragma once
#include "Module.h"
#include "Globals.h"
#include "Light.h"

#include "MathGeoLib/include/Math/float3x3.h"
#include "MathGeoLib/include/Math/float4x4.h"
#include "Primitive.h"

#include <vector>
#include <stdio.h>

#include <iostream>

//todo: REMOVE this before 1st delivery!!
#include "glmath.h"

#define MAX_LIGHTS 8

enum class PrimitiveType
{
	NONE, CUBE_DIRECT_MODE, CUBE_ARRAY, CUBE_ELEMENT, CYLINDER, PLANE
};

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(Application* app, bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void OnResize(int width, int height);

	void RenderPrimitive(PrimitiveType Type);

public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	CPlane Grid;

	bool OpenWindow; 
	bool Wireframe;

	uint VBO;
	uint EBO;
	uint VAO;
	
	//You won't need this after using Frustum
	mat4x4 ProjectionMatrix;
};