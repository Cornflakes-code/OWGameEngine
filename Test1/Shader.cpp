#include "Shader.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include <glad/glad.h>

Shader::Shader()
{}

Shader::~Shader()
{
}

void Shader::use()
{
	if (mShaderProgram)
	{
		glUseProgram(mShaderProgram);
	}
}

void Shader::configure()
{
	if (mShaderProgram)
	{
		glUseProgram(mShaderProgram);
		for (auto& e : uniforms)
		{
			std::string t = e["type"];
			if (t == "int")
			{
				setInt(e["name"], e["value"].get<int>());
			}
			else if (t == "bool")
			{
				setBool(e["name"], e["value"].get<bool>());
			}
			if (t == "float")
			{
				setFloat("horOffset", e["value"].get<float>());
			}
		}
	}
}

void Shader::cleanUp()
{
	glDeleteProgram(mShaderProgram);
}

int Shader::addFragmentShader(const std::string& shader)
{
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const GLchar* const ch = shader.c_str();
	glShaderSource(fragmentShader, 1, &ch, NULL);
	glCompileShader(fragmentShader);

	// check for shader compile errors
	int success;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		char infoLog[512];
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::stringstream str;
		str << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		throw std::exception(str.str().c_str());
	}
	return fragmentShader;
}

int Shader::addVertexShader(const std::string& shader)
{
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const GLchar* const ch = shader.c_str();
	glShaderSource(vertexShader, 1, &ch, NULL);
	glCompileShader(vertexShader);

	// check for shader compile errors
	int success;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		throw std::exception("vertex shader error");
	}
	return vertexShader;
}

std::string Shader::readFile(const std::string& shaderPath)
{
	// retrieve the shader source code from filePath
	std::ifstream f;

	// ensure ifstream objects can throw exceptions:
	f.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	f.open(shaderPath);
	std::stringstream strm;

	// read file's buffer contents into streams
	strm << f.rdbuf();

	// close file handlers
	f.close();

	// convert stream into string
	return strm.str();
}

void Shader::linkShaders(json& obj, const std::string& key)
{
	json elm = obj[key];

	std::string vertexCode;
	for (auto& e : elm["vertexShader"].items())
	{
		vertexCode += e.value();
		vertexCode += "\n";
	}
	std::string fragCode;
	for (auto& e : elm["fragmentShader"].items())
	{
		fragCode += e.value();
		fragCode += "\n";
	}
	linkShaders(addFragmentShader(fragCode),
				addVertexShader(vertexCode));

	struct uniformElement
	{
		std::string name;
		std::string type;
	};
	for (auto& e : elm["uniforms"].items())
	{
		json j = e.value();
		uniforms.push_back(j);
		if (e.value()["name"] == "horOffset")
		{
			setFloat("horOffset", e.value()["value"].get<float>());
		}
	}
}

void Shader::readJsonFile(const std::string& path)
{
	std::ifstream ifs(path);
	json jf = nlohmann::json::parse(ifs);
}

void Shader::linkShaders(int fragmentShader, int vertexShader)
{
	mShaderProgram = glCreateProgram();
	glAttachShader(mShaderProgram, vertexShader);
	glAttachShader(mShaderProgram, fragmentShader);
	glLinkProgram(mShaderProgram);
	// check for linking errors
	int success;
	glGetProgramiv(mShaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetProgramInfoLog(mShaderProgram, 512, NULL, infoLog);
		std::stringstream str;
		str << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		throw std::exception(str.str().c_str());
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::setBool(const std::string &name, bool value)
{
	glUniform1i(glGetUniformLocation(mShaderProgram, name.c_str()), (int)value);
}
void Shader::setInt(const std::string &name, int value)
{
	glUniform1i(glGetUniformLocation(mShaderProgram, name.c_str()), value);
}
void Shader::setFloat(const std::string &name, float value)
{
	glUniform1f(glGetUniformLocation(mShaderProgram, name.c_str()), value);
}
