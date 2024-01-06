#include "GameObject.h"

#include "Application.h"
#include "ComponentManager.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentCamera.h"
#include "ModuleScene.h"
#include "CanvasUI.h"

GameObject::GameObject(string name, GameObject* parent, int id) : mParent(parent), mName(name), mTransform(nullptr), isActive(true)
{
	if(parent != nullptr)
	{
		parent->childrens.push_back(this);
	}

	mTransform = dynamic_cast<ComponentTransform*>(AddComponent(ComponentType::TRANSFORM));

	if (app != nullptr)
	{
		app->scene->AllGameObjectManagers.push_back(this);
		app->scene->Selected_GameObject = this;
		app->editor->actualMesh = this;
		this->isText = false;

		this->UUID = app->RandomIntGenerator();

		for(int i = 0; i < app->scene->AllGameObjectManagers.size(); i++)
		{
			if((app->scene->AllGameObjectManagers.at(i)->UUID == this->UUID) && (this != app->scene->AllGameObjectManagers.at(i)))
			{
				this->UUID = app->RandomIntGenerator();
			}
		}
	}
};

GameObject::~GameObject()
{
	app->scene->AllGameObjectManagers.erase(find(app->scene->AllGameObjectManagers.begin(), app->scene->AllGameObjectManagers.end(), this));

	map<uint32, ResourceElement*>::iterator iterator = app->resource->AllResourcesMap.begin();

	for (iterator; iterator != app->resource->AllResourcesMap.end(); iterator++)
	{
		for (int h = 0; h < iterator->second->ParentsUUID.size(); h++)
		{
			if (UUID == iterator->second->ParentsUUID[h])
			{
				iterator->second->resourceCounter -= 1;
			}
		}
	}

	for (uint i = 0; i < childrens.size(); i++)
	{
		RELEASE(childrens[i]);
		childrens[i] = nullptr;
	}

	childrens.clear();

	for(uint i = 0; i < mComponents.size(); i++)
	{
		ComponentManager* Comp = nullptr;
		Comp = mComponents.at(i);

		if (Comp->Type == ComponentType::MATERIAL)
		{
			ComponentMaterial* Mat = dynamic_cast<ComponentMaterial*>(Comp);
			RELEASE(Mat);
		}
		if (Comp->Type == ComponentType::MESH)
		{
			ComponentMesh* Mes = dynamic_cast<ComponentMesh*>(Comp);
			RELEASE(Mes);
		}
		if (Comp->Type == ComponentType::TRANSFORM)
		{
			ComponentTransform* Tra = dynamic_cast<ComponentTransform*>(Comp);
			RELEASE(Tra);
		}

		if (Comp->Type == ComponentType::CAMERA)
		{
			ComponentCamera* Cam = dynamic_cast<ComponentCamera*>(Comp);
			RELEASE(Cam);
		}

		Comp = nullptr;

		//RELEASE(Comp);
		//RELEASE(mComponents[i]); 
	}

	mComponents.clear(); 

	if(mParent != nullptr)
	{
		mParent = nullptr;
	}

	if(mTransform != nullptr)
	{
		mTransform = nullptr;
	}

	mName = "";
	isActive = false;
	isSelected = false;
};

ComponentManager* GameObject::AddComponent(ComponentType type, UI_Type ui_type, uint width, uint heigth, uint posX, uint posY, const char* imagePath)
{
	ComponentManager* Comp = nullptr;

	switch (type)
	{
	case ComponentType::TRANSFORM:
		if (mTransform == nullptr)
		{
			Comp = new ComponentTransform(this);
			Comp->Type = ComponentType::TRANSFORM;
		}
		break;
	case ComponentType::MESH:
		Comp = new ComponentMesh(this);
		Comp->Type = ComponentType::MESH;
		break;
	case ComponentType::MATERIAL:
		Comp = new ComponentMaterial(this);
		Comp->Type = ComponentType::MATERIAL;
		break;
	case ComponentType::CAMERA:
		Comp = new ComponentCamera(this); 
		Comp->Type = ComponentType::CAMERA; 
		break; 
	case ComponentType::UI:
		Comp = new ComponentUI(ui_type, this, width, heigth, posX, posY, imagePath);
		Comp->Type = ComponentType::UI;
		break;
	case ComponentType::CANVAS:
		Comp = new CanvasUI(this, width, heigth, posX, posY);
		Comp->Type = ComponentType::CANVAS;
		break;
	case ComponentType::NONE:
		LOG(LogTypeCase::L_ERROR, "The Component Type was nullptr");
		Comp->Type = ComponentType::NONE;
		break;
	default:
		break;
	}

	if (Comp != nullptr)
	{
		mComponents.push_back(Comp);
	}

	return Comp;
}

