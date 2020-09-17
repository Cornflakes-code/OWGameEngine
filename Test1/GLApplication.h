#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <functional>
#include <iostream>
#include "OpenInfo.h"

class MyGlWindow
{
public:
	void makeWindow(GLFWwindow* glfwWindow);

	void create(GLFWwindow* glfwWindow, OpenInfo& info);
	void draw(GLFWwindow* glfwWindow, OpenInfo& info);
	void cleanUp(OpenInfo& elm);
private:
	std::function<void(GLFWwindow*)> onClose;
	std::function<void(GLFWwindow* window, int button, int action, int mods)> onMouseClick;
	std::function<void(GLFWwindow* window, int key, int scancode, int action, int mods)> onKeypress;
	std::function<void(GLFWwindow*, int)> onChar;
	std::function<void(GLFWwindow*, int error, const char* description)> onError = [](auto self, int a, const char* desc)
	{
		std::cout << "Error[" << desc << "]" << std::endl;
	};
	std::function<void(GLFWwindow*, int width, int height)> onWindowResize;
	void OnMouseClick2(GLFWwindow* window, int a, int b, int c);
	void onCharCallback(GLFWwindow* window, unsigned int codepoint);
	void onKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void framebufferSizeCallback(GLFWwindow* window, int width, int height);
};