#include "ComponentUI.h"
#include "GameObject.h"
#include "ImporterTexture.h"
#include "ComponentTransform.h"
#include "ButtonUI.h"
#include "CheckerUI.h"
#include "ImageUI.h"
#include "ComponentText.h"
#include "InputText.h"
#include "Application.h"

#include "FileSystemManager.h"

ComponentUI::ComponentUI(UI_Type type, GameObject* gameObject, uint width, uint heigth, uint PosX, uint PosY, const char* imagePath) : ComponentManager(gameObject)
{
	//Text Component
	textComp = nullptr;
	InputTextComp = nullptr;
	textCH = "";
	font = app->font->actualFont;

	actualText = "";
	newText = "";

	//Button
	actualButtonAction = 1;

	//Checker
	CheckSelected = false;
	active = new Texture();
	Importer::ImporterTexture::Load(active, "Assets/Textures/Ckeck-checkedbox.png");

	disabled = new Texture();
	Importer::ImporterTexture::Load(disabled, "Assets/Textures/Unchecked-checkbox.png");

	//Input Text
	IsTextEditing = false;

	isDragabble = false;
	isChildOfText = false;
	isSelected = false;
	isBeeingClicked = false;

	ui_Type = type;
	actualMouseState = MouseState::IDLE_UI;

	PlaneInScene = new UIPlane;
	PlaneInGame = new UIPlane;

	gmAtached = gameObject;

	widthPanel = width;
	heigthPanel = heigth;

	positionX = PosX;
	positionY = PosY;

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

	if (gameObject->mParent != nullptr)
	{
		gameObject->mTransform->mPosition = { (float)positionX, (float)positionY, 0 };
		gameObject->mTransform->mScale = { (float)widthPanel, (float)heigthPanel, 1 };
		gameObject->mTransform->mRotation = { 0, 0, 0, 1 };

		gameObject->mTransform->UpdateTransformation();

		CreatePanel(PlaneInScene->vertex, gameObject->mTransform->mPosition, width, heigth);

		//transform = { (float)PosX, (float)PosY, 0 };

		CreatePanel(PlaneInGame->vertex, gameObject->mTransform->mPosition, width, heigth);
	}
	//}

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
	delete texture;
	texture = nullptr;
	
	delete InputTextComp;
	InputTextComp = nullptr;

	font = nullptr;

	active = nullptr;

	disabled = nullptr;

	PlaneInScene = nullptr;
	PlaneInGame = nullptr;

	gmAtached = nullptr;
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

	if (ui_Type == INPUT_TEXT)
	{
		ComponentMaterial* mat = dynamic_cast<ComponentMaterial*>(gmAtached->GetComponentGameObject(ComponentType::MATERIAL));
		if (actualText == "")
		{
			mat->SetTexture(texture);
		}

		InputText* inputText = (InputText*)this;
		inputText->Update(this);
	}

	if (app->editor->isRunning)
	{
		switch (actualMouseState)
		{
		case IDLE_UI:
			if (ui_Type == BUTTON) {
				ButtonUI* button = (ButtonUI*)this;
				button->OnIdle(this);
			}
			break;
			if (ui_Type == CHECKER) {
				CheckerUI* checker = (CheckerUI*)this;
				checker->OnIdle(this);
			}
			break;
		case HOVER_UI:
			if (ui_Type == BUTTON) {
				ButtonUI* button = (ButtonUI*)this;
				button->OnHover(this);
			}
			if (ui_Type == CHECKER) {
				CheckerUI* checker = (CheckerUI*)this;
				checker->OnHover(this);
			}
			break;
		case CLICK_UI:
			if (ui_Type == INPUT_TEXT)
			{
				InputText* inputText = (InputText*)this;
				inputText->OnClick(this);
			}
			if (ui_Type == BUTTON)
			{
				ButtonUI* button = (ButtonUI*)this;
				button->OnClick(&actualButtonAction);
			}
			if (ui_Type == CHECKER)
			{
				CheckerUI* checker = (CheckerUI*)this;
				checker->OnClick(this);
			}
			break;
		case CLICKED_UI:
			if(app->scene->draggable)
			{
				MoveComponent();
			}
			break;
		case CLICKED_RELEASED_UI:
			this->isBeeingClicked = false;

			if (ui_Type == BUTTON) 
			{
				ButtonUI* button = (ButtonUI*)this;
				button->OnIdle(this);
				isDragabble = false;
			}
			if (ui_Type == CHECKER) {
				CheckerUI* checker = (CheckerUI*)this;
				checker->OnIdle(this);
				isDragabble = false;
			}
			break;
		default:
			break;
		}
	}

	return true;
}

