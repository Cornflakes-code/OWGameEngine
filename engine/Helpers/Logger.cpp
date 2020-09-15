#include "Logger.h"

#include <sstream>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

double Logger::previous_seconds = 0.0;
int Logger::frame_count = 0;

Logger::Logger()
{}

void Logger::update_fps_counter(GLFWwindow* window)
{
	double current_seconds = glfwGetTime();
	double elapsed_seconds = current_seconds - previous_seconds;
	/* limit text updates to 4 per second */
	if (elapsed_seconds > 0.25) 
	{
		previous_seconds = current_seconds;
		double fps = frame_count / elapsed_seconds;
		std::stringstream ss;
		ss << "opengl @ fps: " << fps << "\n";
		glfwSetWindowTitle(window, ss.str().c_str());
		frame_count = 0;
	}
	frame_count++;
}

void Logger::log_gl_params(std::ostream& out)
{
	GLenum params[] = {
	  GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,
	  GL_MAX_CUBE_MAP_TEXTURE_SIZE,
	  GL_MAX_DRAW_BUFFERS,
	  GL_MAX_FRAGMENT_UNIFORM_COMPONENTS,
	  GL_MAX_TEXTURE_IMAGE_UNITS,
	  GL_MAX_TEXTURE_SIZE,
	  GL_MAX_VARYING_FLOATS,
	  GL_MAX_VERTEX_ATTRIBS,
	  GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,
	  GL_MAX_VERTEX_UNIFORM_COMPONENTS,
	  GL_MAX_VIEWPORT_DIMS,
	  GL_STEREO,
	};
	const char* names[] = {
	  "GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS",
	  "GL_MAX_CUBE_MAP_TEXTURE_SIZE",
	  "GL_MAX_DRAW_BUFFERS",
	  "GL_MAX_FRAGMENT_UNIFORM_COMPONENTS",
	  "GL_MAX_TEXTURE_IMAGE_UNITS",
	  "GL_MAX_TEXTURE_SIZE",
	  "GL_MAX_VARYING_FLOATS",
	  "GL_MAX_VERTEX_ATTRIBS",
	  "GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS",
	  "GL_MAX_VERTEX_UNIFORM_COMPONENTS",
	  "GL_MAX_VIEWPORT_DIMS",
	  "GL_STEREO",
	};
	out << "GL Context Params:\n";
	// integers - only works if the order is 0-10 integer return types6
	for (int i = 0; i < 10; i++)
	{
		int v = 0;
		glGetIntegerv(params[i], &v);
		out << names[i] << " " << v << "\n";
	}
	// others
	int v[2];
	v[0] = v[1] = 0;
	glGetIntegerv(params[10], v);
	std::cout << names[10] << " " << v[0] << " " << v[1] << "\n";
	unsigned char s = 0;
	glGetBooleanv(params[11], &s);
	out << names[11] << " " << s << "\n";

	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string

	std::cout << "Renderer: " << renderer << "\n";
	std::cout << "OpenGL version supported: " << version << "\n";
	out << "-----------------------------\n";
}

std::string Logger::toString(GLenum type)
{
	switch (type) 
	{
		case GL_BOOL: return "bool";
		case GL_INT: return "int";
		case GL_FLOAT: return "float";
		case GL_FLOAT_VEC2: return "vec2";
		case GL_FLOAT_VEC3: return "vec3";
		case GL_FLOAT_VEC4: return "vec4";
		case GL_FLOAT_MAT2: return "mat2";
		case GL_FLOAT_MAT3: return "mat3";
		case GL_FLOAT_MAT4: return "mat4";
		case GL_SAMPLER_2D: return "sampler2D";
		case GL_SAMPLER_3D: return "sampler3D";
		case GL_SAMPLER_CUBE: return "samplerCube";
		case GL_SAMPLER_2D_SHADOW: return "sampler2DShadow";
		default: break;
	}
	return "other";
}

void Logger::print_all(GLuint programme, std::ostream& out)
{
	printf("--------------------\nshader programme %i info:\n", programme);
	int params = -1;
	glGetProgramiv(programme, GL_LINK_STATUS, &params);
	out << "GL_LINK_STATUS = " << params << "\n";

	glGetProgramiv(programme, GL_ATTACHED_SHADERS, &params);
	out << "GL_ATTACHED_SHADERS = " << params << "\n";

	glGetProgramiv(programme, GL_ACTIVE_ATTRIBUTES, &params);
	out << "GL_ACTIVE_ATTRIBUTES = " << params << "\n";
	for (int i = 0; i < params; i++) 
	{
		char name[64];
		int max_length = 64;
		int actual_length = 0;
		int size = 0;
		GLenum type;
		glGetActiveAttrib(programme, i, max_length, &actual_length, &size, &type, name);
		if (size > 1)
		{
			for (int j = 0; j < size; j++) 
			{
				std::stringstream long_name;
				long_name << name << "[" << j << "]\n";
				
				int location = glGetAttribLocation(programme, long_name.str().c_str());
				out << "  " << i << ") type:" << toString(type)
					<< " name:" << long_name.str().c_str() 
					<< " location:"	<< location << "\n";
			}
		}
		else 
		{
			int location = glGetAttribLocation(programme, name);
			out << "  " << i << ") type:" << toString(type)
				<< " name:" << name
				<< " location:" << location << "\n";
		}
	}

	glGetProgramiv(programme, GL_ACTIVE_UNIFORMS, &params);
	out << "GL_ACTIVE_UNIFORMS = " << params << "\n";
	for (int i = 0; i < params; i++) 
	{
		char name[64];
		int max_length = 64;
		int actual_length = 0;
		int size = 0;
		GLenum type;
		glGetActiveUniform(programme, i, max_length, &actual_length, &size, &type, name);
		if (size > 1) 
		{
			for (int j = 0; j < size; j++)
			{
				std::stringstream long_name;
				long_name << name << "[" << j << "]\n";
				int location = glGetUniformLocation(programme, long_name.str().c_str());
				out << "  " << i << ") type:" << toString(type)
					<< " name:" << long_name.str().c_str()
					<< " location:" << location << "\n";
			}
		}
		else {
			int location = glGetUniformLocation(programme, name);
			out << "  " << i << ") type:" << toString(type)
				<< " name:" << name
				<< " location:" << location << "\n";
		}
	}
}

std::string Logger::versionString()
{
	std::string ss = glfwGetVersionString();
	if (ss.size())
	{
		return ss;
	}
	else
	{
		std::string s = "OpenGL first program ";
		GLenum e = glGetError();
		if (e == GL_INVALID_ENUM)
			s += "GL_INVALID_ENUM";
		else if (e == GL_INVALID_OPERATION)
			s += "GL_INVALID_OPERATION";
		else s += "NULL";
		return s;
	}
}
