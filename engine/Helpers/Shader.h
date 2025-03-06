#pragma once

#include <string>
#include <map>
#include <json/single_include/nlohmann/json.hpp>
#include <glm/glm.hpp>

#include "../OWEngine/OWEngine.h"
#include "../Renderers/RenderTypes.h"

using json = nlohmann::json;
struct ShaderDataUniforms
{
	enum UniformType
	{
		UFloat, UInt, UV2F, UV3F, UV4F
	};
	ShaderDataUniforms::UniformType ut = UniformType::UFloat;
	std::string name;
	std::string value;
};

struct OWENGINE_API ShaderData
{
	std::string colourName = "";
	std::string shaderV = "";
	std::string shaderF = "";
	std::string shaderG = "";
	std::string PVMName = "pvm";
	std::string projectionName = "projection";
	std::string viewName = "view";
	std::string modelName = "model";
	std::vector<OWRenderTypes::ShaderMutator> mutatorCallbacks;
	std::vector<ShaderDataUniforms> uniforms;
};

// A wrapper for Shaders. Based on the Shader class at https://learnopengl.com/
class OWENGINE_API Shader //: public ResourceSource
{
#pragma warning( push )
#pragma warning( disable : 4251 )
	enum class StandardUniforms
	{
		PVM,
		Projection,
		View,
		Model,
		CameraPosition
	};
	std::map<StandardUniforms, std::string> mUniforms;
	glm::vec2 scaleByAspectRatio(const glm::vec2& toScale) const;
#pragma warning( pop )
	// After scene::setup it is Ok to modify Renderers
	// but only for efficiency reasons. Modifications
	// cannot change what is happenning else the game 
	// loop will be broken.
	mutable bool mFirstTimeRender = true;
	float aspectRatio() const;
	ShaderData mData;
	mutable bool mUseCalled = false;
public:
	Shader() {}
	Shader(const std::string& vertexPath, const std::string& fragPath,
		const std::string& geometryPath = "");
	Shader(const std::string& fileName);
	Shader(const ShaderData& sd);
	~Shader();
	void appendMutator(OWRenderTypes::ShaderMutator pfunc);
	void callMutators(const glm::mat4& proj, const glm::mat4& view,
		const glm::mat4& model, const glm::vec3& cameraPos, 
		OWRenderTypes::ShaderMutator renderCb) const;

	void setStandardUniformNames(const std::string& pvm,
		const std::string& projection = "",
		const std::string& view = "",
		const std::string& model = "",
		const std::string& cameraPos = "");
	void setStandardUniformValues(const glm::mat4& proj,
		const glm::mat4& view, const glm::mat4& model,
		const glm::vec3& cameraPos);
	void create(const std::string& vertexPath, const std::string& fragPath,
			const std::string& geometryPath);
	void loadBoilerPlates();
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
	void setUniform(ShaderDataUniforms::UniformType ut, const std::string& name, 
		const std::string& value, bool useShader = false) const;
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
