#include "ComponentCamera.h"
#include "GameObjectManager.h"

ComponentCamera::ComponentCamera(GameObjectManager* gameObject) : ComponentManager(gameObject)
{
	frustum.type = FrustumType::PerspectiveFrustum; 
	frustum.nearPlaneDistance = 0.1f; 
	frustum.farPlaneDistance = 500.f; 
	frustum.front = float3::unitZ; 
	frustum.up = float3::unitY; 

	frustum.verticalFov = 60.0f * DEGTORAD; 
	frustum.horizontalFov = 2.0f * atanf(tanf(frustum.verticalFov / 2.0f) * 1.3f);

	if (gameObject != nullptr) {
		frustum.pos = gameObject->mTransform->GetPosition();
	}

	else { frustum.pos = float3::zero; } 
}

ComponentCamera::~ComponentCamera() {


}

