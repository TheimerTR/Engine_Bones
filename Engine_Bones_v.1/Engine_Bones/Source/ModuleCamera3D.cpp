#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "GameObjectManager.h"
#include "External/MathGeoLib/include/Math/Quat.h"
#include "ModuleInput.h"
#include "External/ImGuizmo/ImGuizmo.h"
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

	//cameraEditor->frustum.farPlaneDistance = 5000;
	cameraEditor->frustum.Translate(float3(8.0, 3.0, 8.0));

	LookAt(float3::zero);

	//App->renderer3D->SetCameraEditor(cameraEditor);

	return ret; 
}

bool ModuleCamera3D::Start()
{
	bool ret = true; 

	App->renderer3D->SetCameraEditor(cameraEditor);

	// Projection matrix for
	App->renderer3D->OnResize(SCREEN_WIDTH, SCREEN_HEIGHT);

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


// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
	//// Implement a debug camera with keys and mouse
	//// Now we can make this movememnt frame rate independant!
	ImGuiIO& io = ImGui::GetIO();

	//cameraEditor->Culling();

	if (OnScene == true)
	{
		float3 newPos(0, 0, 0);
		float speed = 3.0f * dt;
		if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
			speed = 8.0f * dt;

		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT) cameraEditor->frustum.pos.y += speed;
		if (App->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT) cameraEditor->frustum.pos.y -= speed;

		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)  newPos -= cameraEditor->frustum.front * speed;
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos += cameraEditor->frustum.front * speed;

		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= cameraEditor->frustum.WorldRight() * speed;
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += cameraEditor->frustum.WorldRight() * speed;

		if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
		{
			Focus();
		}

		if (App->input->GetMouseZ() != 0) {

			if (App->input->GetMouseZ() > 0) {
				newPos -= cameraEditor->frustum.front * speed * 4;
			}

			else if (App->input->GetMouseZ() < 0) {
				newPos += cameraEditor->frustum.front * speed * 4;
			}
		}

		cameraEditor->frustum.pos += newPos;

		//Position += newPos;
		////Reference += newPos;

		//// Mouse motion ----------------

		if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
		{
			int dx = -App->input->GetMouseXMotion();
			int dy = -App->input->GetMouseYMotion();

			float Sensitivity = 0.35f * dt;

			Quat rotation;

			if (dx != 0)
			{
				float DeltaX = (float)dx * Sensitivity;

				Quat rotationQuat = Quat::identity;

				rotation = Quat::RotateY(DeltaX);

				rotationQuat = rotationQuat * rotation;

				cameraEditor->frustum.front = rotation.Mul(cameraEditor->frustum.front).Normalized();
				cameraEditor->frustum.up = rotation.Mul(cameraEditor->frustum.up).Normalized();
			}

			if (dy != 0)
			{
				float DeltaY = (float)dy * Sensitivity;

				Quat rotationQuat = Quat::identity;

				rotation = Quat::RotateAxisAngle(cameraEditor->frustum.WorldRight(), DeltaY);

				rotationQuat = rotationQuat * rotation;

				float3 up = rotation.Mul(cameraEditor->frustum.up).Normalized();

				cameraEditor->frustum.front = rotation.Mul(cameraEditor->frustum.front).Normalized();
				cameraEditor->frustum.up = up;
			}

		}

		if(App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_IDLE && (!ImGuizmo::IsUsing() && !ImGuizmo::IsOver()))
		{
			CreateRayCast();
		}

		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
		{
			int dx = -App->input->GetMouseXMotion();
			int dy = -App->input->GetMouseYMotion();

			float Sensitivity = 0.35f * dt;

			float DeltaY = (float)dy * Sensitivity;
			float DeltaX = (float)dx * Sensitivity;

			/*	AABB* box;
				float3 centerObject;
				float3 scale;
				ComponentMesh* mesh = dynamic_cast<ComponentMesh*>(app->editor->actualMesh->GetComponentGameObject(ComponentType::MESH));

				box = &mesh->global_aabb;
				centerObject = box->Centroid();
				scale = box->Size();

				float distance = cameraEditor->frustum.pos.Distance(centerObject);

				float3 focus = (centerObject + scale);

				float3 direction = cameraEditor->frustum.pos - centerObject;

				Quat quat_x(cameraEditor->frustum.WorldRight(), DeltaY);
				Quat quat_y(cameraEditor->frustum.up, DeltaX);
				direction = quat_x.Transform(direction);
				direction = quat_y.Transform(direction);

				cameraEditor->frustum.pos = centerObject + direction;
				LookAt(centerObject); */

			Quat rotation;
			AABB* box;
			float3 centerObject;
			float3 scale;
			ComponentMesh* mesh = dynamic_cast<ComponentMesh*>(app->editor->actualMesh->GetComponentGameObject(ComponentType::MESH));

			float distance = 0;

			if (mesh != nullptr)
			{
				box = &mesh->global_aabb;
				centerObject = box->Centroid();
				scale = box->Size();

				distance = cameraEditor->frustum.pos.Distance(centerObject);
			}
			else
			{
				ComponentTransform* transform = dynamic_cast<ComponentTransform*>(app->editor->actualMesh->GetComponentGameObject(ComponentType::TRANSFORM));
				centerObject.x = transform->mPosition.x;
				centerObject.y = transform->mPosition.y;
				centerObject.z = transform->mPosition.z;
				distance = cameraEditor->frustum.pos.Distance(centerObject);
			}

			if (dx != 0)
			{
				float DeltaX = (float)dx * Sensitivity;

				Quat rotationQuat = Quat::identity;

				rotation = Quat::RotateY(DeltaX);

				rotationQuat = rotationQuat * rotation;

				cameraEditor->frustum.front = rotation.Mul(cameraEditor->frustum.front).Normalized();
				cameraEditor->frustum.up = rotation.Mul(cameraEditor->frustum.up).Normalized();
			}

			if (dy != 0)
			{
				float DeltaY = (float)dy * Sensitivity;

				Quat rotationQuat = Quat::identity;

				rotation = Quat::RotateAxisAngle(cameraEditor->frustum.WorldRight(), DeltaY);

				rotationQuat = rotationQuat * rotation;

				float3 up = rotation.Mul(cameraEditor->frustum.up).Normalized();

				cameraEditor->frustum.front = rotation.Mul(cameraEditor->frustum.front).Normalized();
				cameraEditor->frustum.up = up;

			}

			cameraEditor->frustum.pos = centerObject + (cameraEditor->frustum.front * -distance);
			LookAt(centerObject);
		}

	}
	//LookAt(Reference);

	//Activar si selecci�n de objeto, asignar Reference a centro de obj
	//	LookAt(Reference);

	return UPDATE_CONTINUE;
}

