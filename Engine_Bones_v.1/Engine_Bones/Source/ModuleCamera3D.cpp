#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "GameObjectManager.h"
#include "External/MathGeoLib/include/Math/Quat.h"
#include "ModuleInput.h"
#include "External/Glew/include/glew.h" // extension lib
#include "External\SDL\include\SDL_opengl.h"
#include "External/ImGui/backends/imgui_impl_opengl3.h"
#include <gl/GL.h>

//GameObjectManager* G_Manager = new GameObjectManager();

ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	X = float3(1.0f, 0.0f, 0.0f);
	Y = float3(0.0f, 1.0f, 0.0f);
	Z = float3(0.0f, 0.0f, 1.0f);

	Position = float3(0.0f, 10.0f, 5.0f);
	Reference = float3(0.0f, 0.0f, 0.0f);
	ViewMatrix = IdentityMatrix;

	

	//CalculateViewMatrix();
}

ModuleCamera3D::~ModuleCamera3D()
{}

// -----------------------------------------------------------------
bool ModuleCamera3D::Init()
{
	LOG(LogTypeCase::L_CASUAL, "Setting up the camera");
	bool ret = true;

	cameraEditor = new ComponentCamera(nullptr);

	cameraEditor->frustum.farPlaneDistance = 5000;
	cameraEditor->frustum.pos = float3(8.0f, 3.0f, 8.0f);

	App->renderer3D->SetCameraEditor(cameraEditor);

	LookAt(float3::zero);

	return ret; 
}

bool ModuleCamera3D::Start()
{

	bool ret = true; 


	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG(LogTypeCase::L_CASUAL, "Cleaning camera");

	return true;
}

update_status ModuleCamera3D::PreUpdate(float dt)
{
//	Color c = {0.0f, 0.0f, 0.0f, 1.0f};
//	glClearColor(c.r, c.g, c.b, c.a);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	glMatrixMode(GL_MODELVIEW);
//	glLoadMatrixf((GLfloat*)ViewMatrixOpenGL().v);
//
	return UPDATE_CONTINUE;
}

float4x4 ModuleCamera3D::ViewMatrixOpenGL()
{
	math::float4x4 mat = F_Camera.ViewMatrix();
	return mat.Transposed();
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
	//// Implement a debug camera with keys and mouse
	//// Now we can make this movememnt frame rate independant!

	//float3 newPos(0,0,0);
	//float speed = 3.0f * dt;
	//if(App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
	//	speed = 8.0f * dt;

	//if(App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT) newPos.y += speed;
	//if(App->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT) newPos.y -= speed;
	//
	//if(App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)  newPos += Z * speed;
	//if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos -= Z * speed;

	//if(App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= X * speed;
	//if(App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += X * speed;

	//if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN) Focus(); 

	//if (App->input->GetMouseZ() != 0) {

	//	if (App->input->GetMouseZ() > 0) {
	//		newPos -= Z * speed * 4;
	//	}

	//	else if (App->input->GetMouseZ() < 0) {
	//		newPos += Z * speed * 4;
	//	}
	//}
	//

	//Position += newPos;
	////Reference += newPos;

	//// Mouse motion ----------------

	//if(App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	//{
	//	int dx = -App->input->GetMouseXMotion();
	//	int dy = -App->input->GetMouseYMotion();

	//	float Sensitivity = 0.35f * dt;

	//	//Position -= Reference;

	//	if(dx != 0)
	//	{
	//		float DeltaX = (float)dx * Sensitivity;

	//		float3 rotationAxis(0.0f, 1.0f, 0.0f);
	//		Quat rotationQuat = Quat::RotateAxisAngle(rotationAxis, DeltaX);

	//		X = rotationQuat * X;
	//		Y = rotationQuat * Y;
	//		Z = rotationQuat * Z;
	//	}

	//	if(dy != 0)
	//	{
	//		float DeltaY = (float)dy * Sensitivity;

	//		Quat rotationQuat = Quat::RotateAxisAngle(X, DeltaY);

	//		Y = rotationQuat * Y;
	//		Z = rotationQuat * Z;

	//		if(Y.y < 0.0f)
	//		{
	//			Z = float3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
	//			Y = Z.Cross(X);
	//		}
	//	}

	//	//Reference = Z * Reference.Length();
	//}

	//if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
	//{
	//	int dx = -App->input->GetMouseXMotion();
	//	int dy = -App->input->GetMouseYMotion();

	//	float Sensitivity = 0.35f * dt;

	//	Position -= Reference;

	//	if (dx != 0)
	//	{
	//		float DeltaX = (float)dx * Sensitivity;

	//		float3 rotationAxis(0.0f, 1.0f, 0.0f);
	//		Quat rotationQuat = Quat::RotateAxisAngle(rotationAxis, DeltaX);

	//		X = rotationQuat * X;
	//		Y = rotationQuat * Y;
	//		Z = rotationQuat * Z;
	//	}

	//	if (dy != 0)
	//	{
	//		float DeltaY = (float)dy * Sensitivity;

	//		Quat rotationQuat = Quat::RotateAxisAngle(X, DeltaY);

	//		Y = rotationQuat * Y;
	//		Z = rotationQuat * Z;

	//		if (Y.y < 0.0f)
	//		{
	//			Z = float3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
	//			Y = Z.Cross(X);
	//		}
	//	}

	//	Position = Reference + Z * Length(Position);
	//}


	////LookAt(Reference);

	////Activar si selección de objeto, asignar Reference a centro de obj
	////	LookAt(Reference);


	//// Recalculate matrix -------------
	//CalculateViewMatrix();

	return UPDATE_CONTINUE;
}

