#pragma once

#include <chrono>
#include <string>
#include <iostream>

#ifndef __gl_h_
#include <glad/glad.h>
#endif

#include <glm/glm.hpp>

#include "CommonUtils.h"
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
#pragma warning( push )
#pragma warning( disable : 4251 )
	static OWUtils::Time::time_point previous_seconds;
	static int frame_count;
	static std::string toString(GLenum type);
#pragma warning( pop )
};
