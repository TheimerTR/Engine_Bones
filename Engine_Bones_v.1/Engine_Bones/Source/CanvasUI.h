#pragma once
#ifndef CANVASUI_H_
#define CANVASUI_H_

#include "ComponentManager.h"
#include "ResourceManager.h"
#include "TextureManager.h"
#include "Color.h"

class GameObject;

class CanvasUI : public ComponentManager
{
public:
	GameObject* gmAtached;

	uint widthPanel;
	uint heigthPanel;

public:
	CanvasUI(GameObject* gameObject, uint width, uint heigt, uint PosX, uint PosY);
	~CanvasUI();

	void Enable() override;
	bool Update() override;
	void Disable() override;
	void Draw();

private:

};


#endif // CANVASUI_H_