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

	bool copy; 

	bool AboutWindow;
	bool OpenPreferences;
	bool LogOutput;
	bool GotToBottom;
	bool Vsync;
	bool Gl_DepthTest;
	bool Gl_CullFace;
	bool Gl_Ligthing;
	bool Gl_ColorMaterial;
	bool Gl_Texture2D;
	bool Gl_LineSmooth;
	bool Gl_PolygonSmooth;
	bool Gl_AlphaTest;
	bool OpenAbout; 

	int ThemeSelector;
	int item_current_idx;
	int SelectPrimitive;

	float Volume;
	float WinBright;
};

#endif

