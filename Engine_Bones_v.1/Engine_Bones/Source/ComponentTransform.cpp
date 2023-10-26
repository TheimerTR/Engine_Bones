#include "ComponentTransform.h"
#include "GameObjectManager.h"
#include "External/ImGui/imgui.h"


ComponentTransform::ComponentTransform(GameObjectManager* gameObject) : ComponentManager(gameObject)
{
	mPosition = float3::zero;
	mScale = float3::zero;
	mRotation = Quat::identity;

	mGlobalMatrix = float4x4::identity;
	mLocalMatrix = float4x4::identity;
};

ComponentTransform::~ComponentTransform()
{
	mPosition = float3::zero;
	mScale = float3::zero;
	mRotation = Quat::identity;

	mGlobalMatrix = float4x4::identity;
	mLocalMatrix = float4x4::identity;
};

void ComponentTransform::ShowInfo() 
{

	if (ImGui::TreeNode("Transform##1"))
	{
		ImGui::Text("Local Position"); 
		ImGui::InputFloat3("Position",(float*)&mPosition);
		ImGui::InputFloat3("Scale",(float*)&mScale);
		ImGui::InputFloat3("Rotation",(float*)&mRotation);

		float3 gPosition, gScale; 
		Quat gRotation; 

		mGlobalMatrix.Decompose(gPosition, gRotation, gScale);

		ImGui::Text("Global Position"); 
		ImGui::InputFloat3("Position",(float*)&gPosition); 
		ImGui::InputFloat3("Scale",(float*)&gScale); 
		ImGui::InputFloat3("Rotation",(float*)&gRotation); 

		ImGui::TreePop(); 
	}

}

void ComponentTransform::SetTransform(GameObjectManager* gameObject, float3 position, float3 scale, Quat rotation)
{
	mPosition = position; 
	mScale = scale; 
	mRotation = rotation;

	mLocalMatrix = float4x4::FromTRS(mPosition, mRotation, mScale); 

	if (gameObject->mParent != nullptr)
	{
		gameObject->mTransform->mGlobalMatrix = gameObject->mParent->mTransform->mGlobalMatrix + mLocalMatrix;
	}
}

float3 ComponentTransform::GetPosition() {

	return mPosition; 
}

float3 ComponentTransform::GetScale() {

	return mScale;
}

Quat ComponentTransform::GetRotation() {

	return mRotation;
}

//void ComponentTransform::UpdateTransformation() 
//{
//	mLocalMatrix = float4x4::FromTRS(mPosition, mRotation, mScale);
//
//}

