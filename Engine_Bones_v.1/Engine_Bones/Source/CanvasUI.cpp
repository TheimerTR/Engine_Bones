#include "CanvasUI.h"
#include "GameObject.h"
#include "Application.h"
#include "GameObject.h"

#include "FileSystemManager.h"
#include "ComponentTransform.h"

CanvasUI::CanvasUI(GameObject* gameObject, float width, float heigth, uint PosX, uint PosY) : ComponentManager(gameObject)
{
	gmAtached = gameObject;

	widthPanel = width;
	heigthPanel = heigth;

	posX = PosX;
	posY = PosY;

	this->UUID = app->RandomIntGenerator();

	comp_transform = dynamic_cast<ComponentTransform*>(gmAtached->GetComponentGameObject(ComponentType::TRANSFORM));
	comp_transform->mScale.x = widthPanel;
	comp_transform->mScale.y = heigthPanel;
	comp_transform->mRotation = { 0, 0, 0, 1};

	float3 position, scale;
	Quat rotation;

	float4x4 newTransformation = gameObject->mTransform->mLocalMatrix = gameObject->mParent->mTransform->GetGlobalMatrix().Inverted() * gameObject->mTransform->GetGlobalMatrix();

	newTransformation.Decompose(position, rotation, scale);

	gameObject->mTransform->mPosition = position;
	gameObject->mTransform->mRotation = rotation;
	gameObject->mTransform->mScale = scale;

	gameObject->mTransform->mRotationEuler = rotation.ToEulerXYZ() * RADTODEG;

	gameObject->mTransform->UpdateTransformation();
}


CanvasUI::~CanvasUI()
{

}

void CanvasUI::Enable()
{

}

bool CanvasUI::Update()
{
	comp_transform->mScale.x = app->editor->GameWindowSize.x;
	comp_transform->mScale.y = app->editor->GameWindowSize.y;

	//comp_transform->UpdateTransformation();

	return true;
}

void CanvasUI::Disable()
{

}

void CanvasUI::Draw()
{
	glBegin(GL_LINE_LOOP);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	float3 pos = comp_transform->mPosition;
	float3 scale = comp_transform->mScale;

	widthPanel = scale.x;
	heigthPanel = scale.y;

	glVertex3f(pos.x, pos.y, pos.z);
	glVertex3f(pos.x, (pos.y + heigthPanel), pos.z);
	glVertex3f((pos.x + widthPanel), (pos.y + heigthPanel), pos.z);
	glVertex3f((pos.x + widthPanel), pos.y, pos.z);

	glEnd();
}

void CanvasUI::ShowInfo()
{
	if (ImGui::TreeNode("Canvas"))
	{
		if (ImGui::DragFloat("Width in Scene", &comp_transform->mScale.x))
		{
			comp_transform->UpdateTransformation();
		}

		if (ImGui::DragFloat("Heigth in Scene", &comp_transform->mScale.y))
		{
			comp_transform->UpdateTransformation();
		}

		ImGui::TreePop();
	}
}