void ComponentUI::Disable()
{
	
}

ComponentUI* ComponentUI::CreateGameObjectUI(GameObject* gm, UI_Type type, uint width, uint heigth, uint posX, uint posY, const char* imagePath, const char* text, int buttonFuntion, const char* imagePathDisabled, uint OrinigalPosX, uint OrinigalPosY, uint OrinigalWidth, uint Orinigalheight)
{
	ComponentUI* comp_UI = nullptr;

	switch (type)
	{
	case UI_Type::BUTTON:
		{
			GameObject* Button = new GameObject("Button", gm);
			ComponentTransform* transform = dynamic_cast<ComponentTransform*>(Button->GetComponentGameObject(ComponentType::TRANSFORM));
			comp_UI = dynamic_cast<ComponentUI*>(Button->AddComponent(ComponentType::UI, type, width, heigth, posX, posY, imagePath));
			comp_UI->AsRootPositionX = OrinigalPosX; comp_UI->AsRootPositionY = OrinigalPosY; comp_UI->AsRootWidthPanel = OrinigalWidth; comp_UI->AsRootHeigthPanel = Orinigalheight;
			ButtonUI but_UI = ButtonUI(type, Button, width, heigth, posX, posY, imagePath);
			but_UI.actualFunction = (functions)buttonFuntion;
			comp_UI->gmAtached = Button;
			comp_UI->actualButtonAction = buttonFuntion;
			comp_UI->positionX = but_UI.positionX;
			comp_UI->positionY = but_UI.positionY;

			ComponentMaterial* mat = (ComponentMaterial*)(Button->AddComponent(ComponentType::MATERIAL));
			mat->colorTexture.r = 255;
			mat->colorTexture.g = 255;
			mat->colorTexture.b = 255;
			mat->colorTexture.a = 255;

			if(comp_UI->texture != nullptr)
			{
				mat->texture = comp_UI->texture;
			}
		}
		break;
	case UI_Type::TEXT:
		{
			GameObject* Text = new GameObject("Text", gm);
			ComponentTransform* transform = dynamic_cast<ComponentTransform*>(Text->GetComponentGameObject(ComponentType::TRANSFORM));
			comp_UI = dynamic_cast<ComponentUI*>(Text->AddComponent(ComponentType::UI, type, width, heigth, posX, posY, imagePath));
			ComponentText text_UI = ComponentText(type, Text, Text->mTransform->mScale.x, Text->mTransform->mScale.y, Text->mTransform->mPosition.x, Text->mTransform->mPosition.y, text);
			comp_UI->AsRootPositionX = OrinigalPosX; comp_UI->AsRootPositionY = OrinigalPosY; comp_UI->AsRootWidthPanel = OrinigalWidth; comp_UI->AsRootHeigthPanel = Orinigalheight;
			comp_UI->textComp = &text_UI;
			comp_UI->gmAtached = Text;
			comp_UI->textCH = text_UI.text;
			comp_UI->font = text_UI.font;
			comp_UI->actualText = text_UI.actualText;
			comp_UI->newText = text_UI.newText;
			comp_UI->actualFonts = text_UI.actualFonts;
			comp_UI->positionX = text_UI.positionX;
			comp_UI->positionY = text_UI.positionY;

			ComponentMaterial* mat = (ComponentMaterial*)(Text->AddComponent(ComponentType::MATERIAL));
			mat->colorTexture.r = 255;
			mat->colorTexture.g = 255;
			mat->colorTexture.b = 255;
			mat->colorTexture.a = 255;

			if (comp_UI->texture != nullptr)
			{
				mat->texture = comp_UI->texture;
			}
		}
		break;
	case UI_Type::INPUT_TEXT:
	{
		GameObject* Text = new GameObject("Input Text", gm);
		ComponentTransform* transform = dynamic_cast<ComponentTransform*>(Text->GetComponentGameObject(ComponentType::TRANSFORM));
		comp_UI = dynamic_cast<ComponentUI*>(Text->AddComponent(ComponentType::UI, type, width, heigth, posX, posY, imagePath));
		InputText text_UI = InputText(type, Text, width, heigth, posX, posY, "");
		comp_UI->AsRootPositionX = OrinigalPosX; comp_UI->AsRootPositionY = OrinigalPosY; comp_UI->AsRootWidthPanel = OrinigalWidth; comp_UI->AsRootHeigthPanel = Orinigalheight;
		comp_UI->InputTextComp = &text_UI;
		comp_UI->gmAtached = Text;
		comp_UI->textCH = text_UI.text;
		comp_UI->font = text_UI.font;
		comp_UI->actualText = text_UI.actualText;
		comp_UI->newText = text_UI.newText;
		comp_UI->actualFonts = text_UI.actualFonts;
		comp_UI->positionX = text_UI.positionX;
		comp_UI->positionY = text_UI.positionY;

		ComponentMaterial* mat = (ComponentMaterial*)(Text->AddComponent(ComponentType::MATERIAL));
		mat->colorTexture.r = 255;
		mat->colorTexture.g = 255;
		mat->colorTexture.b = 255;
		mat->colorTexture.a = 255;

		if (comp_UI->texture != nullptr)
		{
			mat->texture = comp_UI->texture;
		}
	}
	break;
	case UI_Type::IMAGE:
		{
			GameObject* Image = new GameObject("Image", gm);
			ComponentTransform* transform = dynamic_cast<ComponentTransform*>(Image->GetComponentGameObject(ComponentType::TRANSFORM));
			comp_UI = (ComponentUI*)(Image->AddComponent(ComponentType::UI, type, width, heigth, posX, posY, imagePath));
			ImageUI* img_UI = new ImageUI(type, Image, width, heigth, posX, posY, imagePath);
			comp_UI->AsRootPositionX = OrinigalPosX; comp_UI->AsRootPositionY = OrinigalPosY; comp_UI->AsRootWidthPanel = OrinigalWidth; comp_UI->AsRootHeigthPanel = Orinigalheight;
			comp_UI->gmAtached = Image;
			comp_UI = (ComponentUI*)img_UI;

			ComponentMaterial* mat = (ComponentMaterial*)(Image->AddComponent(ComponentType::MATERIAL));
			mat->colorTexture.r = 255;
			mat->colorTexture.g = 255;
			mat->colorTexture.b = 255;
			mat->colorTexture.a = 255;

			if (comp_UI->texture != nullptr)
			{
				mat->texture = comp_UI->texture;
			}
		}
		break;
	case UI_Type::CHECKER:
		{
			GameObject* Checker = new GameObject("Checker", gm);
			ComponentTransform* transform = dynamic_cast<ComponentTransform*>(Checker->GetComponentGameObject(ComponentType::TRANSFORM));
			comp_UI = dynamic_cast<ComponentUI*>(Checker->AddComponent(ComponentType::UI, type, width, heigth, posX, posY, imagePathDisabled));
			CheckerUI check_UI = CheckerUI(type, Checker, width, heigth, posX, posY, imagePath, imagePathDisabled);
			comp_UI->AsRootPositionX = OrinigalPosX; comp_UI->AsRootPositionY = OrinigalPosY; comp_UI->AsRootWidthPanel = OrinigalWidth; comp_UI->AsRootHeigthPanel = Orinigalheight;
			comp_UI->actualChecker = (_functions)buttonFuntion;
			comp_UI->positionX = check_UI.positionX;
			comp_UI->positionY = check_UI.positionY;
			comp_UI->widthPanel = check_UI.widthPanel;
			comp_UI->heigthPanel = check_UI.heigthPanel;
			//comp_UI->active = check_UI.textureActive;
			//comp_UI->disabled = check_UI.textureDisabled;

			ComponentMaterial* mat = (ComponentMaterial*)(Checker->AddComponent(ComponentType::MATERIAL));
			mat->colorTexture.r = 255;
			mat->colorTexture.g = 255;
			mat->colorTexture.b = 255;
			mat->colorTexture.a = 255;

			if (comp_UI->texture != nullptr)
			{
				mat->texture = comp_UI->texture;
			}
		}
		break;
	case UI_Type::CANV:
		{
			app->editor->Canvas = new GameObject("Canvas", gm);
			app->editor->Canvas->mTransform->mPosition = { (float)posX, (float)posY, 0 };
			CanvasUI* canv_UI = (CanvasUI*)(app->editor->Canvas->AddComponent(ComponentType::CANVAS, type, app->editor->GameWindowSize.x, app->editor->GameWindowSize.y, 0, 0, nullptr));
		}
		break;
	default:
		break;
	}

	if (app->editor->Canvas != nullptr)
	{
		if (app->editor->Canvas->childrens.size() > 1)
		{
			GameObject* first = app->editor->Canvas->childrens[0];
			app->editor->Canvas->childrens[0] = app->editor->Canvas->childrens[app->editor->Canvas->childrens.size() - 1];
			app->editor->Canvas->childrens[app->editor->Canvas->childrens.size() - 1] = first;
		}
	}

	return comp_UI;
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

void ComponentUI::RegenerateBuffers(uint buffer[], float3 vertex[]) {

	glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float3) * 4, vertex, GL_STATIC_DRAW);
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

	if (gmAtached != nullptr)
	{
		if (app->renderer3D->cameraGame != nullptr && gmAtached->isActive)
		{
			if (AsRootPositionX >= 0 && AsRootPositionY >= 0 && mouse.x >= 0 && mouse.y >= 0 && mouse.x < app->editor->GameWindowSize.x && mouse.y < app->editor->GameWindowSize.y)
			{
				if (mouse.x >= AsRootPositionX && mouse.x <= AsRootPositionX + AsRootWidthPanel && mouse.y >= AsRootPositionY && mouse.y <= AsRootPositionY + AsRootHeigthPanel)
				{
					ret = true;
				}
			}
		}

		if(ui_Type == INPUT_TEXT)
		{
			if (AsRootPositionX >= 0 && AsRootPositionY >= 0 && mouse.x >= 0 && mouse.y >= 0 && mouse.x < app->editor->GameWindowSize.x && mouse.y < app->editor->GameWindowSize.y)
			{
				if (mouse.x <= AsRootPositionX || mouse.x >= AsRootPositionX + AsRootWidthPanel || mouse.y <= AsRootPositionY || mouse.y >= AsRootPositionY + AsRootHeigthPanel)
				{
					IsTextEditing = false;
				}
			}
		}
	}

	return ret;
}

