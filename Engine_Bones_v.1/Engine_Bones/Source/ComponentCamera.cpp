#include "ComponentCamera.h"
#include "GameObjectManager.h"
#include "External/ImGui/imgui.h"

ComponentCamera::ComponentCamera(GameObjectManager* gameObject) : ComponentManager(gameObject)
{
	UUID = app->RandomGenerator.Int();
	//Type = ComponentType::CAMERA;

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

ComponentCamera::~ComponentCamera() 
{
	uncull = true; 
}

bool ComponentCamera::Update()
{
	if (uncull == true)
	{
		UnCull(); 
	}

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

	glBindTexture(GL_TEXTURE_2D, 0);
	glBegin(GL_LINES);
	glLineWidth(3.0f);
	glColor4f(0.25f, 1.0f, 0.0f, 1.0f);

	for (uint i = 0; i < 12; i++)
	{
		glVertex3f(frustum.Edge(i).a.x, frustum.Edge(i).a.y, frustum.Edge(i).a.z);
		glVertex3f(frustum.Edge(i).b.x, frustum.Edge(i).b.y, frustum.Edge(i).b.z);
	}

	glEnd();
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
	if (ImGui::TreeNode("Camera##1"))
	{
		ImGui::Text("Clipping Planes"); 

		if (ImGui::DragFloat("Near", &near_plane, 0.1f, 0.0f))
		{
			frustum.nearPlaneDistance = near_plane;
		}

		if (ImGui::DragFloat("Far", &far_plane, 0.1f))
		{
			frustum.farPlaneDistance = far_plane;
		}

		if(ImGui::SliderFloat("FOV", &vertical_fov, 1, 180))
		{
			SetFov(vertical_fov);
		}

		ImGui::Checkbox("Culling", &culling);

		ImGui::TreePop();
	}
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

void ComponentCamera::Culling()
{
	bool IsInside = false; 


	for (int i = 1; i < app->scene->AllGameObjectManagers.size(); i++)
	{
		if (app->scene->AllGameObjectManagers[i]->isActive == true)
		{
			ComponentMesh* ObjectsToCull = dynamic_cast<ComponentMesh*>(app->scene->AllGameObjectManagers[i]->GetComponentGameObject(ComponentType::MESH));

			if (ObjectsToCull != nullptr)
			{
				if (InsideCulling(ObjectsToCull->global_aabb) == true)
				{
					ObjectsToCull->isVisible = true;
				}
				else
				{
					ObjectsToCull->isVisible = false;
				}
			}
		}
	}
}

bool ComponentCamera::InsideCulling(const AABB& box)
{
	float3 aabbPoints[8];
	int TotalIn = 0;
	box.GetCornerPoints(aabbPoints);

	for (int p = 0; p < 6; p++)
	{
		int InCount = 8;
		int PointIn = 1;

		for (int i = 0; i < 8; i++)
		{
			if (frustum.GetPlane(p).IsOnPositiveSide(aabbPoints[i]))
			{
				PointIn = 0;
				InCount--;
			}
		}

		if (InCount == 0)
		{
			return false;

		}

		TotalIn += PointIn;
	}

	return true; 
}

void ComponentCamera::UnCull()
{
	for (int i = 1; i < app->scene->AllGameObjectManagers.size(); i++)
	{
		if (app->scene->AllGameObjectManagers[i]->isActive == true)
		{
			ComponentMesh* ObjectsToCull = dynamic_cast<ComponentMesh*>(app->scene->AllGameObjectManagers[i]->GetComponentGameObject(ComponentType::MESH));
		
			if (ObjectsToCull != nullptr)
			{
				ObjectsToCull->isVisible = true;
			}
		}
	}

	uncull = false; 
}

