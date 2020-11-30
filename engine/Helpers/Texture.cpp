#include "Texture.h"

#include <iostream>
#include <sstream>

#include "stb/stb_image.h"

#ifndef __gl_h_
#include <glad/glad.h>
#endif

Texture::Texture()
{
	mLocation = GL_INVALID_INDEX;
	mImageUnit = GL_TEXTURE0;
	mTarget = GL_TEXTURE_2D;
}

void Texture::init(int width, int height, GLint filter,
	unsigned char* data, GLenum internalFormat, GLint level,
	GLenum bitmapType)
{
	if (width > 0 && height > 0)
	{
		glGenTextures(1, &mLocation);
		glActiveTexture(mImageUnit);
		glBindTexture(mTarget, mLocation);

		// set the texture wrapping parameters
		// set texture wrapping to GL_REPEAT (default wrapping method)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexImage2D(mTarget,
			level,
			internalFormat, // internal format
			width, height,
			0,  // border = 0
			internalFormat, // format of the pixel data
			bitmapType,
			data);

		// Clamping to edges is important to prevent artifacts when scaling
		glTexParameteri(mTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(mTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexParameteri(mTarget, GL_TEXTURE_MIN_FILTER, filter);
		glTexParameteri(mTarget, GL_TEXTURE_MAG_FILTER, filter);

		/* Enable blending, necessary for our alpha texture */
		if (internalFormat == GL_RGBA)
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}
	}
}
/*
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
		mTextures.push_back({ flip, transparent, texture });
	}
	else
	{
		std::stringstream str;
		str << "Failed to load texture [" << texturePath << "]" << std::endl;
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
*/