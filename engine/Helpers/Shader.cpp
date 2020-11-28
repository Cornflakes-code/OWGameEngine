#include "Shader.h"

#include <fstream>
#include <iostream>
#include <sstream>

#pragma warning( push )
#pragma warning( disable : 4201 )
#include <glm/gtc/type_ptr.hpp>
#pragma warning( pop )

#ifndef __gl_h_
#include <glad/glad.h>
#endif

#include "Logger.h"
#include "ErrorHandling.h"
#include "ShaderFactory.h"
#include "CommonUtils.h"
#include "../Core/GlobalSettings.h"

Shader::Shader()
{}

Shader::Shader(const std::string& vertexPath, const std::string& fragPath,
	const std::string& geometryPath)
{
	readFiles(vertexPath, fragPath, geometryPath);
}

Shader::~Shader()
{
}

void Shader::debugPrint()
{
	Logger::print_all(mShaderProgram);
}

void Shader::use() const
{
	if (mShaderProgram)
	{
		glUseProgram(mShaderProgram);
	}
}

void Shader::setup()
{
	if (mShaderProgram)
	{
		glUseProgram(mShaderProgram);
		processUniforms();
	}
}

void Shader::processUniforms()
{
	for (auto& e : uniforms)
	{
		std::string t = e["type"];
		if (t == "int")
		{
			setInteger(e["name"], e["value"].get<int>());
		}
		if (t == "float")
		{
			setFloat("horOffset", e["value"].get<float>());
		}
	}
}

void Shader::cleanUp()
{
	glDeleteProgram(mShaderProgram);
}

int Shader::addShader(const std::string& sourceCode, unsigned int type, 
					const std::string& errmsg)
{
	GLenum err = glGetError();
	if (!sourceCode.size())
		return 0;
	int shader = glCreateShader(type);
	err = glGetError();
	const GLchar* ch = sourceCode.c_str();
	glShaderSource(shader, 1, &ch, NULL);
	err = glGetError();
	glCompileShader(shader);
	err = glGetError();

	// check for shader compile errors
	int success = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		char infoLog[512];
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::stringstream str;
		str << errmsg << infoLog << std::endl;
		throw NMSLogicException(str.str().c_str());
	}
	err = glGetError();
	return shader;
}

std::string Shader::readFile(const std::string& fileName)
{
	if (fileName.empty())
		return "";
	// Cannot get around the need to link ResourcePathFactory but this at 
	// least allows us to avoid the considerable setup that ResourcePathFactory
	// needs. For a quick and dirty use of the Shader class just use the 
	// Shader::getPath function.
	if (true)
	{
		return ShaderFactory().getShader(fileName);
	}
	else
	{
		return getPath(fileName);
	}
}

// This is a cut down version of ResourcePathFactory::getPath. A full and correct 
// pathname needs to be passed as there is less error checking.
std::string Shader::getPath(const std::string& fileName)
{
	std::ifstream f(fileName, std::ios::in | std::ios::binary);

	// ensure ifstream objects can throw exceptions:
	f.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	std::string fileContents;
	f.seekg(0, std::ios::end);
	std::streampos sz = f.tellg();
	fileContents.reserve(sz);
	f.seekg(0, std::ios::beg);

	fileContents.assign(std::istreambuf_iterator<char>(f), 
						std::istreambuf_iterator<char>());

	return fileContents;
}

void Shader::loadShaders(const std::string& vertexShader,
						 const std::string& fragShader,
						 const std::string& geometryShader)
{
	linkShaders(addVertexShader(vertexShader),
				addFragmentShader(fragShader),
				addGeometryShader(geometryShader));

}

void Shader::readFiles(const std::string& vertexPath,
					   const std::string& fragPath,
					   const std::string& geometryPath)
{
	try
	{
		loadShaders(readFile(vertexPath),
				readFile(fragPath),
				readFile(geometryPath));
	}
	catch (const NMSLogicException& e)
	{
		std::stringstream ss;
		ss << e.what() << " Could be vertex[" << vertexPath << "] frag ["
			<< fragPath << "] Geom [" << geometryPath << "]";
		NMSLogicException ee(ss.str());
		throw ee;
	}
}

int Shader::addVertexShader(const std::string& shader)
{
	return addShader(shader, GL_VERTEX_SHADER, 
			"ERROR::SHADER::VERTEX::COMPILATION_FAILED\n");
}