vector<ComponentManager*> GameObject::GetComponentsGameObject(ComponentType type)
{
	std::vector<ComponentManager*> VecComponentsFound;

	for (auto it = mComponents.begin(); it != mComponents.end(); ++it)
	{
		if ((*it)->Type == type) 
		{
			VecComponentsFound.push_back((*it));
		}
	}

	return VecComponentsFound;
}

ComponentManager* GameObject::GetComponentGameObject(ComponentType type)
{
	std::vector<ComponentManager*> objectMaterials;

	if (type != ComponentType::SHOWNMATERIAL)
	{
		objectMaterials = this->GetComponentsGameObject(type);
	}
	else
	{
		objectMaterials = this->GetComponentsGameObject(ComponentType::MATERIAL);
	}

	for (int i = 0; i < objectMaterials.size(); i++)
	{
		ComponentMaterial* object = (ComponentMaterial*)objectMaterials.at(i);
		
		switch (type)
		{
		case ComponentType::MESH:
			if (object->Type == type)
			{
				return (object);
			}
			break;
		case ComponentType::MATERIAL:
			if (object->Type)
			{
				return (object);
			}
			break;
		case ComponentType::SHOWNMATERIAL:
			if (object->texture != nullptr)
			{
				if (object->Type == ComponentType::MATERIAL && object->texture->ShowTextures)
				{
					return (object);
				}
			}
			break;
		case ComponentType::TRANSFORM:
			if (object->Type == type)
			{
				return (object);
			}
			break;
		case ComponentType::CAMERA: 
			if (object->Type == type)
			{
				return (object); 
			}
		case ComponentType::UI:
			if (object->Type == type)
			{
				return (object);
			}
			break;
		case ComponentType::CANVAS:
			if (object->Type == type)
			{
				return (object);
			}
			break;
		default:
			break;
		}
	}

	return nullptr;
}

bool GameObject::isActiveGameObject()
{
	return isActive; 
}

void GameObject::Enable()
{
	isActive = true; 

	if(mParent != nullptr)
	{
		mParent->Enable(); 
	}
}

void GameObject::Disable()
{
	isActive = false;
}

void GameObject::Update()
{
	GameObject* gameObjects = nullptr;

	for (uint i = 0; i < app->scene->AllGameObjectManagers.size(); i++)
	{
		gameObjects = app->scene->AllGameObjectManagers[i]; 

		if (gameObjects->isActiveGameObject()) {

			for (uint i = 0; i < gameObjects->mComponents.size(); i++)
			{
				gameObjects->mComponents[i]->Update(); 
			}
		}
	}
}

void GameObject::DeleteComponent(ComponentManager* ptr)
{
	if (ptr != nullptr)
	{
		ComponentManager* Comp = nullptr;
		Comp = ptr;

		if (Comp->Type == ComponentType::MATERIAL)
		{
			ComponentMaterial* Mat = dynamic_cast<ComponentMaterial*>(Comp);
			mComponents.erase(find(mComponents.begin(), mComponents.end(), Comp));
			RELEASE(Mat);
		}
		if (Comp->Type == ComponentType::MESH)
		{
			ComponentMesh* Mes = dynamic_cast<ComponentMesh*>(Comp);
			mComponents.erase(find(mComponents.begin(), mComponents.end(), Comp));
			RELEASE(Mes);
		}
		if (Comp->Type == ComponentType::TRANSFORM)
		{
			ComponentTransform* Tra = dynamic_cast<ComponentTransform*>(Comp);
			mComponents.erase(find(mComponents.begin(), mComponents.end(), Comp));
			RELEASE(Tra);
		}
		if (Comp->Type == ComponentType::CAMERA)
		{
			ComponentCamera* Cam = dynamic_cast<ComponentCamera*>(Comp);
			mComponents.erase(find(mComponents.begin(), mComponents.end(), Comp));
			RELEASE(Cam);
		}

		Comp = nullptr;
	}
}