void ModuleCamera3D::Focus(/*const float3 &Spot*/){

	ComponentTransform* selected;
	GameObjectManager* gameObject; 

	//for (int i = 0; i < App->scene->AllGameObjectManagers.size(); i++)
	
	if (App->editor->actualMesh != nullptr)
	{
		AABB* box;
		float3 centerObject;
		float3 scale;
		ComponentMesh* mesh = dynamic_cast<ComponentMesh*>(app->editor->actualMesh->GetComponentGameObject(ComponentType::MESH));

		if(App->editor->actualMesh->mComponents.size() > 0)
		{
			if (mesh != nullptr)
			{
				if (mesh->C_Mesh != nullptr)
				{
					box = &mesh->global_aabb;
					centerObject = box->Centroid();
					scale = box->Size();

					cameraEditor->frustum.pos.Set(centerObject.x + scale.x, centerObject.y + scale.y, centerObject.z + scale.z);

					LookAt(centerObject);
				}
			}
			else
			{
				ComponentTransform* transform = dynamic_cast<ComponentTransform*>(app->editor->actualMesh->GetComponentGameObject(ComponentType::TRANSFORM));
				cameraEditor->frustum.pos.Set(transform->mPosition.x + transform->mScale.x, transform->mPosition.y + transform->mScale.y * 2, transform->mPosition.z + (transform->mScale.z * 2) / 2);
				centerObject.x = transform->mPosition.x;
				centerObject.y = transform->mPosition.y;
				centerObject.z = transform->mPosition.z;

				LookAt(centerObject);
			}
		}

		//gameObject = App->editor->actualMesh;

		//float3 selectedPos;

		//selectedPos.x = gameObject->mTransform->GetPosition().x;
		//selectedPos.y = gameObject->mTransform->GetPosition().y;
		//selectedPos.z = gameObject->mTransform->GetPosition().z;

		//float3 selectedScale;

		//selectedScale.x = gameObject->mTransform->GetScale().x;
		//selectedScale.y = gameObject->mTransform->GetScale().y;
		//selectedScale.z = gameObject->mTransform->GetScale().z;

		

			/*	cameraEditor->frustum.pos = selectedPos + (cameraEditor->frustum.pos - selectedPos).Normalized() * 10;
				cameraEditor->LookAt(selectedPos);*/
	
	}
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
}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt( const float3&Spot)
{
	cameraEditor->LookAt(Spot);
}

