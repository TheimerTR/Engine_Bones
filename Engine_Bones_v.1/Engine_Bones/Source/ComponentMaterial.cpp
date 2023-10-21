#include "ComponentMaterial.h"
#include "GameObjectManager.h"

ComponentMaterial::ComponentMaterial(GameObjectManager* gameObject) : ComponentManager(gameObject)
{
	texture = new Texture();
	CM_TextureID = NULL;
}

ComponentMaterial::~ComponentMaterial()
{
	if (texture->TextureID != 0)
		glDeleteTextures(1, &texture->TextureID);

	texture->imageFormat = 0;
	texture->imageHeight = 0;
	texture->imageType = 0;
	texture->imageWidth = 0;
	texture->Name = "";
	texture->path = "";
	texture->ShowTextures = false;
	texture->TextureID = 0;

	texture = nullptr;
}

void ComponentMaterial::SetTexture(Texture* T_Texture)
{
	texture = T_Texture;
	CM_TextureID = T_Texture->TextureID;
}

Texture* ComponentMaterial::GetTexture()
{
	return texture;
}
