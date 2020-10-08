#pragma once

#include <string>
#include <iostream>

#ifndef __gl_h_
#include <glad/glad.h>
#endif
#pragma warning( push )
#pragma warning( disable : 4201 )
#include <glm/glm.hpp>
#pragma warning( pop )

#include "../OWEngine/OWEngine.h"

struct GLFWwindow;
/*
	OpenGL logging functionality
*/
class OWENGINE_API Logger
{
public:
	Logger();
	void update_fps_counter(GLFWwindow* window) const;
	void log_gl_params() const;
	static void print_all(GLuint programme);
	std::string versionString() const;
private:
	static double previous_seconds;
	static int frame_count;
	static std::string toString(GLenum type);
};
