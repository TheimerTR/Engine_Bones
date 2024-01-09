#pragma once
#ifndef BUTTONUI_H_
#define BUTTONUI_H_

#include "ComponentManager.h"
#include "ResourceManager.h"
#include "TextureManager.h"
#include "ComponentUI.h"
#include "Color.h"

#include <vector>
#include <functional>

class GameObject;

enum functions
{
	PASS_SCENE,
	DEFAULT,
	MAX
};

struct actions
{
	vector<string> nameOfFunctions = { "Pass scene", "None"};
};

class ButtonUI : public ComponentUI
{
public:
	GameObject* gmAtached;

	uint widthPanel;
	uint heigthPanel;

	bool isPressed;

	functions actualFunction;

public:
	ButtonUI(UI_Type type, GameObject* gameObject, uint width, uint heigt, uint PosX, uint PosY, const char* imagePath);
	~ButtonUI();

	bool OnClick(int* action);

	bool OnHover();
	bool OnClicked();

	void ShowInfo(int* action);

	void PassScene();

	void CreatePauseMenu();
};

#endif // BUTTONUI_H_