void GameObject::DeleteChild(GameObject* gameObject)
{
	if (gameObject != nullptr)
	{
		gameObject->mParent->childrens.erase(find(childrens.begin(), childrens.end(), gameObject));
		delete gameObject;
		gameObject = nullptr;
	}

	if (app->scene->AllGameObjectManagers.size() > 1)
	{
		app->editor->actualMesh = app->scene->AllGameObjectManagers.at(app->scene->AllGameObjectManagers.size() - 1);
	}
	else
	{
		app->editor->actualMesh = nullptr;
	}
}

void GameObject::MoveChildIntoParent(int Key)
{
	int posInVector = 0;
	posInVector = this->SearchChildPosInVector();
	GameObject* Temp_gameObject = nullptr;

	if(Key == SDL_SCANCODE_UP)
	{
		if(posInVector > 0)
		{
			Temp_gameObject = this->mParent->childrens.at((posInVector - 1));
			this->mParent->childrens.at((posInVector - 1)) = this->mParent->childrens.at(posInVector);
			this->mParent->childrens.at(posInVector) = Temp_gameObject;
		}
	}

	if(Key == SDL_SCANCODE_DOWN)
	{
		if (posInVector < this->mParent->childrens.size() - 1)
		{
			Temp_gameObject = this->mParent->childrens.at((posInVector + 1));
			this->mParent->childrens.at((posInVector + 1)) = this->mParent->childrens.at(posInVector);
			this->mParent->childrens.at(posInVector) = Temp_gameObject;
		}
	}
}

int GameObject::SearchChildPosInVector()
{
	int i = 0;

	for (int i = 0; i < this->mParent->childrens.size(); i++)
	{
		if (this == this->mParent->childrens.at(i))
		{
			return i;
		}
	}

	return -1;
}

void GameObject::CreateEmptyObject(GameObject* gameObject)
{
	GameObject* _gameObject = new GameObject("Empty_Object", gameObject);
	
	app->scene->Selected_GameObject = app->scene->AllGameObjectManagers.at(app->scene->AllGameObjectManagers.size() - 1);
	app->scene->AllGameObjectManagers.at(app->scene->AllGameObjectManagers.size() - 1)->isSelected = true;

	int C = 0;
	for (int m = 0; m < app->scene->AllGameObjectManagers.size(); m++)
	{
		if (C > 0)
		{
			std::string s = std::to_string(C);
			_gameObject->mName = "Empty_Object" + s;
			int compare = strcmp(app->scene->AllGameObjectManagers.at(m)->mName.c_str(), _gameObject->mName.c_str());

			if (compare == 0)
			{
				std::string stri = std::to_string(C++);
				_gameObject->mName = "Empty_Object" + s;
			}
		}

		if (strcmp(app->scene->AllGameObjectManagers.at(m)->mName.c_str(), _gameObject->mName.c_str()) == 0)
		{
			if (C == 0)
			{
				_gameObject->mName = "Empty_Object";
				C++;
			}
		}
	}
}

void GameObject::ChangeParent(GameObject* gameObject)
{
	if (this != gameObject)
	{
		if(gameObject->mParent != this)
		{
			mParent->childrens.erase(find(mParent->childrens.begin(), mParent->childrens.end(), this));
			this->mParent = nullptr;
			mParent = gameObject;
			mParent->childrens.push_back(this);

			float3 position, scale; 
			Quat rotation; 

			float4x4 newTransformation = mTransform->mLocalMatrix = mParent->mTransform->GetGlobalMatrix().Inverted() * mTransform->GetGlobalMatrix(); 

			newTransformation.Decompose(position, rotation, scale);

			mTransform->mPosition = position;
			mTransform->mRotation = rotation;
			mTransform->mScale = scale;

			mTransform->mRotationEuler = rotation.ToEulerXYZ() * RADTODEG; 

			mTransform->UpdateTransformation(); 
		}
	}
}