int Shader::addFragmentShader(const std::string& shader)
{
	return addShader(shader, GL_FRAGMENT_SHADER, 
			"ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n");
}

int Shader::addGeometryShader(const std::string& shader)
{
	return addShader(shader, GL_GEOMETRY_SHADER, 
			"ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n");
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
	std::string geomCode;
	for (auto& e : elm["geometryShader"].items())
	{
		fragCode += e.value();
		fragCode += "\n";
	}
	linkShaders(addVertexShader(vertexCode), 
				addFragmentShader(fragCode),
				addGeometryShader(geomCode));

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

void Shader::attachShader(int shader)
{
	if (shader)
		glAttachShader(mShaderProgram, shader);
}

void Shader::linkShaders(int vertexShader, int fragmentShader, int geomShader,
						const std::vector<TransformFeedback>& feedbacks)
{
	GLenum err = glGetError();
	mShaderProgram = glCreateProgram();
	attachShader(vertexShader);
	attachShader(fragmentShader);
	attachShader(geomShader);
	err = glGetError();
	if (feedbacks.size())
	{
		// https://open.gl/feedback
		throw NMSNotYetImplementedException("Transform feedbacks");
		//glTransformFeedbackVaryings(mShaderProgram, screenX.name.size(), screenX.name.data(),
			//screenX.interleaved ? GL_INTERLEAVED_ATTRIBS : GL_SEPARATE_ATTRIBS);
	}
	glLinkProgram(mShaderProgram);
	// check for linking errors
	int success;
	glGetProgramiv(mShaderProgram, GL_LINK_STATUS, &success);
	err = glGetError();
	if (!success) {
		char infoLog[512];
		glGetProgramInfoLog(mShaderProgram, 512, NULL, infoLog);
		std::stringstream str;
		str << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		throw std::exception(str.str().c_str());
	}
	err = glGetError();
	glDeleteShader(vertexShader);
	err = glGetError();
	glDeleteShader(fragmentShader);
	err = glGetError();
	glDeleteShader(geomShader);
	err = glGetError();
}

int Shader::getUniformLocation(const std::string& name) const
{
	GLint loc = glGetUniformLocation(mShaderProgram, name.c_str());
	return loc;
}

int Shader::getAttributeLocation(const std::string& name) const
{
	return glGetAttribLocation(mShaderProgram, name.c_str());
}

void Shader::setFloat(const std::string& name, float value, bool useShader) const
{
	if (useShader)
		use();
	glUniform1f(getUniformLocation(name), value);
}

void Shader::setInteger(const std::string& name, int value, bool useShader) const
{
	if (useShader)
		use();
	glUniform1i(getUniformLocation(name), value);
}

void Shader::setVector2f(const std::string& name, float x, float y, 
				bool useShader) const
{
	if (useShader)
		use();
	glUniform2f(getUniformLocation(name), x, y);
}

void Shader::setVector2f(const std::string& name, const glm::vec2 &value, 
				bool useShader)  const
{
	if (useShader)
		use();
	glUniform2f(getUniformLocation(name), value.x, value.y);
}

void Shader::setVector3f(const std::string& name, float x, float y, float z, 
				bool useShader) const
{
	if (useShader)
		use();
	glUniform3f(getUniformLocation(name), x, y, z);
}

void Shader::setVector3f(const std::string& name, const glm::vec3 &value, 
				bool useShader) const
{
	if (useShader)
		use();
	glUniform3f(getUniformLocation(name), value.x, value.y, value.z);
}

void Shader::setVector4f(const std::string& name, float x, float y, float z, 
				float width, bool useShader) const
{
	if (useShader)
		use();
	glUniform4f(getUniformLocation(name), x, y, z, width);
}

void Shader::setVector4f(const std::string& name, const glm::vec4 &value, 
				bool useShader) const
{
	if (useShader)
		use();
	glUniform4f(getUniformLocation(name), value.x, value.y, value.z, value.w);
}

void Shader::setMatrix4(const std::string& name, const glm::mat4 &matrix, 
				bool useShader) const
{
	if (useShader)
		use();
	glUniformMatrix4fv(getUniformLocation(name), 1, false, glm::value_ptr(matrix));
}
