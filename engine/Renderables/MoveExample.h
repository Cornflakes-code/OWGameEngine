#pragma once

#include <algorithm>

#ifndef __gl_h_
#include <glad/glad.h>
#endif

/*
	Texture class showing use of move semantics.
*/
class Texture
{
private:
	// https://www.khronos.org/opengl/wiki/Common_Mistakes
	GLuint obj_ = 0; //Cannot leave this uninitialized.

	void Release()
	{
		glDeleteTextures(1, &obj_);
		obj_ = 0;
	}

public:
	//Other constructors as normal.

	//Free up the texture.
	~Texture() { Release(); }

	//Delete the copy constructor/assignment.
	Texture(const Texture &) = delete;
	Texture &operator=(const Texture &) = delete;

	Texture(Texture &&other) : obj_(other.obj_)
	{
		other.obj_ = 0; //Use the "null" texture for the old object.
	}

	Texture &operator=(Texture &&other)
	{
		//ALWAYS check for self-assignment.
		if (this != &other)
		{
			Release();
			//obj_ is now 0.
			std::swap(obj_, other.obj_);
		}
	}
	GLuint textureID;
	void TexParameter1(GLenum pname, GLint param)
	{
		// Note that this solution emphasizes correctness over 
		// performance; the glGetIntegerv call may not be particularly fast. 
		GLuint boundTexture = 0;
		glGetIntegerv(GL_TEXTURE_BINDING_2D, (GLint*)&boundTexture);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexParameteri(GL_TEXTURE_2D, pname, param);
		glBindTexture(GL_TEXTURE_2D, boundTexture);
	}
	void TexParameter2(GLenum pname, GLint param)
	{
		// A more effective solution is to use Direct State Access, 
		// which requires OpenGL 4.5
		//glTextureParameteri(textureID, GL_TEXTURE_2D, pname, param);
	}
};