#ifndef MODULE_EDITOR
#define MODULE_EDITOR

#include "Module.h"
#include "Application.h"
#include "AssimpManager.h"
#include "GameObjectManager.h"

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

	void InfoGameObjectWindow(GameObjects* gameObject);

	void ResetSelected();

public:

	struct l_Logs
	{
	public:
		std::vector<std::string> Log;
		std::vector<LogTypeCase> Type;
	}l_Logs;

	std::vector<float> mFPSLOG;

	bool copy, AboutWindow, OpenPreferences, LogOutput, Vsync, Gl_Grid, Gl_DepthTest, Gl_CullFace, Gl_Ligthing, Gl_ColorMaterial, Gl_Texture2D,
		Gl_LineSmooth, Gl_PolygonSmooth, Gl_AlphaTest, DemoWindow, OpenAbout, DR_Normals, changeDebug, Hierarchy, InfoGWindow;

	int ThemeSelector;
	int item_current_idx;
	int Log_current_idx;
	int SelectPrimitive;

	GameObjects* actualMesh;

	float Volume;
	float WinBright;
};

#endif