#include "ComponentUI.h"
#include "GameObject.h"
#include "ImporterTexture.h"
#include "ComponentTransform.h"
#include "ButtonUI.h"

#include "FileSystemManager.h"

ComponentUI::ComponentUI(UI_Type type, GameObject* gameObject, uint width, uint heigth, uint PosX, uint PosY, const char* imagePath) : ComponentManager(gameObject)
{
	ui_Type = type;
	actualMouseState = MouseState::IDLE_UI;

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

	positionX = PosX;
	positionY = PosY;

	PlaneInGame->vertex[0] = float3(positionX, PosY + heigth, 0);
	PlaneInGame->vertex[1] = float3(positionX + width, PosY + heigth, 0);
	PlaneInGame->vertex[3] = float3(positionX + width, PosY, 0);
	PlaneInGame->vertex[2] = float3(positionX, PosY, 0);

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
	MousePicker();

	switch (actualMouseState)
	{
	case IDLE_UI:
		break;
	case HOVER_UI:
		break;
	case CLICK_UI:
		if (ui_Type == BUTTON)
		{
			ButtonUI* button = (ButtonUI*)this;
			button->OnClick();
		}
		break;
	case CLICKED_UI:
		if (ui_Type == BUTTON)
		{
			ButtonUI* button = (ButtonUI*)this;
			button->OnClicked();
		}
		break;
	case CLICKED_RELEASED_UI:
		break;
	default:
		break;
	}

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

void ComponentUI::MousePicker()
{
	float2 originPoint = float2(app->editor->mousePosInViewport.x, app->editor->mousePosInViewport.y);

	/*originPoint.x = (originPoint.x) * 2;
	originPoint.y = -(originPoint.y) * 2;*/

	float2 mouse_pos = float2(originPoint.x, originPoint.y);

	switch (actualMouseState)
	{
	case IDLE_UI:
		if (MouseIsInside(mouse_pos))
			actualMouseState = HOVER_UI;
		break;
	case HOVER_UI:
		if (app->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
			actualMouseState = CLICK_UI;

		if (!MouseIsInside(mouse_pos))
			actualMouseState = CLICKED_RELEASED_UI;
		break;
	case CLICK_UI:
		if (app->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
			actualMouseState = CLICKED_UI;
		break;
	case CLICKED_UI:
		if (app->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP && !MouseIsInside(mouse_pos))
			actualMouseState = CLICKED_RELEASED_UI;
		else if (app->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP && MouseIsInside(mouse_pos))
			actualMouseState = HOVER_UI;
		break;
	case CLICKED_RELEASED_UI:
		actualMouseState = IDLE_UI;
		break;
	}
}

bool ComponentUI::MouseIsInside(float2 mouse)
{
	bool ret = false;

	if(mouse.x >= positionX && mouse.x <= positionX + widthPanel && mouse.y >= positionY && mouse.y <= positionY + heigthPanel)
	{
		ret = true;
	}

	return ret;
}