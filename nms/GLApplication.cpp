#include "GLApplication.h"

#include <iostream>
#include <fstream>

#include <glad/glad.h>
#include <json/single_include/nlohmann/json.hpp>
using json = nlohmann::json;


void MyGlWindow::makeWindow(GLFWwindow* glfwWindow) 
{
	
	glfwSetWindowUserPointer(glfwWindow, this);

	#define genericCallback(functionName)\
        [](GLFWwindow* window, auto... args) {\
            auto pointer = static_cast<MyGlWindow*>(glfwGetWindowUserPointer(window));\
            if (pointer->functionName) pointer->functionName(window, args...);\
        }

	onKeypress = [this](GLFWwindow* window, int key, int scancode, int action, int mods) {
		onKeyCallback(window, key, scancode, action, mods);
	};

	onMouseClick = [this](GLFWwindow* window, int button, int action, int mods)
	{
		std::cout << "I'm such a rebel" << std::endl;
		OnMouseClick2(window, button, action, mods);
	};

	onClose = [](GLFWwindow* window) {
		std::cout << "I'm such a rebellion" << std::endl;
	};

	onChar = [this](GLFWwindow* window, int scancode) {
		onCharCallback(window, scancode);
	};

	onWindowResize = [this](GLFWwindow* window, int width, int height) {
		framebufferSizeCallback(window, width, height);
	};

	glfwSetMouseButtonCallback(glfwWindow, genericCallback(onMouseClick));
	glfwSetWindowCloseCallback(glfwWindow, genericCallback(onClose));
	glfwSetCharCallback(glfwWindow, genericCallback(onChar));
	glfwSetKeyCallback(glfwWindow, genericCallback(onKeypress));
	glfwSetFramebufferSizeCallback(glfwWindow, genericCallback(onWindowResize));
}

void MyGlWindow::OnMouseClick2(GLFWwindow* window, int a, int b, int c)
{
	std::cout << "a[" << a  << "] b[" << b << "] c[" << "]" << std::endl;
}

void MyGlWindow::onKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	std::cout << "onKeyCallback - Key[" << key << "] scancode[" << scancode << "] action[" << action << "] mods" << mods << "]" << std::endl;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void MyGlWindow::onCharCallback(GLFWwindow* window, unsigned int codepoint)
{
	std::cout << "onCharCallback - codepoint[" << codepoint << "]" << std::endl;
}

void MyGlWindow::framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	std::cout << "framebufferSizeCallback - width[" << width << "] heaight" << height << "]" << std::endl;
	glViewport(0, 0, width, height);
}

