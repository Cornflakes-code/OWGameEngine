#include "TextureFactory.h"

#include <iostream>
#include <sstream>

#include "stb/stb_image.h"

#include "../Core/ResourcePathFactory.h"
#include "../Core/ErrorHandling.h"

/*
template<typename Q>
typename std::map<std::filesystem::path, Q>::const_iterator
findStoredPath(const std::map<std::filesystem::path, Q>& ss,
	const std::string& fileName, ResourcePathFactory::ResourceType rt)
{
	std::filesystem::path path =
		ResourcePathFactory().appendPath(fileName, rt);

	auto iter = ss.begin();
	while (iter != ss.end())
	{
		if (std::filesystem::equivalent(iter->first, path))
			break;
		++iter;
	}
	return iter;
}
*/

TextureFactory::TextureCache TextureFactory::mLoadedFiles;

const Texture& TextureFactory::getTexture(const std::string& fileName)
{
	std::filesystem::path path =
		ResourcePathFactory().appendPath(fileName,
			ResourcePathFactory::ResourceType::Texture);
	return getTexture(path);
}

const Texture& TextureFactory::getTexture(
		const std::filesystem::path& path)
{
	auto iter = mLoadedFiles.begin();
	while (iter != mLoadedFiles.end())
	{
		if (std::filesystem::equivalent(iter->first, path))
			break;
		++iter;
	}
	if (iter != mLoadedFiles.end())
	{
		return iter->second;
	}

	int width, height, nrChannels;
	bool flip = false;
	if (flip)
	{
		// tell stb_image.h to flip loaded texture's on the screenY-axis.
		stbi_set_flip_vertically_on_load(true);
	}

	unsigned char *data = stbi_load(path.string().c_str(),
								&width, &height, &nrChannels, 0);
//	bool isHdr = stbi_is_hdr(path.u8string().c_str());
	if (data)
	{
		Texture texture;
		Texture::InitData initData;
		if (nrChannels == 1)
			initData.internalFormat = GL_RED;
		else if (nrChannels == 3)
			initData.internalFormat = GL_RGB;
		else if (nrChannels == 4)
			initData.internalFormat = GL_RGBA;
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

void TextureFactory::initTexture(
	const Texture& tb, int width, int height, GLint filter,
	unsigned char* data, GLenum internalFormat, GLint level,
	GLenum bitmapType)
{
	throw NMSException("Not implemented");
}
