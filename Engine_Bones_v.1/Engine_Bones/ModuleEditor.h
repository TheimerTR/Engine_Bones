#ifndef MODULE_EDITOR
#define MODULE_EDITOR

#include "Module.h"
#include "Application.h"

#include <vector>
#include <stdio.h>

class ModuleEditor : public Module
{
public:

	ModuleEditor(Application* app, bool start_enabled = true);
	~ModuleEditor();

	bool Init() override;
	bool DrawEditor();
	bool CleanUp() override;

	void AddFPS(const float aFPS);

	std::vector<float> mFPSLOG;
};

#endif

