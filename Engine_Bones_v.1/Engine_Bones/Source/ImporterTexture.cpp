#include "ImporterTexture.h"

void Importer::ImporterTexture::InitDevil()
{
    ilInit();
    iluInit();
    ilutInit();
    ilutRenderer(ILUT_OPENGL);
}

void Importer::ImporterTexture::ImportTexture(ResourceTexture* R_Texture, const char* buffer, uint size)
{
	if (ilLoadL(IL_DDS, (const void*)buffer, size))
	{
		R_Texture->id = ilutGLBindTexImage();
	}
}

uint64 Importer::ImporterTexture::Save(char** buffer)
{
	ilEnable(IL_FILE_OVERWRITE);

    ILuint size;
    ILubyte* data;
    ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);// To pick a specific DXT compression use
    size = ilSaveL(IL_DDS, nullptr, 0); // Get the size of the data buffer

    if (size > 0) 
	{
        data = new ILubyte[size]; // allocate data buffer
        if (ilSaveL(IL_DDS, data, size) > 0) // Save to buffer with the ilSaveIL function
            *buffer = (char*)data;
    }

    return size;
}

void Importer::ImporterTexture::Load(Texture* texture, const char* buffer)
{
	ILboolean success;
	ILuint Devil_Texure;
	uint TextID;

	ilGenImages(1, &Devil_Texure);
	ilBindImage(Devil_Texure);
	//ilLoadL(IL_TYPE_UNKNOWN, path, _id);
	success = ilLoadImage(buffer);
	TextID = ilutGLBindTexImage();

	uint Text_Size = ilSaveL(IL_DDS, NULL, 0);
	char* Text_Data = new char[Text_Size];

	ilDeleteImages(1, &Devil_Texure);

	if (success)
	{
		//success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

		if (!success || TextID == NULL)
		{
			LOG(LogTypeCase::L_ERROR, "DEVIL can't load Image");
		}
		else
		{
			LOG(LogTypeCase::L_CASUAL, "DEVIL load Image!");
		}
	}

	texture->path = buffer;
	texture->TextureID = TextID;
	texture->imageWidth = ilGetInteger(IL_IMAGE_WIDTH);
	texture->imageHeight = ilGetInteger(IL_IMAGE_HEIGHT);
	texture->imageType = ilGetInteger(IL_IMAGE_TYPE);
	texture->imageFormat = ilGetInteger(IL_IMAGE_FORMAT);
}
