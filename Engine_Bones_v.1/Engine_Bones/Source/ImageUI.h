#pragma once
#ifndef IMAGEUI_H_
#define IMAGEUI_H_

#include "ComponentManager.h"
#include "ResourceManager.h"
#include "TextureManager.h"
#include "ComponentUI.h"
#include "Color.h"
#include "TextureManager.h"

#include <vector>
#include <functional>

class GameObject;

class ImageUI : public ComponentUI
{
public:
	GameObject* gmAtached;

	uint widthPanel;
	uint heigthPanel;

	Texture* image_Path;

	bool scaleToCanvas;

public:
	ImageUI(UI_Type type, GameObject* gameObject, uint width, uint heigt, uint PosX, uint PosY, const char* imagePath);
	~ImageUI();

	void ShowInfo();
};

#endif // BUTTONUI_H_