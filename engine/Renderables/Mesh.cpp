#include "Mesh.h"

#include <glm/ext/matrix_transform.hpp>

#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../Helpers/MoveController.h"

#include "MeshRenderer.h"
#include "../Helpers/TextureFactory.h"


static std::vector<Texture> loadMaterialTextures(aiMaterial *mat,
	aiTextureType type, std::string typeName, std::string texType);

std::string determineTextureType(const aiScene * scene, aiMaterial * mat) 
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

void Mesh::create(aiMesh *mesh, const aiScene *scene)
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
		MeshData::Vertex vertex;

		vertex.position.x = mesh->mVertices[i].x;
		vertex.position.y = mesh->mVertices[i].y;
		vertex.position.z = mesh->mVertices[i].z;

#ifdef MESH_HAS_NORMAL
		vertex.normal.x = mesh->mNormals[i].x;
		vertex.normal.y = mesh->mNormals[i].y;
		vertex.normal.z = mesh->mNormals[i].z;
#endif
		if (mesh->mTextureCoords[0])
		{
			vertex.textureCoord.x = mesh->mTextureCoords[0][i].x;
			vertex.textureCoord.y = mesh->mTextureCoords[0][i].y;
		}
		else
		{
			vertex.textureCoord = glm::vec2(0.0f, 0.0f);
		}

		mData.vertices.push_back(vertex);
	}

	for (UINT i = 0; i < mesh->mNumFaces; i++) 
	{
		aiFace face = mesh->mFaces[i];

		for (UINT j = 0; j < face.mNumIndices; j++)
			mData.indices.push_back(face.mIndices[j]);
	}

	if (mesh->mMaterialIndex >= 0) 
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		std::vector<Texture> diffuseMaps = 
			loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", textype);
		mData.textures.insert(mData.textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	}

	mData.indicesMode = GL_TRIANGLES;
}

int getTextureIndex(aiString* str) 
{
	std::string tistr;
	tistr = str->C_Str();
	tistr = tistr.substr(1);
	return stoi(tistr);
}

// checks all material textures of a given type and loads the 
// textures if they're not loaded yet. 
static std::vector<Texture> loadMaterialTextures(aiMaterial *mat,
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

void Mesh::initx(const std::vector<MeshData::Vertex>& vertices,
	const std::vector<unsigned int>& indices, bool _calcNormals)
{
	if (false)//mData.use_count() != 0)
	{
		throw NMSLogicException("Trying to init a previously initialised model.");
	}
	MeshData* md = new MeshData();
	md->vertices = vertices;
	md->indices = indices;
	if (_calcNormals)
	{
		md->calcNormals();
	}
}

void Mesh::renderer(MeshRenderer* source)
{
	mRenderer = source;
	mRenderer->prepare(this);
}

AABB Mesh::calcBounds() const
{
	return AABB();
}

void Mesh::render(const glm::mat4& proj,
	const glm::mat4& view,
	const glm::mat4& model,
	const MoveController* mover,
	OWUtils::RenderCallbackType renderCb,
	OWUtils::ResizeCallbackType resizeCb) const
{
	if (mover)
	{
		mRenderer->render(this, proj, view, mover->translate(model), renderCb, resizeCb);
	}
	else
	{
		glm::mat4 initialPositionModel = glm::translate(model, initialPosition());
		mRenderer->render(this, proj, view, initialPositionModel, renderCb, resizeCb);
	}
}
