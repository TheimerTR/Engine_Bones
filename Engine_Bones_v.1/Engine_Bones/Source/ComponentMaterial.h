#ifndef COMPONENTMATERIAL_H_
#define COMPONENTMATERIAL_H_

#include "ComponentManager.h"
#include "AssimpManager.h"
#include "TextureManager.h"
#include "Color.h"

class GameObjectManager;

class ComponentMaterial : public ComponentManager
{
public:
	Texture* texture;
	uint CM_TextureID;

	bool Material;
	Color colorTexture;

public:

	ComponentMaterial(GameObjectManager* gameObject);
	~ComponentMaterial();

	void SetTexture(Texture* T_Texture = nullptr);
	Texture* GetTexture();

	void Enable() override;
	bool Update() override;
	void Disable() override;
	void ShowInfo(Texture* texture, int i);

private:

};


#endif // COMPONENTMATERIAL_H_