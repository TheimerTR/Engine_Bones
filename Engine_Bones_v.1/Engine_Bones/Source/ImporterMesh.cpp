#include "ImporterMesh.h"
#include "FileSystemManager.h"

void Importer::ImporterMesh::ImportMesh(ResourceMesh* M_mesh, const aiMesh* aiMesh)
{
	M_mesh->mesh->num_vertex = aiMesh->mNumVertices;
	M_mesh->mesh->vertex = new float[M_mesh->mesh->num_vertex * 3];
	memcpy(M_mesh->mesh->vertex, aiMesh->mVertices, sizeof(float) * M_mesh->mesh->num_vertex * 3);
	LOG(LogTypeCase::L_CASUAL, "New mesh with %d vertices", M_mesh->mesh->num_vertex);

	if (aiMesh->HasFaces())
	{
		M_mesh->mesh->num_index = aiMesh->mNumFaces * 3;
		M_mesh->mesh->index = new uint[M_mesh->mesh->num_index]; // assume each face is a triangle

		for (uint d = 0; d < aiMesh->mNumFaces; ++d)
		{
			if (aiMesh->mFaces[d].mNumIndices != 3)
			{
				LOG(LogTypeCase::L_WARNING, "WARNING, geometry face with != 3 indices!");
			}
			else
			{
				memcpy(&M_mesh->mesh->index[d * 3], aiMesh->mFaces[d].mIndices, 3 * sizeof(uint));
				//memcpy(&M_mesh->normals_Faces[d * 3], scene->mMeshes[i]->mFaces[d].mIndices, 3 * sizeof(uint));
			}
		}

		LOG(LogTypeCase::L_CASUAL, "With %d indices", M_mesh->mesh->num_index);
	}

	if (aiMesh->HasNormals())
	{
		M_mesh->mesh->num_normals = aiMesh->mNumVertices;
		M_mesh->mesh->normals = new float[M_mesh->mesh->num_normals * 3];
		memcpy(M_mesh->mesh->normals, aiMesh->mNormals, sizeof(float) * M_mesh->mesh->num_normals * 3);

		M_mesh->mesh->center_normal = new float[aiMesh->mNumFaces * 3];
		M_mesh->mesh->center = new float[aiMesh->mNumFaces * 3];
		M_mesh->mesh->faces = aiMesh->mNumFaces;

		for (uint i = 0; i < M_mesh->mesh->num_index; i += 3)
		{
			/*float3 x0(M_mesh->mesh->vertex[(M_mesh->mesh->index[i] * 3)], M_mesh->mesh->vertex[(M_mesh->mesh->index[i] * 3) + 1], M_mesh->mesh->vertex[(M_mesh->mesh->index[i] * 3) + 2]);
			float3 x1(M_mesh->mesh->vertex[(M_mesh->mesh->index[i + 1] * 3)], M_mesh->mesh->vertex[(M_mesh->mesh->index[i + 1] * 3) + 1], M_mesh->mesh->vertex[(M_mesh->mesh->index[i + 1] * 3) + 2]);
			float3 x2(M_mesh->mesh->vertex[(M_mesh->mesh->index[i + 2] * 3)], M_mesh->mesh->vertex[(M_mesh->mesh->index[i + 2] * 3) + 1], M_mesh->mesh->vertex[(M_mesh->mesh->index[i + 2] * 3) + 2]);

			float3 v0 = x1 - x0;
			float3 v1 = x2 - x1;
			float3 normalized = v0.Cross(v1);

			M_mesh->mesh->center[i] = (x0.x + x1.x + x2.x) / 3;
			M_mesh->mesh->center[i + 1] = (x0.y + x1.y + x2.y) / 3;
			M_mesh->mesh->center[i + 2] = (x0.z + x1.z + x2.z) / 3;

			M_mesh->mesh->center_normal[i] = normalized.Normalized().x;
			M_mesh->mesh->center_normal[i + 1] = normalized.Normalized().y;
			M_mesh->mesh->center_normal[i + 2] = normalized.Normalized().z;*/
		}
	}

	uint UV_Index = 0;
	if (aiMesh->HasTextureCoords(UV_Index))
	{
		M_mesh->mesh->num_Tex = aiMesh->mNumVertices;
		M_mesh->mesh->textures = new math::float2[aiMesh->mNumVertices];

		for (int j = 0; j < M_mesh->mesh->num_Tex; j++)
		{
			M_mesh->mesh->textures[j].x = aiMesh->mTextureCoords[UV_Index][j].x;
			M_mesh->mesh->textures[j].y = aiMesh->mTextureCoords[UV_Index][j].y;
		}
	}

	AssimpManager::SetBuffers(M_mesh->mesh);
}

