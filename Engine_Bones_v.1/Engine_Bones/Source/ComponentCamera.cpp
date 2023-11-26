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

bool ComponentCamera::Update()
{
	float3 points[8];
	frustum.GetCornerPoints(points);

	return true; 
}

void ComponentCamera::LookAt(const float3& Spot) 
{
	frustum.front = (Spot - frustum.pos).Normalized();
	float3 X = (float3(0.0f, 1.0f, 0.0f).Cross(frustum.front)).Normalized();
	frustum.up = frustum.front.Cross(X);
}

void ComponentCamera::Draw()
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(GetProjectionMatrix());

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(GetViewMatrix());

	Color c = { 0.0f, 0.0f, 0.0f, 1.0f };
	glClearColor(c.r, c.g, c.b, c.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void ComponentCamera::EndDraw()
{
	glDisable(GL_DEPTH_TEST);
}

void ComponentCamera::SetRatio(float ratio)
{
	frustum.verticalFov = 60.0f * DEGTORAD;
	frustum.horizontalFov = ratio * atanf(tanf(frustum.verticalFov / 2.0f) * 1.3f);
}

float* ComponentCamera::GetViewMatrix()
{
	math::float4x4 mat = frustum.ViewMatrix();
	mat.Transposed();

	return (float*)mat.v; 
}

float* ComponentCamera::GetProjectionMatrix()
{
	math::float4x4 mat = frustum.ProjectionMatrix();
	mat.Transposed();

	return (float*)mat.v;
}

float3 ComponentCamera::GetPosition()
{
	return frustum.pos; 
}

