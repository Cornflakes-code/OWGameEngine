#pragma once

#include <string>
#include <vector>

#include "../OWEngine/OWEngine.h"
/*
	Possibly not needed.
*/
class OWENGINE_API Texture
{
#pragma warning( push )
#pragma warning( disable : 4251 )
	struct TextureProps
	{
		unsigned int texture;
		bool transparent;
		bool flip;
	};
	std::vector<TextureProps> mTextures;
#pragma warning( pop )
public:
	void addTexture(const std::string& texturePath, bool transparent, bool flip);
	void use();
};