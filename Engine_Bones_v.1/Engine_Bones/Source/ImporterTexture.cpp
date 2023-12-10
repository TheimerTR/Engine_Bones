#include "ImporterTexture.h"
#include "FileSystemManager.h"

void Importer::ImporterTexture::InitDevil()
{
    ilInit();
    iluInit();
    ilutInit();
    ilutRenderer(ILUT_OPENGL);
}

uint Importer::ImporterTexture::ImportTexture(aiMaterial* mat, ResourceTexture* R_Texture, char** buffer, bool Save)
{
	uint size = 0;

	//Colors Material
	aiColor4D colors;
	aiString pathToTexture;
	std::string nameTexture;
	std::string extTexture;

	//if (ilLoadL(IL_DDS, (const void*)buffer, size))
	//{
	//	R_Texture->id = ilutGLBindTexImage();
	//}

	if (mat->Get(AI_MATKEY_COLOR_DIFFUSE, colors) == AI_SUCCESS)
	{
		R_Texture->SetColor((Color(colors.r, colors.g, colors.b, colors.a)));
	}

	if (mat->GetTexture(aiTextureType_DIFFUSE, 0, &pathToTexture) == AI_SUCCESS)
	{
		FileSystem::StringDivide(pathToTexture.C_Str(), &nameTexture, &extTexture);
		R_Texture->texture->Name = nameTexture;
		R_Texture->name = nameTexture;

		nameTexture = TEXTURES_FOLDER + nameTexture; 
		R_Texture->texture->path = nameTexture.c_str();

		Importer::ImporterTexture::Load(R_Texture->texture, R_Texture->texture->path);

		if (Save == true)
		{
			size = Importer::ImporterTexture::Save(R_Texture, buffer);
		}
	}
	else
	{
		nameTexture = "Material";
		R_Texture->texture->Name = nameTexture.c_str();
		R_Texture->name = nameTexture;
	}

	return size;
}

uint64 Importer::ImporterTexture::Save(ResourceTexture* R_text, char** buffer)
{
	ilEnable(IL_FILE_OVERWRITE);

    ILuint size = 0;
    ILubyte* data;

	ilBindImage(R_text->texture->TextureID);

	if (ilLoadImage(R_text->texture->path))
	{
		ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);// To pick a specific DXT compression use
		size = ilSaveL(IL_DDS, nullptr, 0); // Get the size of the data buffer

		if (size > 0)
		{
			data = new ILubyte[size]; // allocate data buffer
				if (ilSaveL(IL_DDS, data, size) > 0) // Save to buffer with the ilSaveIL function
					*buffer = (char*)data;
		}
	}

	ilBindImage(0);

    return size;
}

void Importer::ImporterTexture::Load(Texture* texture, const char* buffer, uint size)
{
	ILboolean success;
	ILuint Devil_Texure;
	uint TextID;

	ilGenImages(1, &Devil_Texure);
	ilBindImage(Devil_Texure);
	success = ilLoadImage(buffer);
	TextID = ilutGLBindTexImage();

	ilDeleteImages(1, &Devil_Texure);

	if (success)
	{
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