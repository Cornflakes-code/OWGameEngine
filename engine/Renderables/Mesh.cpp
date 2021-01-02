#include "Mesh.h"

#include <glm/ext/matrix_transform.hpp>

#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../Helpers/MoveController.h"

#include "MeshRenderer.h"
#include "../Helpers/TextureFactory.h"


static std::vector<Texture> loadMaterialTextures(aiMaterial *mat,
	aiTextureType type, const char* typeName);

void Mesh::create(aiMesh *mesh, const aiScene *scene)
{
	// data to fill
	bool hasNormals = mesh->HasNormals();
	bool hasTangents = mesh->HasTangentsAndBitangents();
	// walk through each of the mesh's vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		MeshData::Vertex vertex;
		vertex.position.x = mesh->mVertices[i].x;
		vertex.position.y = mesh->mVertices[i].y;
		vertex.position.z = mesh->mVertices[i].z;
		if (hasNormals)
		{
			vertex.normal.x = mesh->mNormals[i].x;
			vertex.normal.y = mesh->mNormals[i].y;
			vertex.normal.z = mesh->mNormals[i].z;
		}

		if (mesh->HasTextureCoords(i))
		{
			// a vertex can contain up to 8 different texture coordinates. We thus 
			// make the assumption that we won't use models where a vertex can have 
			// multiple texture coordinates so we always take the first set (0).
			vertex.textureCoord.x = mesh->mTextureCoords[0][i].x;
			vertex.textureCoord.y = mesh->mTextureCoords[0][i].y;
		}
		else
		{
			vertex.textureCoord = glm::vec2(0.0f);
		}
		if (hasTangents)
		{
			vertex.tangent.x = mesh->mTangents[i].x;
			vertex.tangent.y = mesh->mTangents[i].y;
			vertex.tangent.z = mesh->mTangents[i].z;

			vertex.biTangent.x = mesh->mBitangents[i].x;
			vertex.biTangent.y = mesh->mBitangents[i].y;
			vertex.biTangent.z = mesh->mBitangents[i].z;
		}
		else
		{
			vertex.tangent = glm::vec3(0.0f);
			vertex.biTangent = glm::vec3(0.0f);
		}
		mData.vertices.push_back(vertex);
	}
	// now walk through each of the mesh's faces (a face is a mesh its triangle) 
	// and retrieve the corresponding vertex indices.
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		// retrieve all indices of the face and store them in the indices vector
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			mData.indices.push_back(face.mIndices[j]);
	}
	mData.indicesMode = GL_TRIANGLES;
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	// we assume a convention for sampler names in the shaders. Each diffuse texture should be named
	// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
	// Same applies to other texture as the following list summarizes:
	// diffuse: texture_diffuseN
	// specular: texture_specularN
	// normal: texture_normalN

	// 1. diffuse maps
	std::vector<Texture> diffuseMaps = loadMaterialTextures(
		material, aiTextureType_DIFFUSE, "texture_diffuse");
	mData.textures.insert(mData.textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	// 2. specular maps
	std::vector<Texture> specularMaps = loadMaterialTextures(
		material, aiTextureType_SPECULAR, "texture_specular");
	mData.textures.insert(mData.textures.end(), specularMaps.begin(), specularMaps.end());
	// 3. normal maps
	std::vector<Texture> normalMaps = loadMaterialTextures(
		material, aiTextureType_HEIGHT, "texture_normal");
	mData.textures.insert(mData.textures.end(), normalMaps.begin(), normalMaps.end());
	// 4. height maps
	std::vector<Texture> heightMaps = loadMaterialTextures(
		material, aiTextureType_AMBIENT, "texture_height");

	mData.textures.insert(mData.textures.end(), heightMaps.begin(), heightMaps.end());
}

// checks all material textures of a given type and loads the 
// textures if they're not loaded yet. 
static std::vector<Texture> loadMaterialTextures(aiMaterial *mat,
	aiTextureType type, const char* typeName)
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
void Mesh::makeDeep()
{
	//if (mData.use_count() == 0)
	//{
	//	 Uninitialised object?
	//}
	//else if (mData.use_count() == 1)
	//{
	//	 The original object or we already have a deep copy
	//}
	//if (mData.use_count() > 1)
	//{
	//	 We currently have a shallow_copy
	//	MeshData* originalData = mData.get();
	//	MeshData* md = new MeshData(*originalData);
	//	mData = std::shared_ptr<MeshData>(md);
	//}
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
