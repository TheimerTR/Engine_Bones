#include "ComponentTransform.h"
#include "GameObjectManager.h"
#include "External/ImGui/imgui.h"


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

void ComponentTransform::ShowInfo() 
{

	if (ImGui::TreeNode("Transform##1"))
	{
		ImGui::InputFloat3("Position",(float*)&mPosition );
		ImGui::InputFloat3("Scale",(float*)&mScale);
		ImGui::InputFloat3("Rotation",(float*)&mrotation);

		ImGui::TreePop(); 
	}

}