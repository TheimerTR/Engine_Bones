#include "ComponentTransform.h"
#include "GameObjectManager.h"

ComponentTransform::ComponentTransform(GameObjectManager* gameObject) : ComponentManager(gameObject)
{
	mPosition = float3::zero;
	mScale = float3::zero;
	mrotation = Quat::identity;

	mGlobalMatrix = float4x4::identity;
	mLocalMatrix = float4x4::identity;
};

ComponentTransform::~ComponentTransform()
{

};