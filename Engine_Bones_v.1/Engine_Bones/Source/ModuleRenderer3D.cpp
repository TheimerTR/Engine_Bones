#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "Primitive.h"

#include "External\Glew\include\glew.h"
#include "External\SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include "ModuleEditor.h"
#include "AssimpManager.h"
#include "GameObjectManager.h"
#include "TextureManager.h"

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
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

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

		GLfloat LightModelAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
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

	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);

	TexturesManager* texturesManager = new TexturesManager();
	texturesManager->SetCheckerTexture();

	AssimpManager::AssimpLoader("Assets/Obj/BakerHouse.fbx", "Assets/Textures/Baker_house.dds");
	App->editor->actualMesh = GameObjectManager::AllGameObjects.at(GameObjectManager::AllGameObjects.size() - 1);
	App->editor->actualMesh->Mesh->isSelected = true;

	// Projection matrix for
	OnResize(SCREEN_WIDTH, SCREEN_HEIGHT);

	Grid.axis = true;

	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->camera->GetViewMatrix());

	// light 0 on cam pos
	lights[0].SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);

	for(uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{
	if (Wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	for (int i = 0; i < GameObjectManager::AllGameObjects.size(); i++)
	{
		RenderDraw(GameObjectManager::AllGameObjects.at(i));
	}
	
	if (App->editor->Gl_Grid)
	{
		Grid.Render();
	}

	App->editor->DrawEditor();

	SDL_GL_SwapWindow(App->window->window);

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	LOG(LogTypeCase::L_CASUAL, "Destroying 3D Renderer");
	glBindTexture(GL_TEXTURE_2D, 0);

	AssimpManager::CleanUp();

	aiDetachAllLogStreams();

	SDL_GL_DeleteContext(context);

	return true;
}


void ModuleRenderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//todo: USE MATHGEOLIB here BEFORE 1st delivery! (TIP: Use MathGeoLib/Geometry/Frustum.h, view and projection matrices are managed internally.)
	ProjectionMatrix = perspective(60.0f, (float)width / (float)height, 0.125f, 512.0f);
	glLoadMatrixf(ProjectionMatrix.M);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void ModuleRenderer3D::RenderDraw(GameObjects* gameObject)
{
	//this moves all the objects 0,0,0 from all next obj forwards
	//glTranslatef(0.0f, 0.0f, 0.0f);

	//if (gameObject->Mesh->isSelected)
	//{
	//	if (App->input->GetKey(SDL_SCANCODE_RIGHT))
	//	{
	//		gameObject->Transform->SumPosition({ 0.1f, 0.0f, 0.0f });
	//	}
	//	if (App->input->GetKey(SDL_SCANCODE_LEFT))
	//	{
	//		gameObject->Transform->SumPosition({ -0.1f, 0.0f, 0.0f });
	//	}
	//	if (App->input->GetKey(SDL_SCANCODE_UP))
	//	{
	//		gameObject->Transform->SumPosition({ 0.0f, 0.0f, -0.1f });
	//	}
	//	if (App->input->GetKey(SDL_SCANCODE_DOWN))
	//	{
	//		gameObject->Transform->SumPosition({ 0.0f, 0.0f, 0.1f });
	//	}
	//	if (App->input->GetKey(SDL_SCANCODE_X))
	//	{
	//		gameObject->Transform->SumPosition({ 0.0f, 0.1f, 0.0f });
	//	}
	//	if (App->input->GetKey(SDL_SCANCODE_Z))
	//	{
	//		gameObject->Transform->SumPosition({ 0.0f, -0.1f, 0.0f });
	//	}

	//	//this moves all the objects 0,0,0 from all next obj forwards
	//	glTranslatef(gameObject->Transform->GetPosition().x, gameObject->Transform->GetPosition().y, gameObject->Transform->GetPosition().z);

	//	//LOG(LogTypeCase::L_CASUAL, "TRANSFORM X: %d", gameObject->Transform->GetPosition().x);
	//}

	if (!GameObjectManager::AllGameObjects.empty())
	{
		if (gameObject->Mesh->ShowNormals)
		{
			/*if (App->editor->DR_Normals)
			{*/
				glBegin(GL_LINES);
				glColor4f(0.0f, 1.0f, 0.0f, 1.0f);

				for (uint i = 0; i < gameObject->Mesh->num_normals * 3; i += 3)
				{
					//Dont render vertex normals

					glVertex3f(gameObject->Mesh->vertex[i], gameObject->Mesh->vertex[i + 1], gameObject->Mesh->vertex[i + 2]);
					glVertex3f(gameObject->Mesh->vertex[i] + gameObject->Mesh->normals[i], gameObject->Mesh->vertex[i + 1] + gameObject->Mesh->normals[i + 1], gameObject->Mesh->vertex[i + 2] + gameObject->Mesh->normals[i + 2]);
				}

				for (uint i = 0; i < gameObject->Mesh->num_normals_Faces; i += 9)
				{
					//Dont render vertex normals
					float3 Vec1 = { (float)gameObject->Mesh->index[i], (float)gameObject->Mesh->index[i + 1], (float)gameObject->Mesh->index[i + 2] };
					float3 Vec2 = { (float)gameObject->Mesh->index[i + 3], (float)gameObject->Mesh->index[i + 4], (float)gameObject->Mesh->index[i + 5] };
					float3 Vec3 = { (float)gameObject->Mesh->index[i + 6], (float)gameObject->Mesh->index[i + 7], (float)gameObject->Mesh->index[i + 8] };

					float3 vec1_2 = Vec2 - Vec1;
					float3 vec2_3 = Vec3 - Vec2;

					float3 NormalVecFace;

					NormalVecFace.x = (vec1_2.y * vec2_3.z) - (vec1_2.z - vec2_3.y);
					NormalVecFace.y = (vec1_2.z * vec2_3.x) - (vec1_2.x - vec2_3.z);
					NormalVecFace.z = (vec1_2.x * vec2_3.y) - (vec1_2.y - vec2_3.x);

					NormalVecFace = NormalVecFace / NormalVecFace.Normalize();

					glVertex3f(NormalVecFace.x, NormalVecFace.y, NormalVecFace.z);
				}

				glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
				glEnd();
			//}
		}

		/*glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, Meshes->textureID);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);*/

		if (gameObject->Mesh->ShowTextures)
		{
			if (gameObject->Texture->path != nullptr)
			{
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, 0);
				glBindTexture(GL_TEXTURE_2D, gameObject->Texture->TextureID);
			}
			else
			{
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, 1);
			}
		}

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glBindVertexArray(gameObject->Mesh->VAO);

		glBindBuffer(GL_ARRAY_BUFFER, gameObject->Mesh->VBO);
		glVertexPointer(3, GL_FLOAT, 0, NULL);

		glBindBuffer(GL_ARRAY_BUFFER, gameObject->Mesh->VN);
		glNormalPointer(GL_FLOAT, 0, NULL);

		glBindBuffer(GL_ARRAY_BUFFER, gameObject->Mesh->VT);
		glTexCoordPointer(2, GL_FLOAT, 0, NULL);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gameObject->Mesh->EBO);
		glDrawElements(GL_TRIANGLES, gameObject->Mesh->num_index, GL_UNSIGNED_INT, NULL);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glEnableVertexAttribArray(0);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

		if (gameObject->Mesh->ShowTextures)
		{
			glDisable(GL_TEXTURE_2D);
		}
	}
	else
	{
		LOG(LogTypeCase::L_ERROR, "Unable to render meshes (No meshes loaded)");
	}
}