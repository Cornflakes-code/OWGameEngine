#pragma once
#include <map>
#include <experimental/filesystem>


#include "../OWEngine/OWEngine.h"

#include "CommonUtils.h"
#include "Texture.h"

class OWENGINE_API TextureFactory
{
public:
	const Texture& getTexture(const std::string& fileName);
private:
	void initTexture(
		const Texture& tb, int width, int height, GLint filter,
		unsigned char* data, GLenum internalFormat, GLint level, 
		GLenum bitmapType);
#pragma warning( push )
#pragma warning( disable : 4251 )
	static std::map<std::experimental::filesystem::path, Texture> mLoadedFiles;
#pragma warning( pop )
};