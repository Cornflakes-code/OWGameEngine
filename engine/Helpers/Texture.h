#pragma once

#include <string>
#include <vector>

#include "../OWEngine/OWEngine.h"
/*
	Possibly not needed.
*/
class OWENGINE_API Texture
{
	struct TextureProps
	{
		unsigned int texture;
		bool transparent;
		bool flip;
	};
	std::vector<TextureProps> mTextures;
public:
	void addTexture(const std::string& texturePath, bool transparent, bool flip);
	void use();
};