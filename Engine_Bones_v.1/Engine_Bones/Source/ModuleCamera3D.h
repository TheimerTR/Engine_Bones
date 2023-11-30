#pragma once
#include "Module.h"
#include "Globals.h"
#include "External/MathGeoLib/include/Math/float3.h"
#include "External/MathGeoLib/include/Math/float4x4.h"

#include "External/MathGeoLib/include/Geometry/Frustum.h"
#include "ComponentCamera.h"
#include "ComponentMesh.h"
#include "GameObjectManager.h"

//todo: REMOVE this before 1st delivery!!
#include "glmath.h"

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(Application* app, bool start_enabled = true);
	~ModuleCamera3D();
	bool Init();
	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	bool CleanUp();

	void Look(const float3 &Position, const float3 &Reference, bool RotateAroundReference = false);
	void LookAt(const float3 &Spot);
	void Move(const float3 &Movement);
	//float* GetViewMatrix();
	ComponentMesh* CheckForMesh(GameObjectManager* gameObject);

	void Focus(/*const float3 &Spot*/); 

private:

public:
	
	//You won't need this after using Frustum
	float3 X, Y, Z, Position, Reference;
	
	//ComponentCamera camera; 

private:

	ComponentCamera* cameraEditor; 

	//Frustum mMainCamera; Some help here :)
};