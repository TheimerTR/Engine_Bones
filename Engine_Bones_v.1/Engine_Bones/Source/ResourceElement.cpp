#include "ResourceElement.h"
#include "Application.h"
#include "ComponentManager.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"

ResourceElement::ResourceElement()
{
	name = "";
	resourceCounter = 0;
	AssetsPath = "";
	LibraryPath = "";

	type = ResourceTypes::R_NONE;
	UUID = app->RandomGenerator.Int();
}

ResourceElement::ResourceElement(ResourceTypes R_type)
{
	name = "";
	resourceCounter = 0;
	AssetsPath = "";
	LibraryPath = "";

	this->type = R_type;
	UUID = app->RandomGenerator.Int();
}

ResourceElement::ResourceElement(const char* name, const char* AssetsPath, ResourceTypes R_type, uint32 R_UUID)
{
	this->name = name;
	resourceCounter = 0;
	this->AssetsPath = AssetsPath;
	LibraryPath = "";

	this->type = R_type;
	UUID = (R_UUID == 0) ? app->RandomGenerator.Int() : R_UUID;
}

ResourceElement::~ResourceElement()
{
	app->resource->AllResourcesMap.erase(this->UUID);

	for(int i = 0; i < ParentsUUID.size(); i++)
	{
		for(int h = 0; h < app->scene->AllGameObjectManagers.size(); h++)
		{
			if(ParentsUUID[i] == app->scene->AllGameObjectManagers[h]->UUID)
			{
				if (type == ResourceTypes::R_MESH)
				{
					ComponentMesh* C_Mesh = dynamic_cast<ComponentMesh*>(app->scene->AllGameObjectManagers[h]->GetComponentGameObject(ComponentType::MESH));
					app->scene->AllGameObjectManagers[h]->DeleteComponent(C_Mesh);
				}

				if(type == ResourceTypes::R_TEXTURE)
				{
					ComponentMaterial* C_Mesh = dynamic_cast<ComponentMaterial*>(app->scene->AllGameObjectManagers[h]->GetComponentGameObject(ComponentType::MATERIAL));
					app->scene->AllGameObjectManagers[h]->DeleteComponent(C_Mesh);
				}
			}
		}
	}
	
	name = "";
	resourceCounter = -1;
	AssetsPath = "";
	LibraryPath = "";

	type = ResourceTypes::R_NONE;
	UUID = -1;
}

string ResourceElement::getAssetsPath()
{
	return this->AssetsPath;
}

string ResourceElement::getLibraryPath()
{
	return this->LibraryPath;
}

uint32 ResourceElement::getUUID()
{
	return this->UUID;
}

ResourceTypes ResourceElement::getType()
{
	return this->type;
}

uint ResourceElement::getResourceCounter()
{
	return this->resourceCounter;
}