#pragma once

#include <string>
#include <vector>

#ifndef __gl_h_
#include <glad/glad.h>
#endif

#include "../OWEngine/OWEngine.h"

class OWENGINE_API Texture
{
public:
	struct InitData
	{
		GLenum internalFormat = GL_RGBA;
		GLenum bitmapType = GL_UNSIGNED_BYTE;
		GLint filter = GL_INVALID_INDEX;
		GLint clamp = GL_INVALID_INDEX;
		GLint wrap = GL_INVALID_INDEX;
		GLint level = 0;
	};
	Texture();
	void init(unsigned char* data, int width, int height, const InitData& initData);
	unsigned int location() const { return mLocation;  }
	unsigned int imageUnit() const { return mImageUnit; }
	unsigned int target() const { return mTarget; }
private:
#pragma warning( push )
#pragma warning( disable : 4251 )
	unsigned int mLocation; // from glGenTextures
	unsigned int mImageUnit; // GL_TEXTURE0 -> GL_TEXTUREN
	unsigned int mTarget; // i.e. GL_TEXTURE_2D
#pragma warning( pop )
};