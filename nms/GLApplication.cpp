#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "GLApplication.h"

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

struct MyInfo
{
	int shaderProgram;
	unsigned int VBO, VAO;
};

void* MyGlWindow::createTriangle(GLFWwindow* glfwWindow)
{
	// https://learnopengl.com/Getting-started/Hello-Triangle
	MyInfo* myInfo = new MyInfo();
	const char *vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";
	const char *fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = vec4(1.0f, 0.5f, 0.8f, 1.0f);\n"
		"}\n\0";

	// build and compile our shader program
	// ------------------------------------
	// vertex shader
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// fragment shader
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// link shaders
	myInfo->shaderProgram = glCreateProgram();
	glAttachShader(myInfo->shaderProgram, vertexShader);
	glAttachShader(myInfo->shaderProgram, fragmentShader);
	glLinkProgram(myInfo->shaderProgram);
	// check for linking errors
	glGetProgramiv(myInfo->shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(myInfo->shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		-0.5f, -0.5f, 0.0f, // left  
		 0.5f, -0.5f, 0.0f, // right 
		 0.0f,  0.5f, 0.0f  // top   
//		 ,0.0f, 0.0f, 0.0f
	};

	glGenVertexArrays(1, &(myInfo->VAO));
	glGenBuffers(1, &(myInfo->VBO));
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(myInfo->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, myInfo->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex 
	// attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, 
	// but this rarely happens. Modifying other VAOs requires a call to glBindVertexArray anyways 
	// so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);


	// uncomment this call to draw in wireframe polygons.
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	return myInfo;
}

void MyGlWindow::drawTriangle(GLFWwindow* glfwWindow, void* state)
{
	MyInfo* myInfo = reinterpret_cast<MyInfo*>(state);
	// render
	// ------
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// draw our first triangle
	glUseProgram(myInfo->shaderProgram);
	// seeing as we only have a single VAO there's no need to bind it 
	// every time, but we'll do so to keep things a bit more organized
	glBindVertexArray(myInfo->VAO); 
	glDrawArrays(GL_TRIANGLES, 0, 3);
	// glBindVertexArray(0); // no need to unbind it every time 

	// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
	// 
}

void MyGlWindow::cleanUpTriangle(void* state)
{
	MyInfo* myInfo = reinterpret_cast<MyInfo*>(state);
	glDeleteVertexArrays(1, &(myInfo->VAO));
	glDeleteBuffers(1, &(myInfo->VBO));
	glDeleteProgram(myInfo->shaderProgram);
	delete myInfo;
}
