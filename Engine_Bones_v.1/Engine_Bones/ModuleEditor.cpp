#include "ModuleEditor.h"

#include "ImGui/imgui.h"
#include "ImGui/backends/imgui_impl_sdl2.h"
#include "ImGui/backends/imgui_impl_opengl3.h"
#include <stdio.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include "SDL/include/SDL_opengl.h"
#endif

#include "ImGuiCandy/candy.h"

#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "C_Math.h"

ModuleEditor::ModuleEditor(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	mFPSLOG.reserve(MAX_LOG_FPS);
}

ModuleEditor::~ModuleEditor()
{
	ClearLogs(l_Logs);
	C_Math::ClearInt(mFPSLOG);
}

bool ModuleEditor::Init()
{
	AboutWindow = false;
	LogOutput = false; 
	OpenPreferences = false;
	ThemeSelector = 2;
	WinBright = 0.5f;
	item_current_idx = 3; // Here we store our selection data as an index.

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

	return true;
}

bool ModuleEditor::DrawEditor()
{
	// Create a New frame for ImGuy
	ImGui_ImplSDL2_NewFrame();
	ImGui_ImplOpenGL3_NewFrame();
	ImGui::NewFrame();

	ImGui::ShowDemoWindow(); 

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ImGui::Text("Hello World!");
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
				OpenPreferences = !OpenPreferences;
			}

			if (ImGui::MenuItem("QUIT"))
			{
				App->QuitApp();
			}

			ImGui::EndMenu();
		}
		
		if (ImGui::BeginMenu("Assets"))
		{
			ImGui::Text("Hello World!");
			ImGui::EndMenu();
		}
		
		if (ImGui::BeginMenu("Windows"))
		{
			if (ImGui::MenuItem("Log Console"))
			{
				LogOutput = !LogOutput;
				GotToBottom = !GotToBottom;
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{

			if (ImGui::Button("About"))
			{
				ImGui::OpenPopup("About");
			}
				ImVec2 center = ImGui::GetMainViewport()->GetCenter();
				ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

				bool close = true; 

				if (ImGui::BeginPopupModal("About", &close, ImGuiWindowFlags_AlwaysAutoResize)) 
				{

					ImGui::Text("Bones Engine\n");
					ImGui::Text("An Incredible 3D Game Engine which was made by a dinosaur :O");
					ImGui::Separator();
					ImGui::Text("Authors:\n");
					ImGui::Text("Alberto Hidalgo Garcia\nSonia Cristina Ojeda Lanz");
					ImGui::Text("\nThird Party Libraries used:");
					ImGui::Text("\nLisence:\n MIT Lisence ");

					ImGui::EndPopup();
				}
			
			ImGui::EndMenu();
		}

	}
	ImGui::EndMainMenuBar();

	if (LogOutput)
	{
		ImGui::Begin("Log Output", &LogOutput); 

		//ImGui::Separator();

		for (int i = 0; i < l_Logs.size(); i++)
		{
			ImGui::Text("%s", l_Logs[i].c_str());
		}

		ClearLogs(l_Logs);

		//if (ImGui::Button("Clear"))
		//{
		//	ClearLogs(l_Logs);
		//}

		ImGui::End();
	}

	if(OpenPreferences)
	{
		ImGui::OpenPopup("Preferences");
	}
	
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal("Preferences", &OpenPreferences, ImGuiWindowFlags_AlwaysAutoResize))
	{
		//Brightness modulator
		if (ImGui::SliderFloat("Brightness", &WinBright, 0.f, 1.f))
		{
			SDL_SetWindowBrightness(App->window->window, WinBright);
		}

		//Frame rounding
		ImGuiStyle& style = ImGui::GetStyle();

		if (ImGui::SliderFloat("FrameRounding", &style.FrameRounding, 0.0f, 12.0f, "%.0f"))
		{
			style.GrabRounding = style.FrameRounding; // Make GrabRounding always the same value as FrameRounding
		}

		//Checkbox of window size
		static ImGuiComboFlags flags = 0;
		ImGui::CheckboxFlags("ImGuiComboFlags_PopupAlignLeft", &flags, ImGuiComboFlags_PopupAlignLeft);
		if (ImGui::CheckboxFlags("ImGuiComboFlags_NoArrowButton", &flags, ImGuiComboFlags_NoArrowButton))
			flags &= ~ImGuiComboFlags_NoPreview;     // Clear the other flag, as we cannot combine both
		if (ImGui::CheckboxFlags("ImGuiComboFlags_NoPreview", &flags, ImGuiComboFlags_NoPreview))
			flags &= ~ImGuiComboFlags_NoArrowButton; // Clear the other flag, as we cannot combine both

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

		//FPS_Graph
		ImGui::Text("FPS Limiter");
		float c_FPS = floorf(App->GetFrameRate());
		C_Math::VectorPushBack(mFPSLOG, c_FPS);

		ImGui::PlotHistogram("FPS", &mFPSLOG[0], mFPSLOG.size(), 0, NULL, 0.0f, 100.0f, ImVec2(300, 100));

		if (ImGui::SliderInt("Select the MaxFPS", &App->max_FPS, -1, 200, NULL))
		{
			App->max_FPS = App->max_FPS;
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


	////Simple box to close the app
	//OpenWindow = true;

	//ImGui::Begin("QUIT WINDOW", &OpenWindow);
	//ImGui::Text("PRESS ME TO EXIT!");
	//if (ImGui::Button("QUIT"))
	//{
	//	return UPDATE_STOP;
	//}
	//ImGui::End();

	//// Window Demo from ImGuy
	//ImGui::ShowDemoWindow();

	////Sonia Toolbar
	//bool ShowMenu;

	//ImGui::Begin("Tool bar", &ShowMenu, ImGuiWindowFlags_MenuBar);

	//if (ImGui::BeginMenuBar())
	//{
	//	if (ImGui::BeginMenu("Menu"))
	//	{
	//		if (ImGui::MenuItem("New")) {}
	//		if (ImGui::MenuItem("Open")) {}
	//		if (ImGui::MenuItem("Save")) {}

	//		ImGui::EndMenu();
	//	}

	//	if (ImGui::BeginMenu("Settings"))
	//	{
	//		ImGui::EndMenu();
	//	}

	//	ImGui::EndMenuBar();
	//}

	//ImGui::Text("Lo logree :D");

	//ImGui::End();

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

	ImGuiIO& io = ImGui::GetIO(); (void)io;

	// Rendering
	ImGui::Render();
	glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	return true;
}

void ModuleEditor::DefaultConfig()
{
	WinBright =	1.0f;
	SDL_SetWindowBrightness(App->window->window, WinBright);
	SDL_SetWindowSize(App->window->window, SCREEN_WIDTH, SCREEN_HEIGHT);

	ImGuiStyle& style = ImGui::GetStyle();

	style.FrameRounding = 0.0f;
	style.GrabRounding = style.FrameRounding; // Make GrabRounding always the same value as FrameRounding

	item_current_idx = 3; // Here we store our selection data as an index.

	App->max_FPS = 60;
}

bool ModuleEditor::CleanUp()
{
	ClearLogs(l_Logs);

	// Cleanup of ImGui
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	return true;
}