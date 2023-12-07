#pragma once

#ifndef RESOURCETEXTURE_H_
#define RESOURCETEXTURE_H_

#include "Globals.h"
#include "ResourceElement.h"
#include "Application.h"
#include "TextureManager.h"

using namespace std;

class ResourceTexture : public ResourceElement
{
public:
	ResourceTexture();
	ResourceTexture(const char* Rname, const char* Rassetspath, const char* Rlibrarypath, uint32 Ruuid);

	void SetColor(Color color);

	uint id;
	string path;

	Texture* texture;

	//Colors Material
	Color colors;

	uint UUID;
	uint size;
};

#endif // RESOURCETEXTURE_H_