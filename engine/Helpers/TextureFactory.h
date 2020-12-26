#pragma once
#include <map>
#include <experimental/filesystem>


#include "../OWEngine/OWEngine.h"

#include "CommonUtils.h"
#include "Texture.h"

class OWENGINE_API TextureFactory
{
	typedef std::map<std::experimental::filesystem::path, Texture> TextureCache;
public:
	const Texture& getTexture(const std::string& fileName);
	const Texture& getTexture(
			const std::experimental::filesystem::path& path);
private:
	void initTexture(
		const Texture& tb, int width, int height, GLint filter,
		unsigned char* data, GLenum internalFormat, GLint level, 
		GLenum bitmapType);
#pragma warning( push )
#pragma warning( disable : 4251 )
	static TextureCache mLoadedFiles;
#pragma warning( pop )
};