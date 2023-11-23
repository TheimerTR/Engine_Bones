#pragma once
#include "Module.h"
#include "Globals.h"
#include "External/MathGeoLib/include/Math/float3.h"
#include "External/MathGeoLib/include/Math/float4x4.h"

#include "External/MathGeoLib/include/Geometry/Frustum.h"
//#include "ComponentCamera.h"

//todo: REMOVE this before 1st delivery!!
#include "glmath.h"

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(Application* app, bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	bool CleanUp();

	void Look(const float3 &Position, const float3&Reference, bool RotateAroundReference = false);
	void LookAt(const float3&Spot);
	void Move(const float3&Movement);
	float* GetViewMatrix();

	void Focus(); 

	float4x4 ModuleCamera3D::ViewMatrixOpenGL();

private:

	void CalculateViewMatrix();

public:
	
	//You won't need this after using Frustum
	float3 X, Y, Z, Position, Reference;
	
	//ComponentCamera camera; 

private:

	mat4x4 ViewMatrix;

	Frustum F_Camera;

	//Frustum mMainCamera; Some help here :)
};