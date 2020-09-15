#pragma once
#include <string>

#include <json/single_include/nlohmann/json.hpp>
using json = nlohmann::json;


class Shader
{
	int mShaderProgram = 0;
	std::vector<json> uniforms;
public:
	Shader();
	~Shader();
	void use();
	void configure();
	void cleanUp();
	unsigned int shaderProgram() const { return mShaderProgram; }
	static int addFragmentShader(const std::string& shader);
	static int addVertexShader(const std::string& shader);
	static std::string readFile(const std::string& shaderPath);
	void readJsonFile(const std::string& path);
	void linkShaders(int fragmentShader, int vertexShader);
	void linkShaders(json& obj, const std::string& key);
	void setBool(const std::string &name, bool value);
	void setInt(const std::string &name, int value);
	void setFloat(const std::string &name, float value);
};
