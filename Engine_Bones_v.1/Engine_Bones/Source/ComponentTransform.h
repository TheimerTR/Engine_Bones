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

public: 

	ComponentTransform(GameObjectManager* gameObject);
	~ComponentTransform();

	void ShowInfo(); 


private: 

	//void UpdateMatrix(Transforms* trans) 
	//{
	//	trans = float3x3::FromTRS(trans->position, trans->rotation, scale);
	//}
};


#endif // COMPONENTTRANSFORM_H_