#pragma once
#include <map>
#include <filesystem>


#include "../OWEngine/OWEngine.h"

#include "../Core/CommonUtils.h"

#include "Texture.h"

class OWENGINE_API TextureFactory
{
	typedef std::map<std::filesystem::path, Texture> TextureCache;
public:
	const Texture& getTexture(const std::string& fileName);
	const Texture& getTexture(
			const std::filesystem::path& path);
private:
	/*
		https ://www.reddit.com/r/cpp_questions/comments/1bcy8w1/what_is_this_code_doing/
		class TextureHolder
		{
			private:
			std::map<Textures::ID, std::unique_ptr<sf::Texture>> mTextureMap;
		};
	*/ 
	void initTexture(
		const Texture& tb, int width, int height, GLint filter,
		unsigned char* data, GLenum internalFormat, GLint level, 
		GLenum bitmapType);
#pragma warning( push )
#pragma warning( disable : 4251 )
	static TextureCache mLoadedFiles;
#pragma warning( pop )
};