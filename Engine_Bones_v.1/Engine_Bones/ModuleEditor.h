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
	void ThemeUpdate();

public:
	std::vector<float> mFPSLOG;
	
	std::vector<std::string> l_Logs;

	bool AboutWindow;
	bool OpenPreferences;
	bool LogOutput;
	bool GotToBottom;
	bool Vsync;

	bool copy; 
	bool copy2; 
	bool copy3; 
	bool copy4; 
	bool copy5; 
	bool copy6; 
	bool copy7; 

	int ThemeSelector;
	float WinBright;
	int item_current_idx;
	float Volume;
};

#endif

