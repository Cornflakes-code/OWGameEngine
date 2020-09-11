#include "ErrorHandling.h"

#include <iostream>

#include <glad/glad.h>

void dumpMessage(const std::string& msg, NMSErrorLevel err)
{
	std::cout << msg << "\n";
}

void dumpMessage(const std::stringstream& ss, NMSErrorLevel err)
{
	dumpMessage(ss.str(), err);
}

std::string glCheckError_(GLenum errorCode)
{
	switch (errorCode)
	{
	case GL_INVALID_ENUM:                  return "INVALID_ENUM";
	case GL_INVALID_VALUE:                 return "INVALID_VALUE";
	case GL_INVALID_OPERATION:             return "INVALID_OPERATION";
//		case GL_STACK_OVERFLOW:                return "STACK_OVERFLOW";
//		case GL_STACK_UNDERFLOW:               return "STACK_UNDERFLOW";
	case GL_OUT_OF_MEMORY:                 return "OUT_OF_MEMORY";
	case GL_INVALID_FRAMEBUFFER_OPERATION: return "INVALID_FRAMEBUFFER_OPERATION";
	default: return "Unknown OpenGL Error";
	}
}

void debugCheckGLError(const char *file, int line)
{
	// https://github.com/bkaradzic/bgfx/blob/8d471959eb3cbc16c0e7fdac25efcf842abd2ad1/src/renderer_gl.h#L994
	// https://github.com/ocornut/imgui/blob/50f5be9266ce21391597695dc80b15a4724447cc/examples/opengl3_example/imgui_impl_glfw_gl3.cpp#L270

	// http://www.c-jump.com/bcc/common/Talk3/Math/GLM/GLM.html
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		std::cout << "errorCode[" << err << "] [" << glCheckError_(err) << "]\n";
	}
}
