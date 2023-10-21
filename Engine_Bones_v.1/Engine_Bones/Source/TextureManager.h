#ifndef TEXTUREMANAGER_H_
#define TEXTUREMANAGER_H_

#include "Globals.h"
#include "External/MathGeoLib/include/MathGeoLib.h"
#include <vector>

#include "External\Glew\include\glew.h"
#include "External\SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include "AssimpManager.h"

#include "External/Devil/Include/ilut.h"
#include "External/Devil/Include/ilu.h"
#include "External/Devil/Include/il.h"

#pragma comment (lib, "External/Devil/libx86/DevIL.lib")
#pragma comment (lib, "External/Devil/libx86/ILU.lib")
#pragma comment (lib, "External/Devil/libx86/ILUT.lib")

#pragma comment (lib, "glu32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "External/Glew/libx86/glew32.lib")

using namespace std;

struct Texture
{
	int imageWidth;
	int imageHeight;
	int imageType;
	int imageFormat;

	uint TextureID;

	const char* path;
	bool ShowTextures = true;

	string Name = "";
};

class TexturesManager
{
public:
	TexturesManager()
	{
		CheckerTextureID = 0;
	}

	~TexturesManager()
	{
	}

	vector<Texture*> AllTextures;

	uint CheckerTextureID;

	byte checkerImage[CHECKERS_HEIGHT][CHECKERS_WIDTH][4];

	void SetCheckerTexture()
	{
		for (int i = 0; i < CHECKERS_HEIGHT; i++) {
			for (int j = 0; j < CHECKERS_WIDTH; j++) {
				int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
				checkerImage[i][j][0] = (GLubyte)c;
				checkerImage[i][j][1] = (GLubyte)c;
				checkerImage[i][j][2] = (GLubyte)c;
				checkerImage[i][j][3] = (GLubyte)255;
			}
		}

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glGenTextures(1, &CheckerTextureID);
		glBindTexture(GL_TEXTURE_2D, CheckerTextureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CHECKERS_WIDTH, CHECKERS_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkerImage);
	}

	Texture* TexLoader(const char* path)
	{
		Texture* T_Texture = new Texture();

		ILboolean success;
		ILuint Devil_Texure;
		uint TextID;

		ilGenImages(1, &Devil_Texure);
		ilBindImage(Devil_Texure);
		//ilLoadL(IL_TYPE_UNKNOWN, path, _id);
		success = ilLoadImage(path);
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

		T_Texture->path = path;
		T_Texture->TextureID = TextID;
		T_Texture->imageWidth = ilGetInteger(IL_IMAGE_WIDTH);
		T_Texture->imageHeight = ilGetInteger(IL_IMAGE_HEIGHT);
		T_Texture->imageType = ilGetInteger(IL_IMAGE_TYPE);
		T_Texture->imageFormat = ilGetInteger(IL_IMAGE_FORMAT);

		AllTextures.push_back(T_Texture);

		return T_Texture;
	}
};

#endif // TEXTUREMANAGER_H_
