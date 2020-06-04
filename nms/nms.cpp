#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include "GLApplication.h"

#pragma warning( push )
#pragma warning( disable : 4244 )
#include <linmath.h>
#pragma warning( pop )

#pragma comment( lib, "glfw3.lib" )
#pragma comment (lib, "OpenGL32.lib")


//JFW
// https://about-prog.com/opengl/hello-world-opengl-first-program
// https://learnopengl.com/Introduction

std::string getString()
{
	std::string ss = glfwGetVersionString();
	std::string s = "OpenGL first program ";
	if (ss.size())
	{
		return ss;
	}
	else
	{
		GLenum e = glGetError();
		if (e == GL_INVALID_ENUM)
			s += "GL_INVALID_ENUM";
		else if (e == GL_INVALID_OPERATION)
			s += "GL_INVALID_OPERATION";
		else s += "NULL";
	}
	return s;
}


int main(int argc, char* argv[])
{
	std::string s = getString();
	if (glfwInit())
	{
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		int width = 800;
		int height = 600;

		GLFWwindow* window = glfwCreateWindow(width, height, s.c_str(), nullptr, nullptr);
		MyGlWindow wrapper;
		if (window)
		{
			wrapper.makeWindow(window);
			glfwMakeContextCurrent(window);

			if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			{
				void* userData = wrapper.createTriangle(window);
				//glfwGetFramebufferSize(window, &width, &height);
				//glViewport(0, 0, width, height);
				//glfwSwapInterval(1);
				while (!glfwWindowShouldClose(window))
				{
					if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
						glfwSetWindowShouldClose(window, GL_TRUE);
					wrapper.drawTriangle(window, userData);
					//s = getString();
					//glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
					//glClear(GL_COLOR_BUFFER_BIT);

					glfwSwapBuffers(window);
					glfwPollEvents();
				}
				wrapper.cleanUpTriangle(userData);
			}
			glfwTerminate();
			glfwDestroyWindow(window);
		}
	}
	else
	{
		// init failed.
	}
	return 0;
}
