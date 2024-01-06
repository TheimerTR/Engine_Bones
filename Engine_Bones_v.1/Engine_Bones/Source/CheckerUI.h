#pragma once
#ifndef CHECKERUI_H_
#define CHECKERUI_H_

#include "ComponentManager.h"
#include "ResourceManager.h"
#include "TextureManager.h"
#include "ComponentUI.h"
#include "Color.h"

#include <vector>
#include <functional>

class GameObject;

enum _functions
{
	_VSYNC,
	_DEF,
	_MAX
};

struct values
{
	vector<string> nameOfFunctions = { "Vysnc" , "None" };
};

class CheckerUI : public ComponentUI
{
public:
	GameObject* gmAtached;

	uint widthPanel;
	uint heigthPanel;

	bool isSelected;

	string selectedImagePath;
	string notSelectedImagePath;

	_functions actualFunction;

public:
	CheckerUI(UI_Type type, GameObject* gameObject, uint width, uint heigt, uint PosX, uint PosY, const char* imagePath);
	~CheckerUI();

	bool OnClick(ComponentUI* UI_Element);

	bool OnHover();
	bool OnClicked();

	void ShowInfo(ComponentUI* UI_Element);
};

#endif // CHECKERUI_H_