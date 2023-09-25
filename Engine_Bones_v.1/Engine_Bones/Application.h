#pragma once

#include "Globals.h"
#include "Timer.h"

#include "Module.h"
#include "ModuleCamera3D.h"
#include "ModuleEditor.h"
#include "ModuleRenderer3D.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"

#include<vector>

class Application
{
public:
	class ModuleWindow* window;
	class ModuleInput* input;
	class ModuleRenderer3D* renderer3D;
	class ModuleCamera3D* camera;
	class ModuleEditor* editor;

private:

	Timer	ms_timer;
	float	dt;
	std::vector<Module*> list_modules;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};