// -----------------------------------------------------------------
void ModuleCamera3D::Move(const float3&Movement)
{
	Position += Movement;
	Reference += Movement;
}

ComponentMesh* ModuleCamera3D::CheckForMesh(GameObjectManager* gameObject)
{
	ComponentMesh* C_Mesh = nullptr;

	for(int i = 0; i < gameObject->childrens.size(); i++)
	{
		ComponentMesh* C_Mesh = dynamic_cast<ComponentMesh*>(gameObject->childrens[i]->GetComponentGameObject(ComponentType::MESH));

		if(C_Mesh != nullptr)
		{
			return C_Mesh;
		}

		if(gameObject->childrens[i]->childrens.size() > 0)
		{
			C_Mesh = CheckForMesh(gameObject->childrens[i]);
		}
	}

	return C_Mesh;
}

void ModuleCamera3D::CreateRayCast()
{
	if (App->scene->Root->childrens.empty()) return;

	float2 originPoint = float2((App->input->GetMousePosition().x - App->editor->scenePos.x) / App->editor->viewportSizex, (App->input->GetMousePosition().y - App->editor->scenePos.y) / App->editor->viewportSizey);

	originPoint.x = (originPoint.x - 0.5F) * 2;
	originPoint.y = -(originPoint.y - 0.5F) * 2;

	if(originPoint.x > 1 || originPoint.x < -1)
	{
		return;
	}

	if(originPoint.y > 1 || originPoint.y < -1)
	{
		return;
	}

	RayCast = cameraEditor->frustum.UnProjectLineSegment(originPoint.x, originPoint.y);

	map<float, GameObjectManager*> hitMap;

	SearchForHits(hitMap, App->scene->Root, RayCast);
}

void ModuleCamera3D::SearchForHits(map<float, GameObjectManager*> hits, GameObjectManager* Root, LineSegment& rayCast)
{
	float dist  = 0, dist_out = 0;

	ComponentMesh* meshRoot = dynamic_cast<ComponentMesh*>(Root->GetComponentGameObject(ComponentType::MESH));

	if (meshRoot != nullptr)
	{
		if (rayCast.Intersects(meshRoot->global_aabb, dist, dist_out))
		{
			hits[dist] = Root;
		}
	}

	if(!Root->childrens.empty())
	{
		for (int i = 0; i < Root->childrens.size(); i++)
		{
			SearchForHits(hits, Root->childrens[i], rayCast);
		}
	}
}
