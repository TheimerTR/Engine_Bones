#include "ComponentTransform.h"
#include "GameObject.h"
#include "External/ImGui/imgui.h"
#include "Application.h"


ComponentTransform::ComponentTransform(GameObject* gameObject) : ComponentManager(gameObject)
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

void ComponentTransform::SetTransform(GameObject* gameObject, float3 position, float3 scale, Quat rotation)
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

void ComponentTransform::UpdateTransformation(bool isUI)
{
	mRotation = Quat::FromEulerXYZ(mRotationEuler.x * DEGTORAD, mRotationEuler.y * DEGTORAD, mRotationEuler.z * DEGTORAD).Normalized(); 
	
	mLocalMatrix = float4x4::FromTRS(mPosition, mRotation, mScale);

	//mGlobalMatrix = float4x4::FromTRS(gPosition, gRotation, gScale);

	for (int i = 1; i < app->scene->AllGameObjectManagers.size(); i++) {

		ComponentTransform* objectsToTransform = dynamic_cast<ComponentTransform*>(app->scene->AllGameObjectManagers[i]->GetComponentGameObject(ComponentType::TRANSFORM));

		ComponentTransform* parent = objectsToTransform->Owner->mParent->mTransform; 

		/*if (Owner->GetComponentGameObject(ComponentType::UI) != nullptr){

			float3 glPosition, glScale, mlPosition, mlScale; 
			Quat glRotation, mlRotation; 
		

			parent->mGlobalMatrix.Decompose(glPosition, glRotation, glScale); 
			objectsToTransform->mLocalMatrix.Decompose(mlPosition, mlRotation, mlScale); 

			glPosition.z = 0;
			glRotation.z = 0;
			glPosition.z = 0; 
			mlPosition.z = 0;
			mlRotation.z = 0;

			objectsToTransform->gPosition.x = glPosition.x * mlPosition.x; 
			objectsToTransform->gPosition.y = glPosition.y * mlPosition.y; 
			objectsToTransform->gPosition.z = 0;
			objectsToTransform->gRotation.x = glRotation.x * mlRotation.x; 
			objectsToTransform->gRotation.y = glRotation.y * mlRotation.y; 
			objectsToTransform->gRotation.z = 0; 
			objectsToTransform->gScale.x = glScale.x * mlScale.x;
			objectsToTransform->gScale.y = glScale.y * mlScale.y;
			objectsToTransform->gScale.z = glScale.z * mlScale.z;
	
			objectsToTransform->mGlobalMatrix.FromTRS(objectsToTransform->gPosition, objectsToTransform->gRotation, objectsToTransform->gScale);
		}

		else {*/

		if(!isUI)
		{
			objectsToTransform->mGlobalMatrix = parent->mGlobalMatrix * objectsToTransform->mLocalMatrix;
		}
		else
		{
			objectsToTransform->mGlobalMatrix = parent->mGlobalMatrix * objectsToTransform->mLocalMatrix;

			float3 position, scale;
			Quat rotation;

			objectsToTransform->mGlobalMatrix.Decompose(position, rotation, scale);

			position.z = 0;

			objectsToTransform->mGlobalMatrix = float4x4::FromTRS(position, rotation, mScale * 400);
		}

		//}

		objectsToTransform->UpdateBox(); 

	}
}

void ComponentTransform::UpdateGuizmoTransformation(float4x4 &globalTransformation)
{
	mGlobalMatrix = globalTransformation; 

	ComponentTransform* parent = Owner->mParent->mTransform;

	/*if (Owner->GetComponentGameObject(ComponentType::UI) != nullptr) {

		float3 posGlobal, scaleGlobal; 
		Quat rotGlobal;
		float3 posLocal, scaleLocal;
		Quat rotLocal;
		float3 posTransformation, scaleTransformation;
		Quat rotTransformation;

		globalTransformation.Decompose(posTransformation, rotTransformation, scaleTransformation);

		posGlobal.z = 0; 
		rotGlobal.z = 0; 
		posLocal.z = 0; 
		rotLocal.z = 0; 
		posTransformation.z = 0; 
		rotTransformation.z = 0; 

		posGlobal = posTransformation; 
		rotGlobal = rotTransformation; 
		scaleGlobal = scaleTransformation; 

		mGlobalMatrix.FromTRS(posGlobal, rotGlobal, scaleGlobal);

		mLocalMatrix = parent->mGlobalMatrix.Inverted() * mGlobalMatrix;

		mLocalMatrix.Decompose(mPosition, mRotation, mScale);

		mRotationEuler = mRotation.ToEulerXYZ() * RADTODEG;
	}*/

	//else {
		mLocalMatrix = parent->mGlobalMatrix.Inverted() * mGlobalMatrix;

		mLocalMatrix.Decompose(mPosition, mRotation, mScale);

		mRotationEuler = mRotation.ToEulerXYZ() * RADTODEG;
	//}

	UpdateTransformation(); 
}

void ComponentTransform::UpdateBox() {

	//for (int i = 0; i < app->scene->AllGameObjectManagers.size(); i++) 
	//{

		ComponentMesh* boxesToTransform = dynamic_cast<ComponentMesh*>(Owner->GetComponentGameObject(ComponentType::MESH));

		if (boxesToTransform != nullptr)
		{
			boxesToTransform->obb = boxesToTransform->GetMesh()->local_aabb; 
			boxesToTransform->obb.Transform(mGlobalMatrix); 

			boxesToTransform->global_aabb.SetNegativeInfinity(); 
			boxesToTransform->global_aabb.Enclose(boxesToTransform->obb); 
		}
	/*}*/
}

