#ifndef COMPONENTCAMERA_H_
#define COMPONENTCAMERA_H_

#include "ComponentManager.h"
#include "AssimpManager.h"

#include "External/MathGeoLib/include/Geometry/Frustum.h"


class GameObjectManager;

class ComponentCamera : public ComponentManager
{

public:

	ComponentCamera(GameObjectManager* gameObject);
	~ComponentCamera();

	bool Update(); 

	void LookAt(const float3& Spot); 
	void Draw(); 
	void EndDraw(); 
	//float* GetViewMatrix();

	void SetRatio(float ratio); 

	math::float4x4 GetViewMatrix();
	math::float4x4 GetProjectionMatrix();

	float3 GetPosition(); 

public:

	Frustum frustum;

private:


};


#endif // COMPONENTMESH_H_