#include "ModuleEditor.h"
#include "ModuleRenderer3D.h"

#include "External/Glew/include/glew.h"
#include "External/ImGui/imgui.h"
#include "External/ImGui/backends/imgui_impl_sdl2.h"
#include "External/ImGui/backends/imgui_impl_opengl3.h"
#include <stdio.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include "External/SDL/include/SDL_opengl.h"
#endif

#include "External/ImGuiCandy/candy.h"

#include "External/mmgr/mmgr.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "C_Math.h"
#include "AssimpManager.h"
#include "ComponentManager.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "TextureManager.h"
#include "FileSystemManager.h"

#include"External/Assimp/include/version.h"

static ImGuiComboFlags flags;

ModuleEditor::ModuleEditor(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	mFPSLOG.reserve(MAX_LOG_FPS);
	l_Logs.Log.reserve(MAX_LOG_FPS);
	l_Logs.Type.reserve(MAX_LOG_FPS);
}

ModuleEditor::~ModuleEditor()
{
	ClearLogs(l_Logs.Log);
	C_Math::ClearVec(mFPSLOG);
	C_Math::ClearVec(l_Logs.Type);
}

bool ModuleEditor::Init()
{
	AboutWindow = false;
	LogOutput = false; 
	Hierarchy = false;
	OpenPreferences = false;
	DemoWindow = false;
	OpenAbout = false;
	InfoGWindow = false;
	isMovingParent = false;
	isMovingChild = false;
	ThemeSelector = 2;
	SelectPrimitive = 0;
	Log_current_idx = 3;
	OG_ChildListPos = 0;
	Actual_ChildPos = 0;

	moveEntityTo = nullptr;
	hoveredItem = nullptr;

	numcap = true; 

	// Cheking Version of ImGuI and Init the Context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // IF using Docking Branch

	// Select the color of the UI
	ImGui::StyleColorsDark();

	// Init SDL2 and OPENGL3 to render 
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
	ImGui_ImplOpenGL3_Init("#version 130");

	DefaultConfig();

	return true;
}

