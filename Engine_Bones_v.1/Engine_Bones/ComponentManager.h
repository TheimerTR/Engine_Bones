#ifndef COMPONENTMANAGER_H_
#define COMPONENTMANAGER_H_

#include "Globals.h"
#include "External/MathGeoLib/include/MathGeoLib.h"
#include <vector>

#include "AssimpManager.h"

#pragma comment (lib, "External/Devil/libx86/DevIL.lib")
#pragma comment (lib, "External/Devil/libx86/ILU.lib")
#pragma comment (lib, "External/Devil/libx86/ILUT.lib")

#pragma comment (lib, "glu32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "External/Glew/libx86/glew32.lib")

using namespace std;

struct Texture
{
	uint TextureID;
	uint Text_Size;

	const char* path;
	char* Text_Data;
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
		T_Texture->Text_Size = Text_Size;
		T_Texture->Text_Data = Text_Data;

		AllTextures.push_back(T_Texture);

		return T_Texture;
	}
};

#endif // COMPONENTMANAGER_H_