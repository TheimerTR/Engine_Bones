#ifndef MODULE_EDITOR
#define MODULE_EDITOR

#include "Module.h"
#include "Application.h"
#include "AssimpManager.h"
#include "External/ImGui/imgui.h"
#include "External/ImGuizmo/ImGuizmo.h"
#include "GameObjectManager.h"
#include "ResourceElement.h"
#include "External/ImGui/imgui.h"

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
	void ResourceAssetWindow();
	void AssetsWindow(const char* directory, std::vector<std::string>& file_list, std::vector<std::string>& dir_list, bool leaf);
	void AssetsWindowFolders(const char* directory, std::vector<std::string>& file_list, std::vector<std::string>& dir_list, bool& leaf);
	void ImportToScene(string path);
	void OsOpenInShell(const char* path); 

	void CreateDockingSpace(); 

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
		isMovingChild, numcap, RGB_Mode, RGB, AssetWindow;

	int ThemeSelector;
	int item_current_idx;
	int Log_current_idx;
	int SelectPrimitive;

	string actualDir;
	string actualFile;
	string selectedFile;
	string selectedDir;

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

	string name; 
	string nameGame; 

	string caps = "caps: ";

	float viewportSizex; 
	float viewportSizey; 

	bool startScene = false; 

	ImVec2 scenePos;
	ImVec2 sceneSize;
	ImGuizmo::OPERATION CurrentOperation;

	bool aabbDraw = false;
	bool obbDraw = false;

	//ImVec2 sceneSize = ImVec2(0, 0);
	//ImVec2 scenePos  = ImVec2(0, 0);
};

#endif