#include "TextureFactory.h"

#include <iostream>
#include <sstream>

#include "stb/stb_image.h"

#include "ResourcePathFactory.h"

std::map<std::experimental::filesystem::path, Texture> 
	TextureFactory::mLoadedFiles;

const Texture& TextureFactory::getTexture(const std::string& fileName)
{
	std::experimental::filesystem::path path =
		ResourcePathFactory().appendPath(fileName,
			ResourcePathFactory::ResourceType::Texture);

	auto iter = mLoadedFiles.begin();
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

	int width, height, nrChannels;
	bool flip = false;
	bool transparent = false;
	if (flip)
	{
		// tell stb_image.h to flip loaded texture's on the screenY-axis.
		stbi_set_flip_vertically_on_load(true);
	}

	unsigned char *data = stbi_load(path.u8string().c_str(), 
								&width, &height, &nrChannels, 0);
	if (data)
	{
		Texture texture;
		Texture::InitData initData;

		initData.internalFormat = transparent ? GL_RGBA : GL_RGB;
		initData.filter = GL_LINEAR;
		initData.clamp = GL_CLAMP_TO_EDGE;
		texture.init(data, width, height, initData);

		mLoadedFiles[path] = texture;
		stbi_image_free(data);
		return mLoadedFiles[path];
	}
	else
	{
		std::stringstream str;
		str << "Failed to load texture [" << path << "]" << std::endl;
		throw std::exception(str.str().c_str());
	}
}

