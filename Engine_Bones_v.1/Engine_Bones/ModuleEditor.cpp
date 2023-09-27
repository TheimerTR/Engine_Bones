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
	mFPSLOG.clear();
}

bool ModuleEditor::Init()
{
	AboutWindow = false;

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
			if (ImGui::MenuItem("QUIT"))
			{
				App->QuitApp();
			}
			if (ImGui::BeginMenu("Theme"))
			{
				if (ImGui::MenuItem("Classic"))
				{
					ImGui::StyleColorsClassic();
				}
				if (ImGui::MenuItem("Ligth"))
				{
					ImGui::StyleColorsLight();
				}
				if (ImGui::MenuItem("Dark"))
				{
					ImGui::StyleColorsDark();
				}
				if (ImGui::MenuItem("Nord"))
				{
					ImCandy::Theme_Nord();
				}
				if (ImGui::MenuItem("Blender Dark"))
				{
					ImCandy::Theme_Blender();
				}
				if (ImGui::MenuItem("Cyberpunk Neon"))
				{
					ImCandy::Theme_Cyberpunk();
				}	
				/*if (ImGui::MenuItem("Rainbow"))
				{
					static double s1 = 0.0;
					ImGui::PushStyleColor(ImGuiCol_Border, ImCandy::Rainbow(s1));
				}*/
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		
		if (ImGui::BeginMenu("Assets"))
		{
			ImGui::Text("Hello World!");
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

				if (ImGui::BeginPopupModal("About", &close, ImGuiWindowFlags_AlwaysAutoResize)) {

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

	if(ImGui::Begin("Configuration"))
	{
		float c_FPS = floorf(App->GetFrameRate());
		C_Math::VectorPushBack(mFPSLOG, c_FPS);

		ImGui::PlotHistogram("FPS", &mFPSLOG[0], mFPSLOG.size(), 0, NULL, 0.0f, 100.0f, ImVec2(300, 100));

		if (ImGui::SliderInt("Select the MaxFPS", &App->max_FPS, -1, 200, NULL))
		{
			App->max_FPS = App->max_FPS;
		}
	}
	ImGui::End();

	if (LogOutput)
	{
		ImGui::Begin("Log Output", &LogOutput);

		if (ImGui::Button("Clear")) ClearLogs(l_Logs);
		//ImGui::SameLine();
		//bool copyButton = ImGui::Button("Copy");
		ImGui::Separator();
		ImGui::BeginChild("scrolling");
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 1));
		//if (copyButton) ImGui::LogToClipboard();

		for (int i = 0; i < l_Logs.size(); i++)
		{
			ImGui::Text("%s", l_Logs[i].c_str());
		}
		ImGui::End();

		if (GotToBottom)
			ImGui::SetScrollHereY(1.0f);
		GotToBottom = false;

		ImGui::PopStyleVar();
		ImGui::EndChild();
		ImGui::End();
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


	ImGuiIO& io = ImGui::GetIO(); (void)io;

	// Rendering
	ImGui::Render();
	glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	return true;
}

bool ModuleEditor::CleanUp()
{
	// Cleanup of ImGui
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	return true;
}

void ModuleEditor::AddFPS(const float aFPS)
{

}
