#include "ComponentUI.h"
#include "GameObject.h"
#include "ImporterTexture.h"
#include "ComponentTransform.h"

#include "FileSystemManager.h"

ComponentUI::ComponentUI(UI_Type type, GameObject* gameObject, uint width, uint heigth, uint PosX, uint PosY, const char* imagePath) : ComponentManager(gameObject)
{
	ui_Type = type;

	PlaneInScene = new UIPlane;
	PlaneInGame = new UIPlane;

	gmAtached = gameObject;

	widthPanel = width;
	heigthPanel = heigth;

	this->UUID = app->RandomIntGenerator();

	texture = nullptr;

	if (imagePath != nullptr)
	{
		Importer::ImporterTexture::Load(texture, imagePath);

		if (texture != nullptr)
		{
			PlaneInGame->textureID = PlaneInScene->textureID = texture->TextureID;
		}
	}

	float3 transform;

	transform = gameObject->mTransform->gPosition;

	PlaneInScene->vertex[0] = float3(transform.x, transform.y + heigth, transform.z);
	PlaneInScene->vertex[1] = float3(transform.x + width, transform.y + heigth, transform.z);
	PlaneInScene->vertex[3] = float3(transform.x + width, transform.y, transform.z);
	PlaneInScene->vertex[2] = float3(transform.x, transform.y, transform.z);

	/*PlaneInScene->vertex[0] = float3(0, 0, 0);
	PlaneInScene->vertex[1] = float3(0, 1, 0);
	PlaneInScene->vertex[2] = float3(1, 1, 0);
	PlaneInScene->vertex[3] = float3(1, 0, 0);*/

	PlaneInGame->vertex[0] = float3(PosX, PosY + heigth, 0);
	PlaneInGame->vertex[1] = float3(PosX + width, PosY + heigth, 0);
	PlaneInGame->vertex[3] = float3(PosX + width, PosY, 0);
	PlaneInGame->vertex[2] = float3(PosX, PosY, 0);

	PlaneInScene->GenerateBuffers();
	PlaneInGame->GenerateBuffers();
}


ComponentUI::~ComponentUI()
{
	texture = nullptr;
}

void ComponentUI::SetTexture(Texture* T_Texture)
{

}

Texture* ComponentUI::GetTexture()
{
	return texture;
}

void ComponentUI::Enable()
{

}

bool ComponentUI::Update()
{

	return true;
}

void ComponentUI::Disable()
{
	
}

void UIPlane::GenerateBuffers()
{
	uint* index = new uint[6];

	index[0] = 0;
	index[1] = 1;
	index[2] = 2;
	index[3] = 2;
	index[4] = 1;
	index[5] = 3;

	//Cube Vertex
	glGenBuffers(1, &(GLuint)buffer[0]);
	glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float3) * 4, vertex, GL_STATIC_DRAW);

	//Cube Vertex definition
	glGenBuffers(1, &(GLuint)buffer[1]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * 6, index, GL_STATIC_DRAW);

	//UVs definition
	glGenBuffers(1, &(GLuint)buffer[2]);
	glBindBuffer(GL_ARRAY_BUFFER, buffer[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float2) * 4, uv, GL_STATIC_DRAW);
}