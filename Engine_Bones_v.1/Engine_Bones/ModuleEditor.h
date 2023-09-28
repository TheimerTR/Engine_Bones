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
	void DefaultConfig();

public:
	std::vector<float> mFPSLOG;
	
	std::vector<std::string> l_Logs;

	bool AboutWindow;
	bool OpenPreferences;
	bool LogOutput;
	bool GotToBottom;
	bool Vsync;

	int ThemeSelector;
	float WinBright;
	int item_current_idx;
};

#endif

