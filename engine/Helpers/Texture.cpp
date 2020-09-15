#include "Texture.h"

#include <iostream>
#include <sstream>

#include <glad/glad.h>
#include "stb/stb_image.h"


void Texture::addTexture(const std::string& texturePath, bool transparent, bool flip)
{
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object

	// set the texture wrapping parameters
	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	if (flip)
	{
		stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the screenY-axis.
	}

	// The FileSystem::getPath(...) is part of the GitHub repository so we 
	// can find files on any IDE/platform; replace it with your own image path.
	unsigned char *data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		if (transparent)
		{
			// note that the awesomeface.png has transparency and thus an alpha channel, 
			// so make sure to tell OpenGL the data type is of GL_RGBA
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		glGenerateMipmap(GL_TEXTURE_2D);
		mTextures.push_back({ texture, transparent, flip });
	}
	else
	{
		char infoLog[512];
		std::stringstream str;
		str << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		std::cout << "Failed to load texture" << std::endl;
		throw std::exception(str.str().c_str());
	}
	stbi_image_free(data);
}

void Texture::use()
{
	if (!mTextures.size())
		return;
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mTextures[0].texture);
	for (int i = 1; i < mTextures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, mTextures[i].texture);
	}
}
