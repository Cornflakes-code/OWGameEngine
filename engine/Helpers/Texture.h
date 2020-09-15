#pragma once

#include <string>
#include <vector>
/*
	Possibly not needed.
*/
class Texture
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