#pragma once
#ifndef COMPONENT_TEXT_H_
#define COMPONENT_TEXT_H_

#include "ComponentManager.h"
#include "ResourceManager.h"
#include "TextureManager.h"
#include "ComponentUI.h"
#include "Color.h"
#include "TextureManager.h"
#include "ModuleFont.h"
#include "ComponentMesh.h"

#include <vector>
#include <functional>

class GameObject;

enum FONTS
{
	ARIAL,
	ROBOTO,
	ELIANTO,
	MAX_DEF,
};

struct fonts
{
	vector<string> nameOfFunctions = { "Arial", "Roboto", "Elianto" };
};

class ComponentText : public ComponentUI
{
public:
	GameObject* gmAtached;

	uint widthPanel;
	uint heigthPanel;

	string text;

	Font* font;

	bool hasBeenModified;

	FONTS actualFonts;

public:
	ComponentText(UI_Type type, GameObject* gameObject, uint width, uint heigt, uint PosX, uint PosY, const char* imagePath);
	~ComponentText();

	void ShowInfo();

	void DoText();

	void ModifyText();
};

#endif // COMPONENT_TEXT_H_