#pragma once
#ifndef CANVASUI_H_
#define CANVASUI_H_

#include "ComponentManager.h"
#include "ResourceManager.h"
#include "TextureManager.h"
#include "ComponentTransform.h"
#include "Color.h"

class GameObject;

class CanvasUI : public ComponentManager
{
public:
	GameObject* gmAtached;

	float widthPanel;
	float heigthPanel;
	ComponentTransform* comp_transform;

public:
	CanvasUI(GameObject* gameObject, float width, float heigt, uint PosX, uint PosY);
	~CanvasUI();

	void Enable() override;
	bool Update() override;
	void Disable() override;
	void Draw();

	void ShowInfo();

private:

};


#endif // CANVASUI_H_