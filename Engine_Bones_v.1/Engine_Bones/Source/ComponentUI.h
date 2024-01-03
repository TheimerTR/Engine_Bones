#pragma once
#ifndef COMPONENTUI_H_
#define COMPONENTUI_H_

#include "ComponentManager.h"
#include "ResourceManager.h"
#include "TextureManager.h"
#include "Color.h"

class GameObject;

struct UIPlane {
	float3 vertex[4];
	float2 uv[4];
	uint buffer[3];

	uint textureID;

	void GenerateBuffers();
};	

enum UI_Type
{
	BUTTON,
	TEXT,
	IMAGE,
	CHECKER,
	DEF
};

enum MouseState
{
	HOVER_UI,
	CLICK_UI,
	CLICKED_UI,
	IDLE_UI,
	CLICKED_RELEASED_UI
};

class ComponentUI : public ComponentManager
{
public:
	UI_Type ui_Type;
	Texture* texture;

	UIPlane* PlaneInScene;
	UIPlane* PlaneInGame;
	GameObject* gmAtached;

	uint widthPanel;
	uint heigthPanel;

	MouseState actualMouseState;

	bool isDragabble;

	int positionX;
	int positionY;

	float4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

public:
	ComponentUI(UI_Type type, GameObject* gameObject, uint width, uint heigt, uint PosX, uint PosY, const char* imagePath);
	~ComponentUI();

	void SetTexture(Texture* T_Texture = nullptr);
	bool MouseIsInside(float2 mouse);
	Texture* GetTexture();
	
	void MousePicker();

	void Enable() override;
	bool Update() override;
	void Disable() override;

private:

};


#endif // COMPONENTMATERIAL_H_