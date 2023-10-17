#ifndef COMPONENTMANAGER_H_
#define COMPONENTMANAGER_H_

#include "Globals.h"
#include "External/MathGeoLib/include/MathGeoLib.h"
#include <vector>

#include "AssimpManager.h"
#include "TextureManager.h"

class GameObjectManager;

using namespace std;

enum  ComponentType 
{
	TRANSFORM,
	MESH,
	MATERIAL,
	NONE,
};

class ComponentManager
{
public:

	/*virtual void RenderInspector() {};*/

	virtual void Enable() {};
	virtual void Update() {};
	virtual void Disable() {};

public:
	bool active;
	ComponentType Type;
	GameObjectManager* Owner;

public:

	ComponentManager(GameObjectManager* gameObject);
	~ComponentManager();


};

//class C_Transform : public ComponentManager
//{
//public:
//	C_Transform() 
//	{
//		mPosition = float3::zero;
//		mScale = float3::zero;
//		mrotation = Quat::identity;
//
//		mGlobalMatrix = float4x4::identity;
//		mLocalMatrix = float4x4::identity;
//	};
//	~C_Transform();
//
//	void RenderInspector() override;
//
//	float3 mPosition;
//	float3 mScale;
//	Quat mrotation;
//
//	float4x4 mGlobalMatrix;
//	float4x4 mLocalMatrix;


#endif // COMPONENTMANAGER_H_