void MyGlWindow::create(GLFWwindow* glfwWindow, OpenInfo& info)
{
	// https://learnopengl.com/Getting-started/Hello-Triangle

	const char *vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"layout(location = 1) in vec3 aColor;\n"
//		"out vec4 vertexColor;\n"
		"out vec3 ourColor;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
//		"	vertexColor = vec4(0.5, 0.0, 0.0, 1.0);\n"
		" ourColor = aColor;\n"
		"}\0";
	std::ifstream ifs("./../nms/vao.json");
	//std::ifstream ifs("E:/Apps/NMS/nms/nms/vao.json");
	nlohmann::json jf = nlohmann::json::parse(ifs);
	{
		OpenInfoObject* elm = new OpenInfoObject();
		elm->shader.linkShaders(jf, "vao_texture");
		elm->texture.addTexture("./../nms/container.jpg", false, false);
		elm->texture.addTexture("./../nms/awesomeface.png", true, true);
		std::vector<float>vertices = {
			// positions          // colors           // texture coords
			 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
			 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
			-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
			-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
		};
		std::vector<unsigned int> indices = {
			0, 1, 3, // first triangle
			1, 2, 3  // second triangle
		};
		std::function<void(int)> pos1 =
			[](int x)-> void { glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); };
		elm->attrPointers.push_back(pos1);
		std::function<void(int)> pos2 =
			[](int x)-> void { glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); };
		elm->attrPointers.push_back(pos2);
		std::function<void(int)> pos3 =
			[](int x)-> void { glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); };
		elm->attrPointers.push_back(pos3);
		elm->triangles.push_back(vertices);
		elm->indices = indices;
		info.objects.emplace_back(elm);
	}
	if (false)
	{
		const char *fragmentShaderSource = "#version 330 core\n"
			"out vec4 FragColor;\n"
//			"in vec4 vertexColor;\n"
			"in vec3 ourColor;\n"
			"void main()\n"
			"{\n"
//			"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
			"	FragColor = vec4(ourColor, 1.0);\n"
			"}\n\0";
		OpenInfoObject* elm = new OpenInfoObject();
		Shader shader;
		elm->shader.linkShaders(jf, "vao1");
//		elm->shader.linkShaders(elm.shader.addFragmentShader(fragmentShaderSource),
//			elm->shader.addVertexShader(vertexShaderSource));
		std::vector<float> tri = {
			-0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
			 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f
		};
		std::function<void(int)> pos =
			[](int x)-> void { glVertexAttribPointer(x, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); };
		elm->attrPointers.push_back(pos);
		pos = [](int x)-> void { glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))); };
		elm->attrPointers.push_back(pos);
		elm->triangles.push_back(tri);
		info.objects.emplace_back(elm);
	}
	if (false)
	{
		const char *fragmentShaderSource = "#version 330 core\n"
			"out vec4 FragColor;\n"
			"void main()\n"
			"{\n"
			"   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
			"}\n\0";
		OpenInfoObject* elm = new OpenInfoObject();
		elm->shader.linkShaders(elm->shader.addFragmentShader(fragmentShaderSource),
			elm->shader.addVertexShader(vertexShaderSource));
		std::vector<float> tri = {
			-0.2f, -0.2f, 0.0f,
			 0.2f, -0.2f, 0.0f,
			 0.0f,  0.2f, 0.0f
		};
		elm->triangles.push_back(tri);
		std::function<void(int)> pos =
			[](int x)-> void { glVertexAttribPointer(x, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); };
		elm->attrPointers.push_back(pos);

		info.objects.emplace_back(elm);
	}
	if (false)
	{
		const char *fragmentShaderSource = "#version 330 core\n"
			"out vec4 FragColor;\n"
			"void main()\n"
			"{\n"
			"   FragColor = vec4(0.5f, 1.0f, 0.5f, 1.0f);\n"
			"}\n\0";
		OpenInfoObject* elm = new OpenInfoObject();
		elm->shader.linkShaders(elm->shader.addFragmentShader(fragmentShaderSource),
			elm->shader.addVertexShader(vertexShaderSource));
		std::vector<float> tri = {
			-0.1f, -0.1f, 0.0f,
			 0.1f, -0.1f, 0.0f,
			 0.0f,  0.1f, 0.0f
		};
		elm->triangles.push_back(tri);
		std::function<void(int)> pos =
			[](int x)-> void { glVertexAttribPointer(x, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); };
		elm->attrPointers.push_back(pos);
		info.objects.emplace_back(elm);
	}

	for (auto& elm : info.objects)
	{
		elm->configure();
	}

	// uncomment this call to draw in wireframe polygons.
	//info.polygonDrawMode(GL_FRONT_AND_BACK, GL_LINE);
}

void MyGlWindow::draw(GLFWwindow* glfwWindow, OpenInfo& info)
{
	// render
	// ------
	info.clearColour(GL_COLOR_BUFFER_BIT, 0.2f, 0.3f, 0.3f, 1.0f);
	info.use();
	//glBindVertexArray(0); // no need to unbind it every time 

	// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
	// 
}

void MyGlWindow::cleanUp(OpenInfo& info)
{
	info.cleanup();
}
