#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "Primitive.h"

#include "External\Glew\include\glew.h"
#include "External\SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include "ModuleEditor.h"
#include "ResourceManager.h"
#include "TextureManager.h"
#include "ModuleScene.h"

#include "GameObject.h"
#include "ComponentManager.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentUI.h"
#include "CanvasUI.h"
#include "ComponentText.h"

#include "ImporterTexture.h"

#include "External/ImGui/imgui.h"
#include "External/ImGui/backends/imgui_impl_sdl2.h"
#include "External/ImGui/backends/imgui_impl_opengl3.h"
#include <stdio.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include "External/SDL/include/SDL_opengl.h"
#endif

#include "External/Devil/Include/ilut.h"
#include "External/Devil/Include/ilu.h"
#include "External/Devil/Include/il.h"

#pragma comment (lib, "External/Devil/libx86/DevIL.lib")
#pragma comment (lib, "External/Devil/libx86/ILU.lib")
#pragma comment (lib, "External/Devil/libx86/ILUT.lib")

#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "External/Glew/libx86/glew32.lib")
#pragma comment (lib, "glu32.lib") /* link Microsoft OpenGL lib   */

#ifdef _DEBUG
#pragma comment (lib, "External/MathGeoLib/libx86/LibDebug/MathGeoLib.lib") /* link Microsoft OpenGL lib   */
#else
#pragma comment (lib, "External/MathGeoLib/libx86/LibRelease/MathGeoLib.lib") /* link Microsoft OpenGL lib   */
#endif // _DEBUG

ModuleRenderer3D::ModuleRenderer3D(Application* app, bool start_enabled) : Module(app, start_enabled), context(), Wireframe(false)
{
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
bool ModuleRenderer3D::Init()
{
	LOG(LogTypeCase::L_CASUAL, "Creating 3D Renderer context");
	bool ret = true;
	
	//Create context
	context = SDL_GL_CreateContext(App->window->window);
	if(context == NULL)
	{
		LOG(LogTypeCase::L_CASUAL, "OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	GLenum errorGlew = glewInit();

	//Initialize Projection Matrix
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();

	//Check for error
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		LOG(LogTypeCase::L_ERROR, "Error initializing OpenGL! %s\n", gluErrorString(error));
		ret = false;
	}

	if(ret == true)
	{
		//Use Vsync
		if(VSYNC && SDL_GL_SetSwapInterval(1) < 0)
			LOG(LogTypeCase::L_CASUAL, "Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG(LogTypeCase::L_CASUAL, "Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);
		
		//Initialize clear color
		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG(LogTypeCase::L_CASUAL, "Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.1f);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		GLfloat LightModelAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);
		
		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 2.5f);
		lights[0].Init();
		
		GLfloat MaterialAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);

		lights[0].Active(true);

		//OpenGL Enable
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_TEXTURE_2D);
		//glEnable(GL_MULTISAMPLE);
	}

	Importer::ImporterTexture::InitDevil();

	TexturesManager* texturesManager = new TexturesManager();
	texturesManager->SetCheckerTexture();

	//AssimpManager::AssimpLoader("Assets/ModelsFbx/DefinitiveStreet.fbx", nullptr);
	//AssimpManager::AssimpLoader("Assets/ModelsFbx/GrassStreet.fbx");
	//AssimpManager::AssimpLoader("Assets/ModelsFbx/FloorRoad.fbx");
	//AssimpManager::AssimpLoader("Assets/ModelsFbx/Houses.fbx");
	//AssimpManager::AssimpLoader("Assets/ModelsFbx/CrossRoad.fbx", nullptr);

	Grid.axis = true;

	OnResize(app->window->width, app->window->height); 

	return ret;
}

