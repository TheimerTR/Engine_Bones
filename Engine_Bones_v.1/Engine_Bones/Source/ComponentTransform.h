#ifndef COMPONENTTRANSFORM_H_
#define COMPONENTTRANSFORM_H_

#include "ComponentManager.h"

class GameObjectManager;

class ComponentTransform : public ComponentManager
{

public:

	float3 mPosition;
	float3 mScale;
	Quat mRotation;

	float4x4 mGlobalMatrix;
	float4x4 mLocalMatrix;

	float3 gPosition;
	float3 gScale;
	Quat gRotation;

	float3 mRotationEuler; 

	float3 gRotationEuler; 

	bool transform = false; 

public: 

	ComponentTransform(GameObjectManager* gameObject);
	~ComponentTransform();

	bool Update();

	void ShowInfo(); 

	void SetTransform(GameObjectManager* gameObject, float3 position, float3 scale, Quat rotation); 

	void UpdateTransformation(); 

	void UpdateGuizmoTransformation(float4x4& globalTransformation); 

	void UpdateBox(); 

	float3 GetPosition(); 
	float3 GetScale(); 
	Quat GetRotation(); 
	float4x4 GetGlobalMatrix(); 
	const float* GetGlobalTransposed(); 

	
private: 
	float4x4 localTransform = math::float4x4::identity;
	//GameObjectManager* gameObject; 

	//void UpdateMatrix(Transforms* trans) 
	//{
	//	trans = float3x3::FromTRS(trans->position, trans->rotation, scale);
	//}
};


#endif // COMPONENTTRANSFORM_H_