#ifndef MODULE_EDITOR
#define MODULE_EDITOR

#include "Module.h"
#include "Application.h"
#include "AssimpManager.h"
#include "GameObjectManager.h"
#include "ResourceElement.h"

#include <vector>
#include <stdio.h>
#include <map>

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
	void AddEntity(GameObjectManager* gm);
	void AddChildren(GameObjectManager* gm);
	void AddChildrenWithChildrens(GameObjectManager* gm);
	void AddComponentInInspector(GameObjectManager* gm);

	void HierarchyWindowDisplay(GameObjectManager* gameObject);
	void ResourceWindowDisplay();
	void InfoGameObjectWindow(GameObjectManager* gameObject);
	void ResourceAssetWindow(ResourceElement* actualResource);
	void OsOpenInShell(const char* path); 

public:

	struct l_Logs
	{
	public:
		std::vector<std::string> Log;
		std::vector<LogTypeCase> Type;
	}l_Logs;

	std::vector<float> mFPSLOG;

	bool copy, AboutWindow, OpenPreferences, LogOutput, Vsync, Gl_Grid, Gl_DepthTest, Gl_CullFace, Gl_Ligthing, Gl_ColorMaterial, Gl_Texture2D,
		Gl_LineSmooth, Gl_PolygonSmooth, Gl_AlphaTest, DemoWindow, OpenAbout, DR_Normals, changeDebug, Hierarchy, InfoGWindow, ResourceWindow, isMovingParent,
		isMovingChild, numcap;

	int ThemeSelector;
	int item_current_idx;
	int Log_current_idx;
	int SelectPrimitive;

	GameObjectManager* actualMesh;
	GameObjectManager* moveEntityTo;
	GameObjectManager* hoveredItem;
	vector<GameObjectManager*> OG_ChildList;

	ResourceElement* actualResource;
	ResourceElement* hoveredResource;

	int OG_ChildListPos, Actual_ChildPos;

	float Volume;
	float WinBright;
	int treeFlags;

	string caps = "caps: ";
};

#endif