bool ModuleRenderer3D::Start()
{
	//ActiveCameraEditor->UpdateProjection();

	return true; 
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	ActiveCameraEditor->Draw(); 

/*	ActiveCameraEditor->Culling()*/; 

	// light 0 on cam pos
	lights[0].SetPos(ActiveCameraEditor->GetPosition().x, ActiveCameraEditor->GetPosition().y, ActiveCameraEditor->GetPosition().z);

	for(uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	if (Wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	for (int i = 0; i < App->scene->AllGameObjectManagers.size(); i++)
	{
		if (!App->scene->AllGameObjectManagers[i]->isText)
		{
			std::vector<ComponentManager*> objectMeshes = App->scene->AllGameObjectManagers[i]->GetComponentsGameObject(ComponentType::MESH);

			for (int j = 0; j < objectMeshes.size(); j++)
			{
				ComponentMaterial* objectTexture = dynamic_cast<ComponentMaterial*>(App->scene->AllGameObjectManagers[i]->GetComponentGameObject(ComponentType::SHOWNMATERIAL));
				ComponentMesh* objectMesh = dynamic_cast<ComponentMesh*>(objectMeshes.at(j));
				ComponentTransform* objectTransform = dynamic_cast<ComponentTransform*>(App->scene->AllGameObjectManagers[i]->GetComponentGameObject(ComponentType::TRANSFORM));

				if (objectTexture != nullptr)
				{
					RenderDraw(objectMesh, objectTransform, objectTexture);
				}
				else
				{
					RenderDraw(objectMesh, objectTransform);
				}
			}
		}
	}

	for (int i = 0; i < App->scene->AllGameObjectManagers.size(); i++)
	{
		ComponentUI* objectUI = dynamic_cast<ComponentUI*>(App->scene->AllGameObjectManagers[i]->GetComponentGameObject(ComponentType::UI));

		if (objectUI != nullptr)
		{
			if (App->scene->AllGameObjectManagers[i]->mParent != App->editor->Canvas && App->scene->AllGameObjectManagers[i]->mParent != App->scene->pause)
			{
				App->scene->AllGameObjectManagers[i]->ChangeParent(App->editor->Canvas);
			}
		}
	}

	for (int i = 0; i < App->scene->AllGameObjectManagers.size(); i++)
	{
		ComponentMaterial* objectTexture = dynamic_cast<ComponentMaterial*>(App->scene->AllGameObjectManagers[i]->GetComponentGameObject(ComponentType::MATERIAL));
		ComponentUI* objectUI = dynamic_cast<ComponentUI*>(App->scene->AllGameObjectManagers[i]->GetComponentGameObject(ComponentType::UI));
		CanvasUI* canvas_UI = dynamic_cast<CanvasUI*>(App->scene->AllGameObjectManagers[i]->GetComponentGameObject(ComponentType::CANVAS));

		if (canvas_UI != nullptr)
		{
			RenderCanvas(App->scene->AllGameObjectManagers[i], canvas_UI);
		}

		if (objectUI != nullptr)
		{
			RenderUI(App->scene->AllGameObjectManagers[i], objectUI, false, objectTexture);
		}
	}

	if (App->editor->Gl_Grid)
	{
		Grid.Render();
	}

	App->scene->Root->Update();

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{
	ActiveCameraEditor->EndDraw();

	CheckingActiveGame(); 

	if (cameraGame != nullptr)
	{
		cameraGame->Draw(); 

		if (cameraGame->culling == true && cameraGame->activeGame == true)
		{
			cameraGame->Culling();
		}

		else { cameraGame->UnCull();  }
	

		for (uint i = 0; i < MAX_LIGHTS; ++i)
			lights[i].Render();

		if (Wireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		for (int i = 0; i < App->scene->AllGameObjectManagers.size(); i++)
		{
			if (!App->scene->AllGameObjectManagers[i]->isText)
			{
				std::vector<ComponentManager*> objectMeshes = App->scene->AllGameObjectManagers[i]->GetComponentsGameObject(ComponentType::MESH);

				for (int j = 0; j < objectMeshes.size(); j++)
				{
					ComponentMaterial* objectTexture = dynamic_cast<ComponentMaterial*>(App->scene->AllGameObjectManagers[i]->GetComponentGameObject(ComponentType::SHOWNMATERIAL));
					ComponentMesh* objectMesh = dynamic_cast<ComponentMesh*>(objectMeshes.at(j));
					ComponentTransform* objectTransform = dynamic_cast<ComponentTransform*>(App->scene->AllGameObjectManagers[i]->GetComponentGameObject(ComponentType::TRANSFORM));

					if (objectTexture != nullptr)
					{
						RenderDraw(objectMesh, objectTransform, objectTexture);
					}
					else
					{
						RenderDraw(objectMesh, objectTransform);
					}
				}
			}
		}

		for (int i = 1; i < App->scene->AllGameObjectManagers.size(); i++)
		{
			if (App->scene->AllGameObjectManagers[i]->isActive)
			{
				ComponentMaterial* objectTexture = dynamic_cast<ComponentMaterial*>(App->scene->AllGameObjectManagers[i]->GetComponentGameObject(ComponentType::MATERIAL));
				ComponentUI* objectUI = dynamic_cast<ComponentUI*>(App->scene->AllGameObjectManagers[i]->GetComponentGameObject(ComponentType::UI));
				CanvasUI* canvas_UI = dynamic_cast<CanvasUI*>(App->scene->AllGameObjectManagers[i]->GetComponentGameObject(ComponentType::CANVAS));
				
				if (app->scene->pause != nullptr)
				{
					for (int j = 0; j < app->scene->pause->childrens.size(); j++)
					{
						if (App->scene->AllGameObjectManagers[i] == app->scene->pause->childrens[j])
						{
							app->scene->pause->childrens[j]->isActive = app->scene->openPauseMenu;

							if (objectTexture != nullptr)
							{
								objectTexture->colorTexture.a = 0.5;
							}
						}
					}
				}

				if (canvas_UI != nullptr)
				{
					RenderCanvas(App->scene->AllGameObjectManagers[i], canvas_UI);
				}
				else
				{
					if (objectUI != nullptr)
					{
						RenderUI(App->scene->AllGameObjectManagers[i], objectUI, true, objectTexture);
					}
				}
			}
		}

		if (App->editor->Gl_Grid)
		{
			Grid.Render();
		}

		App->scene->Root->Update();

		cameraGame->EndDraw(); 
	}

	glClearColor(0.08f, 0.08f, 0.08f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	App->editor->DrawEditor();

	SDL_GL_SwapWindow(App->window->window);

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	LOG(LogTypeCase::L_CASUAL, "Destroying 3D Renderer");
	glBindTexture(GL_TEXTURE_2D, 0);

	ResourceManager::CleanUp();

	aiDetachAllLogStreams();

	SDL_GL_DeleteContext(context);

	return true;
}

//void ModuleRenderer3D::UpdateProjection(ComponentCamera* cam)
//{
//	if (cam != nullptr)
//	{
//		glMatrixMode(GL_PROJECTION);
//		glLoadIdentity();
//
//		glLoadMatrixf(cam->GetProjectionMatrix());
//
//		glMatrixMode(GL_MODELVIEW);
//		glLoadIdentity();
//	}
//}

void ModuleRenderer3D::OnResize(int width, int height)
{
	float ratio = (float)width / (float)height;
	//ActiveCameraEditor->SetRatio(ratio);
	glViewport(0, 0, width, height);

	app->window->width = width; 
	app->window->height = height;

	if (ActiveCameraEditor != nullptr)
	{/*
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();*/

		glLoadMatrixf(ActiveCameraEditor->GetProjectionMatrix());

		ActiveCameraEditor->FrameBuffer(width, height);

		/*glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();*/
	}
}

void ModuleRenderer3D::RenderDraw(ComponentMesh* mesh, ComponentTransform* transform ,ComponentMaterial* texture)
{
	if (mesh != nullptr)
	{
		if (transform != nullptr)
		{
			glPushMatrix();
			glMultMatrixf(transform->GetGlobalTransposed());
		}

		if (mesh->GetMesh()->ShowNormals)
		{
				glBegin(GL_LINES);
				glColor4f(0.0f, 1.0f, 0.0f, 1.0f);

				for (uint i = 0; i < mesh->GetMesh()->num_vertex * 3; i += 3)
				{
					glVertex3f(mesh->GetMesh()->vertex[i], mesh->GetMesh()->vertex[i + 1], mesh->GetMesh()->vertex[i + 2]);
					glVertex3f(mesh->GetMesh()->vertex[i] + mesh->GetMesh()->normals[i], mesh->GetMesh()->vertex[i + 1] + mesh->GetMesh()->normals[i + 1], mesh->GetMesh()->vertex[i + 2] + mesh->GetMesh()->normals[i + 2]);
				}

				for (uint i = 0; i < mesh->GetMesh()->num_index; i += 3)
				{
					glVertex3f(mesh->GetMesh()->center[i], mesh->GetMesh()->center[i + 1], mesh->GetMesh()->center[i + 2]);
					glVertex3f(mesh->GetMesh()->center[i] + mesh->GetMesh()->center_normal[i] * 2, mesh->GetMesh()->center[i + 1] + mesh->GetMesh()->center_normal[i + 1] * 2, mesh->GetMesh()->center[i + 2] + mesh->GetMesh()->center_normal[i + 2] * 2);
				}

				for (uint i = 0; i < mesh->GetMesh()->num_index; i += 3)
				{
					float3 x0(mesh->GetMesh()->vertex[(mesh->GetMesh()->index[i] * 3)], mesh->GetMesh()->vertex[(mesh->GetMesh()->index[i] * 3) + 1], mesh->GetMesh()->vertex[(mesh->GetMesh()->index[i] * 3) + 2]);
					float3 x1(mesh->GetMesh()->vertex[(mesh->GetMesh()->index[i + 1] * 3)], mesh->GetMesh()->vertex[(mesh->GetMesh()->index[i + 1] * 3) + 1], mesh->GetMesh()->vertex[(mesh->GetMesh()->index[i + 1] * 3) + 2]);
					float3 x2(mesh->GetMesh()->vertex[(mesh->GetMesh()->index[i + 2] * 3)], mesh->GetMesh()->vertex[(mesh->GetMesh()->index[i + 2] * 3) + 1], mesh->GetMesh()->vertex[(mesh->GetMesh()->index[i + 2] * 3) + 2]);

					float3 v0 = x1 - x0;
					float3 v1 = x2 - x1;
					float3 normalized = v0.Cross(v1);

					mesh->GetMesh()->center[i] = (x0.x + x1.x + x2.x) / 3;
					mesh->GetMesh()->center[i + 1] = (x0.y + x1.y + x2.y) / 3;
					mesh->GetMesh()->center[i + 2] = (x0.z + x1.z + x2.z) / 3;

					mesh->GetMesh()->center_normal[i] = normalized.Normalized().x;
					mesh->GetMesh()->center_normal[i + 1] = normalized.Normalized().y;
					mesh->GetMesh()->center_normal[i + 2] = normalized.Normalized().z;
				}

				glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
				glEnd();
				glLineWidth(1);
		}

		if (texture != nullptr)
		{
			if (texture->GetTexture()->ShowTextures)
			{
				if (texture->CM_TextureID != 0)
				{
					glEnable(GL_TEXTURE_2D);
					glBindTexture(GL_TEXTURE_2D, texture->CM_TextureID);
				}
			}
		}
		else
		{
			//CheckerImageDefault
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 1);
		}

		if (mesh->isVisible)
		{
			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_NORMAL_ARRAY);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);

			//glBindVertexArray(mesh->VAO);

			glBindBuffer(GL_ARRAY_BUFFER, mesh->GetMesh()->VBO);
			glVertexPointer(3, GL_FLOAT, 0, NULL);

			glBindBuffer(GL_ARRAY_BUFFER, mesh->GetMesh()->VN);
			glNormalPointer(GL_FLOAT, 0, NULL);

			glBindBuffer(GL_ARRAY_BUFFER, mesh->GetMesh()->VT);
			glTexCoordPointer(2, GL_FLOAT, 0, NULL);

			if (texture != nullptr)
			{
				if (texture->Material)
				{
					//glColor4f(0.30980, 0.4590, 0.05882, 1);
					glColor4f(texture->colorTexture.r, texture->colorTexture.g, texture->colorTexture.b, texture->colorTexture.a);
				}
			}

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->GetMesh()->EBO);
			glDrawElements(GL_TRIANGLES, mesh->GetMesh()->num_index, GL_UNSIGNED_INT, NULL);

			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			//glEnableVertexAttribArray(0);

			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_NORMAL_ARRAY);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}

		glBindTexture(GL_TEXTURE_2D, 0);

		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

		if (transform != nullptr) {
			glPopMatrix(); 
		}
	}
	else
	{
		LOG(LogTypeCase::L_ERROR, "Unable to render meshes (No meshes loaded)");
	}

	if (transform->transform == true) {
		transform->UpdateTransformation(); 
		transform = false; 
	}

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

void ModuleRenderer3D::RenderUI(GameObject* gm, ComponentUI* UI_Element, bool isGameMode, ComponentMaterial* texture)
{
	UIPlane* ui_Plane = nullptr;

	ComponentTransform* transform = dynamic_cast<ComponentTransform*>(gm->GetComponentGameObject(ComponentType::TRANSFORM));

	//Put plane cases! if game or editor

	if(!isGameMode)
	{
		ui_Plane = UI_Element->PlaneInScene;
		glPushMatrix();
		glMultMatrixf((float*)gm->mTransform->GetGlobalTransposed());
	}

	if (isGameMode)
	{
		ui_Plane = UI_Element->PlaneInScene;
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0.0, App->editor->GameWindowSize.x, App->editor->GameWindowSize.y, 0.0, 0, -1.0);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glPushMatrix();
		glMultMatrixf((float*)gm->mTransform->GetGlobalTransposed());
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);

	glColor4f(UI_Element->color[0], UI_Element->color[1], UI_Element->color[2], UI_Element->color[3]);

	if (texture != nullptr)
	{
		if (texture->Material)
		{
			glColor4f(texture->colorTexture.r, texture->colorTexture.g, texture->colorTexture.b, texture->colorTexture.a);
		}
	}

	glEnable(GL_TEXTURE_2D);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnableClientState(GL_VERTEX_ARRAY);

	if (UI_Element->textCH == "")
	{
		if (texture != nullptr)
		{
			if (texture->GetTexture()->ShowTextures)
			{
				if (texture->texture->TextureID != 0)
				{
					if (texture->CM_TextureID != 0)
					{
						glBindTexture(GL_TEXTURE_2D, texture->CM_TextureID);
					}
					else if(UI_Element->texture != nullptr)
					{
						if(UI_Element->texture->TextureID != 0)
						{
							glBindTexture(GL_TEXTURE_2D, UI_Element->texture->TextureID);
						}
					}
				}
			}
		}

		glBindBuffer(GL_ARRAY_BUFFER, ui_Plane->buffer[0]);
		glVertexPointer(3, GL_FLOAT, 0, NULL);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, ui_Plane->buffer[2]);
		glTexCoordPointer(2, GL_FLOAT, 0, NULL);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ui_Plane->buffer[1]);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
	}
	else
	{
		//ComponentText* text = UI_Element->textComp;
		const char* letters = UI_Element->textCH.c_str();

		std::vector<ComponentManager*> objectPanels = gm->GetComponentsGameObject(ComponentType::MESH);

		for (int j = 0; j < objectPanels.size(); j++)
		{
			if (UI_Element->font->AllCharacters.at(letters[j]).textID != 0)
			{
				auto test = UI_Element->font->AllCharacters.at(letters[j]);
				glBindTexture(GL_TEXTURE_2D, test.textID);
			}

			ComponentMesh* panel = dynamic_cast<ComponentMesh*>(objectPanels.at(j));

			if (panel != nullptr)
			{
				if (panel->isActive)
				{
					if (panel->GetMesh() != nullptr)
					{
						glBindBuffer(GL_ARRAY_BUFFER, panel->GetMesh()->VBO);
						glVertexPointer(3, GL_FLOAT, 0, NULL);
						glBindBuffer(GL_ARRAY_BUFFER, 0);

						glEnableClientState(GL_TEXTURE_COORD_ARRAY);
						glBindBuffer(GL_ARRAY_BUFFER, panel->GetMesh()->VT);
						glTexCoordPointer(2, GL_FLOAT, 0, NULL);

						glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, panel->GetMesh()->EBO);

						glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
					}
				}
			}
		}
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_ALPHA_TEST);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	//if (!isGameMode)
		glPopMatrix();

	transform->UpdateTransformation(App->scene->isRescaleUI);
}