uint64 Importer::ImporterMesh::Save(const ResourceMesh* M_mesh, char** fileBuffer)
{
	uint ranges[4] = { M_mesh->mesh->num_vertex, M_mesh->mesh->num_index, M_mesh->mesh->num_Tex, M_mesh->mesh->num_normals };
	uint size = sizeof(ranges) + sizeof(uint) * M_mesh->mesh->num_index + sizeof(uint) * M_mesh->mesh->num_vertex * 3 + sizeof(uint) * M_mesh->mesh->num_normals * 3 + sizeof(uint) * M_mesh->mesh->num_Tex * 2;

	*fileBuffer = new char[size]; // Allocate
	char* cursor = *fileBuffer;

	uint bytes = sizeof(ranges); // First store ranges
	memcpy(cursor, ranges, bytes);
	cursor += bytes;

	// Store indices
	bytes = sizeof(uint) * M_mesh->mesh->num_index;
	memcpy(cursor, M_mesh->mesh->index, bytes);
	cursor += bytes;

	// Store vertex
	bytes = sizeof(uint) * M_mesh->mesh->num_vertex * 3;
	memcpy(cursor, M_mesh->mesh->vertex, bytes);
	cursor += bytes;

	// Store normals
	if(M_mesh->mesh->num_normals > 0)
	{
		bytes = sizeof(uint) * M_mesh->mesh->num_normals * 3;
		memcpy(cursor, M_mesh->mesh->normals, bytes);
		cursor += bytes;
	}
	
	// Store textures
	if(M_mesh->mesh->num_Tex > 0)
	{
		bytes = sizeof(uint) * M_mesh->mesh->num_Tex * 2;
		memcpy(cursor, M_mesh->mesh->textures, bytes);
		cursor += bytes;
	}

	return size;
}

void Importer::ImporterMesh::Load(ResourceMesh* M_mesh, const char* fileBuffer)
{
	const char* cursor = fileBuffer;

	M_mesh->mesh->Name = M_mesh->name;

	// amount of indices / vertices / normals / texture_coords
	uint ranges[4];
	uint bytes = sizeof(ranges);
	memcpy(ranges, cursor, bytes);
	cursor += bytes;

	M_mesh->mesh->num_index = ranges[0];
	M_mesh->mesh->num_vertex = ranges[1];
	M_mesh->mesh->num_normals = ranges[2];
	M_mesh->mesh->num_Tex = ranges[3];

	// Load indices
	bytes = sizeof(uint) * M_mesh->mesh->num_index;
	M_mesh->mesh->index = new uint[M_mesh->mesh->num_index];
	memcpy(M_mesh->mesh->index, cursor, bytes);
	cursor += bytes;

	// Load vertex
	bytes = sizeof(uint) * M_mesh->mesh->num_vertex * 3;
	M_mesh->mesh->vertex = new float[M_mesh->mesh->num_vertex * 3];
	memcpy(M_mesh->mesh->vertex, cursor, bytes);
	cursor += bytes;

	// Load normals
	if (M_mesh->mesh->num_normals > 0)
	{
		bytes = sizeof(uint) * M_mesh->mesh->num_normals * 3;
		M_mesh->mesh->normals = new float[M_mesh->mesh->num_normals * 3];
		memcpy(M_mesh->mesh->normals, cursor, bytes);
		cursor += bytes;
	}

	// Load textures
	if (M_mesh->mesh->num_Tex > 0)
	{
		bytes = sizeof(uint) * M_mesh->mesh->num_Tex * 2;
		M_mesh->mesh->textures = new math::float2[M_mesh->mesh->num_vertex * 2];
		memcpy(M_mesh->mesh->textures, cursor, bytes);
		cursor += bytes;
	}

	//AssimpManager::SetBuffers(M_mesh->mesh);
}
