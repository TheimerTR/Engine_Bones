#ifndef COMPONENTCAMERA_H_
#define COMPONENTCAMERA_H_

#include "ComponentManager.h"
#include "ResourceManager.h"

#include "External/MathGeoLib/include/Geometry/Frustum.h"


class GameObject;

class ComponentCamera : public ComponentManager
{

public:

	ComponentCamera(GameObject* gameObject);
	~ComponentCamera();

	bool Update(); 
	void UpdateFrustum();
	void UpdateProjection();

	void LookAt(const float3& Spot); 
	void Draw(); 
	void EndDraw(); 

	void ShowCameraInfo();

	void SetPos(float3 pos);
	void SetRatio(float ratio); 
	void SetFov(float vertical);

	float* GetViewMatrix() const;
	float* GetProjectionMatrix() const;

	float3 GetPosition(); 

	void FrameBuffer(int w, int h); 

	void Culling();
	bool InsideCulling(const AABB& box); 
	void UnCull();

public:

	Frustum frustum;

	uint frameID = 0;
	uint frameTexture = 0;
	uint rbo = 0;
	uint frameDepth = 0;

	bool culling = true; 
	bool uncull = false; 
	bool activeGame; 

private:
	float width = 0.0f;
	float height = 0.0f;
	float aspect_ratio = 0.0f;
	float near_plane = 0.0f;
	float far_plane = 0.0f;
	float vertical_fov = 0.0f;

private:


};


#endif // COMPONENTMESH_H_