void ModuleRenderer3D::RenderCanvas(GameObject* gm, CanvasUI* UI_Canvas)
{
	if (App->editor->Canvas != nullptr)
	{
		ComponentTransform* transform = dynamic_cast<ComponentTransform*>(gm->GetComponentGameObject(ComponentType::TRANSFORM));
		CanvasUI* canvasUI = (CanvasUI*)(App->editor->Canvas->GetComponentGameObject(ComponentType::CANVAS));
		canvasUI->Draw();

		transform->UpdateTransformation(App->scene->isRescaleUI);
	}
}

void ModuleRenderer3D::SetCameraEditor(ComponentCamera* camera)
{
	ActiveCameraEditor = camera;

	if (ActiveCameraEditor != nullptr)
	{
		ActiveCameraEditor->FrameBuffer(app->window->width, app->window->height); 
	}
}

void ModuleRenderer3D::SetCameraGame(ComponentCamera* game)
{
	if (cameraGame != nullptr)
	cameraGame->activeGame = false;

	cameraGame = game;

	cameraGame->activeGame = true;

	if (cameraGame != nullptr)
	{
		cameraGame->FrameBuffer(app->window->width, app->window->height);
	}
}

void ModuleRenderer3D::CheckingActiveGame()
{
	if (cameraGame != nullptr)
	{
		for (int i = 0; i < app->scene->AllGameObjectManagers.size(); i++)
		{
			ComponentCamera* cameras = dynamic_cast<ComponentCamera*>(app->scene->AllGameObjectManagers[i]->GetComponentGameObject(ComponentType::CAMERA));

			if (cameras != nullptr)
			{
				if (cameras->activeGame == true && cameraGame != cameras)
				{
					SetCameraGame(cameras);
				}
			}
		}
	}
}
