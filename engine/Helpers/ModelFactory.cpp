#include "ModelFactory.h"
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../Core/GlobalSettings.h"
#include "../Renderables/Mesh.h"

#include "LogStream.h"
#include "Texture.h"
#include "ResourcePathFactory.h"
#include "TextureFactory.h"
#include "stb/stb_image.h"

ModelFactory::ModelCache ModelFactory::mLoadedFiles;

ModelFactory::ModelFactory()
{}

std::shared_ptr<Mesh::ModelData>
	ModelFactory::create(const std::string& modelFileName,
							const std::string& textureFileName,
							bool cache)
{
	std::experimental::filesystem::path path =
		ResourcePathFactory().appendPath(modelFileName,
			ResourcePathFactory::ResourceType::Model);

	ModelCache::iterator iter = mLoadedFiles.begin();
	while (iter != mLoadedFiles.end())
	{
		if (std::experimental::filesystem::equivalent(iter->first, path))
			break;
		++iter;
	}
	if (iter != mLoadedFiles.end())
	{
		return iter->second;
	}
	iter = loadModel(path);
	if (iter == mLoadedFiles.end())
	{
		LogStream(LogStreamLevel::Info)
			<< "Unable to load Mesh from file ["
			<< modelFileName << "] using path ["
			<< path << "]";
	}
	std::string textureName = textureFileName.empty() ?
				modelFileName : textureFileName;
	std::experimental::filesystem::path texturePath =
		ResourcePathFactory().appendPath(textureName,
			ResourcePathFactory::ResourceType::Texture);
	// Note: Multiple Textures not yet handled. Need to update mImageUnit
	// to match what is in the shader
	if (texturePath != path)
	{
		// Do not want to find the same file as the model
		Texture texture = TextureFactory().getTexture(texturePath);
		int x, y, bytesPerPixel;
		unsigned char* data = stbi_load(texturePath.u8string().c_str(), 
				&x, &y, &bytesPerPixel, 4);
		if (data)
		{
			Texture::InitData initData;
			initData.filter = GL_LINEAR;
			texture.init(data, x, y, initData);
			stbi_image_free(data);
			iter->second->textures.push_back(texture);
			return iter->second;
		}
		else
		{
			LogStream(LogStreamLevel::Info) 
				<< "Unable to load texture from texture file [" 
				<< texturePath << "] for model [" << modelFileName <<"]";
		}
	}
	else
	{
		LogStream(LogStreamLevel::Info) 
			<< "Unable to find unique texture file for model [" 
			<< path << "]";
	}
	throw NMSLogicException(std::stringstream() << "Unable to create Mesh [" << path << "]");
}

ModelFactory::ModelCache::iterator ModelFactory::loadModel(
		const std::experimental::filesystem::path& path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path.u8string().c_str(),
								aiProcess_Triangulate |
								aiProcess_GenSmoothNormals |
								aiProcess_FlipUVs);
	if (!scene)
	{
		LogStream(LogStreamLevel::Info)
			<< "Mesh load failed for model ["
			<< path << "]";
		return mLoadedFiles.end();
	}
	const aiMesh* aimesh = scene->mMeshes[0];
	Mesh::ModelData* md = new Mesh::ModelData();
	const aiVector3D aiZeroVector(0.0f, 0.0f, 0.0f);
	for (unsigned int i = 0; i < aimesh->mNumVertices; i++)
	{
		const aiVector3D* pPos = &(aimesh->mVertices[i]);
		const aiVector3D* pNormal = &(aimesh->mNormals[i]);
		const aiVector3D* pTexCoord = aimesh->HasTextureCoords(0) ? 
				&(aimesh->mTextureCoords[0][i]) : &aiZeroVector;

		Mesh::Vertex vert(glm::vec3(pPos->x, pPos->y, pPos->z),
			glm::vec3(pNormal->x, pNormal->y, pNormal->z),
			glm::vec2(pTexCoord->x, pTexCoord->y));

		md->vertices.push_back(vert);
	}

	for (unsigned int i = 0; i < aimesh->mNumFaces; i++)
	{
		const aiFace& face = aimesh->mFaces[i];
		assert(face.mNumIndices == 3);
		md->indices.push_back(face.mIndices[0]);
		md->indices.push_back(face.mIndices[1]);
		md->indices.push_back(face.mIndices[2]);
	}
	md->indicesMode = GL_TRIANGLES;
	auto ret = mLoadedFiles.insert(
			{ path, std::shared_ptr<Mesh::ModelData>(md) });
	return ret.first;
}
