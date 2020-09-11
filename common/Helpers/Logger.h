#pragma once

#include <string>
#include <iostream>

#include <glad/glad.h>
#include <glm/glm.hpp>

struct GLFWwindow;

class Logger
{
public:
	Logger();
	void update_fps_counter(GLFWwindow* window);
	void log_gl_params(std::ostream& out);
	static void print_all(GLuint programme, std::ostream& out);
	std::string versionString();
private:
	static double previous_seconds;
	static int frame_count;
	static std::string toString(GLenum type);
};
