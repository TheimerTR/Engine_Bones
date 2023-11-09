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
		void ImportTexture(Texture* texture, const char* fileBuffer = nullptr, uint size = 0);
		uint64 Save(const Texture* texture = nullptr, char** fileBuffer = nullptr);
		void Load(Texture* texture = nullptr, const char* fileBuffer = nullptr, uint size = 0);
	}
};


#endif //IMPORTER_TEXTURE_H_
