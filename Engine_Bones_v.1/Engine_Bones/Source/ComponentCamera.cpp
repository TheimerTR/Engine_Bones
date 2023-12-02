#include "ComponentCamera.h"
#include "GameObjectManager.h"

ComponentCamera::ComponentCamera(GameObjectManager* gameObject) : ComponentManager(gameObject)
{
	UUID = app->RandomGenerator.Int();
	Type = ComponentType::CAMERA;

	/* Set camera vars*/
	width = 16;
	height = 9;
	aspect_ratio = width / height; // We set aspect ratio 16:9 by now

	near_plane = 1.0;
	far_plane = 1000;
	vertical_fov = 60; /* In degrees */

	/* Set frustum vars */
	frustum.type = PerspectiveFrustum;
	frustum.pos.Set(0, 0, 0);
	frustum.front.Set(0, 0, 1);
	frustum.up.Set(0, 1, 0);
	frustum.nearPlaneDistance = near_plane;
	frustum.farPlaneDistance = far_plane;
	frustum.verticalFov = vertical_fov * DEGTORAD;
	frustum.horizontalFov = Atan(aspect_ratio * Tan(frustum.verticalFov / 2)) * 2;

	if (gameObject != nullptr) {
		Owner = gameObject; 
		frustum.pos = gameObject->mTransform->GetPosition();
	}
	else 
	{
		//Owner = app->scene->Root; 
		frustum.pos = float3::zero; 
	} 
}

ComponentCamera::~ComponentCamera() {


}

bool ComponentCamera::Update()
{
	UpdateFrustum();

	return true; 
}

void ComponentCamera::LookAt(const float3& Spot) 
{
	float3 direction = Spot - frustum.pos;
	float3x3 matrix = float3x3::LookAt(frustum.front, direction.Normalized(), frustum.up, float3(0, 1, 0));

	frustum.front = matrix.MulDir(frustum.front).Normalized();
	frustum.up = matrix.MulDir(frustum.up).Normalized();
}

void ComponentCamera::UpdateFrustum()
{
	const ComponentTransform* transform = dynamic_cast<ComponentTransform*>(Owner->GetComponentGameObject(ComponentType::TRANSFORM));

	float4x4 trans = transform->mGlobalMatrix;

	frustum.pos = trans.TranslatePart();
	frustum.front = trans.WorldZ().Normalized();
	frustum.up = frustum.front.Cross(-frustum.WorldRight()).Normalized();
}

void ComponentCamera::UpdateProjection()
{
	if (this != nullptr)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		glLoadMatrixf(this->GetProjectionMatrix());

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}

}

void ComponentCamera::Draw()
{
	glEnable(GL_DEPTH_TEST);

	glLoadIdentity();

	//glLoadIdentity();
;

	//glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf((GLfloat*)this->GetProjectionMatrix());

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf((GLfloat*)this->GetViewMatrix());

	glBindFramebuffer(GL_FRAMEBUFFER, frameID);

	glClearColor(0.08f, 0.08f, 0.08f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


}

void ComponentCamera::EndDraw()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);
}

void ComponentCamera::ShowCameraInfo()
{

}

void ComponentCamera::SetPos(float3 pos)
{
	frustum.pos = pos;
}

void ComponentCamera::SetFov(float vertical)
{
	frustum.verticalFov = vertical * DEGTORAD;
	frustum.horizontalFov = Atan(this->aspect_ratio * Tan(frustum.verticalFov / 2)) * 2;
}

void ComponentCamera::SetRatio(float ratio)
{
	this->aspect_ratio = ratio;
	frustum.horizontalFov = Atan(this->aspect_ratio * Tan(frustum.verticalFov / 2)) * 2;
}

float* ComponentCamera::GetViewMatrix() const
{
	static float4x4 matrix;
	matrix = frustum.ViewMatrix();
	matrix.Transpose();

	return (float*)matrix.v;
}

float* ComponentCamera::GetProjectionMatrix() const
{
	static float4x4 matrix;
	matrix = frustum.ProjectionMatrix();
	matrix.Transpose();

	return (float*)matrix.v;
}

float3 ComponentCamera::GetPosition()
{
	return frustum.pos; 
}

void ComponentCamera::FrameBuffer(int w, int h)
{
	SetRatio((float)w / (float)h); 

	if (frameID != 0)
		glDeleteFramebuffers(1, (GLuint*)&frameID);

	if (frameTexture != 0)
		glDeleteTextures(1, (GLuint*)&frameTexture);

	if (rbo != 0)
		glDeleteRenderbuffers(1, (GLuint*)&rbo);

	glGenFramebuffers(1, (GLuint*)&frameID); 
	glBindFramebuffer(GL_FRAMEBUFFER, frameID); 

	glGenTextures(1, (GLuint*)&frameTexture); 
	glBindTexture(GL_TEXTURE_2D, frameTexture); 
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frameTexture, 0);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, rbo, 0);

	glGenRenderbuffers(1, (GLuint*)&rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		LOG(LogTypeCase::L_ERROR, "Error Framebuffer");
	}

	//glBindFramebuffer(GL_FRAMEBUFFER, 0);


}

