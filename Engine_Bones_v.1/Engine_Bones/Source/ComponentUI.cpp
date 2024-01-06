#include "ComponentUI.h"
#include "Application.h"
#include "GameObject.h"
#include "ImporterTexture.h"
#include "ComponentTransform.h"
#include "ButtonUI.h"

#include "FileSystemManager.h"

ComponentUI::ComponentUI(UI_Type type, GameObject* gameObject, uint width, uint heigth, uint PosX, uint PosY, const char* imagePath) : ComponentManager(gameObject)
{
	textComp = nullptr;
	textCH = "";
	font = app->font->actualFont;

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
		texture = new Texture();
		Importer::ImporterTexture::Load(texture, imagePath);

		if (texture != nullptr)
		{
			PlaneInGame->textureID = PlaneInScene->textureID = texture->TextureID;
		}
	}

	float3 transform;

	transform = gameObject->mTransform->gPosition;

	CreatePanel(PlaneInScene->vertex, transform, width, heigth);

	transform = { (float)PosX, (float)PosY, 0 };

	CreatePanel(PlaneInGame->vertex, transform, width, heigth);

	PlaneInScene->uv[0] = float2(0, 1);
	PlaneInScene->uv[1] = float2(1, 1);
	PlaneInScene->uv[3] = float2(1, 0);
	PlaneInScene->uv[2] = float2(0, 0);

	PlaneInGame->uv[0] = float2(0, 1);
	PlaneInGame->uv[1] = float2(1, 1);
	PlaneInGame->uv[3] = float2(1, 0);
	PlaneInGame->uv[2] = float2(0, 0);

	GenerateBuffers(PlaneInScene->buffer, PlaneInScene->vertex, PlaneInScene->uv);
	GenerateBuffers(PlaneInGame->buffer, PlaneInGame->vertex, PlaneInGame->uv);
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

void ComponentUI::GenerateBuffers(uint buffer[], float3 vertex[], float2 uv[])
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

void ComponentUI::CreatePanel(float3 vertex[], float3 transform, uint width, uint heigth)
{
	vertex[0] = float3(transform.x, transform.y + heigth, transform.z);
	vertex[1] = float3(transform.x + width, transform.y + heigth, transform.z);
	vertex[3] = float3(transform.x + width, transform.y, transform.z);
	vertex[2] = float3(transform.x, transform.y, transform.z);
}