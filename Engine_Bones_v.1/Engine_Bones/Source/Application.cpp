#include "Application.h"

#include "Module.h"
#include "ModuleCamera3D.h"
#include "ModuleEditor.h"
#include "ModuleRenderer3D.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"
#include "ModuleScene.h"

extern Application* app = nullptr;
Application::Application()
{
	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);
	editor = new ModuleEditor(this);
	scene = new ModuleScene(this);
	physFSManager = new PhysFSManager(this);
	resource = new ModuleResource(this);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(physFSManager);
	AddModule(resource);

	// Renderer last!
	AddModule(scene);
	AddModule(renderer3D);
	AddModule(editor);
}

Application::~Application()
{
	for (std::vector<Module*>::iterator it = list_modules.begin(); it != list_modules.end(); ++it)
	{
		delete (*it);
		(*it) = nullptr;
	}
}

bool Application::Init()
{
	bool ret = true;

	app = this;

	max_FPS = 60;
	volume_general = 50;
	QuitApplication = false;

	// Call Init() in all modules
	for (std::vector<Module*>::const_iterator it = list_modules.cbegin(); it != list_modules.cend() && ret; ++it)
	{
		(*it)->Init();
	}

	// After all Init calls we call Start() in all modules
	LOG(LogTypeCase::L_CASUAL, "Application Start --------------");
	for (std::vector<Module*>::const_iterator it = list_modules.cbegin(); it != list_modules.cend() && ret; ++it)
	{
		(*it)->Start();
	}
	
	ms_timer.Start();
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	dt = (float)ms_timer.Read() / 1000.0f;
	ms_timer.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	if(!editor->Vsync)
	{
		Uint32 last_frame_ms = ms_timer.Read();
		float wait_time = (1000.f / (float)max_FPS) - (float)last_frame_ms;

		if((1000.f / (float)max_FPS) > last_frame_ms)
		{
			SDL_Delay(static_cast<Uint32>(fabs(wait_time)));
		}
	}
}

float Application::GetDeltaTime() const
{
	return dt;
}

float Application::GetFrameRate() const
{
	return 1.f / dt;
}

void Application::QuitApp() 
{
	QuitApplication = true;
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();
	
	for (std::vector<Module*>::const_iterator it = list_modules.cbegin(); it != list_modules.cend() && ret == UPDATE_CONTINUE; ++it)
	{
		ret = (*it)->PreUpdate(dt);
	}

	for (std::vector<Module*>::const_iterator it = list_modules.cbegin(); it != list_modules.cend() && ret == UPDATE_CONTINUE; ++it)
	{
		ret = (*it)->Update(dt);
	}

	for (std::vector<Module*>::const_iterator it = list_modules.cbegin(); it != list_modules.cend() && ret == UPDATE_CONTINUE; ++it)
	{
		ret = (*it)->PostUpdate(dt);
	}

	if(QuitApplication)
	{
		ret = UPDATE_STOP;
	}

	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;
	for (std::vector<Module*>::reverse_iterator it = list_modules.rbegin(); it != list_modules.rend() && ret; ++it)
	{
		ret = (*it)->CleanUp();
	}

	editor = nullptr;
	app = nullptr;

	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}

int Application::RandomIntGenerator()
{
	return RandomGenerator.Int();
}