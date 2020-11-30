#include "ModelFactory.h"
#include <string>
#include <assimp/Importer.hpp>

#include "../Core/GlobalSettings.h"
#include "../Renderables/Model.h"

#include "LogStream.h"
#include "Texture.h"
#include "ResourcePathFactory.h"
#include "stb/stb_image.h"

ModelLoader::ModelLoader()
{}

bool ModelLoader::create(const std::string& fileName, Model& model, bool cache)
{
	return false;
	/*
	std::string modelPath = globals->resourceCache()->
			getPath(fileName, ResourcePathFactory::ResourceType::Model);

	std::map<std::string, Model*>::const_iterator found = mCache.find(modelPath);
	if (found != mCache.end())
	{
		model = *(found->second);
	}
	else
	{
		Texture* tex = new Texture;
		std::string texturePath = globals->resourceCache()->
			getPath(fileName, ResourcePathFactory::ResourceType::Texture);
		unsigned char* data = nullptr;

		// Do not want to find the same file as the model
		if (texturePath != modelPath)
		{
			tex->addTexture
			int width, height, nrChannels;
			unsigned char* data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 4);
		}
		if (!data)
		{
			LogStream(LogStreamLevel::Info) << "Unable to load texture for model [" << fileName
				<< "] using texture file [" << texturePath << "].";
			return false;
		}
	Assimp::Importer importer;
	}
	*/
}