void ModuleCamera3D::Focus(){

	ComponentTransform* selected;
	GameObjectManager* gameObject; 

	for (int i = 0; i < App->scene->AllGameObjectManagers.size(); i++)
	{
		if (App->scene->AllGameObjectManagers[i]->isSelected == true) {

			gameObject = App->scene->AllGameObjectManagers[i];
		}
		
	}

	float3 selectedPos;

	selectedPos.x = gameObject->mTransform->GetPosition().x; 
	selectedPos.y = gameObject->mTransform->GetPosition().y; 
	selectedPos.z = gameObject->mTransform->GetPosition().z; 

	float3 selectedScale;

	selectedScale.x = gameObject->mTransform->GetScale().x;
	selectedScale.y = gameObject->mTransform->GetScale().y;
	selectedScale.z = gameObject->mTransform->GetScale().z;

	Position = selectedPos + (Position - selectedPos).Normalized() * 10;
	LookAt(selectedPos);

}

// -----------------------------------------------------------------
void ModuleCamera3D::Look(const float3&Position, const float3&Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;

	Z = (Position - Reference).Normalized();
	X = (float3(0.0f, 1.0f, 0.0f).Cross(Z)).Normalized();
	Y = Z.Cross(X);

	if(!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt( const float3&Spot)
{
	//Reference = Spot;

	//Z = (cameraEditor->frustum.pos - Spot).Normalized();
	//X = (float3(0.0f, 1.0f, 0.0f).Cross(Z)).Normalized();
	//Y = Z.Cross(X);

	cameraEditor->LookAt(Spot); 

	//CalculateViewMatrix();
}


// -----------------------------------------------------------------
void ModuleCamera3D::Move(const float3&Movement)
{
	Position += Movement;
	Reference += Movement;

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
float* ModuleCamera3D::GetViewMatrix()
{
	return ViewMatrix.M;
}

// -----------------------------------------------------------------
void ModuleCamera3D::CalculateViewMatrix()
{
	//todo: USE MATHGEOLIB here BEFORE 1st delivery! (TIP: Use MathGeoLib/Geometry/Frustum.h, view and projection matrices are managed internally.)
	ViewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -(X.Dot(Position)), -(Y.Dot(Position)), -(Z.Dot(Position)), 1.0f);
}