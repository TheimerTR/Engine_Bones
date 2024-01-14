#pragma once
#ifndef COMPONENTUI_H_
#define COMPONENTUI_H_

#include "ComponentManager.h"
#include "ResourceManager.h"
#include "TextureManager.h"
#include "Color.h"
#include "ModuleFont.h"

class GameObject;
class ComponentText;
class InputText;
enum FONTS;
enum _functions;

struct UIPlane {
	float3 vertex[4];
	float2 uv[4];
	uint buffer[3];

	uint textureID;
};	

enum UI_Type
{
	BUTTON,
	TEXT,
	IMAGE,
	CHECKER,
	CANV,
	INPUT_TEXT,
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
	
	uint AsRootWidthPanel;
	uint AsRootHeigthPanel;

	MouseState actualMouseState;

	bool isDragabble;

	int positionX;
	int positionY;
	
	int AsRootPositionX;
	int AsRootPositionY;

	float4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

	//Text Component
	ComponentText* textComp;
	InputText* InputTextComp;
	string textCH;
	Font* font;
	string actualText;
	string newText;
	FONTS actualFonts;
	_functions actualChecker;

	//Button
	int actualButtonAction;

	//Checker
	Texture* active;
	Texture* disabled;
	bool CheckSelected;

	//Input Text
	bool IsTextEditing;

public:
	ComponentUI(UI_Type type, GameObject* gameObject, uint width, uint heigt, uint PosX, uint PosY, const char* imagePath);
	~ComponentUI();

	void SetTexture(Texture* T_Texture = nullptr);
	bool MouseIsInside(float2 mouse);
	Texture* GetTexture();
	
	void MousePicker();

	void CreatePanel(float3 vertex[], float3 transform, uint width, uint heigth);
	void GenerateBuffers(uint buffer[], float3 vertex[], float2 uv[]);
	void RegenerateBuffers(uint buffer[], float3 vertex[]);

	void Enable() override;
	bool Update() override;
	void Disable() override;

	void MoveComponent(); 

	ComponentUI* CreateGameObjectUI(GameObject* gm, UI_Type type = UI_Type::DEF, uint width = 20, uint heigth = 20, uint posX = 0, uint posY = 0, const char* imagePath = nullptr, const char* text = "Text", int buttonFuntion = 1, const char* imagePathDisabled = nullptr, uint OrinigalPosX = 0, uint OrinigalPosY = 0, uint OrinigalWidth = 0, uint Orinigalheight = 0);

private:

};


#endif // COMPONENTUI_H_