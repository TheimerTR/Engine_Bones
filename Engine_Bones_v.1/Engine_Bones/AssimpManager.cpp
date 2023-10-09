#include "AssimpManager.h"

#include<stdlib.h>
#include<time.h>
#include<vector>
#include<string>

#include "Glew\include\glew.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include "Application.h"
#include "ModuleRenderer3D.h"
#include "FileSystemManager.h"
#include "Globals.h"

#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include "SDL/include/SDL_opengl.h"
#endif

#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "Glew/libx86/glew32.lib")
#pragma comment (lib, "glu32.lib") /* link Microsoft OpenGL lib   */

extern C_Mesh* CM = nullptr;

void AssimpManager::AssimpLoader(const char* path)
{
	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene != nullptr && scene->HasMeshes())
	{
		// Use scene->mNumMeshes to iterate on scene->mMeshes array
		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			Mesh* M_mesh = new Mesh();

			M_mesh->num_vertex = scene->mMeshes[i]->mNumVertices;
			M_mesh->vertex = new float[M_mesh->num_vertex * 3];
			memcpy(M_mesh->vertex, scene->mMeshes[i]->mVertices, sizeof(float) * M_mesh->num_vertex * 3);
			LOG(LogTypeCase::L_CASUAL, "New mesh with %d vertices", M_mesh->num_vertex);

			if (scene->mMeshes[i]->HasFaces())
			{
				M_mesh->num_index = scene->mMeshes[i]->mNumFaces * 3;
				M_mesh->index = new uint[M_mesh->num_index]; // assume each face is a triangle

				for (uint d = 0; d < scene->mMeshes[i]->mNumFaces; ++d)
				{
					if (scene->mMeshes[i]->mFaces[d].mNumIndices != 3)
					{
						LOG(LogTypeCase::L_WARNING, "WARNING, geometry face with != 3 indices!");
					}
					else
					{
						memcpy(&M_mesh->index[d * 3], scene->mMeshes[i]->mFaces[d].mIndices, 3 * sizeof(uint));
					}
				}

				LOG(LogTypeCase::L_CASUAL, "With %d indices", M_mesh->num_index);
			}

			if (scene->mMeshes[i]->HasNormals())
			{
				M_mesh->num_normals = scene->mMeshes[i]->mNumVertices;
				M_mesh->normals = new float[M_mesh->num_normals * 3];
				memcpy(M_mesh->normals, scene->mMeshes[i]->mNormals, sizeof(float) * M_mesh->num_normals * 3);
			}

			if (scene->mMeshes[i]->HasTextureCoords(0))
			{
				M_mesh->num_Tex = scene->mMeshes[i]->mNumVertices;
				M_mesh->textures = new float[scene->mMeshes[i]->mNumVertices * 2];

				for (int j = 0; j < M_mesh->num_Tex; j++)
				{
					int a = j * 2;
					M_mesh->textures[a] = scene->mMeshes[i]->mTextureCoords[0][j].x;
					M_mesh->textures[a + 1] = scene->mMeshes[i]->mTextureCoords[0][j].y;
				}
			}

			FileSystem::StringDivide(path, &M_mesh->Name, nullptr);

			CM = new C_Mesh();
			CM->SetMesh(M_mesh);
			CM->SetPath(path);

			M_mesh->VAO = 0;
			M_mesh->VBO = 0;
			M_mesh->EBO = 0;
			M_mesh->VN = 0;
			M_mesh->VT = 0;

			glGenVertexArrays(1, &M_mesh->VAO);
			glBindVertexArray(M_mesh->VAO);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

			glGenBuffers(1, (GLuint*)&(M_mesh->VBO));
			glBindBuffer(GL_ARRAY_BUFFER, M_mesh->VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * M_mesh->num_vertex * 3, M_mesh->vertex, GL_STATIC_DRAW);

			glGenBuffers(1, (GLuint*)&(M_mesh->EBO));
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, M_mesh->EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * M_mesh->num_index, M_mesh->index, GL_STATIC_DRAW);

			glGenBuffers(1, (GLuint*)&(M_mesh->VN));
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, M_mesh->VN);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * M_mesh->num_normals, M_mesh->normals, GL_STATIC_DRAW);

			glGenBuffers(1, (GLuint*)&(M_mesh->VT));
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, M_mesh->VT);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * M_mesh->num_Tex, M_mesh->textures, GL_STATIC_DRAW);

			Meshes.push_back(M_mesh);
		}

		aiReleaseImport(scene);
	}
	else
		LOG(LogTypeCase::L_ERROR, "Error loading scene % s", path);
}

