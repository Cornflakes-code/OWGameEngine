#include "ErrorHandling.h"

#include <iostream>

#ifndef __gl_h_
#include <glad/glad.h>
#endif

#include "LogStream.h"

NMSException::NMSException(const std::string& msg)
	: std::exception(msg.c_str())
{
}

static std::string toString(NMSErrorLevel err)
{
	switch (err)
	{
		case NMSErrorLevel::NMSInfo: return "NMSInfo";
		case NMSErrorLevel::NMSWarning: return "NMSWarning";
		case NMSErrorLevel::NMSCritical: return "NMSCritical";
		default: throw NMSException("Unknow NMSErrorLevel");
	}
}

std::string glCheckError_(unsigned int errorCode)
{
	switch (static_cast<GLenum>(errorCode))
	{
	//case GL_NO_ERROR:						return "No Error";
	case GL_INVALID_ENUM:                  return "INVALID_ENUM";
	case GL_INVALID_VALUE:                 return "INVALID_VALUE";
	case GL_INVALID_OPERATION:             return "INVALID_OPERATION";
	case GL_STACK_OVERFLOW:                return "STACK_OVERFLOW";
	case GL_STACK_UNDERFLOW:               return "STACK_UNDERFLOW";
	case GL_OUT_OF_MEMORY:                 return "OUT_OF_MEMORY";
	case GL_INVALID_FRAMEBUFFER_OPERATION: return "INVALID_FRAMEBUFFER_OPERATION";
	default: return "Unknown OpenGL Error";
	}
}

void debugCheckGLError(const char* file, int line)
{
	// https://github.com/bkaradzic/bgfx/blob/8d471959eb3cbc16c0e7fdac25efcf842abd2ad1/src/renderer_gl.h#L994
	// https://github.com/ocornut/imgui/blob/50f5be9266ce21391597695dc80b15a4724447cc/examples/opengl3_example/imgui_impl_glfw_gl3.cpp#L270

	// http://www.c-jump.com/bcc/common/Talk3/Math/GLM/GLM.html
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		LogStream(LogStreamLevel::Warning) << "errorCode[" << err << "] [" << glCheckError_(err)
			<< "in file[" << file << "] line [" << line << "]\n";
	}
}
