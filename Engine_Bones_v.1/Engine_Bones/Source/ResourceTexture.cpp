#include "ResourceTexture.h"

ResourceTexture::ResourceTexture() : ResourceElement(ResourceTypes::R_TEXTURE)
{
	id = 0;
	path = "";
	texture = new Texture();
}

ResourceTexture::ResourceTexture(const char* Rname, const char* Rassetspath, const char* Rlibrarypath, uint32 Ruuid) : ResourceElement(ResourceTypes::R_TEXTURE)
{
	this->name = Rname;
	this->LibraryPath = Rlibrarypath;
	this->AssetsPath = Rassetspath;
	this->UUID = Ruuid;

	id = 0;
	path = "";
	texture = new Texture();
}