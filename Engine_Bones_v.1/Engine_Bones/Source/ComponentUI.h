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
	CANVAS,
	BUTTON,
	TEXT,
	IMAGE,
	CHECKER
};

class ComponentUI : public ComponentManager
{
public:
	Texture* texture;

	UIPlane* PlaneInScene;
	UIPlane* PlaneInGame;

public:

	ComponentUI(UI_Type type, GameObject* gameObject, uint width, uint heigt, uint PosX, uint PosY);
	~ComponentUI();

	void SetTexture(Texture* T_Texture = nullptr);
	Texture* GetTexture();

	void Enable() override;
	bool Update() override;
	void Disable() override;
	void ShowInfo(Texture* texture, int i);

private:

};


#endif // COMPONENTMATERIAL_H_