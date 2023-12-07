#ifndef IMPORTER_TEXTURE_H_
#define IMPORTER_TEXTURE_H_

#include "Globals.h"
#include "Module.h"
#include "Application.h"
#include "AssimpManager.h"
#include "GameObjectManager.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "TextureManager.h"
#include "ResourceTexture.h"

#include <vector>
#include <stdio.h>

using namespace std;

struct aiMaterial;
class Mesh;
struct Texture;

namespace Importer
{
	namespace ImporterTexture
	{
		void InitDevil();
		uint ImportTexture(aiMaterial* mat, ResourceTexture* R_Texture = nullptr, char** buffer = nullptr);
		void ImportTextureWithMeta(aiMaterial* mat, ResourceTexture* R_Texture = nullptr, const char* buffer = nullptr, uint size = 0);
		uint64 Save(ResourceTexture* R_text, char** buffer = nullptr);
		void Load(Texture* texture = nullptr, const char* buffer = nullptr, uint size = 0);
	}
};


#endif //IMPORTER_TEXTURE_H_
