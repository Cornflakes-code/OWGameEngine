#include "ModelFactory.h"
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../Core/GlobalSettings.h"
#include "../Helpers/ModelData.h"
#include "../Renderables/Mesh.h"

#include "LogStream.h"
#include "Texture.h"
#include "ResourcePathFactory.h"
#include "stb/stb_image.h"

ModelFactory::ModelFactory()
{}

ModelData* processNode(aiNode *node, const aiScene *scene);

ModelData* ModelFactory::create(const std::string& modelFileName, bool cache)
{
	std::experimental::filesystem::path modelPath =
		ResourcePathFactory().appendPath(modelFileName,
			ResourcePathFactory::ResourceType::Model);

	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(modelPath.u8string().c_str(),
		aiProcess_Triangulate | aiProcess_ConvertToLeftHanded); // | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		LogStream(LogStreamLevel::Info)
			<< "Mesh load failed for model ["
			<< modelPath << "] ASSIMP error [" << importer.GetErrorString() << "]";
		return nullptr;
	}
	ModelData* root = processNode(scene->mRootNode, scene);
	root->parent = nullptr;
	return root;
}

ModelData* processNode(aiNode *node, const aiScene *scene)
{
	ModelData* retval = new ModelData;

	// process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		Mesh m;
		m.create(scene->mMeshes[node->mMeshes[i]], scene);
		retval->meshes.push_back(m);
	}

	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ModelData* child =  processNode(node->mChildren[i], scene);
		child->parent = retval;
		retval->children.push_back(child);
	}
	return retval;
}
