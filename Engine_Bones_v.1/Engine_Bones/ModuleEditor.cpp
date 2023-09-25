#include "ModuleEditor.h"

#include "ImGui/imgui.h"
#include "ImGui/backends/imgui_impl_sdl2.h"
#include "ImGui/backends/imgui_impl_opengl3.h"

#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"

ModuleEditor::ModuleEditor(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

ModuleEditor::~ModuleEditor()
{
}

bool ModuleEditor::Init()
{
	// Cheking Version of ImGuI and Init the Context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	//io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

	// Select the color of the UI
	ImGui::StyleColorsDark();

	// Init SDL2 and OPENGL3 to render 
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
	ImGui_ImplOpenGL3_Init();

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
			ImGui::Text("Hello World!");
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Assets"))
		{
			ImGui::Text("Hello World!");
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{
			ImGui::Text("Hello World!");
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	if(ImGui::Begin("Configuration"))
	{
		//ImGui::PlotHistogram("FPS", &mFPSLOG.data(), mFPS.size());

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


	//ImGuiIO& io = ImGui::GetIO(); (void)io;

	//// Rendering
	//ImGui::Render();
	//glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);

	//ImGui::Render();
	//ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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