void AssimpManager::ChangeDebugMode(bool type)
{
	if (type)
	{
		// Stream log messages to Debug window
		struct aiLogStream stream;
		stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
		aiAttachLogStream(&stream);
	}
	else
	{
		aiDetachAllLogStreams();
	}
}

void AssimpManager::CleanUp()
{
	for (int i = 0; i < Meshes.size(); i++)
	{
		if (Meshes[i]->VBO != 0)
		{
			glDeleteBuffers(1, &Meshes[i]->VBO);
		}
		if (Meshes[i]->VAO != 0)
		{
			glDeleteBuffers(1, &Meshes[i]->VAO);
		}
		if (Meshes[i]->EBO != 0)
		{
			glDeleteBuffers(1, &Meshes[i]->EBO);
		}
		if (Meshes[i]->VN != 0)
		{
			glDeleteBuffers(1, &Meshes[i]->VN);
		}
		if (Meshes[i]->VT != 0)
		{
			glDeleteBuffers(1, &Meshes[i]->VT);
		}
	}
}

template <class T>
void AssimpManager::ClearAssimpVec(std::vector<T>& vector)
{
	if (vector.size() > 0)
	{
		for (int i = 0; i < vector.size() - 1; i++)
		{
			vector.pop_back();
		}
	}

	vector.clear();
}

void  AssimpManager::Clear_Mesh(Mesh* mesh)
{
	if (mesh->VBO != 0)
	{
		glDeleteBuffers(1, &mesh->VBO);
	}
	if (mesh->VAO != 0)
	{
		glDeleteBuffers(1, &mesh->VAO);
	}
	if (mesh->EBO != 0)
	{
		glDeleteBuffers(1, &mesh->EBO);
	}
	if (mesh->VN != 0)
	{
		glDeleteBuffers(1, &mesh->VN);
	}
	if (mesh->VT != 0)
	{
		glDeleteBuffers(1, &mesh->VT);
	}

	mesh->Name = "";

	mesh->num_index = 0;
	mesh->index = nullptr;
	mesh->num_vertex = 0;
	mesh->vertex = nullptr;

	mesh->num_normals = 0;
	mesh->normals = nullptr;

	mesh->num_Tex = 0;
	mesh->textures = nullptr;

	Meshes.pop_back();
}

C_Mesh::C_Mesh()
{
	mesh = nullptr;
	path = "NULL";
	CM = this;
};

void C_Mesh::SetMesh(Mesh* mesh)
{
	this->mesh = mesh;
}

void C_Mesh::SetPath(std::string path)
{
	this->path = path;
}

//Mesh* AssimpManager::LoadMesh(aiMesh* MeshToLoad)
//{
//	LOG(LogTypeCase::L_CASUAL, "%s", MeshToLoad->mName.C_Str());
//
//	Mesh* M_mesh = new Mesh();
//
//	M_mesh->num_vertex = MeshToLoad->mNumVertices;
//	M_mesh->vertex = new float[M_mesh->num_vertex * 3];
//	memcpy(M_mesh->vertex, MeshToLoad->mVertices, sizeof(float) * M_mesh->num_vertex * 3);
//	LOG(LogTypeCase::L_CASUAL, "New mesh with %d vertices: ", M_mesh->num_vertex);
//
//	if (MeshToLoad->HasFaces())
//	{
//		M_mesh->num_index = MeshToLoad->mNumVertices * 3;
//		M_mesh->index = new uint[M_mesh->num_index]; // assume each face is a triangle
//		for (uint i = 0; i < MeshToLoad->mNumFaces; ++i)
//		{
//			if (MeshToLoad->mFaces[i].mNumIndices != 3)
//			{
//				LOG(LogTypeCase::L_WARNING, "WARNING, geometry face with != 3 indices!");
//			}
//			else
//			{
//				memcpy(&M_mesh->index[i * 3], MeshToLoad->mFaces[i].mIndices, 3 * sizeof(uint));
//			}
//		}
//
//		LOG(LogTypeCase::L_CASUAL, "With %d indices: ", M_mesh->index);
//	}
//
//	app->renderer3D->VBO = (uint)&M_mesh->vertex;
//	app->renderer3D->EBO = (uint)&M_mesh->index;
//
//	glGenBuffers(1, &app->renderer3D->VBO);
//	glBindBuffer(GL_ARRAY_BUFFER, app->renderer3D->VBO);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(M_mesh->vertex), M_mesh->vertex, GL_STATIC_DRAW);
//
//	glGenBuffers(1, &app->renderer3D->EBO);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, app->renderer3D->EBO);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(M_mesh->index), M_mesh->index, GL_STATIC_DRAW);
//
//	return M_mesh;
//}