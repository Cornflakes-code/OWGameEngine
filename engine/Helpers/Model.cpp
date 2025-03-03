#include "Model.h"

#include <glm/ext/matrix_transform.hpp>

#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "TextureFactory.h"

// checks all material textures of a given type and loads the 
// textures if they're not loaded yet. 
static std::vector<Texture> loadMaterialTextures(aiMaterial* mat,
	aiTextureType type, std::string  typeName, std::string texType)
{
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		std::string f = str.C_Str();
		Texture texture = TextureFactory().getTexture(f);
		texture.samplerName(std::string(typeName) + std::to_string(i + 1));
		textures.push_back(texture);
	}
	return textures;
}

std::string determineTextureType(const aiScene* scene, aiMaterial* mat)
{
	aiString textypeStr;
	mat->GetTexture(aiTextureType_DIFFUSE, 0, &textypeStr);
	std::string textypeteststr = textypeStr.C_Str();
	if (textypeteststr == "*0" || textypeteststr == "*1" ||
		textypeteststr == "*2" || textypeteststr == "*3" ||
		textypeteststr == "*4" || textypeteststr == "*5")
	{
		if (scene->mTextures[0]->mHeight == 0)
		{
			return "embedded compressed texture";
		}
		else
		{
			return "embedded non-compressed texture";
		}
	}

	if (textypeteststr.find('.') != std::string::npos)
	{
		return "textures are on disk";
	}

	return ".";
}

void OWModel::create(aiMesh* mesh, const aiScene* scene)
{
	// Copied from ModelLoader::processMesh in \assimp\samples\
	// SimpleTexturedDirectx11\SimpleTexturedDirectx11\ModelLoader.cpp

	// data to fill
	std::string textype;
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];

		if (textype.empty())
		{
			textype = determineTextureType(scene, mat);
		}
	}

	for (UINT i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;

		vertex.position.x = mesh->mVertices[i].x;
		vertex.position.y = mesh->mVertices[i].y;
		vertex.position.z = mesh->mVertices[i].z;

		vertex.normal.x = mesh->mNormals[i].x;
		vertex.normal.y = mesh->mNormals[i].y;
		vertex.normal.z = mesh->mNormals[i].z;
		if (mesh->mTextureCoords[0])
		{
			vertex.textureCoord.x = mesh->mTextureCoords[0][i].x;
			vertex.textureCoord.y = mesh->mTextureCoords[0][i].y;
		}
		else
		{
			vertex.textureCoord = glm::vec2(0.0f, 0.0f);
		}

		vertices.push_back(vertex);
	}

	for (UINT i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for (UINT j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		std::vector<Texture> diffuseMaps =
			loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", textype);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	}
}

void OWModel::calcNormals()
{
	for (int i = 0; i < indices.size(); i += 3)
	{
		int i0 = indices[i];
		int i1 = indices[i + 1];
		int i2 = indices[i + 2];

		glm::vec3 v1 = vertices[i1].position - vertices[i0].position;
		glm::vec3 v2 = vertices[i2].position - vertices[i0].position;
		glm::vec3 v2Normal = glm::normalize(v2);
		glm::vec3  normal = glm::cross(v1, v2Normal);

		vertices[i0].normal += normal;
		vertices[i1].normal += normal;
		vertices[i2].normal += normal;
	}

	for (int i = 0; i < vertices.size(); i++)
		vertices[i].normal = glm::normalize(vertices[i].normal);
}
AABB OWModel::bounds() const
{
	static constexpr float _max = std::numeric_limits<float>::max();
	glm::vec3 minPoint(_max, _max, _max);
	glm::vec3 maxPoint(-_max, -_max, -_max);

	for (const Vertex& v : vertices)
	{
		if (v.position.x < minPoint.x)
			minPoint.x = v.position.x;
		if (v.position.x > maxPoint.x)
			maxPoint.x = v.position.x;

		if (v.position.y < minPoint.y)
			minPoint.y = v.position.y;
		if (v.position.y > maxPoint.y)
			maxPoint.y = v.position.y;

		if (v.position.z < minPoint.z)
			minPoint.z = v.position.z;
		if (v.position.z > maxPoint.z)
			maxPoint.z = v.position.z;
	}
	return AABB(minPoint, maxPoint);
}
