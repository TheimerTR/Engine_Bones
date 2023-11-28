#include "ComponentTransform.h"
#include "GameObjectManager.h"
#include "External/ImGui/imgui.h"
#include "Application.h"


ComponentTransform::ComponentTransform(GameObjectManager* gameObject) : ComponentManager(gameObject)
{
	mPosition = float3::zero;
	mScale = float3::zero;
	mRotation = Quat::identity;

	mRotationEuler = mRotation.ToEulerXYZ(); 

	gPosition = float3::zero; 
	gScale = float3::zero; 
	gRotation = Quat::identity; 

	gRotationEuler = gRotation.ToEulerXYZ(); 

	mGlobalMatrix = float4x4::identity;
	mLocalMatrix = float4x4::identity;

};

ComponentTransform::~ComponentTransform()
{
	mPosition = float3::zero;
	mScale = float3::zero;
	mRotation = Quat::identity;

	gPosition = float3::zero;
	gScale = float3::zero;
	gRotation = Quat::identity;

	mGlobalMatrix = float4x4::identity;
	mLocalMatrix = float4x4::identity;
};

void ComponentTransform::ShowInfo() 
{

	if (ImGui::TreeNode("Transform##1"))
	{
		//ImGui::Text("Local Position"); 
		//ImGui::InputFloat3("Position",(float*)&mPosition);
		//ImGui::InputFloat3("Scale",(float*)&mScale);
		//ImGui::InputFloat3("Rotation",(float*)&mRotation);

	

		if (ImGui::DragFloat3("Position", &mPosition[0], 0.1f))
		{
			UpdateTransformation();
		}

		if (ImGui::DragFloat3("Rotation", &mRotationEuler[0], 0.1f))
		{
			UpdateTransformation();
		}

		if (ImGui::DragFloat3("Scale", &mScale[0], 0.1f))
		{
			UpdateTransformation();
		}

		ImGui::Text("Global Position"); 

		mGlobalMatrix.Decompose(gPosition, gRotation, gScale); 

		ImGui::Text("Position %f, %f, %f", gPosition.x, gPosition.y, gPosition.z);
		ImGui::Text("Rotation %f, %f, %f", gRotationEuler.x, gRotationEuler.y, gRotationEuler.z);
		ImGui::Text("Scale %f, %f, %f", gScale.x, gScale.y, gScale.z);

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

float4x4 ComponentTransform::GetGlobalMatrix() {
	return mGlobalMatrix; 
}

const float* ComponentTransform::GetGlobalTransposed() 
{
	return mGlobalMatrix.Transposed().ptr(); 
}

void ComponentTransform::UpdateTransformation() 
{
	mRotation = Quat::FromEulerXYZ(mRotationEuler.x * DEGTORAD, mRotationEuler.y * DEGTORAD, mRotationEuler.z * DEGTORAD); 
	
	mLocalMatrix = float4x4::FromTRS(mPosition, mRotation, mScale);

	//mGlobalMatrix = float4x4::FromTRS(gPosition, gRotation, gScale);

	for (int i = 1; i < app->scene->AllGameObjectManagers.size(); i++) {

		ComponentTransform* objectsToTransform = dynamic_cast<ComponentTransform*>(app->scene->AllGameObjectManagers[i]->GetComponentGameObject(ComponentType::TRANSFORM));

		ComponentTransform* parent = objectsToTransform->Owner->mParent->mTransform; 

		objectsToTransform->mGlobalMatrix = parent->mGlobalMatrix * objectsToTransform->mLocalMatrix; 

		//objectsToTransform->Owner->mTransform->UpdateBox(); 

		
	}
	 
}

void ComponentTransform::UpdateBox() {

	for (int i = 0; i < app->scene->AllGameObjectManagers.size(); i++) 
	{

		ComponentMesh* boxesToTransform = dynamic_cast<ComponentMesh*>(app->scene->AllGameObjectManagers[i]->GetComponentGameObject(ComponentType::MESH));

		if (boxesToTransform != nullptr)
		{
			boxesToTransform->obb = boxesToTransform->GetMesh()->local_aabb; 
			boxesToTransform->obb.Transform(mGlobalMatrix); 

			boxesToTransform->global_aabb.SetNegativeInfinity(); 
			boxesToTransform->global_aabb.Enclose(boxesToTransform->obb); 
		}
	}
}