bool ModuleEditor::DrawEditor()
{
	// Create a New frame for ImGuy
	ImGui_ImplSDL2_NewFrame();
	ImGui_ImplOpenGL3_NewFrame();
	ImGui::NewFrame();

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("QUIT"))
			{
				App->QuitApp();
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Editor"))
		{
			if (ImGui::BeginMenu("Theme"))
			{
				if (ImGui::MenuItem("Classic"))
				{
					ThemeSelector = 0;
					ImGui::StyleColorsClassic();
				}
				if (ImGui::MenuItem("Ligth"))
				{
					ThemeSelector = 1;
					ImGui::StyleColorsLight();
				}
				if (ImGui::MenuItem("Dark"))
				{
					ThemeSelector = 2;
					ImGui::StyleColorsDark();
				}
				if (ImGui::MenuItem("Nord"))
				{
					ThemeSelector = 3;
					ImCandy::Theme_Nord();
				}
				if (ImGui::MenuItem("Blender Dark"))
				{
					ThemeSelector = 4;
					ImCandy::Theme_Blender();
				}
				if (ImGui::MenuItem("Cyberpunk Neon"))
				{
					ThemeSelector = 5;
					ImCandy::Theme_Cyberpunk();
				}
				/*if (ImGui::MenuItem("Rainbow"))
				{
					static double s1 = 0.0;
					ImGui::PushStyleColor(ImGuiCol_Border, ImCandy::Rainbow(s1));
				}*/

				ImGui::EndMenu();
			}

			if (ImGui::MenuItem("Preferences"))
			{
				ThemeUpdate();
				OpenPreferences = !OpenPreferences;
			}

			ImGui::EndMenu();
		}
		
		if (ImGui::BeginMenu("Assets"))
		{
			if (ImGui::MenuItem("Create Empty"))
			{
				GameObjectManager* gameObject = new GameObjectManager("Empty_Object", app->scene->Root);
				App->scene->Selected_GameObject = App->scene->AllGameObjectManagers.at(App->scene->AllGameObjectManagers.size() - 1);
				App->scene->AllGameObjectManagers.at(App->scene->AllGameObjectManagers.size() - 1)->isSelected = true;
			}

			if (ImGui::BeginMenu("Render Primitive"))
			{
				if (ImGui::MenuItem("Cube"))
				{
					AssimpManager::AssimpLoader("Assets/Primitives/Cube.fbx", "Assets/Textures/Grass.dds");
					App->scene->Selected_GameObject = App->scene->AllGameObjectManagers.at(App->scene->AllGameObjectManagers.size() - 1);
					App->scene->AllGameObjectManagers.at(App->scene->AllGameObjectManagers.size() - 1)->isSelected = true;
				}
				if (ImGui::MenuItem("Cylinder"))
				{
					AssimpManager::AssimpLoader("Assets/Primitives/Cylinder.fbx");
				}
				if (ImGui::MenuItem("Pyramid"))
				{
					AssimpManager::AssimpLoader("Assets/Primitives/Pyramid.fbx");
					App->scene->Selected_GameObject = App->scene->AllGameObjectManagers.at(App->scene->AllGameObjectManagers.size() - 1);
					App->scene->AllGameObjectManagers.at(App->scene->AllGameObjectManagers.size() - 1)->isSelected = true;
				}	

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Render Obj"))
			{
				if (ImGui::MenuItem("House"))
				{
					AssimpManager::AssimpLoader("Assets/Obj/BakerHouse.fbx", "Assets/Textures/Baker_house.dds");
					App->scene->Selected_GameObject = App->scene->AllGameObjectManagers.at(App->scene->AllGameObjectManagers.size() - 1);
					App->scene->AllGameObjectManagers.at(App->scene->AllGameObjectManagers.size() - 1)->isSelected = true;
				}
				if (ImGui::MenuItem("Warrior"))
				{
					AssimpManager::AssimpLoader("Assets/Obj/warrior.fbx");
					App->scene->Selected_GameObject = App->scene->AllGameObjectManagers.at(App->scene->AllGameObjectManagers.size() - 1);
					App->scene->AllGameObjectManagers.at(App->scene->AllGameObjectManagers.size() - 1)->isSelected = true;
				}

				if (ImGui::MenuItem("Sunrise"))
				{
					ComponentMaterial* objectTexture = nullptr; 
					TexturesManager* texturesManager = new TexturesManager();

					AssimpManager::AssimpLoader("Assets/Obj/SunAnimation.fbx");
					App->scene->Selected_GameObject = App->scene->AllGameObjectManagers.at(App->scene->AllGameObjectManagers.size() - 1);
					App->scene->AllGameObjectManagers.at(App->scene->AllGameObjectManagers.size() - 1)->isSelected = true;

					objectTexture = dynamic_cast<ComponentMaterial*>(App->scene->AllGameObjectManagers.at(App->scene->AllGameObjectManagers.size() - 2)->AddComponent(ComponentType::MATERIAL));
					objectTexture->SetTexture(texturesManager->TexLoader("Assets/Textures/TEX_SunMan_BaseColor.dds"));

					objectTexture = dynamic_cast<ComponentMaterial*>(App->scene->AllGameObjectManagers.at(App->scene->AllGameObjectManagers.size() - 1)->AddComponent(ComponentType::MATERIAL));
					objectTexture->SetTexture(texturesManager->TexLoader("Assets/Textures/TEX_SunManEye_BaseColor.dds"));
				}

				ImGui::EndMenu();
			}

			ImGui::EndMenu();
		}
		
		if (ImGui::BeginMenu("Windows"))
		{
			if (ImGui::MenuItem("Log Console"))
			{
				ThemeUpdate();
				LogOutput = !LogOutput;
			}	
			
			if (ImGui::MenuItem("Hierarchy"))
			{
				ThemeUpdate();
				Hierarchy = !Hierarchy;
			}	
			
			if (ImGui::MenuItem("GUI Demo Window"))
			{
				ThemeUpdate();
				DemoWindow = !DemoWindow;
			}

			if (ImGui::MenuItem("Inspector"))
			{
				ThemeUpdate(); 
				InfoGWindow = !InfoGWindow;
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("About"))
			{
				ThemeUpdate();
				OpenAbout = !OpenAbout;
			}
			
			ImGui::EndMenu();
		}
	}
	ImGui::EndMainMenuBar();

	//Demo Window
	if(DemoWindow)
	{
		ImGui::ShowDemoWindow();
	}

	if(Hierarchy)
	{
		ImVec2 pos = { 200, 500 };
		ImGui::SetNextWindowSize(pos);

		ImGui::Begin("Hierarchy", &Hierarchy);

		HierarchyWindowDisplay(app->scene->Root);

		ImGui::End();
	}

	if (isMovingParent)
	{
		isMovingChild = false;

		ImGuiWindowFlags window_flags = 0;
		window_flags |= ImGuiWindowFlags_NoBackground;
		window_flags |= ImGuiWindowFlags_NoTitleBar;
		window_flags |= ImGuiWindowFlags_NoMouseInputs;

		ImVec2 pos = ImGui::GetMousePos();
		ImVec2 size = { 3000, 20 };
		ImGui::SetNextWindowPos(pos);
		ImGui::SetNextWindowSize(size);

		ImGui::Begin("Moving", 0, window_flags);
		ImGui::Text(" New Parent %s, click to confirm, escape to cancel", hoveredItem->mName.c_str());
		ImGui::End();
	}

	if(isMovingChild)
	{
		isMovingParent = false;

		ImGuiWindowFlags window_flags = 0;
		window_flags |= ImGuiWindowFlags_NoBackground;
		window_flags |= ImGuiWindowFlags_NoTitleBar;
		window_flags |= ImGuiWindowFlags_NoMouseInputs;

		ImVec2 pos = ImGui::GetMousePos();
		ImVec2 size = { 3000, 20 };
		ImGui::SetNextWindowPos(pos);
		ImGui::SetNextWindowSize(size);

		ImGui::Begin("Moving", 0, window_flags);
		ImGui::Text(" Press arrow Up and Down to move, enter to confirm, escape to cancel");
		ImGui::End();

		int Key = 0;

		Actual_ChildPos = actualMesh->SearchChildPosInVector();

		if(App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN)
		{
			Key = SDL_SCANCODE_UP;

			actualMesh->MoveChildIntoParent(Key);
		}
		
		if(App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN)
		{
			Key = SDL_SCANCODE_DOWN;

			actualMesh->MoveChildIntoParent(Key);
		}
	}

	//Output Window
	if (LogOutput)
	{
		//ImGui::Separator();

		ImGui::Begin("Log Output", &LogOutput);

		switch ((LogTypeCase)Log_current_idx)
		{
			case LogTypeCase::L_CASUAL:

				for (int i = 0; i < l_Logs.Log.size(); i++)
				{
					if (l_Logs.Type[i] == LogTypeCase::L_CASUAL)
					{
                         ImGui::Text("%s", l_Logs.Log[i].c_str());
					}
				}
				break;

			case LogTypeCase::L_WARNING:

				for (int i = 0; i < l_Logs.Log.size(); i++)
				{
					if (l_Logs.Type[i] == LogTypeCase::L_WARNING)
					{
						ImGui::Text("%s", l_Logs.Log[i].c_str());
					}
				}
				break;

			case LogTypeCase::L_ERROR:

				for (int i = 0; i < l_Logs.Log.size(); i++)
				{
					if (l_Logs.Type[i] == LogTypeCase::L_ERROR)
					{
						ImGui::Text("%s", l_Logs.Log[i].c_str());
					}
				}
				break;

			case LogTypeCase::L_ALL:

				for (int i = 0; i < l_Logs.Log.size(); i++)
				{
					ImGui::Text("%s", l_Logs.Log[i].c_str());
				}
				break;

				default:
					break;
		}

		const char* items[] = { "NORMAL", "WARNING", "ERROR", "ALL" };
		const char* combo_preview_value = items[Log_current_idx];  // Pass in the preview value visible before opening the combo (it could be anything)
		if (ImGui::BeginCombo("Log Selector", combo_preview_value, flags))
		{
			for (int n = 0; n < IM_ARRAYSIZE(items); n++)
			{
				const bool is_selected = (Log_current_idx == n);

				if (ImGui::Selectable(items[n], is_selected))
				{
					Log_current_idx = n;
				}

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}

		//ClearLogs(l_Logs);

		if (ImGui::Button("Clear"))
		{
			ClearLogs(l_Logs.Log);
			C_Math::ClearVec(l_Logs.Type);
		}

		ImGui::End();
	}

	if(InfoGWindow)
	{
		if (actualMesh != nullptr)
		{
			InfoGameObjectWindow(actualMesh);
		}
	}

	if (OpenAbout)
	{
		ImGui::OpenPopup("About");
	}

	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	bool close = true;

	if (ImGui::BeginPopupModal("About", &OpenAbout, ImGuiWindowFlags_AlwaysAutoResize))
	{

		ImGui::Text("Bones Engine\n");
		ImGui::Text("An amazing 3D Engine for your amazing 3D games");
		copy = ImGui::Button("View on GitHub");
		if (copy) {

			OsOpenInShell("https://github.com/TheimerTR/Engine_Bones");
		}
		ImGui::Separator();
		ImGui::Text("Authors:\n");
		ImGui::Text("Alberto Hidalgo Garcia\nSonia Cristina Ojeda Lanz");

		ImGui::Text("\nThird Party Libraries used:");
		
		std::string strVer;
		SDL_version version;
		SDL_GetVersion(&version);

		strVer = std::to_string(version.major) + '.' + std::to_string(version.minor) + '.' + std::to_string(version.patch);
		ImGui::Text("SDL v %s ", strVer.c_str());

		copy = ImGui::Button("Copy Link SDL");

		if (copy) {

			OsOpenInShell("https://www.libsdl.org/"); 
		}

		std::string gui = (ImGui::GetVersion());
		ImGui::Text("ImGui v %s", gui.c_str());

		copy = ImGui::Button("Copy Link ImGui");

		if (copy) {

			OsOpenInShell("https://github.com/ocornut/imgui"); 
		}

		std::string gl = (const char*)glGetString(GL_VERSION);
		ImGui::Text("OpenGL v %s", gl.c_str());

		copy = ImGui::Button("Copy Link OpenGL");

		if (copy) {

			OsOpenInShell("https://www.opengl.org/"); 
		}

		std::string glew = (const char*)glewGetString(GLEW_VERSION);
		ImGui::Text("Glew v %s", glew.c_str());

		copy = ImGui::Button("Copy Link Glew");

		if (copy) {

			OsOpenInShell("https://glew.sourceforge.net/");
		}

		ImGui::Text("ImGuiCandy");

		copy = ImGui::Button("Copy Link Candy");

		if (copy) {

			OsOpenInShell("https://github.com/Raais/ImguiCandy"); 
		}

		ImGui::Text("MathGeoLib");

		copy = ImGui::Button("Copy Link MathGeoLib");

		if (copy) {

			OsOpenInShell("https://github.com/juj/MathGeoLib"); 
		}

		ImGui::Text("Parson");

		copy = ImGui::Button("Copy Link Parson");

		if (copy) {

			OsOpenInShell("https://github.com/kgabis/parson"); 
		}

		ImGui::Text("Parson");

		copy = ImGui::Button("Copy Link Parson");

		if (copy) {

			OsOpenInShell("https://github.com/kgabis/parson"); 
		}

		ImGui::Text("Devil v1.8.0");

		copy = ImGui::Button("Copy Link Devil");

		if (copy) {

			OsOpenInShell("https://openil.sourceforge.net/"); 
		}

		std::string assimp = std::to_string(aiGetVersionMajor()) + '.' + std::to_string(aiGetVersionMinor()) + '.' + std::to_string(aiGetVersionRevision());
		ImGui::Text("Assimp v %s", gl.c_str());

		copy = ImGui::Button("Copy Link Assimp");

		if (copy) {

			OsOpenInShell("https://www.assimp.org/");
		}

		ImGui::Text("\nLisence: MIT Lisence ");

		ImGui::Text("Copyright (c) 2023 TheimerTR & SoniaOL\nPermission is hereby granted, free of charge, to any person obtaining a copy\nof this softwareand associated documentation files(the Software), to deal\n");
		ImGui::Text("in the Software without restriction, including without limitation the rights\nto use, copy, modify, merge, publish, distribute, sublicense, and /or sell\n");
		ImGui::Text("copies of the Software, and to permit persons to whom the Software is\nfurnished to do so, subject to the following conditions :\n");
		ImGui::Text("The above copyright noticeand this permission notice shall be included in all\ncopies or substantial portions of the Software.\n\n");
		ImGui::Text("THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\nIMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\n");
		ImGui::Text("FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE\nAUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER\n");
		ImGui::Text("LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,\nOUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE\n");
		ImGui::Text("SOFTWARE.");

		ImGui::EndPopup();

	}

	//Open preferences window
	if (OpenPreferences)
	{
		ImGui::OpenPopup("Preferences");
	}
	
	ImVec2 center2 = ImGui::GetMainViewport()->GetCenter();
	ImVec2 size2 = { 700, 550 };
	ImGui::SetNextWindowPos(center2, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	ImGui::SetNextWindowSize(size2);

	if (ImGui::BeginPopupModal("Preferences", &OpenPreferences, ImGuiWindowFlags_AlwaysAutoResize))
	{

		if (ImGui::CollapsingHeader("Configuration"))
		{
			ImGui::SeparatorText("General");

			//Brightness modulator
			if (ImGui::SliderFloat("Brightness", &WinBright, 0.f, 1.f))
			{
				SDL_SetWindowBrightness(App->window->window, WinBright);
			}

			//Volume modulator
			if (ImGui::SliderFloat("General Volume", &Volume, 0.0f, SDL_MIX_MAXVOLUME))
			{
				//ChangeGeneralVolume((int)Volume);
			}

			//Frame rounding
			ImGuiStyle& style = ImGui::GetStyle();

			if (ImGui::SliderFloat("Frame Rounding", &style.FrameRounding, 0.0f, 12.0f, "%.0f"))
			{
				style.GrabRounding = style.FrameRounding; // Make GrabRounding always the same value as FrameRounding
			}

			//Checkbox of window size
			//flags = 0;
			ImGui::CheckboxFlags("ImGuiComboFlags_PopupAlignLeft", &flags, ImGuiComboFlags_PopupAlignLeft);

			ImGui::SameLine();

			if (ImGui::CheckboxFlags("ImGuiComboFlags_NoArrowButton", &flags, ImGuiComboFlags_NoArrowButton))
			{
				flags &= ~ImGuiComboFlags_NoPreview;     // Clear the other flag, as we cannot combine both
			}
			ImGui::SameLine();

			if (ImGui::CheckboxFlags("ImGuiComboFlags_NoPreview", &flags, ImGuiComboFlags_NoPreview))
			{
				flags &= ~ImGuiComboFlags_NoArrowButton; // Clear the other flag, as we cannot combine both
			}

				// Using the generic BeginCombo() API, you have full control over how to display the combo contents.
				// (your selection data could be an index, a pointer to the object, an id for the object, a flag intrusively
				// stored in the object itself, etc.)
				const char* items[] = { "1920-1080", "1536-864", "1366-768", "1280-720", "360-800" };
				const char* combo_preview_value = items[item_current_idx];  // Pass in the preview value visible before opening the combo (it could be anything)

				if (ImGui::BeginCombo("Window Size", combo_preview_value, flags))
				{
					for (int n = 0; n < IM_ARRAYSIZE(items); n++)
					{
						const bool is_selected = (item_current_idx == n);

						if (ImGui::Selectable(items[n], is_selected))
						{
							item_current_idx = n;

							int w, h;
							switch (item_current_idx)
							{
							case 0:
								w = 1920;
								h = 1080;
								break;
							case 1:
								w = 1536;
								h = 864;
								break;
							case 2:
								w = 1366;
								h = 768;
								break;
							case 3:
								w = 1280;
								h = 720;
								break;
							case 4:
								w = 360;
								h = 800;
								break;
							default:
								break;
							}

							SDL_SetWindowSize(App->window->window, w, h);
						}

						// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
						if (is_selected)
						{
							ImGui::SetItemDefaultFocus();
						}
					}

					ImGui::EndCombo();
				}

			ImGui::Separator();

		}

			if (ImGui::CollapsingHeader("Render Options"))
			{
				if (numcap == true) {
					if (SDL_HasRDTSC() == true) {
						caps += "RDTSC,"; 
					}
					if (SDL_HasMMX() == true) {
						caps += "MMX,";
					}
					if (SDL_HasSSE() == true) {
						caps += "SSE,";
					}
					if (SDL_HasSSE2() == true) {
						caps += "SSE2,";
					}
					if (SDL_HasSSE3() == true) {
						caps += "SSE3,";
					}
					if (SDL_HasSSE41() == true) {
						caps += "SSE41,";
					}
					if (SDL_HasSSE42() == true) {
						caps += "SSE42,";
					}
					if (SDL_HasAVX() == true) {
						caps += "AVX,";
					}
					if (SDL_HasAltiVec() == true) {
						caps += "AltiVec,";
					}
					if (SDL_Has3DNow() == true) {
						caps += "3DNow,";
					}

					numcap = false;  
				}

				ImGui::Text("CPUs: %d (Cache: %dkb)", SDL_GetCPUCount(), SDL_GetCPUCacheLineSize());
				ImGui::Text("System RAM: %dGb", SDL_GetSystemRAM() / 1000);
				ImGui::Text("Caps: %s", caps.c_str()); 
				ImGui::Separator();

				ImGui::Text("GPU: %s", glGetString(GL_VENDOR));
				ImGui::Text("Brand: %s", glGetString(GL_RENDERER)); 

				sMStats stats = m_getMemoryStatistics();
				ImGui::Text("Total Reported Memory: %u bytes", stats.totalReportedMemory); 
				ImGui::Text("Total Actual Memory: %u bytes", stats.totalActualMemory); 
				ImGui::Text("Peak Reported Memory: %u bytes", stats.peakReportedMemory); 
				ImGui::Text("Peak Actual Memory: %u bytes", stats.peakActualMemory); 
				ImGui::Separator(); 

				if (ImGui::TreeNode("FPS##2"))
				{
					//Vsync
					if (ImGui::Checkbox("Vsync", &Vsync))
					{
						if (Vsync)
						{
							if (SDL_GL_SetSwapInterval(1) < 0)
							{
								LOG(LogTypeCase::L_WARNING, "Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
							}
						}
						else
						{
							SDL_GL_SetSwapInterval(1);
						}
					}

					//FPS_Graph
					ImGui::Text("FPS Limiter");
					float c_FPS = floorf(App->GetFrameRate());
					C_Math::VectorPushBack(mFPSLOG, c_FPS);

					ImGui::PlotHistogram("FPS", &mFPSLOG[0], mFPSLOG.size(), 0, NULL, 0.0f, 100.0f, ImVec2(300, 100));

					if (!Vsync)
					{
						if (ImGui::SliderInt("Select the MaxFPS", &App->max_FPS, -1, 200, NULL))
						{
							App->max_FPS = App->max_FPS;
						}
					}

					ImGui::TreePop();
					ImGui::Separator();

				}

				if (ImGui::TreeNode("General##2"))
				{
					//Wireframe Mode
					ImGui::Checkbox("Wireframe Rendering Mode", &App->renderer3D->Wireframe);

					//Normals Mode
					ImGui::Checkbox("Normals Rendering Mode", &DR_Normals);

					//Debug Mode
					if (ImGui::Checkbox("Debug Rendering Mode", &changeDebug))
					{
						AssimpManager::ChangeDebugMode(changeDebug);
					};

					//Gl_Grid
					if (ImGui::Checkbox("Gl_GRID", &Gl_Grid))
					{
						Gl_Grid != Gl_Grid;
					}

					ImGui::SameLine();
					
					//GL_DEPTH_TEST
					if (ImGui::Checkbox("GL_DEPTH_TEST", &Gl_DepthTest))
					{
						if (Gl_DepthTest)
						{
							glEnable(GL_DEPTH_TEST);
						}
						else
						{
							glDisable(GL_DEPTH_TEST);
						}
					}

					ImGui::SameLine();

					if (ImGui::Checkbox("GL_CULL_FACE", &Gl_CullFace))
					{
						if (Gl_CullFace)
						{
							glEnable(GL_CULL_FACE);
						}
						else
						{
							glDisable(GL_CULL_FACE);
						}
					}

					ImGui::SameLine();

					if (ImGui::Checkbox("GL_LIGHTING", &Gl_Ligthing))
					{
						if (Gl_Ligthing)
						{
							glEnable(GL_LIGHTING);
						}
						else
						{
							glDisable(GL_LIGHTING);
						}
					}

					ImGui::SameLine();

					if (ImGui::Checkbox("GL_COLOR_MATERIAL", &Gl_ColorMaterial))
					{
						if (Gl_ColorMaterial)
						{
							glEnable(GL_COLOR_MATERIAL);
						}
						else
						{
							glDisable(GL_COLOR_MATERIAL);
						}
					}

					ImGui::SameLine();

					if (ImGui::Checkbox("GL_TEXTURE_2D", &Gl_Texture2D))
					{
						if (Gl_Texture2D)
						{
							glEnable(GL_TEXTURE_2D);
						}
						else
						{
							glDisable(GL_TEXTURE_2D);
						}
					}

					if (ImGui::Checkbox("GL_LINE_SMOOTH", &Gl_LineSmooth))
					{
						if (Gl_LineSmooth)
						{
							glEnable(GL_LINE_SMOOTH);
						}
						else
						{
							glDisable(GL_LINE_SMOOTH);
						}
					}

					ImGui::SameLine();

					if (ImGui::Checkbox("GL_POLYGON_SMOOTH", &Gl_PolygonSmooth))
					{
						if (Gl_PolygonSmooth)
						{
							glEnable(GL_POLYGON_SMOOTH);
						}
						else
						{
							glDisable(GL_POLYGON_SMOOTH);
						}
					}

					ImGui::SameLine();

					if (ImGui::Checkbox("GL_ALPHA_TEST", &Gl_AlphaTest))
					{
						if (Gl_AlphaTest)
						{
							glEnable(GL_ALPHA_TEST);
						}
						else
						{
							glDisable(GL_ALPHA_TEST);
						}
					}

					ImGui::TreePop();
					ImGui::Separator();
				}
			}

			//Default Config
			if (ImGui::Button("Default"))
			{
				DefaultConfig();
			}

			//Close Preferences
			if (ImGui::Button("CLOSE"))
			{
				OpenPreferences = false;
			}

			ImGui::EndPopup();
		

	}

		ImGuiIO& io = ImGui::GetIO(); (void)io;

		// Rendering
		ImGui::Render();
		glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
			SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
		}

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		return true;
}

void ModuleEditor::HierarchyWindowDisplay(GameObjectManager* gameObject)
{
	//Add Items in Root
	if (!ImGui::IsItemHovered())
	{
		if (ImGui::BeginPopupContextWindow())
		{
			if (ImGui::BeginMenu("Add Entity"))
			{
				if (ImGui::MenuItem("Create Empty"))
				{
					actualMesh->CreateEmptyObject(App->scene->Root);
				}

				if (ImGui::BeginMenu("Render Primitive"))
				{
					if (ImGui::MenuItem("Cube"))
					{
						AssimpManager::AssimpLoader("Assets/Primitives/Cube.fbx", "Assets/Textures/Grass.dds");
						App->scene->Selected_GameObject = App->scene->AllGameObjectManagers.at(App->scene->AllGameObjectManagers.size() - 1);
						App->scene->AllGameObjectManagers.at(App->scene->AllGameObjectManagers.size() - 1)->isSelected = true;
					}
					if (ImGui::MenuItem("Cylinder"))
					{
						AssimpManager::AssimpLoader("Assets/Primitives/Cylinder.fbx");
						App->scene->Selected_GameObject = App->scene->AllGameObjectManagers.at(App->scene->AllGameObjectManagers.size() - 1);
						App->scene->AllGameObjectManagers.at(App->scene->AllGameObjectManagers.size() - 1)->isSelected = true;
					}
					if (ImGui::MenuItem("Pyramid"))
					{
						AssimpManager::AssimpLoader("Assets/Primitives/Pyramid.fbx");
						App->scene->Selected_GameObject = App->scene->AllGameObjectManagers.at(App->scene->AllGameObjectManagers.size() - 1);
						App->scene->AllGameObjectManagers.at(App->scene->AllGameObjectManagers.size() - 1)->isSelected = true;
					}

					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Render Obj"))
				{
					if (ImGui::MenuItem("House"))
					{
						AssimpManager::AssimpLoader("Assets/Obj/BakerHouse.fbx", "Assets/Textures/Baker_house.dds");
						App->scene->Selected_GameObject = App->scene->AllGameObjectManagers.at(App->scene->AllGameObjectManagers.size() - 1);
						App->scene->AllGameObjectManagers.at(App->scene->AllGameObjectManagers.size() - 1)->isSelected = true;
					}
					if (ImGui::MenuItem("Warrior"))
					{
						AssimpManager::AssimpLoader("Assets/Obj/warrior.fbx");
						App->scene->Selected_GameObject = App->scene->AllGameObjectManagers.at(App->scene->AllGameObjectManagers.size() - 1);
						App->scene->AllGameObjectManagers.at(App->scene->AllGameObjectManagers.size() - 1)->isSelected = true;
					}

					if (ImGui::MenuItem("Sunrise"))
					{
						ComponentMaterial* objectTexture = nullptr;
						TexturesManager* texturesManager = new TexturesManager();

						AssimpManager::AssimpLoader("Assets/Obj/SunAnimation.fbx");
						App->scene->Selected_GameObject = App->scene->AllGameObjectManagers.at(App->scene->AllGameObjectManagers.size() - 1);
						App->scene->AllGameObjectManagers.at(App->scene->AllGameObjectManagers.size() - 1)->isSelected = true;

						objectTexture = dynamic_cast<ComponentMaterial*>(App->scene->AllGameObjectManagers.at(App->scene->AllGameObjectManagers.size() - 2)->AddComponent(ComponentType::MATERIAL));
						objectTexture->SetTexture(texturesManager->TexLoader("Assets/Textures/TEX_SunMan_BaseColor.dds"));

						objectTexture = dynamic_cast<ComponentMaterial*>(App->scene->AllGameObjectManagers.at(App->scene->AllGameObjectManagers.size() - 1)->AddComponent(ComponentType::MATERIAL));
						objectTexture->SetTexture(texturesManager->TexLoader("Assets/Textures/TEX_SunManEye_BaseColor.dds"));
					}

					ImGui::EndMenu();
				}

				ImGui::EndMenu();
			}

			ImGui::EndPopup();
		}
	}

	//Start TreeNode
	GameObjectManager* gm = nullptr;

	treeFlags = ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow;

	for (int i = 0; i < gameObject->childrens.size(); i++)
	{
		gm = gameObject->childrens[i];

		if (gm->childrens.size() > 0)
		{
			if (gm == actualMesh)
			{
				treeFlags |= ImGuiTreeNodeFlags_Selected;
			}
			else
			{
				treeFlags |= ImGuiTreeNodeFlags_None;
			}

			bool isOpen = ImGui::TreeNodeEx((void*)(intptr_t)i, treeFlags, gm->mName.c_str());

			if(ImGui::IsItemHovered())
			{
				hoveredItem = gm;
			}

			if (ImGui::IsItemClicked())
			{
				actualMesh = gm;
				InfoGWindow = true;

				if(moveEntityTo != nullptr && isMovingParent)
				{
					moveEntityTo->ChangeParent(gm);
					isMovingParent = false;
				}
			}

			if(ImGui::BeginPopupContextItem())
			{
				if(ImGui::BeginMenu("Add Entity")) 
				{
					AddEntity(gm);

					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Add Component"))
				{
					AddComponentInInspector(gm);

					ImGui::EndMenu();
				}

				if (ImGui::MenuItem("Change Parent To:"))
				{
					moveEntityTo = gm;
					isMovingParent = true;
				}

				if (ImGui::MenuItem("Delete Entity"))
				{
					gm->mParent->DeleteChild(gm);
				}

				ImGui::EndPopup();
			}

			if (isOpen)
			{
				HierarchyWindowDisplay(gm);
				ImGui::TreePop();
			}
		}
		else
		{
			int gmFlags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

			if (gm == actualMesh)
			{
				gmFlags |= ImGuiTreeNodeFlags_Selected;
			}
			else
			{
				gmFlags |= ImGuiTreeNodeFlags_None;
			}

			ImGui::TreeNodeEx((void*)(intptr_t)i, gmFlags, gm->mName.c_str());

			if (ImGui::IsItemHovered())
			{
				hoveredItem = gm;
			}

			if (ImGui::IsItemClicked())
			{
				actualMesh = gm;
				InfoGWindow = true;

				if (moveEntityTo != nullptr && isMovingParent)
				{
					moveEntityTo->ChangeParent(gm);
					isMovingParent = false;
				}
			}

			if (ImGui::BeginPopupContextItem())
			{
				if (ImGui::BeginMenu("Add Entity"))
				{
					AddEntity(gm);

					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Add Component"))
				{
					AddComponentInInspector(gm);

					ImGui::EndMenu();
				}

				if (ImGui::MenuItem("Change Parent To:"))
				{
					moveEntityTo = gm; 
					isMovingParent = true;
				}

				if(ImGui::MenuItem("Move Into Child List:"))
				{
					OG_ChildListPos = gm->SearchChildPosInVector();
					OG_ChildList = gm->mParent->childrens;

					if (OG_ChildListPos != -1)
					{
						isMovingChild = true;
					}
					else
					{
						LOG(LogTypeCase::L_ERROR, "Child Was not found in Parent list");
					}
				}

				if (gm->mParent != App->scene->AllGameObjectManagers.at(0))
				{
					if (ImGui::MenuItem("Quit Parent"))
					{
						gm->ChangeParent(App->scene->Root);
					}
				}

				if (ImGui::MenuItem("Delete Entity"))
				{
					gm->mParent->DeleteChild(gm);
				}

				ImGui::EndPopup();
			}
		}
	}
}

void ModuleEditor::AddEntity(GameObjectManager* gm)
{
	if (ImGui::MenuItem("Create Empty"))
	{
		actualMesh->CreateEmptyObject(gm);
	}

	if (ImGui::BeginMenu("Render Primitive"))
	{
		if (ImGui::MenuItem("Cube"))
		{
			AssimpManager::AssimpLoader("Assets/Primitives/Cube.fbx", "Assets/Textures/Grass.dds");
			App->scene->Selected_GameObject = App->scene->AllGameObjectManagers.at(App->scene->AllGameObjectManagers.size() - 1);
			App->scene->AllGameObjectManagers.at(App->scene->AllGameObjectManagers.size() - 1)->isSelected = true;
			AddChildren(gm);
		}
		if (ImGui::MenuItem("Cylinder"))
		{
			AssimpManager::AssimpLoader("Assets/Primitives/Cylinder.fbx");
			App->scene->Selected_GameObject = App->scene->AllGameObjectManagers.at(App->scene->AllGameObjectManagers.size() - 1);
			App->scene->AllGameObjectManagers.at(App->scene->AllGameObjectManagers.size() - 1)->isSelected = true;
			AddChildren(gm);
		}
		if (ImGui::MenuItem("Pyramid"))
		{
			AssimpManager::AssimpLoader("Assets/Primitives/Pyramid.fbx");
			App->scene->Selected_GameObject = App->scene->AllGameObjectManagers.at(App->scene->AllGameObjectManagers.size() - 1);
			App->scene->AllGameObjectManagers.at(App->scene->AllGameObjectManagers.size() - 1)->isSelected = true;
			AddChildren(gm);
		}

		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Render Obj"))
	{
		if (ImGui::MenuItem("House"))
		{
			AssimpManager::AssimpLoader("Assets/Obj/BakerHouse.fbx", "Assets/Textures/Baker_house.dds");
			App->scene->Selected_GameObject = App->scene->AllGameObjectManagers.at(App->scene->AllGameObjectManagers.size() - 1);
			App->scene->AllGameObjectManagers.at(App->scene->AllGameObjectManagers.size() - 1)->isSelected = true;
			AddChildrenWithChildrens(gm);
		}
		if (ImGui::MenuItem("Warrior"))
		{
			AssimpManager::AssimpLoader("Assets/Obj/warrior.fbx");
			App->scene->Selected_GameObject = App->scene->AllGameObjectManagers.at(App->scene->AllGameObjectManagers.size() - 1);
			App->scene->AllGameObjectManagers.at(App->scene->AllGameObjectManagers.size() - 1)->isSelected = true;
			AddChildren(gm);
		}

		if (ImGui::MenuItem("Sunrise"))
		{
			ComponentMaterial* objectTexture = nullptr;
			TexturesManager* texturesManager = new TexturesManager();

			AssimpManager::AssimpLoader("Assets/Obj/SunAnimation.fbx");
			App->scene->Selected_GameObject = App->scene->AllGameObjectManagers.at(App->scene->AllGameObjectManagers.size() - 1);
			App->scene->AllGameObjectManagers.at(App->scene->AllGameObjectManagers.size() - 1)->isSelected = true;

			objectTexture = dynamic_cast<ComponentMaterial*>(App->scene->AllGameObjectManagers.at(App->scene->AllGameObjectManagers.size() - 2)->AddComponent(ComponentType::MATERIAL));
			objectTexture->SetTexture(texturesManager->TexLoader("Assets/Textures/TEX_SunMan_BaseColor.dds"));

			objectTexture = dynamic_cast<ComponentMaterial*>(App->scene->AllGameObjectManagers.at(App->scene->AllGameObjectManagers.size() - 1)->AddComponent(ComponentType::MATERIAL));
			objectTexture->SetTexture(texturesManager->TexLoader("Assets/Textures/TEX_SunManEye_BaseColor.dds"));
			AddChildren(gm);
		}

		ImGui::EndMenu();
	}
}

void ModuleEditor::AddChildren(GameObjectManager* gm)
{
	App->scene->Root->childrens.erase(find(App->scene->Root->childrens.begin(), App->scene->Root->childrens.end(), App->scene->AllGameObjectManagers.at(App->scene->AllGameObjectManagers.size() - 1)));
	App->scene->AllGameObjectManagers.at(App->scene->AllGameObjectManagers.size() - 1)->mParent = gm;
	gm->childrens.push_back(App->scene->AllGameObjectManagers.at(App->scene->AllGameObjectManagers.size() - 1));
}

void ModuleEditor::AddChildrenWithChildrens(GameObjectManager* gm)
{
	App->scene->Root->childrens.erase(find(App->scene->Root->childrens.begin(), App->scene->Root->childrens.end(), App->scene->AllGameObjectManagers.at(App->scene->AllGameObjectManagers.size() - 1)->mParent));
	App->scene->AllGameObjectManagers.at(App->scene->AllGameObjectManagers.size() - 1)->mParent->mParent = gm;
	gm->childrens.push_back(App->scene->AllGameObjectManagers.at(App->scene->AllGameObjectManagers.size() - 1)->mParent);
}

void ModuleEditor::AddComponentInInspector(GameObjectManager* gm)
{
	if (ImGui::BeginMenu("Mesh"))
	{
		if (ImGui::MenuItem("Cube"))
		{
			AssimpManager::SimpleAssimpLoader("Assets/Primitives/Cube.fbx", gm, "Assets/Textures/Grass.dds");
		}

		if (ImGui::MenuItem("Cylinder"))
		{
			AssimpManager::SimpleAssimpLoader("Assets/Primitives/Cylinder.fbx", gm, "Assets/Textures/Grass.dds");
		}

		if (ImGui::MenuItem("Pyramid"))
		{
			AssimpManager::SimpleAssimpLoader("Assets/Primitives/Pyramid.fbx", gm, "Assets/Textures/Grass.dds");
		}

		ImGui::EndMenu();
	}

	if (ImGui::MenuItem("Texture"))
	{
		TexturesManager* texturesManager = new TexturesManager();
		ComponentMaterial* C_Texture = dynamic_cast<ComponentMaterial*>(gm->AddComponent(ComponentType::MATERIAL));
		C_Texture->SetTexture(nullptr);
	}
}

void ModuleEditor::InfoGameObjectWindow(GameObjectManager* gameObject)
{
	ImVec2 size4 = { 350, 400 };
	ImGui::SetNextWindowSize(size4);

	ImGui::Begin("Inspector", &InfoGWindow);

	if (gameObject != nullptr)
	{
		std::vector<ComponentManager*> objectMeshes = gameObject->GetComponentsGameObject(ComponentType::MESH);
		std::vector<ComponentManager*> objectMaterials = gameObject->GetComponentsGameObject(ComponentType::MATERIAL);

		//ComponentMesh* objectMesh = (ComponentMesh*)gameObject->GetComponentGameObject(ComponentType::MESH);
		//ComponentMaterial* objectTexture = (ComponentMaterial*)gameObject->GetComponentGameObject(ComponentType::MATERIAL);

		//ComponentMesh* objectMesh = dynamic_cast<ComponentMesh*>(objectMeshes.at(i));
		//ComponentMaterial* objectTexture = dynamic_cast<ComponentMaterial*>(App->scene->AllGameObjectManagers[i]->GetComponentGameObject(ComponentType::MATERIAL));

		for (int i = 0; i < objectMeshes.size(); i++)
		{
			ComponentMesh* objectMesh = (ComponentMesh*)objectMeshes.at(i);

			if (objectMesh != NULL)
			{
				if (objectMesh->C_Mesh != nullptr)
				{
					if (ImGui::TreeNode((std::string("Mesh##%s") + to_string(i).c_str()).c_str()))
					{
						Mesh* mesh = objectMesh->C_Mesh;
						
						objectMesh->ShowInfo(mesh, i);

						ImGui::TreePop();
					}
				}
			}
		}

		for (int i = 0; i < objectMaterials.size(); i++)
		{
			ComponentMaterial* objectMaterial = (ComponentMaterial*)objectMaterials.at(i);

			if (ImGui::TreeNode((std::string("Texture##%s") + to_string(i).c_str()).c_str()))
			{
					if (objectMaterial != NULL)
					{
						Texture* texture = objectMaterial->GetTexture();

						objectMaterial->ShowInfo(texture, i);
					}

				ImGui::TreePop();
			}
		}

		ComponentTransform* transform;
		transform = gameObject->mTransform;

		transform->ShowInfo();

		if (ImGui::Button("Add Component"))
		{
			ImGui::OpenPopup("AddComponent");
		}

		if (ImGui::BeginPopup("AddComponent"))
		{

			AddComponentInInspector(gameObject);

			ImGui::EndPopup();
		}

		if (ImGui::Button("Delete Object"))
		{
			gameObject->mParent->DeleteChild(gameObject);
		}
	}

	ImGui::End();
}

void ModuleEditor::DefaultConfig()
{
	//Set Brightness of desk to 1.0f
	WinBright =	1.0f;
	SDL_SetWindowBrightness(App->window->window, WinBright);

	//Window size to 1280-720
	SDL_SetWindowSize(App->window->window, SCREEN_WIDTH, SCREEN_HEIGHT);

	//Frame Rounding to 0
	ImGuiStyle& style = ImGui::GetStyle();
	style.FrameRounding = 0.0f;
	style.GrabRounding = style.FrameRounding; // Make GrabRounding always the same value as FrameRounding
	flags = 0;

	//Window size toolbar to 1280-720
	item_current_idx = 3; // Here we store our selection data as an index.

	//Reset FPS to 60
	App->max_FPS = 60;

	//Quit Vsync
	Vsync = false;
	SDL_GL_SetSwapInterval(0);

	//General Volume
	Volume = 50;
	//ChangeGeneralVolume((int)Volume);

	//Wireframe Rendering
	App->renderer3D->Wireframe = false;

	//Normals Rendering
	DR_Normals = false;

	//Debug Rendering
	changeDebug = false;

	//GL_GRID
	Gl_Grid = true;

	//GL_DEPTH_TEST
	Gl_DepthTest = true;
	glEnable(GL_DEPTH_TEST);
	
	//GL_CULL_FACE
	Gl_CullFace = false;
	glDisable(GL_CULL_FACE);
	
	//GL_Ligthing
	Gl_Ligthing = true;
	glEnable(GL_LIGHTING);
	
	//Gl_ColorMaterial
	Gl_ColorMaterial = true;
	glEnable(GL_COLOR_MATERIAL);
	
	//Gl_Texture2D
	Gl_Texture2D = true;
	glEnable(GL_TEXTURE_2D);

	//Gl_LineSmooth
	Gl_LineSmooth = true;
	glEnable(GL_LINE_SMOOTH);
	
	//Gl_LineSmooth
	Gl_PolygonSmooth = true;
	glEnable(Gl_PolygonSmooth);
	
	//Gl_AlphaTest
	Gl_AlphaTest = true;
	glEnable(GL_ALPHA_TEST);
}

bool ModuleEditor::CleanUp()
{
	ClearLogs(l_Logs.Log);
	C_Math::ClearVec(mFPSLOG);
	C_Math::ClearVec(l_Logs.Type);

	// Cleanup of ImGui
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	return true;
}

void ModuleEditor::ThemeUpdate()
{
	switch (ThemeSelector)
	{
	case 0:
		ImGui::StyleColorsClassic();
		break;
	case 1:
		ImGui::StyleColorsLight();
		break;
	case 2:
		ImGui::StyleColorsDark();
		break;
	case 3:
		ImCandy::Theme_Nord();
		break;
	case 4:
		ImCandy::Theme_Blender();
		break;
	case 5:
		ImCandy::Theme_Cyberpunk();
		break;
	default:
		break;
	}
}

void ModuleEditor::OsOpenInShell(const char* path)
{
#ifdef _WIN32
	// Note: executable path must use backslashes!
	::ShellExecuteA(NULL, "open", path, NULL, NULL, SW_SHOWDEFAULT);
#else
#if __APPLE__
	const char* open_executable = "open";
#else
	const char* open_executable = "xdg-open";
#endif
	char command[256];
	snprintf(command, 256, "%s \"%s\"", open_executable, path);
	system(command);
#endif
}