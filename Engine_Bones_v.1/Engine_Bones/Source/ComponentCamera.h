#ifndef COMPONENTCAMERA_H_
#define COMPONENTCAMERA_H_

#include "ComponentManager.h"
#include "AssimpManager.h"


class GameObjectManager;

class ComponentCamera : public ComponentManager
{

public:

	ComponentCamera(GameObjectManager* gameObject);
	~ComponentCamera();

	void LookAt(const float3& Spot); 
	void Draw(); 
	//float* GetViewMatrix();

	void SetRatio(float ratio); 

	float* GetViewMatrix();
	float* GetProjectionMatrix(); 

public:

	Frustum frustum;

private:


};


#endif // COMPONENTMESH_H_