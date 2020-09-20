#pragma once
#include <string>

#include <json/single_include/nlohmann/json.hpp>
#include <glm/glm.hpp>

#include "ResourceSource.h"

using json = nlohmann::json;
/*
	A wrapper for Shaders. Based on the Shader class at https://learnopengl.com/
*/
class OWENGINE_API Shader: public ResourceSource
{
public:
	Shader();
	Shader(const std::string& vertexPath, const std::string& fragPath,
		const std::string& geometryPath);
	~Shader();
	void readFiles(const std::string& vertexPath, const std::string& fragPath,
			const std::string& geometryPath);
	void loadShaders(const std::string& vertexShader, 
					 const std::string& fragShader,
					 const std::string& geometryShader);

	void use() const;
	void setup();
	void cleanUp();
	void debugPrint();
	int program() const { return mShaderProgram; }
	int getUniformLocation(const std::string& name) const;
	int getAttributeLocation(const std::string& name) const;
	void setFloat(const std::string& name, float value, 
					bool useShader = false) const;
	void setInteger(const std::string& name, int value, 
					bool useShader = false) const;
	void setVector2f(const std::string& name, float x, float y, 
					 bool useShader = false) const;
	void setVector2f(const std::string& name, const glm::vec2 &value, 
					 bool useShader = false) const;
	void setVector3f(const std::string& name, float x, float y, float z, 
					 bool useShader = false) const;
	void setVector3f(const std::string& name, const glm::vec3 &value, 
					 bool useShader = false) const;
	void setVector4f(const std::string& name, float x, float y, float z, 
					 float width, bool useShader = false) const;
	void setVector4f(const std::string& name, const glm::vec4 &value, 
					 bool useShader = false) const;
	void setMatrix4(const std::string& name, const glm::mat4 &matrix, 
					 bool useShader = false) const;
private:
#pragma warning( push )
#pragma warning( disable : 4251 )
	struct TransformFeedback
	{
		bool interleaved;
		std::vector<std::string> name;
	};

	std::vector<json> uniforms;
	int mShaderProgram = 0;
	void processUniforms();
	static int addShader(const std::string& sourceCode, 
					unsigned int type, const std::string& errmsg);
	static std::string readFile(const std::string& fileName);
	void attachShader(int shader);
	void linkShaders(int vertexShader, int fragmentShader, int geomShader,
		const std::vector<TransformFeedback>& feedbacks 
			= std::vector<TransformFeedback>());
	void linkShaders(json& obj, const std::string& key);
	static int addVertexShader(const std::string& shader);
	static int addFragmentShader(const std::string& shader);
	static int addGeometryShader(const std::string& shader);
#pragma warning( pop )
};
