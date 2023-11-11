#ifndef PHYSFSMANAGER_H_
#define PHYSFSMANAGER_H_

#include "Module.h"
#include "Globals.h"

#include <string>
#include <vector>

struct SDL_RWops;
int close_sdl_rwops(SDL_RWops* rw);

struct aiFileIO;

//struct BASS_FILEPROCS;
class Config;

struct PathNode
{
	PathNode() : path("") {};
	//Path in reference to the first PathNode
	std::string path;
	//Folder / file itself
	std::string localPath;

	std::vector<PathNode> children;

	bool isLeaf = true;
	bool isFile = true;

	bool IsLastFolder() const
	{
		for (uint i = 0; i < children.size(); i++)
			if (children[i].isFile == false)
				return false;
		return true;
	}

	bool operator ==(const PathNode node) const
	{
		return path == node.path;
	}
};

class PhysFSManager : public Module
{
public:

	PhysFSManager(bool start_enabled = true);// const char* game_path = nullptr);

	// Destructor
	~PhysFSManager();

	// Called before render is available
	//bool Init(Config& config) override;

	// Called before quitting
	bool CleanUp() override;

	void CreateLibraryDirectories();

	//Previous functions (Before taking Thor File System)


	// Utility functions
	bool AddPath(const char* path_or_zip);
	bool Exists(const char* file) const;
	bool CreateDir(const char* dir);
	bool IsDirectory(const char* file) const;
	const char* GetWriteDir() const;
	void DiscoverFiles(const char* directory, std::vector<std::string>& file_list, std::vector<std::string>& dir_list) const;
	void GetAllFilesWithExtension(const char* directory, const char* extension, std::vector<std::string>& file_list) const;
	PathNode GetAllFiles(const char* directory, std::vector<std::string>* filter_ext = nullptr, std::vector<std::string>* ignore_ext = nullptr) const;
	void GetRealDir(const char* path, std::string& output) const;
	std::string GetPathRelativeToAssets(const char* originalPath) const;

	bool HasExtension(const char* path) const;
	bool HasExtension(const char* path, std::string extension) const;
	bool HasExtension(const char* path, std::vector<std::string> extensions) const;

	std::string NormalizePath(const char* path) const;
	void SplitFilePath(const char* full_path, std::string* path, std::string* file = nullptr, std::string* extension = nullptr) const;

	// Open for Read/Write
	unsigned int Load(const char* path, const char* file, char** buffer) const;
	unsigned int Load(const char* file, char** buffer) const;

	bool DuplicateFile(const char* file, const char* dstFolder, std::string& relativePath);
	bool DuplicateFile(const char* srcFile, const char* dstFile);

	unsigned int Save(const char* file, const void* buffer, unsigned int size, bool append = false) const;
	bool Remove(const char* file);

	uint64 GetLastModTime(const char* filename);
	std::string GetUniqueName(const char* path, const char* name) const;
};

#endif // PHYSFSMANAGER_H_