void ComponentUI::CreatePanel(float3 vertex[], float3 transform, uint width, uint heigth)
{
	vertex[0] = float3(transform.x, transform.y + 1, transform.z);
	vertex[1] = float3(transform.x + 1, transform.y + 1, transform.z);
	vertex[3] = float3(transform.x + 1, transform.y, transform.z);
	vertex[2] = float3(transform.x, transform.y, transform.z);
}

void ComponentUI::MoveComponent()
{
	float2 originPoint = float2(app->editor->mousePosInViewport.x, app->editor->mousePosInViewport.y);

	float2 mouse_pos = float2(originPoint.x, originPoint.y);

	if(mouse_pos.x >= 0 && mouse_pos.y >= 0 && mouse_pos.x < app->editor->GameWindowSize.x && mouse_pos.y < app->editor->GameWindowSize.y)
	{
		ComponentTransform* transform = dynamic_cast<ComponentTransform*>(Owner->GetComponentGameObject(ComponentType::TRANSFORM));

		float3 gPos;
		float3 gSca;
		Quat gRot;

		transform->mGlobalMatrix.Decompose(gPos, gRot, gSca);

		float distx = mouse_pos.x - gPos.x;
		float disty = mouse_pos.y - gPos.y;

		ComponentUI* comp_UI = this;

		int dx = app->input->GetMouseXMotion();
		int dy = app->input->GetMouseYMotion();

		transform->mPosition.x += dx * 0.001;
		transform->mPosition.y += dy * 0.001;
		transform->mPosition.z = 0;

		if (comp_UI->Owner->childrens.size() > 0)
		{
			comp_UI->AsRootPositionX += dx * 0.00001;
			comp_UI->AsRootPositionY += dy * 0.00001;

			comp_UI->positionX += dx * 0.001;
			comp_UI->positionY += dy * 0.001;
		}

		if (comp_UI->Owner->childrens.size() == 0)
		{
			comp_UI->AsRootPositionX += dx * 1;
			comp_UI->AsRootPositionY += dy * 1;

			comp_UI->positionX += dx * 0.001;
			comp_UI->positionY += dy * 0.001;
		}

		transform->UpdateTransformation();
		
	}
}